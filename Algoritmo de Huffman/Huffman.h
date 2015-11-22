#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <queue>
#include <functional>
#include <cstring>
#include <string>
#include <algorithm>
#include <map>
#include "nodo.h"

using namespace std;
#define SALTOLINEA 257
typedef pair<int, Nodo*> iN;
typedef pair<string, char> Sc;
//struct para la cola de prioridad con Clase definida por el usuario
struct DereferenceCompareNode : public std::binary_function<iN*, iN*, bool>
{
	bool operator()(iN lhs, iN rhs) const
	{
		if (lhs.first != rhs.first) return lhs.first > rhs.first;
		return lhs.second->getChar() > rhs.second->getChar();
	}
};
typedef vector<Sc> vSc;
typedef vector<string> vcS;
typedef priority_queue <iN, vector<iN>, DereferenceCompareNode > cola;
typedef priority_queue <Sc, vector<Sc>, greater<Sc> > colaString;

class Huffman
{
public:
	//atributos
	char valores[255];
	vSc codHuff;
	vcS codHuffpConsultas;
	string archivo;
	int* frec;
	cola ColaPrioridad;
	bool listoParaComprimir;
	//metodos
	Huffman()
	{
		listoParaComprimir = false;
	}

	~Huffman()
	{
	}
private:
	bool compare(Sc a, Sc b){
		//if(a.first.compare(b.first) == 0)
		//	return ;
		if (a.first.size() == b.first.size())
			return (a.first.compare(b.first)<0);
		else
			return a.first.size()<b.first.size();


	}

	Nodo* ProcesarArbol(cola ColaPrioridad){

		Nodo* Arbol = nullptr;
		while (!ColaPrioridad.empty()){
			//coje los 2 menores y crea un arbol binario con ellos
			iN Menor = ColaPrioridad.top(); ColaPrioridad.pop();
			//	std::cout << Menor.first << std::endl;
			if (ColaPrioridad.empty()){
				//si ya no hay mas elementos devolver el arbol
				Arbol = Menor.second;
				break;
			}
			iN Mayor = ColaPrioridad.top(); ColaPrioridad.pop();
			//el nuevo valor del arbol
			int nuevoVal = Menor.first + Mayor.first;
			//se crea el arbol con ese nuevo valor
			//	cout<<nuevoVal<<endl;
			Nodo* NuevoArbol = new Nodo(nuevoVal, Menor.second, Mayor.second);
			//se lo reingresa a la cola de prioridad
			ColaPrioridad.push(iN(nuevoVal, NuevoArbol));
		}

		return Arbol;
	}

	void recorreInOrder(Nodo * n, bool esDerecha, int idx){
		if (n == nullptr) return;

		if (esDerecha)
			valores[idx] = '0';
		else
			valores[idx] = '1';

		Nodo*Izq = n->getIzquierda1();
		Nodo*Der = n->getDerecha0();
		if (Izq == nullptr && Der == nullptr){
			codHuff.push_back(Sc(valores, n->getChar()));
		}
		if (Izq != nullptr){
			if (Izq->getChar() == '\0')
				recorreInOrder(Izq, false, idx + 1);
			else{
				valores[idx + 1] = '1';
				//	cout<<valores<<"1"<<": "<<Izq->getChar()<<endl;
				codHuff.push_back(Sc(valores, Izq->getChar()));
				valores[idx + 1] = '\0';
			}
		}
		if (Der != nullptr){
			if (Der->getChar() == '\0')
				recorreInOrder(Der, true, idx + 1);
			else{
				valores[idx + 1] = '0';
				//	cout<<valores<<"0"<<": "<<Der->getChar()<<endl;
				codHuff.push_back(Sc(valores, Der->getChar()));
				valores[idx + 1] = '\0';
			}
		}
		//limpiar el codigo Huffman
		valores[idx] = '\0';
	}

