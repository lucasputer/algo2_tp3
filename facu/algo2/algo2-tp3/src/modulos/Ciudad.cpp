#include "Ciudad.h"

using namespace aed2;

Ciudad::Ciudad() : cantRobots(0) {}
	
Ciudad::Ciudad( Mapa& m) :map(m) {
	cantRobots = 0;
	Lista<String>::const_Iterador iter = m.Estaciones();
	while(iter.HaySiguiente()){
		robotsEnEstacion.Definir(iter.Siguiente(), ColaPrior<Pair>());
		iter.Avanzar();
	}

}

Ciudad::~Ciudad(){}

void Ciudad::Entrar(const ConjString& cs, const String& e){
	int rurActual = cantRobots;
	cantRobots++;
	cantInfracciones.AgregarAtras(0);
	estaciones.AgregarAtras(e);
	tags.AgregarAtras(cs);
	vivos.AgregarAtras(true);
	DiccString< DiccString<bool> > estacionesPermitidas;
	Lista<Mapa::Senda>::const_Iterador iteradorSendas = map.Sendas();
	while(iteradorSendas.HaySiguiente()){
		String estacion1 = iteradorSendas.Siguiente().estacion1;
		String estacion2 = iteradorSendas.Siguiente().estacion2;
		if(!estacionesPermitidas.Definido(estacion1)){
			estacionesPermitidas.Definir(estacion1,DiccString<bool>());
		}
		RestriccionTP* restr = &(map.Restriccion(estacion1, estacion2));
		bool result = restr->Verifica(cs);
		estacionesPermitidas.Obtener(estacion1).Definir(estacion2,result);
		iteradorSendas.Avanzar();
	}
	sendasPermitidas.AgregarAtras(estacionesPermitidas);
	ColaPrior<Pair>* heapEstacion  = &(robotsEnEstacion.Obtener(e));
	Pair* p = new Pair();
	p->infracciones = 0;
	p->rur = rurActual;
	ColaPrior<Pair>::Iterador itColaPrior = heapEstacion->Encolar(*p);
	delete p;
	sacarRobotDeEstacion.AgregarAtras(itColaPrior);
}

void Ciudad::Mover(const Nat& u, const String& e){
	String estacionAnterior = estaciones[u];
	String min, max;
	if (estacionAnterior < e){
		min = estacionAnterior;
		max = e;
	}else{
		min = e;
		max = estacionAnterior;
	}
	sacarRobotDeEstacion[u].Eliminar();
	bool result = sendasPermitidas[u].Obtener(max).Obtener(min);
	if (!result){
		cantInfracciones[u]++;
	}
	ColaPrior<Pair>* heapEstacion = &(robotsEnEstacion.Obtener(e)); //ESTO NO DEBERIA SER UN PUNTERO? SI
	Pair p;
	p.infracciones = cantInfracciones[u];
	p.rur = u;
	sacarRobotDeEstacion[u] = heapEstacion->Encolar(p);
	estaciones[u] = e;
}

void Ciudad::Inspeccion(const String& e){
	ColaPrior<Pair>* heapEstacion = &(robotsEnEstacion.Obtener(e));
	Pair infraccionesRur = heapEstacion->Maximo();
	if (infraccionesRur.infracciones > 0) {
		heapEstacion->Desencolar();
		vivos[infraccionesRur.rur] = false;
	}
}


Ciudad::const_ItVectorNat Ciudad::Robots() const{
	return Ciudad::const_ItVectorNat(&(vivos));
}

Lista<Estacion>::const_Iterador Ciudad::Estaciones() const {
	return map.Estaciones();
}

Nat Ciudad::Infracciones(const Nat& n) const{
	return cantInfracciones[n];
}

String Ciudad::Estacion_(const Nat& n) const{
	return estaciones[n];
}

ConjString Ciudad::Tags(const Nat& n) const{
	return tags[n];
}


//Implementacion iterador
Ciudad::const_ItVectorNat::const_ItVectorNat(const Vector<bool>* v) : elementos_(v), actual_(0) {}

Ciudad::const_ItVectorNat::const_ItVectorNat(const Ciudad::const_ItVectorNat& otro)
  : elementos_(otro.elementos_), actual_(otro.actual_)
{}

Ciudad::const_ItVectorNat::const_ItVectorNat()
  : elementos_(NULL), actual_(0)
{}

bool Ciudad::const_ItVectorNat::HayMas() const
{
  bool res = false;
  Nat i = actual_;

  while(i < elementos_->Longitud() && !res){
  	bool estaVivo = (*elementos_)[i];
  	if(estaVivo){
  		res = true;
  	}
  	i++;
  }
  return res;
}

Nat Ciudad::const_ItVectorNat::Proximo()
{
 bool encontreProximo = false;
 Nat i = actual_;
 while(i < elementos_->Longitud() && !encontreProximo){
 	bool estaVivo = (*elementos_)[i];
  	if(estaVivo){
 		actual_ = i + 1;
 		encontreProximo = true;
 	}else{
 		i++;
 	}
 }
 return i;
}