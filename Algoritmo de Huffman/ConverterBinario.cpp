
#include "ConverterBinario.h"


ConverterBinario::ConverterBinario()
{}
ConverterBinario::ConverterBinario(char* ruta_archivo)
{
	strcpy(this->rutaArchivo, ruta_archivo);
}
ConverterBinario::~ConverterBinario()
{}

void ConverterBinario::guardarContenido()
{
	char bit;
	int cantidad_bits = 0;
	int indice = 1;
	TBITS byte;

	bit = fgetc(archivoTexto);
	while (bit != EOF)
	{
		byte.set_bit(indice, (bit-'0'));

		if (indice == 8)
		{
			indice = 1;
			fwrite(&byte, sizeof(char), 1, archivoBinario);
			memset(&byte, 0, sizeof(char));
		}
		else
		{
			indice++;
		}
		bit = fgetc(archivoTexto);
		cantidad_bits++;
	}

	if (indice > 1)
	{
		fwrite(&byte, sizeof(char), 1, archivoBinario);
	}
}
void ConverterBinario::leerContenido()
{
	TBITS byte;
	while (fread(&byte, sizeof(char), 1, archivoBinario) != 0)
	{
		for (int i = 1; i <= 8; i++)
			cout << char(byte.get_bit(i) + '0');
	}
	cout << endl;
}



TBITS* ConverterBinario::convertir_arraybits_bytes(short longitud, char* arraybits, TBITS* bytes)
{
	int indice = 1;
	int k = 0;
	for (int i = 0; i < longitud; i++)
	{
		bytes[k].set_bit(indice, arraybits[i] - '0');
		indice == 8 ? indice = 1, k++ : indice++;
	}

	return bytes;
}

void ConverterBinario::_guardarFilaToken(FILE* archivoBinario, int letra, char* arraybits)
{
	short longitud = strlen(arraybits);
	int cantidad_bytes = (longitud / 8);

	if (longitud % 8 != 0)
		cantidad_bytes++;

	TBITS* bytes = new TBITS[cantidad_bytes];
	memset(bytes, 0, cantidad_bytes);

	fwrite(&letra, sizeof(int), 1, archivoBinario);
	fwrite(&longitud, sizeof(short), 1, archivoBinario);

	bytes = convertir_arraybits_bytes(longitud, arraybits, bytes);
	fwrite(bytes, sizeof(char), cantidad_bytes, archivoBinario);
}

void ConverterBinario::_guardarNoFilaToken(FILE* archivoBinario, int caracter)
{
	short longitud = 0;
	fwrite(&caracter, sizeof(int), 1, archivoBinario);
	fwrite(&longitud, sizeof(short), 1, archivoBinario);
}

void ConverterBinario::guardarToken()
{
	int letra;
	char cadena[MAX];
	char arraybits[MAX];

	for (size_t i = 0; i < 258; i++)
	{
		fscanf(archivoTexto, "%s %s\n", cadena, arraybits);
		letra = atoi(cadena);

		if (arraybits[0] != '-')
			_guardarFilaToken(archivoBinario, letra, arraybits);
		else
			_guardarNoFilaToken(archivoBinario, letra);
	}

}

void ConverterBinario::leerToken()
{
	int letra;
	short longitud;
	int cantidad_bytes;
	TBITS* bytes;
	fseek(archivoBinario, 0, SEEK_SET);
	for (int i = 0; i < 258; i++)
	{
		fread(&letra, sizeof(int), 1, archivoBinario);
		fread(&longitud, sizeof(short), 1, archivoBinario);

		if (longitud > 0)
		{
			cantidad_bytes = (longitud / 8);
			if (longitud % 8 != 0)
				cantidad_bytes++;

			bytes = new TBITS[cantidad_bytes];
			fread(bytes, sizeof(char), cantidad_bytes, archivoBinario);

			printf("%d ", letra);
			int k = 0;
			int indice = 1;
			for (int i = 0; i < longitud; i++)
			{
				printf("%d", bytes[k].get_bit(indice));
				indice == 8 ? indice = 1, k++ : indice++;
			}
			printf("\n");
		}
		else
		{
			bytes = NULL;
			printf("%d -\n", letra);
		}
	}
}


void ConverterBinario::Convertir_Texto_Binario()
{
	archivoTexto = fopen(rutaArchivo, "rt");
	archivoBinario = fopen(OUTPUT_ARCHIVO, "wb");

	guardarToken();
	guardarContenido();

	fclose(archivoTexto);
	fclose(archivoBinario);
}
void ConverterBinario::leerArchivoBinario()
{
	archivoBinario = fopen(OUTPUT_ARCHIVO, "rb");

	leerToken();
	leerContenido();

	fclose(archivoBinario);
}