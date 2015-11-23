#pragma once
#include <stdio.h>
#include <iostream>

using namespace std;

#define MAX 200


struct TBITS{
	unsigned char b1 : 1;
	unsigned char b2 : 1;
	unsigned char b3 : 1;
	unsigned char b4 : 1;
	unsigned char b5 : 1;
	unsigned char b6 : 1;
	unsigned char b7 : 1;
	unsigned char b8 : 1;

	void set_bit(int indice, bool valor)
	{
		switch (indice)
		{
		case 1:
			b1 = valor;
			break;
		case 2:
			b2 = valor;
			break;
		case 3:
			b3 = valor;
			break;
		case 4:
			b4 = valor;
			break;
		case 5:
			b5 = valor;
			break;
		case 6:
			b6 = valor;
			break;
		case 7:
			b7 = valor;
			break;
		case 8:
			b8 = valor;
			break;
		default:
			break;
		}
	}

	char get_bit(int indice)
	{
		switch (indice)
		{
		case 1:
			return b1;
			break;
		case 2:
			return b2;
			break;
		case 3:
			return b3;
			break;
		case 4:
			return b4;
			break;
		case 5:
			return b5;
			break;
		case 6:
			return b6;
			break;
		case 7:
			return b7;
			break;
		case 8:
			return b8;
			break;
		default:
			break;
		}
		return -1;
	}
};

ref class ConverterBinario
{
private:
	FILE* archivoTexto;
	FILE* archivoBinario;
	
	char* ruta_archivo;

	TBITS* convertir_arraybits_bytes(short longitud, char* arraybits, TBITS* bytes);
	void _guardarFilaToken(FILE* archivoBinario, int letra, char* arraybits);
	void _guardarNoFilaToken(FILE* archivoBinario, int caracter);
	
public:
	ConverterBinario();
	ConverterBinario(char* ruta_archivo);
	~ConverterBinario();

	void guardarToken();
	void leerToken();

	void guardarContenido();
	void leerContenido();

	void Convertir_Texto_Binario();
	void leerArchivoBinario();
};

