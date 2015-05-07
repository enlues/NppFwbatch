#pragma once

#ifndef FILEFW_H
#define FILEFW_H

using namespace std;

#include <map>
#include <PluginDefinition.h>
#include "Utils.h"

// Clase para gestionar los nombres de los ficheros
class Filefw
{
private:

	//Estructura para la informacion de los ficheros
	struct fileData  {
		wchar_t *_name;
		wchar_t *_path;
		wchar_t *_suffix;
	};


	//Coleccion con los datos de los ficheros
	std::map <wchar_t*, fileData> filesfw;

	//Obtenemos el path raiz desde donde esta
	int pathBase(wchar_t* pathFich, wchar_t* pathFichOut);
	//Obtenemos el nombre del fichero sin el sufijo
	int fileBase(wchar_t* nomFich, wchar_t* nomFichOut);

	//Obtenemos la ruta + nombre de fichero completa
	int fullPathByType(wchar_t* pathFich, wchar_t* nomFich, wchar_t* tipoSolicitado, wchar_t* pathCompletaOut);

	void currentFileName(wchar_t* fileName);
	void currentPath(wchar_t* path);

public:
	Filefw();
	~Filefw();

	//Abre el tipo de fichero indicado asociado al actual
	void openFileByType(wchar_t* tipo);
	//Abre todos los ficheros asociados al actual
	void openAllFiles();


	//Obtenemos el tipo de fichero (que hay dentro del struct)
	wchar_t* FileType(wchar_t* nomFich);
	//Obtenemos el tipo de fichero para recorrer el map
	wchar_t* FileTypeMap(wchar_t* nomFich);
	//Indica si es un tipo de fichero valido
	bool isFileTypeOK(wchar_t* nomFich);
	//Indica si el fichero es del tipo seleccionado
	bool Filefw::isType(wchar_t* FileType);
	//Busca el texto en el fichero del tipo indicado (si no está abierto lo abre)
	void searchTextInFileType(const char * text, wchar_t* typeFile);
	//Busca el texto sobre el que está el cursor en el fichero del tipo indicado (si no está abierto lo abre)
	void searchCursorInFileType(wchar_t* typeFile);

	

};

#endif


