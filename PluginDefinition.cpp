//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

using namespace std;


#include <tchar.h>

#include "PluginDefinition.h"
#include "menuCmdID.h"

//
// put the headers you need here
//
#include <stdlib.h>
#include <time.h>
#include <shlwapi.h>

#include <map>

//Coleccion con los datos de los ficheros
//std::map <std::string, fileData> ficheros;
#include "Ficheros.h"
Ficheros fichero;


const TCHAR sectionName[] = TEXT("Insert Extesion");
const TCHAR keyName[] = TEXT("doCloseTag");
const TCHAR configFileName[] = TEXT("pluginDemo.ini");

#ifdef UNICODE 
	#define generic_itoa _itow
#else
	#define generic_itoa itoa
#endif

FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;


TCHAR iniFilePath[MAX_PATH];
bool doCloseTag = false;

#define DOCKABLE_DEMO_INDEX 15

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
	::WritePrivateProfileString(sectionName, keyName, doCloseTag?TEXT("1"):TEXT("0"), iniFilePath);
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{
	//
	// Firstly we get the parameters from your plugin config file (if any)
	//

	// get path of plugin configuration
	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)iniFilePath);

	// if config path doesn't exist, we create it
	if (PathFileExists(iniFilePath) == FALSE)
	{
		::CreateDirectory(iniFilePath, NULL);
	}

	// make your plugin config file full file path name
	PathAppend(iniFilePath, configFileName);

	// get the parameter value from plugin config
	doCloseTag = (::GetPrivateProfileInt(sectionName, keyName, 0, iniFilePath) != 0);


    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );



	// Shortcut :
	// Following code makes the first command
	// bind to the shortcut Alt-Q
	
	ShortcutKey *pShKey = new ShortcutKey;
	pShKey->_isAlt = false;
	pShKey->_isCtrl = false;
	pShKey->_isShift = false;
	pShKey->_key = 0x78; //VK_9


	setCommand(0, TEXT("Abrir ficheros asociados"), abreTodosFicheros, pShKey, false);

	setCommand(1, TEXT("---"), NULL, NULL, false);
	setCommand(2, TEXT("Abrir definición de SQL"), definicionSQL, NULL, false);
	setCommand(3, TEXT("Descripción del mensaje"), definicionMSG, NULL, false);

}


//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
	delete funcItem[4]._pShKey;
	delete funcItem[9]._pShKey;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//

//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

static DWORD WINAPI threadTextPlayer(void *text2display)
{
	// Open a new document
    ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);

    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return FALSE;

    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
    srand((unsigned int)time(NULL));
    int rangeMin = 0;
    int rangeMax = 250;
    for (size_t i = 0 ; i < strlen((char *)text2display) ; i++)
    {
		char charToShow[2] = {((char *)text2display)[i], '\0'};

        int ranNum = rangeMin + (int)((double)rand() / ((double)RAND_MAX + 1) * rangeMax);
        Sleep(ranNum + 30);

        ::SendMessage(curScintilla, SCI_APPENDTEXT, 1, (LPARAM)charToShow);
		::SendMessage(curScintilla, SCI_GOTOPOS, ::SendMessage(curScintilla, SCI_GETLENGTH, 0, 0), 0);
    }

    return TRUE;
};



