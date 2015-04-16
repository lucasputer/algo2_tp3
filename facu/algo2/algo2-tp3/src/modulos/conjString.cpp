#include "conjString.h"

ConjString::ConjString(){}

ConjString::~ConjString(){}

void ConjString::Agregar(const String& s){
	container.Definir(s, true);
}

void ConjString::Borrar(const String& s){
	container.Definir(s, false);
}

bool ConjString::Pertenece(const String& s) const{
	return container.Definido(s) && container.Obtener(s);
}