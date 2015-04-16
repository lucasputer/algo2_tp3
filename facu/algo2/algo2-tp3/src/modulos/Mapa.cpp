#include "Mapa.h"

using namespace aed2;

Mapa::Mapa(){}

Mapa::~Mapa(){}

void Mapa::Agregar(const String& e){
	assert(!(this->Esta(e)));
	_estaciones.AgregarAdelante(e);
	DiccString<RestriccionTP>* diccR = new DiccString<RestriccionTP>();//Ojo, aca hay copy elision
	_restricciones.Definir(e, *diccR);
	delete diccR;
}

void Mapa::Conectar(const String& e1, const String& e2, const RestriccionTP& r){
	assert(this->Esta(e1) && this->Esta(e2));

	if (e1 < e2){
		Conectar(e2, e1, r);

	}else{
		DiccString<RestriccionTP>* diccR = &(this->_restricciones.Obtener(e1));
		diccR->Definir(e2, r);
		Senda tEstaciones = Senda(e1, e2);
		_sendas.AgregarAdelante(tEstaciones);
	}

}

bool Mapa::Esta(const String& e) const{
	return _restricciones.Definido(e);
}

bool Mapa::Conectadas(const String& e1, const String& e2) const{
	assert(this->Esta(e1) && this->Esta(e2));
	
	if (e1 < e2)
		return Conectadas(e2, e1);

	DiccString<RestriccionTP>* diccR = &(this->_restricciones.Obtener(e1));
	return diccR->Definido(e2);
}

RestriccionTP& Mapa::Restriccion(const String& e1, const String& e2) const {
	assert(this->Esta(e1) && this->Esta(e2));
	assert(this->Conectadas(e1, e2));

	if (e1 < e2)
		return Restriccion(e2, e1);

	DiccString<RestriccionTP>* diccR = &(this->_restricciones.Obtener(e1));
	return diccR->Obtener(e2);
}

Lista<Estacion>::const_Iterador Mapa::Estaciones() const {
	Lista<Estacion>::const_Iterador it = this->_estaciones.CrearIt();
	return it;
}

Lista<Mapa::Senda>::const_Iterador Mapa::Sendas() const {
	Lista<Mapa::Senda>::const_Iterador it = this->_sendas.CrearIt();
	return it;
}