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

fileData Ficheros::ficheroSQL() {
	/*
	// Como recorrer todos los ficheros:
	for (std::map<std::string, fileData>::iterator i = ficheros.begin(); i != ficheros.end(); ++i){
		//::MessageBox(nppData._nppHandle, (i->second)._suffix, (i->second)._name, MB_OK);
	}
	*/
	return ficheros["sql"];

}


std::map <std::string, fileData> Ficheros::Todos() {
	return ficheros;
}

#endif