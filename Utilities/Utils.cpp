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
	// TODO revisar esta parte, está mal
	SendScintilla(SCI_SETTARGETSTART, (WPARAM)0, SCI_UNUSED);
	SendScintilla(SCI_SETTARGETEND, (WPARAM)1000000, SCI_UNUSED);

	int pos = SendScintilla(SCI_SEARCHINTARGET, (WPARAM)strlen(pattern), (LPARAM)pattern);

	goPoss(pos);
}