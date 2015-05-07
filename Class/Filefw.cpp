#include "Filefw.h"

#ifndef FILEFW_CPP
#define FILEFW_CPP


Filefw::Filefw()
{
	// Carga el map con la informaci�n necesaria a pi��n
	filesfw[TEXT("cfgGen")]._name = TEXT("cfgGen");
	filesfw[TEXT("cfgGen")]._path = TEXT("cfg");
	filesfw[TEXT("cfgGen")]._suffix = TEXT("_ConfigGen.properties");

	filesfw[TEXT("cfgMsg")]._name = TEXT("cfgMsg");
	filesfw[TEXT("cfgMsg")]._path = TEXT("cfg");
	filesfw[TEXT("cfgMsg")]._suffix = TEXT("_ConfigMsgText.properties");

	filesfw[TEXT("cpp")]._name = TEXT("cpp");
	filesfw[TEXT("cpp")]._path = TEXT("fuentescpp");
	filesfw[TEXT("cpp")]._suffix = TEXT(".cpp");

	filesfw[TEXT("h")]._name = TEXT("h");
	filesfw[TEXT("h")]._path = TEXT("include");
	filesfw[TEXT("h")]._suffix = TEXT(".h");

	filesfw[TEXT("sql")]._name = TEXT("sql");
	filesfw[TEXT("sql")]._path = TEXT("sql");
	filesfw[TEXT("sql")]._suffix = TEXT("_ConfigSQL.properties");
}

Filefw::~Filefw()
{
}

// Devuelve el tipo de fichero conforme al mapeo de ficheros cargado.
// NULL si no lo encuentra
wchar_t* Filefw::FileType(wchar_t* nomFich) {

	wchar_t * aux;
	// Recorremos todos los tipos de fichero mapeados
	for (std::map<wchar_t*, fileData>::iterator i = filesfw.begin(); i != filesfw.end(); ++i){
		//Buscamos que tenga ese sufijo
		aux = wcsstr(nomFich, (i->second)._suffix);
		if (aux != NULL) {
			// Revisamos que realmente es un sufijo y no que est� incluido en el nombre del fichero
			if (wcslen(aux) == wcslen((i->second)._suffix)) {
				return (i->second)._name;
			}
		}
	}
	return NULL;

}


// Indica si es un fichero valido incluido en el listado
bool Filefw::isFileTypeOK(wchar_t* nomFich) {

	if (nomFich == NULL) {
		return false;
	}

	wchar_t * aux;
	// Recorremos todos los tipos de fichero mapeados
	for (std::map<wchar_t*, fileData>::iterator i = filesfw.begin(); i != filesfw.end(); ++i){
		//Buscamos que tenga ese sufijo
		aux = wcsstr(nomFich, (i->second)._suffix);
		if (aux != NULL) {
			// Revisamos que realmente es un sufijo y no que est� incluido en el nombre del fichero
			if (wcslen(aux) == wcslen((i->second)._suffix)) {
				return true;
			}
		}
	}
	return false;

}

bool Filefw::isType(wchar_t* FileType) {
	try {
		wchar_t* aux = NULL;

		wchar_t nomFich[MAX_PATH];
		getFullPathFile(nomFich, MAX_PATH);

		if (nomFich == NULL) {
			return false;
		}

		aux = Filefw::FileTypeMap(nomFich);

		if (aux != NULL)
		{
			if (wcscmp(aux, TEXT("cpp")) == 0)
			{
				return true;
			}
		}
			
		return false;
	}
	catch (...) {
		return false;
	}
}

// Devuelve el tipo de fichero conforme al mapeo de ficheros cargado.
// NULL si no lo encuentra
wchar_t* Filefw::FileTypeMap(wchar_t* nomFich) {

	wchar_t * aux;
	// Recorremos todos los tipos de fichero mapeados
	for (std::map<wchar_t*, fileData>::iterator i = filesfw.begin(); i != filesfw.end(); ++i){
		//Buscamos que tenga ese sufijo
		aux = wcsstr(nomFich, (i->second)._suffix);
		if (aux != NULL) {
			// Revisamos que realmente es un sufijo y no que est� incluido en el nombre del fichero
			if (wcslen(aux) == wcslen((i->second)._suffix)) {
				return i->first;
			}
		}
	}
	return NULL;
}

