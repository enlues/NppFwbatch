#pragma once

#ifndef FICHEROS_H
#define FICHEROS_H

using namespace std;

#include <map>
#include <PluginDefinition.h>


// Clase para gestionar los nombres de los ficheros
class Ficheros
{
private:

	//Coleccion con los datos de los ficheros
	std::map <std::string, fileData> ficheros;
	TCHAR* pathBase();

public:
	Ficheros();
	Ficheros(std::map <std::string, fileData> lista);
	~Ficheros();
	std::map <std::string, fileData> Todos();

	//Obtenemos el tipo de fichero (que hay dentro del struct)
	TCHAR* tipoFichero(TCHAR* nomFich);
	//Obtenemos el tipo de fichero para recorrer el map
	std::string tipoFicheroMap(TCHAR* nomFich);
	//Indica si es un tipo de fichero valido
	bool tipoFicheroValido(TCHAR* nomFich);
	//Indica si es un tipo de fichero CPP
	bool esCPP(TCHAR* nomFich);

	//Obtenemos el path raiz desde donde esta
	int pathBase(TCHAR* pathFich, TCHAR* pathFichOut);
	//Obtenemos el nombre del fichero sin el sufijo
	int nomFichBase(TCHAR* nomFich, TCHAR* nomFichOut);
	//Obtenemos la ruta + nombre de fichero completa
	int pathCompleta(TCHAR* pathFich, TCHAR* nomFich, std::string tipoSolicitado, TCHAR* pathCompletaOut);
	

};

#endif


