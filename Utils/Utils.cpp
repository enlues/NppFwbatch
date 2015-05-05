#include "PluginDefinition.h"
#include "Utils.h"

// filename
wchar_t * getFileName()
{
	wchar_t fileName[MAX_PATH];
	SendNpp(NPPM_GETFILENAME, MAX_PATH, (LPARAM) fileName);
    return fileName;
}

// path
wchar_t * getPathFile()
{
	wchar_t path[MAX_PATH];
	SendNpp(NPPM_GETCURRENTDIRECTORY, MAX_PATH, (LPARAM)path);
    return path;
}

bool isFileReadable(char *path)
{
}

wchar_t * getFullPathfile();
{
}

void openFile(wchar_t * filepath);
{
	SendNpp(NPPM_DOOPEN, 0, (LPARAM)filepath);
}