int Filefw::pathBase(wchar_t* pathFich, wchar_t* pathFichOut) {

	wchar_t * sufijo = TEXT("\\..\\");

	//int len = wcslen(pathFich) ;
	wcscpy(pathFichOut, pathFich);
	wcscat(pathFichOut, sufijo);

	return wcslen(pathFichOut);
}

int Filefw::fileBase(wchar_t* nomFich, wchar_t* nomFichOut){

	int longitud = 0;
	wchar_t* tipoActual = Filefw::FileTypeMap(nomFich);
	wcscpy(nomFichOut, TEXT(""));
	longitud = wcslen(nomFich) - wcslen(filesfw[tipoActual]._suffix);
	wcsncat(nomFichOut, nomFich, longitud);
	return wcslen(nomFichOut);

}

int Filefw::fullPathByType(wchar_t* pathFich, wchar_t* nomFich, wchar_t* tipoSolicitado, wchar_t* pathCompletaOut) {

	wchar_t pathCompleta[MAX_PATH] = TEXT("");
	wchar_t nomCompleto[MAX_PATH] = TEXT("");
	// Construimos el path completo
	Filefw::pathBase(pathFich, pathCompleta);
	wcscat(pathCompleta, filesfw[tipoSolicitado]._path);
	wcscat(pathCompleta, TEXT("\\"));

	//Construimos el nombre del fichero completo
	Filefw::fileBase(nomFich, nomCompleto);
	wcscat(nomCompleto, filesfw[tipoSolicitado]._suffix);

	//Construimos toda la ruta completa
	wcscpy(pathCompletaOut, TEXT(""));
	wcscat(pathCompletaOut, pathCompleta);
	wcscat(pathCompletaOut, nomCompleto);

	return wcslen(pathCompletaOut);
}


void Filefw::currentPath(wchar_t* path){

	getPathFile(path, MAX_PATH);

}

void Filefw::currentFileName(wchar_t* fileName){

	getFileName(fileName, MAX_PATH);

}

void Filefw::openFileByType(wchar_t* typeFile){

	wchar_t path[MAX_PATH];
	wchar_t fileName[MAX_PATH];
	wchar_t fullPathOut[MAX_PATH];

	Filefw::currentPath(path);
	Filefw::currentFileName(fileName);
	if (path == NULL || fileName == NULL) return;
	if (Filefw::fullPathByType(path, fileName, typeFile, fullPathOut) > 0) {
		openFile(fullPathOut);
	}

}

void Filefw::openAllFiles() {

	wchar_t originalpath[MAX_PATH];

	getFullPathFile(originalpath, MAX_PATH);

	for (std::map<wchar_t*, fileData>::iterator i = filesfw.begin(); i != filesfw.end(); ++i){
		Filefw::openFileByType(i->first);
		openFile(originalpath);
	}

}


void Filefw::searchTextInFileType(const char * text, wchar_t* typeFile)
{

	Filefw::openFileByType(typeFile);
	searchAndGo(text);
	
}

void Filefw::searchCursorInFileType(wchar_t* typeFile)
{
	wchar_t word[MAX_PATH];
	char text[MAX_PATH];
	int longitud = MAX_PATH;
	wchar_t* aux = NULL;
	wchar_t nomFich[MAX_PATH];

	if (getCurrentWord(text, longitud) > 0) {

		// nos guardamos el fichero actual
		getFullPathFile(nomFich, MAX_PATH);
		aux = Filefw::FileTypeMap(nomFich);

		mbstowcs(word, text, longitud);
		Filefw::openFileByType(typeFile);
		if (!searchAndGo(text))
		{
			Filefw::openFileByType(aux);
		}
	}
}

#endif