	void RecorrerArbol(Nodo* raiz){
		//Se recorre para obtener los codigos de Huffman
		//recorrido InOrder
		memset(valores, '\0', sizeof valores);
		codHuff = vSc();
		codHuffpConsultas = vcS(258);
		recorreInOrder(raiz->getIzquierda1(), false, 0);
		recorreInOrder(raiz->getDerecha0(), true, 0);
		//generar la lista de chars con sus valores en cod huffman
		for (int i = 0; i < codHuff.size(); i++) {
			//el caracter sera el indice
			int idx = (int)codHuff[i].second;
			cout << idx << ": " << codHuff[i].second << endl;
			codHuffpConsultas[idx] = codHuff[i].first;
		}

	}

	public:
	vcS ProcesarFrecuencias(string _archivo){

		archivo = _archivo;
		ifstream input;
		input.open(archivo.c_str(), ifstream::in);
		frec = new int[258];
		memset(frec, 0, sizeof(frec)); //INICIO FRECUENCIAS CON 0

		string read;
		bool first = true;
		while (getline(input, read)){
			//por cada linea procesar cada caracter ASCII 255
			if (first){

				first = false;
			}
			else{
				frec[SALTOLINEA]++; //almacenar frec salto de linea que no se lee porque el getline se lo come.	
			}
			for (int i = 0; i < (int)read.size(); i++) //leer hasta salto de linea
			{
				if ((int)read[i]<0)  continue;//omitir caracteres que no son ASCII
				frec[(int)read[i]]++;
			}



		}
		input.close();
		//se inicia la cola antes del ordenamiento
		ColaPrioridad = cola();
		return OrdenaFrecuencias(ColaPrioridad);
	}
	private:
	vcS OrdenaFrecuencias(cola &ColaPrioridad){
		vcS resultado = vcS();
		resultado.push_back(string("Frecuencias:\n "));

		for (int i = 0; i < 256; i++)
		{
			if (frec[i] != 0){
				stringstream ss;
				//cout << (char)i << ": " << frec[i] << " " << endl;
				ss << (char)i << ": " << frec[i];
				//se agrega lo que se mostraba en la consola en un vector de Strings que llenara los 4 list box
				resultado.push_back(ss.str());
				ColaPrioridad.push(iN(frec[i], new Nodo(ic(frec[i], (char)i))));
			}

		}
		
		if (frec[SALTOLINEA] != 0){
			//salto de linea
			stringstream ss;
			ss << "Salto de Línea: " << frec[SALTOLINEA];
			resultado.push_back(ss.str());
			ColaPrioridad.push(iN(frec[SALTOLINEA], new Nodo(ic(frec[SALTOLINEA], '\n'))));
		}
		//cout << endl;
		return resultado;
	}
	public:
	vcS Comprimir(){
	
		vcS resultado = vcS();
		if (listoParaComprimir){
			//2) Procesar arbol
			Nodo* raiz = ProcesarArbol(ColaPrioridad);
			//3) Recorrer arbol para obtener los codigos que se guardan en el vector codHuff
			RecorrerArbol(raiz);
			//Mostrar Codigo Huffman
			//4)Ordenar el codigo Huffman para mostrar
			sort(codHuff.begin(), codHuff.end(), compare);
			for (int i = 0; i < codHuff.size(); i++) {
				string cha_s = ""; cha_s += codHuff[i].second;
				stringstream ss;
				//cout << codHuff[i].first << " : " << (cha_s[0] == '\n' ? "*S" : (cha_s[0] == ' ' ? "*E" : cha_s)) << endl;
				ss << codHuff[i].first << " : " << (cha_s[0] == '\n' ? "Salto Línea" : (cha_s[0] == ' ' ? "Espacio" : cha_s));
				resultado.push_back(ss.str());
			}

			//inicio de codificacion. 
			ofstream out("comprimido.txt", ofstream::out);
			ifstream input(archivo.c_str(), ifstream::in);
			//5)escribir cabecera del nuevo archivo
			for (int i = 0; i < codHuffpConsultas.size(); i++) {
				/*if(i == 10) //si es salto de linea imprime *S
				out<<"*S";
				else if(i == 32)//si es espacio imprime *E
				out<<"*E";
				else*/
				out << i;
				if (codHuffpConsultas[i].empty()){
					out << " -";
				}
				else
					out << " " << codHuffpConsultas[i];
				out << endl;
			}
			//6) Leer nuevamente y escribir en un nuevo archivo
			input.clear();
			input.seekg(0, input.beg);//regresar al inicio
			string read;
			while (getline(input, read)){
				//por cada linea procesar cada caracter ASCII 255
				for (int i = 0; i < (int)read.size(); i++) //leer hasta salto de linea
				{
					//	cerr<<(int)((char)read[i])<<read[i]<<endl;
					if ((int)read[i]<0)  continue;//omitir caracteres que no son ASCII
					out << codHuffpConsultas[(int)read[i]];
				}
				//escribir salto de linea (10)
				out << codHuffpConsultas[10];
			}
			out << endl;
			//al finalizar
			input.close(); //soltar archivo
			out.close();//soltar archivo
			delete frec; //libera memoria
		}
		return resultado;
	}
	vcS Descomprimir(string ruta){
		//el ultimo item del resultado es el texto desencriptado que debe ir en el Textbox de resultado
		vcS resultado = vcS();

		archivo = ruta;
		/** Procesar Frecuencias **/
		ifstream input;
		//para obtener la equivalencia del codigo en O(log(n))
		map<string, char> DecodeHuffman;
		std::map<string, char>::iterator it;
		input.open(archivo.c_str(), ifstream::in);


		string read_cod;
		int read_ch;
		resultado.push_back("Códigos Huffman de los caractéres:");
		for (int i = 0; i < 258; i++) {
			input >> read_ch >> read_cod;
			if (read_cod == "-") continue;
			//cout << (char)read_ch << ": " << read_cod << endl;
			stringstream ss;
			string charr = ""; charr += (char)read_ch;
			ss << ((char)read_ch == '\n' ? "Salto Línea" : ((char)read_ch == ' ' ? "Espacio" : charr)) << ": " << read_cod;
			resultado.push_back(ss.str());
			DecodeHuffman.insert(Sc(read_cod, (char)read_ch));
		}

		string buff;
		//lee el comprimido
		input >> buff;
		//sliding window technique para obtener los datos
		string cod;
		stringstream textoDescomprimido;
		for (int i = 0; i < buff.size();) {
			cod = "";
			for (int j = 0; i + j < buff.size(); j++) {
				cod += buff[i + j];
				it = DecodeHuffman.find(cod);
				if (it == DecodeHuffman.end()){
					continue;//no se encontro, sigue creciendo el window
				}
				//	cout<< cod<<endl;
				char a = it->second; //character
				//cout << a;
				textoDescomprimido << a;
				//	cout <<i<<endl;
				i += j + 1;
				//	cout << i<<endl;
				break;
			}

		}
		resultado.push_back(textoDescomprimido.str());
		ofstream salida("descomprimido.txt", ofstream::out);
		salida << textoDescomprimido.str();
		input.close();
		salida.close();

		return resultado;

	}

private:

};