// Abre todos los ficheros asociados al que está abierto
void abreTodosFicheros(void)
{
	int which = -1;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
	if (which == -1)
		return;
	HWND curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

	// Ejemplos de como...
	// Obtenemos el directorio actual del fichero
	//::SendMessage(nppData._nppHandle, NPPM_GETCURRENTDIRECTORY, 0, (LPARAM)path);
	// Obtenemos el nombre del fichero
	//::SendMessage(nppData._nppHandle, NPPM_GETFILENAME, 0, (LPARAM)path);
	// Obtenemos el directorio más el nombre del fichero
	//::SendMessage(nppData._nppHandle, NPPM_GETFULLCURRENTPATH, 0, (LPARAM)path);
	
	// Pruebas mostrando cosas:

	
	TCHAR path[MAX_PATH];
	TCHAR pathOriginal[MAX_PATH];
	TCHAR nomFichero[MAX_PATH];
	TCHAR pathOut[MAX_PATH];

	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTDIRECTORY, 0, (LPARAM)path);
	::SendMessage(nppData._nppHandle, NPPM_GETFILENAME, 0, (LPARAM)nomFichero);

	//::MessageBox(nppData._nppHandle, path, nomFichero, MB_OK);

	//Revisamos que el fichero abierto es de uno de los tipos validos
	if (! fichero.tipoFicheroValido(nomFichero)) {
		::MessageBox(nppData._nppHandle, TEXT("Tipo de fichero no valido para esta funcionalidad"), TEXT("ERROR"), MB_OK);
		return;
	}

	//Obtenemos elfichero en el que estamos
	::SendMessage(nppData._nppHandle, NPPM_GETFULLCURRENTPATH, 0, (LPARAM)pathOriginal);

	//Abrimos todos los ficheros

	std::map <std::string, fileData> todos = fichero.Todos();
	
	for (std::map<std::string, fileData>::iterator i = todos.begin(); i != todos.end(); ++i){
		
		fichero.pathCompleta(path, nomFichero, i->first, pathOut);
		//::MessageBox(nppData._nppHandle, pathOut, nomFichero, MB_OK);
		::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, (LPARAM)pathOut);
	}
	
	//Volvemos al fichero en el que estabamos 
	::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, (LPARAM)pathOriginal);
}


void definicionSQL()
{

	int which = -1;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
	if (which == -1)
		return;
	HWND curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

	TCHAR path[MAX_PATH];
	TCHAR nomFichero[MAX_PATH];
	TCHAR pathOut[MAX_PATH];

	TCHAR nombreSQL[MAX_PATH];
	int max_nombreSQL = MAX_PATH;
	TCHAR buf[MAX_PATH]; //PARA PRUEBAS, BORRAR LUEGO LA VARIABLE

	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTDIRECTORY, 0, (LPARAM)path);
	::SendMessage(nppData._nppHandle, NPPM_GETFILENAME, 0, (LPARAM)nomFichero);

	//Revisamos que el fichero abierto es de uno de los tipos validos
	if (! fichero.esCPP(nomFichero)) {
		::MessageBox(nppData._nppHandle, TEXT("Tipo de fichero no valido para esta funcionalidad"), TEXT("ERROR"), MB_OK);
		return;
	}

	//Obtenemos el nombre del SQL seleccionado
	Ed_GetCurrentWord(nombreSQL, max_nombreSQL);
	//::MessageBox(nppData._nppHandle, nombreSQL, TEXT("PALABRA SELECCIONADA"), MB_OK);


	//Abrimos el SQL
	fichero.pathCompleta(path, nomFichero, "sql", pathOut);
	::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, (LPARAM)pathOut);


	//TODO: Buscamos el nombre del SQL seleccionado

	::MessageBox(nppData._nppHandle, nombreSQL, TEXT("PALABRA A BUSCAR"), MB_OK);
	// https://github.com/npp-community/nppcr_npp/blob/master/PowerEditor/src/Notepad_plus.cpp
	// http://nppqcp.googlecode.com/svn-history/r52/trunk/NppQCP.cpp

	// Longitud del fichero seleccionado
	int docLength = (int) ::SendMessage(curScintilla, SCI_GETLENGTH, 0, 0);

	// Parametros para la busqueda
	//::SendMessage(curScintilla, SCI_SETTARGETSTART, 0, 0);
	//::SendMessage(curScintilla, SCI_SETTARGETEND, docLength, 0);
	//::SendMessage(curScintilla, SCI_SETSEARCHFLAGS, SCFIND_WHOLEWORD, 0);
	//::SendMessage(curScintilla, SCI_SEARCHANCHOR, 0, 0);
	
	//int busqueda = ::SendMessage(nppData._nppHandle, SCI_SEARCHINTARGET, wcslen(nombreSQL), (LPARAM)nombreSQL);


	int busqueda = ::SendMessage(curScintilla, SCI_FINDTEXT, 0, (LPARAM)nombreSQL);

	_stprintf(buf, TEXT("tamaño fichero: %d Resultado: %d"), docLength, busqueda);
	::MessageBox(nppData._nppHandle, buf, TEXT("RESULTADO BUSQUEDA"), MB_OK);

	// Posicionamos el cursor en la posición indicada por la busqueda
	::SendMessage(curScintilla, SCI_SETCURRENTPOS, (WPARAM)busqueda, 0);


	//int nIndex = (int)Win32.SendMessage(nppData._nppHandle, SCI_SEARCHINTARGET, Text.Length, Text);


}


