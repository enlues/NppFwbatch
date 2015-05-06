#include "PluginDefinition.h"
#include "Utils.h"
#include "Version.h"
#include "AboutDialog.h"
#include "resource.h"

// --- Local variables ---
//static bool do_active_commenting;	// active commenting - create or extend a document block
//static bool do_active_wrapping;	// active wrapping - wrap text inside of document blocks...todo

static NppData nppData;
static SciFnDirect pSciMsg;			// For direct scintilla call
static sptr_t pSciWndData;			// For direct scintilla call
static HANDLE _hModule;				// For dialog initialization

// --- Menu callbacks ---
static void NppFwbatchItFunction();
//static void activeWrapping();
static void showAbout();

// --- Global variables ---
ShortcutKey sk = {false, false, false, 0x78};
FuncItem funcItem[nbFunc] = {
	{TEXT("NppFwbatch - Function"), NppFwbatchItFunction,   0, false, &sk},
	{TEXT("About..."),          showAbout,				0, false, NULL},
	{TEXT(""),                  NULL,					0, false, NULL}, // separator
	{TEXT("About..."),          showAbout,				0, false, NULL},
	{TEXT(""),                  NULL,					0, false, NULL}, // separator
	{TEXT("About..."),          showAbout,				0, false, NULL},
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

void NppFwbatchItFunction()
{
	int startLine;

	// LEEME!!!!!!!!!!!!!!
	// Como no metas aquí alguna llamada el programa falla, misterios misteriosos
	if(!updateScintilla()) return;
	startLine = SendScintilla(SCI_LINEFROMPOSITION, SendScintilla(SCI_GETCURRENTPOS));
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