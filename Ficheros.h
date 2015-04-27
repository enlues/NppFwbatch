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

public:
	Ficheros();
	Ficheros(std::map <std::string, fileData> lista);
	~Ficheros();
	std::map <std::string, fileData> Todos();
	fileData ficheroSQL();
};

#endif