void definicionMSG()
{

	int which = -1;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
	if (which == -1)
		return;
	HWND curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

	TCHAR path[MAX_PATH];
	TCHAR nomFichero[MAX_PATH];
	TCHAR pathOut[MAX_PATH];

	TCHAR nombreMSG[MAX_PATH];
	TCHAR textoMSG[MAX_PATH];
	int max_nombreMSG = MAX_PATH;

	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTDIRECTORY, 0, (LPARAM)path);
	::SendMessage(nppData._nppHandle, NPPM_GETFILENAME, 0, (LPARAM)nomFichero);

	//Revisamos que el fichero abierto es de uno de los tipos validos
	if (!fichero.esCPP(nomFichero)) {
		::MessageBox(nppData._nppHandle, TEXT("Tipo de fichero no valido para esta funcionalidad"), TEXT("ERROR"), MB_OK);
		return;
	}

	//Obtenemos el nombre del SQL seleccionado
	Ed_GetCurrentWord(nombreMSG, max_nombreMSG);

	//Abrimos el SQL
	fichero.pathCompleta(path, nomFichero, "cfgMsg", pathOut);
	::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, (LPARAM)pathOut);

	//TODO: Buscamos el nombre del MSG seleccionado


	// Mostramos el texto en un MessageBox
	::MessageBox(nppData._nppHandle, textoMSG, nombreMSG, MB_OK);


}


//Obtenemos la palabra seleccionada
int Ed_GetCurrentWord(TCHAR *word, int& max_length)
{
	CHAR *selected_text = NULL;
	HWND scintilla;
	int sel_length = 0;

	int which = -1;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
	if (which == -1)
		return -1;

	scintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

	/* populate input field with selected word or word under cursor */

	sel_length = SendMessage(scintilla, SCI_GETSELECTIONEND, 0, 0) -
		SendMessage(scintilla, SCI_GETSELECTIONSTART, 0, 0);

	if (sel_length){
		selected_text = new CHAR[sel_length + 1];
		SendMessage(scintilla, SCI_GETSELTEXT, 0, WPARAM(selected_text));
	}
	else{
		int cur, start, end;
		Sci_TextRange text_range;

		cur = SendMessage(scintilla, SCI_GETCURRENTPOS, 0, 0);
		start = SendMessage(scintilla, SCI_WORDSTARTPOSITION, cur, true);
		end = SendMessage(scintilla, SCI_WORDENDPOSITION, start, true);
		sel_length = end - start + 1;

		selected_text = new CHAR[sel_length + 1];

		text_range.chrg.cpMin = start;
		text_range.chrg.cpMax = end;
		text_range.lpstrText = selected_text;

		SendMessage(scintilla, SCI_GETTEXTRANGE, 0, WPARAM(&text_range));
	}

	if (sel_length >= max_length){
		selected_text[max_length - 1] = 0;
		sel_length = max_length - 1;
	}

	mbstowcs(word, selected_text, sel_length);

	max_length = sel_length;

	return 0;
}
