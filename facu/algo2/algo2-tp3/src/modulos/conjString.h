#ifndef CONJSTRING_H
#define CONJSTRING_H

#include <iostream>
#include <assert.h>
#include "aed2.h"
#include "diccString.h"
using namespace aed2;


class ConjString {

	public:
		ConjString();
		~ConjString();
		void Agregar(const String&);
		void Borrar(const String&);
		bool Pertenece(const String&) const;

	private:
		DiccString<bool> container;

};

#endif