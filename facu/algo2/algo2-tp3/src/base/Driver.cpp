#include "Driver.h"
#include "ArbolSintactico.h"

using namespace aed2;

Driver::Driver(const Conj<Estacion> &estacionesIniciales) {
   Conj<Estacion>::const_Iterador it = estacionesIniciales.CrearIt();
   Nat cantEst = 0;
   while (it.HaySiguiente()){
		_ciudad.map.Agregar(it.Siguiente());
		_ciudad.robotsEnEstacion.Definir(it.Siguiente(), ColaPrior<Ciudad::Pair>());
       it.Avanzar();
       cantEst++;        
   }
}

Driver::~Driver() {}

Nat Driver::CantidadEstaciones() const
{
	Lista<Estacion>::const_Iterador it = _ciudad.map.Estaciones();
	Nat res = 0;
	while(it.HaySiguiente()){
		res++;
		it.Avanzar();
	}	
	return res;
}

Estacion Driver::IesimaEstacion(Nat i) const 
{
	Lista<Estacion>::const_Iterador it = _ciudad.map.Estaciones();
	Nat j = 0;
	while(it.HaySiguiente() && j<i){
		j++;
		it.Avanzar();
	}	
	return it.Siguiente();
}


Nat Driver::CantidadDeSendasParaEstacion(const Estacion &e) const
{
 Lista<Mapa::Senda>::const_Iterador it = _ciudad.map.Sendas();
	Nat cantidad = 0;
	while (it.HaySiguiente()){
	   if (e == it.Siguiente().estacion1 || e == it.Siguiente().estacion2){
	       cantidad++;
	   }
	   it.Avanzar();
	}
	return cantidad;
}

Estacion Driver::IesimaEstacionDeSenda(const Estacion &e, Nat i) const
{
 Lista<Mapa::Senda>::const_Iterador it = _ciudad.map.Sendas();
   Nat k = 0;
   while(it.HaySiguiente() && k <= i){
       const Mapa::Senda* s = &(it.Siguiente()); //MIRAAAAR
       if(s->estacion1 == e || s->estacion2 == e){
       	k++;
       }
	   if (k <= i) {
		it.Avanzar();
	   }
   }
   return it.Siguiente().estacion1 == e ? it.Siguiente().estacion2 : it.Siguiente().estacion1;
}

Restriccion Driver::IesimaRestriccionDeSenda(const Estacion &e1, Nat i) const
{
 Estacion e2 = IesimaEstacionDeSenda(e1, i);
   return _ciudad.map.Restriccion(e1,e2).aString();
}

RestriccionTP* arbolARest(ArbolSintactico* as) {
   RestriccionTP* restr = NULL;
   if (as != NULL) {
       if (as->raiz == "!") {
           restr = RestriccionTP::Negar(*arbolARest(as->izq));
       } else if (as->raiz == "|") {
           restr = RestriccionTP::Or(*arbolARest(as->izq), *arbolARest(as->der));
       } else if (as->raiz == "&") {
           restr = RestriccionTP::And(*arbolARest(as->izq), *arbolARest(as->der));
       } else {
           restr = new RestriccionTP(as->raiz);
       }
   }
   return restr;
}

void Driver::AgregarSenda(const Estacion &e1, const Estacion &e2, Restriccion r)
{
    ArbolSintactico* expr = ArbolSintactico::LeerDeString(r);
	RestriccionTP* restr = arbolARest(expr);
	_ciudad.map.Conectar(e1, e2, *restr);
	delete restr;
    delete expr;
}

Nat Driver::CantidadRobotsActivos() const
{
Nat cantidad = 0;
   Ciudad::const_ItVectorNat vit = _ciudad.Robots();
   while(vit.HayMas()){
       cantidad++;
       vit.Proximo();
   }
   return cantidad;
}

RUR Driver::IesimoRobotActivo(Nat i) const
{
	bool encontrado = false;
   Nat counter = 0;
   RUR last = 0;
   Ciudad::const_ItVectorNat vit = _ciudad.Robots();
   while(!encontrado){
       last = vit.Proximo();
	   if (counter == i)
		   encontrado = true;
		else
			counter++;
   }
   return last;   
}

Estacion Driver::EstacionActualIesimoRobotActivo(Nat i) const
{
   RUR iesimoRurActivo = IesimoRobotActivo(i);
   return _ciudad.Estacion_(iesimoRurActivo);
}

Conj<Caracteristica> Driver::CaracteristicasIesimoRobotActivo(Nat i) const
{
   RUR iesimoRurActivo = IesimoRobotActivo(i);
   return caracteristicasRobots[iesimoRurActivo];
}

Nat Driver::CantInfraccionesIesimoRobotActivo(Nat i) const
{
   RUR iesimoRurActivo = IesimoRobotActivo(i);
	return _ciudad.Infracciones(iesimoRurActivo);
}

RUR Driver::ElMasInfractor() const
{
  Ciudad::const_ItVectorNat it = _ciudad.Robots();
   Nat elMasInfractor = it.Proximo();
   while(it.HayMas()){
       Nat infractor1 = it.Proximo();
       if (_ciudad.Infracciones(infractor1) > _ciudad.Infracciones(elMasInfractor)){
           elMasInfractor = infractor1;
       }
   }
   return elMasInfractor;
}

void Driver::Entrar(const Conj<Caracteristica> &cs, const Estacion &estacionInicial)
{
caracteristicasRobots.AgregarAtras(cs);
   ConjString* diccStr = new ConjString();
   Conj<Caracteristica>::const_Iterador it = cs.CrearIt();
   while(it.HaySiguiente()){
       diccStr->Agregar(it.Siguiente());
       it.Avanzar();
   }
   _ciudad.Entrar(*diccStr, estacionInicial);
   delete diccStr;
}

void Driver::Mover(RUR robot, const Estacion &destino)
{
 _ciudad.Mover(robot, destino);
}

void Driver::Inspeccion(const Estacion &e)
{
   _ciudad.Inspeccion(e);
}

