#ifndef DICCSTRING_H
#define DICCSTRING_H

#include <iostream>
#include <assert.h>
#include "aed2.h"
using namespace aed2;


template<typename T>
class DiccString {

	public:
		DiccString();
		DiccString(const DiccString&);
		~DiccString();
		void Definir(const String&, const T&);
		bool Definido(const String&) const;
		T& Obtener(const String&) const;

	private:
		struct Nodo {
			Nodo();
			Nodo(const Nodo&);
			~Nodo();
	    	T* significado;
			Arreglo< Nodo* > caracteres;
		};

		Nodo* raiz;
};

template<class T>
DiccString<T>::Nodo::Nodo() : significado(NULL){
	caracteres = Arreglo< Nodo* >(256);
	for(Nat i = 0; i < caracteres.Tamanho(); i++) {
		caracteres.Definir(i, NULL);
	}
}

template<class T>
DiccString<T>::Nodo::Nodo(const Nodo& other) : significado(NULL){
	caracteres = Arreglo< Nodo* >(256);
	if (other.significado != NULL)
		this->significado = new T(*(other.significado));
	for(Nat i = 0; i < caracteres.Tamanho(); i++) {
		if (other.caracteres[i] != NULL){
			Nodo* temp = new Nodo(*(other.caracteres[i]));
			caracteres.Definir(i, temp);
		} else {
			caracteres.Definir(i, NULL);
		}
	}
}

template<class T>
DiccString<T>::Nodo::~Nodo() {
	if(significado != NULL){
		delete significado;
	}
	for(Nat i = 0; i < caracteres.Tamanho(); i++) {
		delete caracteres[i];	
		caracteres.Borrar(i);
	}
}


template<class T>
DiccString<T>::DiccString(){
	this->raiz = new Nodo();
}

template<class T>
DiccString<T>::DiccString(const DiccString& other){
	this->raiz = new Nodo(*(other.raiz));
}

template<class T>
DiccString<T>::~DiccString(){
	delete raiz;
}


template<class T>
void DiccString<T>::Definir(const String& c, const T& value){
	Nat i = 0;
	Nodo* actual = raiz;
	while(i < c.length()){
		if(actual->caracteres[(Nat)c[i]] == NULL){
			Nodo* newNode = new Nodo();
			actual->caracteres.Definir((Nat)c[i],newNode);
		}
		actual = actual->caracteres[(Nat)c[i]];
		i++;
	}
	if(actual->significado != NULL){
		delete actual->significado;
	}
	actual->significado = new T(value);
}


template<class T>
bool DiccString<T>::Definido(const String& c) const{
	Nat i = 0;
	bool esta = true;
	Nodo* actual = raiz;
	while(i < c.length() && esta){
		if (actual->caracteres[(Nat)c[i]] == NULL){
			esta = false;
		}else{
			actual = (actual->caracteres[(Nat)c[i]]);
		}
		i++;
	}
	return (esta && !(actual->significado == NULL));
}


template<class T>
T& DiccString<T>::Obtener(const String& c) const{
	Nodo* actual = raiz;
	Nat i = 0;
	while(i < c.length()){
		actual = actual->caracteres[(Nat)c[i]];
		i++;
	}
	return *(actual->significado);
}


#endif //DICCSTRING_H
