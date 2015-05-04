#include "Ficheros.h"

#ifndef FICHEROS_CPP
#define FICHEROS_CPP


Ficheros::Ficheros()
{
	// Carga el map con la información necesaria a piñón
	ficheros["cfgGen"]._name = TEXT("cfgGen");
	ficheros["cfgGen"]._path = TEXT("cfg");
	ficheros["cfgGen"]._suffix = TEXT("_ConfigGen.properties");

	ficheros["cfgMsg"]._name = TEXT("cfgMsg");
	ficheros["cfgMsg"]._path = TEXT("cfg");
	ficheros["cfgMsg"]._suffix = TEXT("_ConfigMsgText.properties");

	ficheros["cpp"]._name = TEXT("cpp");
	ficheros["cpp"]._path = TEXT("fuentescpp");
	ficheros["cpp"]._suffix = TEXT(".cpp");

	ficheros["h"]._name = TEXT("h");
	ficheros["h"]._path = TEXT("include");
	ficheros["h"]._suffix = TEXT(".h");

	ficheros["sql"]._name = TEXT("sql");
	ficheros["sql"]._path = TEXT("sql");
	ficheros["sql"]._suffix = TEXT("_ConfigSQL.properties");
}

Ficheros::Ficheros(std::map <std::string, fileData> lista)
{
	// Copia la lista pasada para utilizarla
	ficheros.clear();
	ficheros.insert(lista.begin(), lista.end());
}


Ficheros::~Ficheros()
{
}

// Devuelve el tipo de fichero conforme al mapeo de ficheros cargado.
// NULL si no lo encuentra
TCHAR* Ficheros::tipoFichero(TCHAR* nomFich) {

	TCHAR * aux;
	// Recorremos todos los tipos de fichero mapeados
	for (std::map<std::string, fileData>::iterator i = ficheros.begin(); i != ficheros.end(); ++i){
		//Buscamos que tenga ese sufijo
		aux = wcsstr(nomFich, (i->second)._suffix);
		if (aux != NULL) {
			// Revisamos que realmente es un sufijo y no que está incluido en el nombre del fichero
			if (wcslen(aux) == wcslen((i->second)._suffix)) {
				return (i->second)._name;
			}
		}
	}
	return NULL;

}


// Indica si es un fichero valido incluido en el listado
bool Ficheros::tipoFicheroValido(TCHAR* nomFich) {

	if (nomFich == NULL) {
		return false;
	}

	TCHAR * aux;
	// Recorremos todos los tipos de fichero mapeados
	for (std::map<std::string, fileData>::iterator i = ficheros.begin(); i != ficheros.end(); ++i){
		//Buscamos que tenga ese sufijo
		aux = wcsstr(nomFich, (i->second)._suffix);
		if (aux != NULL) {
			// Revisamos que realmente es un sufijo y no que está incluido en el nombre del fichero
			if (wcslen(aux) == wcslen((i->second)._suffix)) {
				return true;
			}
		}
	}
	return false;

}

bool Ficheros::esCPP(TCHAR* nomFich) {
	std::string aux;

	if (nomFich == NULL) {
		return false;
	}

	aux = Ficheros::tipoFicheroMap(nomFich);

	if (aux.compare("cpp") != 0){
		return false;
	} else {
		return true;
	}

}

// Devuelve el tipo de fichero conforme al mapeo de ficheros cargado.
// NULL si no lo encuentra
std::string Ficheros::tipoFicheroMap(TCHAR* nomFich) {

	TCHAR * aux;
	// Recorremos todos los tipos de fichero mapeados
	for (std::map<std::string, fileData>::iterator i = ficheros.begin(); i != ficheros.end(); ++i){
		//Buscamos que tenga ese sufijo
		aux = wcsstr(nomFich, (i->second)._suffix);
		if (aux != NULL) {
			// Revisamos que realmente es un sufijo y no que está incluido en el nombre del fichero
			if (wcslen(aux) == wcslen((i->second)._suffix)) {
				return i->first;
			}
		}
	}
	return "";

}

std::map <std::string, fileData> Ficheros::Todos() {
	return ficheros;
}


int Ficheros::pathBase(TCHAR* pathFich, TCHAR* pathFichOut) {

	TCHAR * sufijo = TEXT("\\..\\");

	//int len = wcslen(pathFich) ;
	wcscpy(pathFichOut, pathFich);
	wcscat(pathFichOut, sufijo);

	return wcslen(pathFichOut);
}

int Ficheros::nomFichBase(TCHAR* nomFich, TCHAR* nomFichOut){

	int longitud = 0;
	std::string tipoActual = Ficheros::tipoFicheroMap(nomFich);
	wcscpy(nomFichOut, TEXT(""));
	longitud = wcslen(nomFich) - wcslen(ficheros[tipoActual]._suffix);
	wcsncat(nomFichOut, nomFich, longitud);
	return wcslen(nomFichOut);

}

int Ficheros::pathCompleta(TCHAR* pathFich, TCHAR* nomFich, std::string tipoSolicitado, TCHAR* pathCompletaOut) {

	TCHAR pathCompleta[MAX_PATH] = TEXT("");
	TCHAR nomCompleto[MAX_PATH] = TEXT("");
	// Construimos el path completo
	Ficheros::pathBase(pathFich, pathCompleta);
	wcscat(pathCompleta, ficheros[tipoSolicitado]._path);
	wcscat(pathCompleta, TEXT("\\"));

	//Construimos el nombre del fichero completo
	Ficheros::nomFichBase(nomFich, nomCompleto);
	wcscat(nomCompleto, ficheros[tipoSolicitado]._suffix);

	//Construimos toda la ruta completa
	wcscpy(pathCompletaOut, TEXT(""));
	wcscat(pathCompletaOut, pathCompleta);
	wcscat(pathCompletaOut, nomCompleto);

	return wcslen(pathCompletaOut);
}

#endif