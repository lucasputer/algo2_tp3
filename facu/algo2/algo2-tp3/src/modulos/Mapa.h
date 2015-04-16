#ifndef MAPA_H
#define MAPA_H

#include "aed2.h"
#include "Tipos.h"
#include "diccString.h"
#include "RestriccionTP.h"

using namespace aed2;

class Mapa
{
public:
	Mapa();
	~Mapa();
	
	struct Senda {
		Estacion estacion1;
		Estacion estacion2;
		Senda(String e1, String e2) : estacion1(e1), estacion2(e2) {}
	};

// Declaración de funciones.

	void Agregar(const String& e); // Funcion iAgregar
	void Conectar(const String& e1, const String& e2, const RestriccionTP& r); // Funcion iConectar
	bool Esta(const String& e) const; // Funcion iEsta?
	bool Conectadas(const String& e1, const String& e2) const; // Funcion iConectadas?
	RestriccionTP& Restriccion(const String& e1, const String& e2) const; // Funcion iRestriccion, devuelve la restriccion entre estaciones
	Lista<Estacion>::const_Iterador Estaciones() const; // Funcion iEstaciones, devuelve iterador de Lista de las estaciones
	Lista<Senda>::const_Iterador Sendas() const ; // Funcion iSendas, devuelve iterador de Lista de las sendas

private:
		
	Lista<Estacion> _estaciones;
	Lista<Senda> _sendas;
	DiccString<DiccString <RestriccionTP> > _restricciones;

};

#endif // MAPA_H
