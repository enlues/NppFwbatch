#include "PluginDefinition.h"
#include "Utils.h"

// filename
void getFileName(wchar_t * fileName, int size)
{
	SendNpp(NPPM_GETFILENAME, size, (LPARAM)fileName);
}

// path
void getPathFile(wchar_t * path, int size)
{
	SendNpp(NPPM_GETCURRENTDIRECTORY, size, (LPARAM)path);
}

void getFullPathFile(wchar_t * fullPath, int size)
{
	SendNpp(NPPM_GETFULLCURRENTPATH, size, (LPARAM)fullPath);
}

void openFile(wchar_t * filepath)
{
	SendNpp(NPPM_DOOPEN, 0, (LPARAM)filepath);
}

void goPoss(int pos)
{
	SendScintilla(SCI_GOTOPOS, (WPARAM)pos, SCI_UNUSED);
}

bool searchAndGo(const char * pattern)
{

	int docLength = SendScintilla(SCI_GETLENGTH, 0, SCI_UNUSED);
	int wordLength = strlen(pattern);

	SendScintilla(SCI_SETTARGETSTART, (WPARAM)0, SCI_UNUSED);
	SendScintilla(SCI_SETTARGETEND, (WPARAM)docLength, SCI_UNUSED);

	int pos = SendScintilla(SCI_SEARCHINTARGET, (WPARAM)strlen(pattern), (LPARAM)pattern);

	if (pos > 0)
	{
		goPoss(pos);
		selectText(pos, pos + wordLength);
		return true;
	}
	else
	{
		return false;
	}
}
void selectText(int posIni, int posFin) {
	SendScintilla(SCI_SETSELECTIONSTART, (WPARAM)posIni, SCI_UNUSED);
	SendScintilla(SCI_SETSELECTIONEND, (WPARAM)posFin, SCI_UNUSED);

}

//Obtenemos la palabra seleccionada o posicionada debajo del cursor
int getCurrentWord(char* word, int& max_length)
{
	CHAR *selected_text = NULL;
	int sel_length = 0;

	/* populate input field with selected word or word under cursor */
	sel_length = SendScintilla(SCI_GETSELECTIONEND, (WPARAM)0, SCI_UNUSED) -
		SendScintilla(SCI_GETSELECTIONSTART, (WPARAM)0, SCI_UNUSED);
	if (sel_length){
		selected_text = new CHAR[sel_length + 1];
		SendScintilla( SCI_GETSELTEXT, 0, WPARAM(selected_text));
	} else{
		int cur, start, end;
		Sci_TextRange text_range;
		cur = SendScintilla(SCI_GETCURRENTPOS, (WPARAM)0, SCI_UNUSED);
		start = SendScintilla( SCI_WORDSTARTPOSITION, cur, true);
		end = SendScintilla( SCI_WORDENDPOSITION, start, true);
		sel_length = end - start + 1;

		selected_text = new CHAR[sel_length + 1];

		text_range.chrg.cpMin = start;
		text_range.chrg.cpMax = end;
		text_range.lpstrText = selected_text;

		SendScintilla( SCI_GETTEXTRANGE, 0, WPARAM(&text_range)); 
	}
	if (sel_length >= max_length){
		selected_text[max_length - 1] = 0;
		sel_length = max_length - 1;
	}

	strcpy_s(word, max_length, selected_text);

	max_length = sel_length;

	return sel_length;
}


void DebugMsg(wchar_t * texto)
{
	MessageBox(NULL, texto, TEXT("DEBUG"), MB_OK);
}

