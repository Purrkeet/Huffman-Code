#include <iostream>

using namespace std;

typedef pair<int, char> ic;

class Nodo{
public:
	Nodo* Derecha0;
	Nodo* Izquiera1;
	ic Valor; //peso, caracter : si no contiene caracter almacena '\0'    
	Nodo(){
		Valor = ic(0, '\0');
		Derecha0 = Izquiera1 = nullptr;
	}
	Nodo(ic val){
		Valor = ic(val);
		Derecha0 = Izquiera1 = nullptr;
	}
	Nodo(int val, Nodo* der, Nodo* izq){
		Valor = ic(val, '\0');
		Derecha0 = der;
		Izquiera1 = izq;
	}


	Nodo * getDerecha0(){
		return Derecha0;
	}
	Nodo * getIzquierda1(){
		return Izquiera1;
	}
	ic getValue(){
		return Valor;
	}
	char getChar() const{
		return Valor.second;
	}
	void setDerecha0(Nodo* n){
		Derecha0 = n;
	}
	void setIzquierda1(Nodo* n){
		Izquiera1 = n;
	}
	void setValue(ic v){
		Valor = v;
	}

};
/*bool operator<(const Nodo& lhs, const Nodo& rhs){
return lhs.getChar()<lhs.getChar();
}*/