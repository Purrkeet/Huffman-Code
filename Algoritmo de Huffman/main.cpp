#include "Formulario.h"
using namespace System;
using namespace AlgoritmodeHuffman;
[STAThreadAttribute]
int main(){
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Formulario ^oForm = gcnew Formulario();
	Application::Run(oForm);


}