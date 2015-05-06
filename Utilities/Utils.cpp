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

void searchAndGo(const char * pattern)
{
	int docLength = (int) SendScintilla(SCI_GETLENGTH, 0, 0);

	// TODO revisar esta parte, está mal
	SendScintilla(SCI_SETTARGETSTART, (WPARAM)0, SCI_UNUSED);
	SendScintilla(SCI_SETTARGETEND, (WPARAM)docLength, SCI_UNUSED);

	int pos = SendScintilla(SCI_SEARCHINTARGET, (WPARAM)strlen(pattern), (LPARAM)pattern);

	goPoss(pos);
}


//Obtenemos la palabra seleccionada
int getCurrentWord(wchar_t *word, int& max_length)
{
	CHAR *selected_text = NULL;
	int sel_length = 0;

	/* populate input field with selected word or word under cursor */

	sel_length = SendScintilla( SCI_GETSELECTIONEND, 0, 0) -
		SendScintilla( SCI_GETSELECTIONSTART, 0, 0);

	if (sel_length){
		selected_text = new CHAR[sel_length + 1];
		SendScintilla( SCI_GETSELTEXT, 0, WPARAM(selected_text));
	}
	else{
		int cur, start, end;
		Sci_TextRange text_range;

		cur = SendScintilla( SCI_GETCURRENTPOS, 0, 0);
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

	mbstowcs(word, selected_text, sel_length);

	max_length = sel_length;

	return 0;
}


void DebugMsg(wchar_t * texto)
{

	MessageBox(NULL, texto, TEXT("DEBUG"), MB_OK);

}
