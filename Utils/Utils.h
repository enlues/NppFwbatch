#ifndef UTILS_H
#define UTILS_H

// Scintilla Utilities
bool isFileReadable(char *path);
wchar_t * getFileName();
wchar_t * getPathFile();
wchar_t * getFullPathfile();
void openFile(char * path);

// ya pensamos que más queremos
char * getLine(int lineNum);

#endif
