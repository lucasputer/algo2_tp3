#include "RestriccionTP.h"

using namespace aed2;

RestriccionTP::RestriccionTP(const String& t) : nombre(t), izq(NULL), der(NULL), tam(1) {}

RestriccionTP::RestriccionTP(const String& t, const RestriccionTP* i, const RestriccionTP* d, Nat n) : nombre(t), izq(i), der(d), tam(n) {}

RestriccionTP::RestriccionTP(const RestriccionTP& r) : nombre(r.nombre), izq(NULL), der(NULL), tam(r.tam) {
	if (r.izq)
		this->izq = new RestriccionTP(*(r.izq));
	if (r.der)
		this->der = new RestriccionTP(*(r.der));
}

RestriccionTP::~RestriccionTP() {
	delete this->izq;
	delete this->der;
}

RestriccionTP* RestriccionTP::Negar(const RestriccionTP& r) {
	return new RestriccionTP("NOT", &r, NULL, r.tam+1);
}

RestriccionTP* RestriccionTP::And(const RestriccionTP& r, const RestriccionTP& rOtra) {
	return new RestriccionTP("AND", &r, &rOtra, r.tam+rOtra.tam+1);
}

RestriccionTP* RestriccionTP::Or(const RestriccionTP& r, const RestriccionTP& rOtra) {
	return new RestriccionTP("OR", &r, &rOtra, r.tam+rOtra.tam+1);
}

RestriccionTP* RestriccionTP::Copiar(const RestriccionTP& r) {
	return new RestriccionTP(r);
}

bool RestriccionTP::Verifica(const ConjString& ts) const {
	if (this->nombre == "NOT")
		return !(this->izq->Verifica(ts));
	else if (this->nombre == "AND")
		return this->izq->Verifica(ts) && this->der->Verifica(ts);
	else if (this->nombre == "OR")
		return this->izq->Verifica(ts) || this->der->Verifica(ts);
	return ts.Pertenece(this->nombre);
}

String RestriccionTP::aString() const {
	return "("+this->aStringParcial()+")";
}

String RestriccionTP::aStringParcial() const {
	if (izq == NULL && der == NULL)
		return nombre;
	else if (der == NULL)
		return "! " + izq->aStringParcial();
	else
		return nombre == "AND" ? izq->aStringParcial() + " & " + der->aStringParcial() : izq->aStringParcial() + " | " + der->aStringParcial();
}

std::ostream& operator<<(std::ostream& os, const RestriccionTP& a) {
    return os << a.aString();
}