//Atributos
//char valores[255];
//vSc codHuff;
//vcS codHuffpConsultas;
//string archivo;
//int* frec;
//cola ColaPrioridad;
//bool listoParaComprimir; //iniciar como false, y cada vez que se abre un nuevo archivo
//metodos


//int main(int argc, char* argv[]) {
//	if (argc == 1) return 0;
//	if (argc == 2){
//
//		//COMPRIMIENDO
//		////archivo = argv[1];
//		///** Procesar Frecuencias **/
//		//ifstream input;
//
//
//		////input.open(archivo.c_str(), ifstream::in);
//		////0) Paso cero, Procesar Frecuencias
//		////ProcesarFrecuencias(input);
//		///* Fin de Proceso de frecuencias */
//
//		///** Crear Codigo Huffman **/
//		////1) Ordena el arreglo segun frecuencias omitiendo los de frecuencia 0
//		//
//		//  //la cola almacena un par (frecuencia, Nodo*caracter)
//		////OrdenaFrecuencias(ColaPrioridad);
//		////2) Procesar arbol
//		//Nodo* raiz = ProcesarArbol(ColaPrioridad);
//		////3) Recorrer arbol para obtener los codigos que se guardan en el vector codHuff
//		//RecorrerArbol(raiz);
//		////Mostrar Codigo Huffman
//		////4)Ordenar el codigo Huffman para mostrar
//		//sort(codHuff.begin(), codHuff.end(), compare);
//		//for (int i = 0; i < codHuff.size(); i++) {
//		//	string cha_s = ""; cha_s += codHuff[i].second;
//		//	cout << codHuff[i].first << " : " << (cha_s[0] == '\n' ? "*S" : (cha_s[0] == ' ' ? "*E" : cha_s)) << endl;
//		//}
//
//		////inicio de codificacion. 
//		//ofstream out("out.txt", ofstream::out);
//		////5)escribir cabecera del nuevo archivo
//		//for (int i = 0; i < codHuffpConsultas.size(); i++) {
//		//	/*if(i == 10) //si es salto de linea imprime *S
//		//	out<<"*S";
//		//	else if(i == 32)//si es espacio imprime *E
//		//	out<<"*E";
//		//	else*/
//		//	out << i;
//		//	if (codHuffpConsultas[i].empty()){
//		//		out << " -";
//		//	}
//		//	else
//		//		out << " " << codHuffpConsultas[i];
//		//	out << endl;
//		//}
//		////6) Leer nuevamente y escribir en un nuevo archivo
//		//input.clear();
//		//input.seekg(0, input.beg);//regresar al inicio
//		//string read;
//		//while (getline(input, read)){
//		//	//por cada linea procesar cada caracter ASCII 255
//		//	for (int i = 0; i < (int)read.size(); i++) //leer hasta salto de linea
//		//	{
//		//		//	cerr<<(int)((char)read[i])<<read[i]<<endl;
//		//		if ((int)read[i]<0)  continue;//omitir caracteres que no son ASCII
//		//		out << codHuffpConsultas[(int)read[i]];
//		//	}
//		//	//escribir salto de linea (10)
//		//	out << codHuffpConsultas[10];
//		//}
//		//out << endl;
//		////al finalizar
//		//input.close(); //soltar archivo
//		//out.close();//soltar archivo
//		//delete frec; //libera memoria
//		
//	}
//	else {
//		//Descomprimir si recibe mas de un parametro, el ultimo es el archivo de entrada
//		string archivo;
//		archivo = argv[2];
//		/** Procesar Frecuencias **/
//		ifstream input;
//		map<string, char> DecodeHuffman;
//		std::map<string, char>::iterator it;
//		input.open(archivo.c_str(), ifstream::in);
//
//
//		string read_cod;
//		int read_ch;
//		for (int i = 0; i < 258; i++) {
//			input >> read_ch >> read_cod;
//			if (read_cod == "-") continue;
//			cout << (char)read_ch << ": " << read_cod << endl;
//			DecodeHuffman.insert(Sc(read_cod, (char)read_ch));
//		}
//
//		//string buff;
//		////lee el comprimido
//		//input >> buff;
//		////sliding window technique para obtener los datos
//		//string cod;
//		//for (int i = 0; i < buff.size();) {
//		//	cod = "";
//		//	for (int j = 0; i + j < buff.size(); j++) {
//		//		cod += buff[i + j];
//		//		it = DecodeHuffman.find(cod);
//		//		if (it == DecodeHuffman.end()){
//		//			continue;//no se encontro, sigue creciendo el window
//		//		}
//		//		//	cout<< cod<<endl;
//		//		char a = it->second; //character
//		//		cout << a;
//		//		//	cout <<i<<endl;
//		//		i += j + 1;
//		//		//	cout << i<<endl;
//		//		break;
//		//	}
//
//		//}
//
//	}
//	return 0;
//}