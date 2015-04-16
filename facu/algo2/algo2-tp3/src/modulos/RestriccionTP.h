#ifndef RestriccionTP_H
#define RestriccionTP_H

#include "aed2.h"
#include "Tipos.h"
#include "conjString.h"

using namespace aed2;

class RestriccionTP
{
private:
	RestriccionTP(const String& t, const RestriccionTP* i, const RestriccionTP* d, Nat n);
	String nombre;
	const RestriccionTP* izq;
	const RestriccionTP* der;
	Nat tam;
	String aStringParcial() const;

public:
	RestriccionTP(const String& t);
	RestriccionTP(const RestriccionTP& r);
	~RestriccionTP();
	static RestriccionTP* Negar(const RestriccionTP& r);
	static RestriccionTP* And(const RestriccionTP& r, const RestriccionTP& rOtra);
	static RestriccionTP* Or(const RestriccionTP& r, const RestriccionTP& rOtra);
	static RestriccionTP* Copiar(const RestriccionTP& r);
	bool Verifica(const ConjString& ts) const;
	String aString() const;
};

#endif //RestriccionTP_H
