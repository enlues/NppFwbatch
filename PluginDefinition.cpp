#include "PluginDefinition.h"
#include "Utils.h"
#include "Version.h"
#include "AboutDialog.h"
#include "resource.h"
#include "Filefw.h"

// --- Local variables ---
//static bool do_active_commenting;	// active commenting - create or extend a document block
//static bool do_active_wrapping;	// active wrapping - wrap text inside of document blocks...todo

static NppData nppData;
static SciFnDirect pSciMsg;			// For direct scintilla call
static sptr_t pSciWndData;			// For direct scintilla call
static HANDLE _hModule;				// For dialog initialization

// --- Menu callbacks ---
static void NppFwbatchOpenAllFiles();

static void NppFwbatchOpenFileCPP();
static void NppFwbatchOpenFileSQL();
static void NppFwbatchOpenFileMSG();
static void NppFwbatchOpenFileGEN();
static void NppFwbatchOpenFileH();
static void NppGoSQL();
static void NppGoMSG();

//static void activeWrapping();
static void showAbout();

// --- Global variables ---
ShortcutKey sk1 = {false, false, false, 0x78};
FuncItem funcItem[nbFunc] = {
	{ TEXT("Open All Files"), NppFwbatchOpenAllFiles, 0, false, &sk1 },
	{ TEXT(""), NULL, 0, false, NULL }, // separator
	{ TEXT("Open File CPP"), NppFwbatchOpenFileCPP, 0, false, NULL },
	{ TEXT("Open File SQL"), NppFwbatchOpenFileSQL, 0, false, NULL },
	{ TEXT("Open File cfgMsg"), NppFwbatchOpenFileMSG, 0, false, NULL },
	{ TEXT("Open File cfgGen"), NppFwbatchOpenFileGEN, 0, false, NULL },
	{ TEXT("Open File H"), NppFwbatchOpenFileH, 0, false, NULL },
	{TEXT(""),                  NULL,					0, false, NULL}, // separator
	{ TEXT("Search in SQL"), NppGoSQL, 0, false, NULL },
	{ TEXT("Search in MSG"), NppGoMSG, 0, false, NULL },
	{ TEXT(""), NULL, 0, false, NULL }, // separator
	{TEXT("About..."),          showAbout,				0, false, NULL}
};


inline LRESULT SendScintilla(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return pSciMsg(pSciWndData, Msg, wParam, lParam);
}

inline LRESULT SendNpp(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return SendMessage(nppData._nppHandle, Msg, wParam, lParam);
}

bool updateScintilla()
{
	HWND curScintilla;

	// Get the current scintilla
	int which = -1;
	SendNpp(NPPM_GETCURRENTSCINTILLA, SCI_UNUSED, (LPARAM)&which);
	if(which == -1) return false;
	curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

	// Get the function and pointer to it for more efficient calls
	pSciMsg = (SciFnDirect) SendMessage(curScintilla, SCI_GETDIRECTFUNCTION, 0, 0);
	pSciWndData = (sptr_t) SendMessage(curScintilla, SCI_GETDIRECTPOINTER, 0, 0);

	return true;
}

// --- Configuration ---

void pluginInit(HANDLE hModule)
{
	_hModule = hModule;
}

void pluginCleanUp()
{
}

void setNppInfo(NppData notepadPlusData)
{
	nppData = notepadPlusData;
}


// --- Menu call backs -----------------------------------------------------------------------------------------------------------

void NppFwbatchOpenAllFiles()
{
	Filefw filefw;

	if (!updateScintilla()) return;

	filefw.openAllFiles();

	// LEEME!!!!!!!!!!!!!!
	// Como no metas aquí alguna llamada a SendScintilla el programa falla, misterios misteriosos
	SendScintilla(SCI_LINEFROMPOSITION, SendScintilla(SCI_GETCURRENTPOS));
	
}

void NppFwbatchOpenFileSQL()
{
	Filefw filefw;

	if (!updateScintilla()) return;

	filefw.openFileByType(TEXT("sql"));
}
void NppFwbatchOpenFileMSG()
{
	Filefw filefw;

	if (!updateScintilla()) return;

	filefw.openFileByType(TEXT("cfgMsg"));
}
void NppFwbatchOpenFileGEN()
{
	Filefw filefw;

	if (!updateScintilla()) return;

	filefw.openFileByType(TEXT("cfgGen"));
}
void NppFwbatchOpenFileCPP()
{
	Filefw filefw;

	if (!updateScintilla()) return;

	filefw.openFileByType(TEXT("cpp"));
}
void NppFwbatchOpenFileH()
{
	Filefw filefw;

	if (!updateScintilla()) return;

	filefw.openFileByType(TEXT("h"));
}
void NppGoSQL()
{
	Filefw filefw;

	if (!updateScintilla()) return;
	if (!filefw.isType(TEXT("cpp"))) return;

	filefw.searchCursorInFileType(TEXT("sql"));
}


void NppGoMSG()
{
	Filefw filefw;

	if (!updateScintilla()) return;
	if (!filefw.isType(TEXT("cpp"))) return;
	
	filefw.searchCursorInFileType(TEXT("cfgMsg"));
}




void showAbout()
{
	CreateDialog((HINSTANCE) _hModule, MAKEINTRESOURCE(IDD_ABOUTDLG), nppData._nppHandle, abtDlgProc);
}


// --- Notification callbacks ---

void handleNotification(SCNotification *notifyCode)
{
	static bool do_newline = false;
	NotifyHeader nh = notifyCode->nmhdr;
	int ch = notifyCode->ch;

	switch(nh.code)
	{
	case SCN_UPDATEUI: // Now is when we can check to see if we do the commenting
		// REVISAME
		if(do_newline)
		{
			do_newline = false;
			if(!updateScintilla()) return;
		}
		break;
	case SCN_CHARADDED:
		break;
	case NPPN_READY:
		break;
	case NPPN_SHUTDOWN:
		break;
	case NPPN_BUFFERACTIVATED:
	case NPPN_LANGCHANGED:
		break;
	}
	return;
}