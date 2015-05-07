#ifndef UTILS_H
#define UTILS_H

// Scintilla Utilities
// Scintilla Utilities
void getPathFile(wchar_t * path, int size);
void getFileName(wchar_t * fileName, int size);
void getFullPathFile(wchar_t * fullPath, int size);
void openFile(wchar_t * path);
void goPoss(int pos);
void searchAndGo(const char * pattern);
int getCurrentWord(wchar_t *word, int& max_length);
void DebugMsg(wchar_t * texto);

#endif
