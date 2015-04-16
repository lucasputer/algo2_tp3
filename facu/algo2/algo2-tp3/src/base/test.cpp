#include "mini_test.h"

#include "Driver.h"
#include "ArbolSintactico.h"

#include <string>
#include <iostream>

// valgrind --leak-check=full -v ./test

using namespace aed2;

/**
 * Imprime un elemento a un string, en vez de a una pantalla,
 * a través del operador <<
 */
template <typename T>
std::string to_str(const T& t)
{
	std::stringstream ss;
	ss << t;

	return ss.str();
}

/**
 * Esta función se puede utilizar para comparar dos colecciones
 * iterables que representen conjuntos, es decir, que no tengan
 * elementos repetidos.
 */
template<typename T, typename S>
bool Comparar(const T& t, const S& s)
{
  typename T::const_Iterador it1 = t.CrearIt();
  typename S::const_Iterador it2 = s.CrearIt();

	// me fijo si tienen el mismo tamanho

	Nat len1 = 0;
	while( it1.HaySiguiente() ) {
		len1++;
		it1.Avanzar();
	}

	Nat len2 = 0;
	while( it2.HaySiguiente() ) {
		len2++;
		it2.Avanzar();
	}

	if ( len1 != len2 )
		return false;

	it1 = t.CrearIt();
	it2 = s.CrearIt();

	// me fijo que tengan los mismos elementos

	while( it1.HaySiguiente() )
	{
		bool esta = false;
		it2 = s.CrearIt();

		while( it2.HaySiguiente() ) {
		  if ( it1.Siguiente() == it2.Siguiente() ) {
			esta = true;
			break;
		  }
		  it2.Avanzar();
		}

		if ( !esta ) {
			return false;
		}

		it1.Avanzar();
	}

  return true;
}


// ---------------------------------------------------------------------

/**
 * Ejemplo de caso de test, con llamadas a las rutinas de aserción
 * definidas en mini_test.h
 */
void test_ciudad_simple()
{
    Conj<Estacion> estaciones;
    estaciones.Agregar("Belgrano");
    estaciones.Agregar("Retiro");
    estaciones.Agregar("Martinez");

    Driver caba(estaciones);

    caba.AgregarSenda("Belgrano", "Retiro", "(trenDePasajeros | trenDeCarga) & !trenDeLaAlegria");
    caba.AgregarSenda("Martinez", "Retiro", "trenDeLaAlegria");
    caba.AgregarSenda("Martinez", "Belgrano", "trenDePasajeros");

    Conj<Caracteristica> r1, r2, r3;
    r1.Agregar("trenDePasajeros");
    r2.Agregar("trenDeCarga");
    r3.Agregar("trenDeLaAlegria");

    caba.Entrar(r1,"Belgrano"); // RUR 0
    caba.Entrar(r2,"Retiro");   // RUR 1
    caba.Entrar(r3,"Martinez"); // RUR 2

    ASSERT_EQ(caba.CantidadEstaciones(), 3);
    ASSERT_EQ(caba.CantidadRobotsActivos(), 3);
    ASSERT_EQ(caba.CantidadDeSendasParaEstacion("Belgrano"), 2);

    caba.Mover(0,"Retiro");    // RUR 0: 0 infracciones
    caba.Mover(0,"Martinez");  // RUR 0: 1 infracciones

    caba.Mover(1,"Belgrano");  // RUR 1: 0 infracciones
    caba.Mover(1,"Martinez");  // RUR 1: 1 infracciones

    caba.Mover(2,"Belgrano");  // RUR 2: 1 infracciones
    caba.Mover(2,"Retiro");    // RUR 2: 2 infracciones

    Dicc<RUR,Nat> infraccionesRobots;

    // Chequeo infracciones iterando los robots
    for (Nat i = 0;  i< caba.CantidadRobotsActivos(); ++i)
        infraccionesRobots.Definir(caba.IesimoRobotActivo(i),caba.CantInfraccionesIesimoRobotActivo(i));

    ASSERT_EQ(infraccionesRobots.Significado(0), 1);
    ASSERT_EQ(infraccionesRobots.Significado(1), 1);
    ASSERT_EQ(infraccionesRobots.Significado(2), 2);

    ASSERT_EQ(caba.ElMasInfractor(),2);

    // Vuela un robot
    caba.Inspeccion("Retiro");
    ASSERT_EQ(caba.CantidadRobotsActivos(),2);

}



void test_diccString(){
	using std::string;
	//LO CREO EN LA HEAP PARA QUE EL DESTRUCTOR DEL SCOPE NO LA DESTRUYA DOS VECES
	string longString = "ESTO ES UN TEXTO MUY LARGO PARA VER SI LA ESTRUCTURA SE BANCA STRING MUYMUYMUYUMUYMYUYMUYYUMYUYM LARGOS: Asereje ja de je de jebe tu de jebere";
	DiccString<string>* dicc = new DiccString<string>();
	dicc->Definir("mickey", "minie");
	ASSERT(!dicc->Definido("donald"));
	ASSERT(dicc->Definido("mickey"));
	dicc->Definir("Donald", "Pluto");
	ASSERT(dicc->Definido("Donald"));
	dicc->Definir("Peron", "Evita");
	ASSERT(dicc->Definido("Peron"));
	ASSERT(!(dicc->Definido("peron"))); //caseSensitive
	ASSERT_EQ(dicc->Obtener("Peron"), "Evita");
	ASSERT_EQ(dicc->Obtener("mickey"), "minie");
	dicc->Definir("mickey", "carrio");
	ASSERT(!(dicc->Obtener("mickey") == "minie"));
	ASSERT_EQ(dicc->Obtener("mickey"), "carrio");
	dicc->Definir(longString, ".");
	ASSERT_EQ(dicc->Obtener(longString), ".");
	dicc->Definir(longString, "-");
	ASSERT_EQ(dicc->Obtener(longString), "-");

	DiccString< DiccString<string> > diccDeDiccs; //ESTE ES IMPORTANTE

	diccDeDiccs.Definir("clave", *dicc);
	ASSERT_EQ(diccDeDiccs.Obtener("clave").Obtener("mickey"), "carrio");
	diccDeDiccs.Obtener("clave").Definir("giglioti", "volvete a colon");
	//ASSERT(dicc->Definido("giglioti")); // el valor se copia
	ASSERT(diccDeDiccs.Obtener("clave").Definido("giglioti"));

	DiccString<string>* otro = new DiccString<string>();
	diccDeDiccs.Definir("clave2", *otro);

	diccDeDiccs.Obtener("clave").Definir("arroz", "con leche");
	//ASSERT_EQ(dicc->Obtener("arroz"), "con leche"); // el valor se copia
	ASSERT(!(diccDeDiccs.Obtener("clave2").Definido("ASD")));
	delete dicc;	
	delete otro;
}


void test_conjString(){
	ConjString conj;
	conj.Agregar("HOLA");
	ASSERT(conj.Pertenece("HOLA"));
	conj.Borrar("HOLA");
	ASSERT(!conj.Pertenece("HOLA"));
	conj.Agregar("HOLA");
	ASSERT(conj.Pertenece("HOLA"));
	String larga = "SGSDFGSDFGSDFGDFGDFHDFHFDGHFDGHDFGHFDGHBVCBNNRRGFFGDFGDF DFGDFGDFG";
	conj.Agregar(larga);
	ASSERT(conj.Pertenece(larga));
	conj.Borrar(larga);
	ASSERT(!conj.Pertenece(larga));
	conj.Agregar(larga);
	ASSERT(conj.Pertenece(larga));
}




void restriccion_crear() {
	RestriccionTP r = RestriccionTP("hola");
	ConjString ts = ConjString();
	ts.Agregar("hola");
	ASSERT_EQ(r.Verifica(ts), true);
}

void restriccion_not() {
	RestriccionTP* a = new RestriccionTP("hola");
	RestriccionTP* b = RestriccionTP::Negar(*a);
	ConjString ts = ConjString();
	ts.Agregar("hola");
	ASSERT_EQ(b->Verifica(ts), false);
	RestriccionTP* c = RestriccionTP::Negar(*b);
	ASSERT_EQ(c->Verifica(ts), true);
	delete c;
}

void restriccion_and() {
	RestriccionTP* a = new RestriccionTP("uno");
	RestriccionTP* b = new RestriccionTP("dos");
	RestriccionTP* c = new RestriccionTP("tres");
	RestriccionTP* d = new RestriccionTP("cuatro");
	ConjString ts = ConjString();
	ts.Agregar("uno");
	ts.Agregar("dos");
	ts.Agregar("tres");
	RestriccionTP* e = RestriccionTP::And(*a, *b);
	ASSERT_EQ(e->Verifica(ts), true);
	RestriccionTP* f = RestriccionTP::And(*e, *c);
	ASSERT_EQ(f->Verifica(ts), true);
	RestriccionTP* g = RestriccionTP::And(*f, *d);
	ASSERT_EQ(g->Verifica(ts), false);
	delete g;
}

void restriccion_or() {
	RestriccionTP* a = new RestriccionTP("uno");
	RestriccionTP* b = new RestriccionTP("dos");
	RestriccionTP* c = new RestriccionTP("tres");
	RestriccionTP* d = new RestriccionTP("cuatro");
	ConjString tsa = ConjString();
	tsa.Agregar("uno");
	ConjString tsb = ConjString();
	tsb.Agregar("dos");
	ConjString tsc = ConjString();
	tsc.Agregar("uno");
	tsc.Agregar("dos");
	RestriccionTP* e = RestriccionTP::Or(*a, *c);
	ASSERT_EQ(e->Verifica(tsa), true);
	ASSERT_EQ(e->Verifica(tsb), false);
	ASSERT_EQ(e->Verifica(tsc), true);
	RestriccionTP* f = RestriccionTP::Or(*b, *d);
	ASSERT_EQ(f->Verifica(tsa), false);
	ASSERT_EQ(f->Verifica(tsb), true);
	ASSERT_EQ(f->Verifica(tsc), true);
	delete e;
	delete f;
}

void restriccion_copiar_simple() {
	RestriccionTP* a = new RestriccionTP("uno");
	RestriccionTP* b = new RestriccionTP(*a);
	ConjString tsa = ConjString();
	tsa.Agregar("uno");
	ASSERT_EQ(a->Verifica(tsa), b->Verifica(tsa));
	delete a;
	delete b;	
}

RestriccionTP* arbolARestriccion(ArbolSintactico* as) {
	RestriccionTP* restr = NULL;
	if (as != NULL) {
		if (as->raiz == "!") {
			restr = RestriccionTP::Negar(*arbolARestriccion(as->izq));
		} else if (as->raiz == "|") {
			restr = RestriccionTP::Or(*arbolARestriccion(as->izq), *arbolARestriccion(as->der));
		} else if (as->raiz == "&") {
			restr = RestriccionTP::And(*arbolARestriccion(as->izq), *arbolARestriccion(as->der));
		} else {
			restr = new RestriccionTP(as->raiz);
		}
	}
	return restr;
}

void restriccion_de_arbol_sintactico() {
	ArbolSintactico* as = ArbolSintactico::LeerDeString("(rojo | Amarillo) & !VERDE");
	RestriccionTP* restr = arbolARestriccion(as);
	ConjString tsa = ConjString();
	tsa.Agregar("VERDE");
	ASSERT_EQ(restr->Verifica(tsa), false);
	ConjString tsb = ConjString();
	tsb.Agregar("rojo");
	ASSERT_EQ(restr->Verifica(tsb), true);
	tsb.Agregar("VERDE");
	ASSERT_EQ(restr->Verifica(tsb), false);
	
	RestriccionTP* copia = RestriccionTP::Copiar(*restr);
	ASSERT_EQ(restr->Verifica(tsa), copia->Verifica(tsa));
	ASSERT_EQ(restr->Verifica(tsb), copia->Verifica(tsb));

	delete as;
	delete restr;
	delete copia;
}

void cola_prior_simple(){
	ColaPrior<int> cp = ColaPrior<int>();
	ASSERT_EQ(cp.EsVacia(), true);
	cp.Encolar(93);
}

void cola_prior_encolar_desencolar(){
	ColaPrior<int> cp = ColaPrior<int>();
	ASSERT_EQ(cp.EsVacia(), true);
	cp.Encolar(93);
	cp.Encolar(73);
	cp.Encolar(83);
	cp.Encolar(7893);
	ASSERT_EQ(cp.Maximo(), 7893);
	cp.Desencolar();
	ASSERT_EQ(cp.EsVacia(), false);
	ASSERT_EQ(cp.Maximo(), 93);
	cp.Desencolar();
	ASSERT_EQ(cp.EsVacia(), false);
	cp.Encolar(53);
	cp.Encolar(33);
	cp.Encolar(3);
	ASSERT_EQ(cp.Maximo(), 83);
	cp.Desencolar();
	ASSERT_EQ(cp.Maximo(), 73);
	cp.Desencolar();
	ASSERT_EQ(cp.EsVacia(), false);
	ASSERT_EQ(cp.Maximo(), 53);
	cp.Desencolar();
	ASSERT_EQ(cp.Maximo(), 33);
	cp.Desencolar();
	ASSERT_EQ(cp.Maximo(), 3);
	cp.Desencolar();
	ASSERT_EQ(cp.EsVacia(), true);
}

void cola_prior_iterador(){
	ColaPrior<int> cp = ColaPrior<int>();
	ASSERT_EQ(cp.EsVacia(), true);

	ColaPrior<int>::Iterador it1 = cp.Encolar(93);
	ASSERT_EQ(cp.EsVacia(), false);
	ASSERT_EQ(it1.HayMas(), true);
	ASSERT_EQ(it1.Actual(), 93);

	ColaPrior<int>::Iterador it2 = cp.Encolar(73);
	ASSERT_EQ(cp.EsVacia(), false);
	ASSERT_EQ(it2.HayMas(), true);
	ASSERT_EQ(it2.Actual(), 73);

	it1.Eliminar();
	ASSERT_EQ(cp.EsVacia(), false);
	ASSERT_EQ(it1.HayMas(), false);

	ASSERT_EQ(cp.Maximo(), 73);
	cp.Desencolar();
	ASSERT_EQ(cp.EsVacia(), true);

	cp.Encolar(94);
	cp.Encolar(55);
	ColaPrior<int>::Iterador it3 = cp.CrearIt();
	ColaPrior<int>::const_Iterador it4 = cp.CrearIt();
	ASSERT_EQ(it3.HayMas(), false);
	ASSERT_EQ(it4.HayMas(), false);
}

void mapa_agregar(){
	Mapa map = Mapa();
	ASSERT_EQ(map.Esta("bsas"), false);
	map.Agregar("bsas");
	ASSERT_EQ(map.Esta("bsas"), true);
}

void mapa_conectar(){
	Mapa map = Mapa();
	ArbolSintactico* as1 = ArbolSintactico::LeerDeString("(rojo | Amarillo) & !VERDE");
	ArbolSintactico* as2 = ArbolSintactico::LeerDeString("(rojo & Amarillo) | VERDE");
	RestriccionTP* r1 = arbolARestriccion(as1);
	RestriccionTP* r2 = arbolARestriccion(as2);

	ASSERT_EQ(map.Esta("bsas"), false);
	map.Agregar("bsas");
	map.Agregar("honkon");
	map.Agregar("chisinau");

	map.Conectar("honkon", "bsas", *r1);
	ASSERT_EQ(map.Esta("bsas"), true);
	ASSERT_EQ(map.Esta("honkon"), true);
	ASSERT_EQ(map.Esta("chisinau"), true);
	ASSERT_EQ(map.Conectadas("bsas", "honkon") , true);
	ASSERT_EQ(map.Conectadas("honkon", "bsas") , true);
	map.Conectar("bsas", "chisinau", *r2);
	ASSERT_EQ(map.Conectadas("bsas", "chisinau") , true);
	ASSERT_EQ(map.Conectadas("bsas", "honkon") , true);
	ASSERT_EQ(map.Conectadas("chisinau", "honkon") , false);
	
	delete as1;
	delete as2;
	delete r1;
	delete r2;
}

void mapa_restriccion(){
	Mapa map = Mapa();
	ArbolSintactico* as1 = ArbolSintactico::LeerDeString("(rojo | Amarillo) & !VERDE");
	ArbolSintactico* as2 = ArbolSintactico::LeerDeString("(rojo & Amarillo) | VERDE");
	RestriccionTP* r1 = arbolARestriccion(as1);
	RestriccionTP* r2 = arbolARestriccion(as2);

	ASSERT_EQ(map.Esta("bsas"), false);
	map.Agregar("bsas");
	map.Agregar("honkon");
	map.Agregar("chisinau");

	map.Conectar("honkon", "bsas", *r1);
	ASSERT_EQ(map.Esta("bsas"), true);
	ASSERT_EQ(map.Esta("honkon"), true);
	ASSERT_EQ(map.Esta("chisinau"), true);
	ASSERT_EQ(map.Conectadas("bsas", "honkon") , true);
	ASSERT_EQ(map.Conectadas("honkon", "bsas") , true);
	map.Conectar("bsas", "chisinau", *r2);
	ASSERT_EQ(map.Conectadas("bsas", "chisinau") , true);
	ASSERT_EQ(map.Conectadas("bsas", "honkon") , true);
	ASSERT_EQ(map.Conectadas("chisinau", "honkon") , false);

	RestriccionTP* vuelta1 = &(map.Restriccion("honkon", "bsas"));
	RestriccionTP* vuelta2 = &(map.Restriccion("chisinau", "bsas"));
	ConjString cs = ConjString();
	cs.Agregar("rojo");
	ASSERT_EQ(vuelta1->Verifica(cs) , true);
	ASSERT_EQ(vuelta2->Verifica(cs) , false);
	cs.Borrar("rojo");
	ASSERT_EQ(vuelta1->Verifica(cs) , false);
	ASSERT_EQ(vuelta2->Verifica(cs) , false);
	cs.Agregar("VERDE");
	ASSERT_EQ(vuelta1->Verifica(cs) , false);
	ASSERT_EQ(vuelta2->Verifica(cs) , true);

	delete as1;
	delete as2;
	delete r1;
	delete r2;
}

void test_ciudad_capo(){
	Mapa* map = new Mapa();
	ArbolSintactico* as1 = ArbolSintactico::LeerDeString("(rojo | Amarillo) & !VERDE");
	ArbolSintactico* as2 = ArbolSintactico::LeerDeString("(rojo & Amarillo) | VERDE");
	RestriccionTP* r1 = arbolARestriccion(as1);
	RestriccionTP* r2 = arbolARestriccion(as2);

	ASSERT_EQ(map->Esta("bsas"), false);
	map->Agregar("bsas");
	map->Agregar("honkon");
	map->Agregar("chisinau");

	map->Conectar("honkon", "bsas", *r1);
	ASSERT_EQ(map->Esta("bsas"), true);
	ASSERT_EQ(map->Esta("honkon"), true);
	ASSERT_EQ(map->Esta("chisinau"), true);
	ASSERT_EQ(map->Conectadas("bsas", "honkon") , true);
	ASSERT_EQ(map->Conectadas("honkon", "bsas") , true);
	map->Conectar("bsas", "chisinau", *r2);
	ASSERT_EQ(map->Conectadas("bsas", "chisinau") , true);
	ASSERT_EQ(map->Conectadas("bsas", "honkon") , true);
	ASSERT_EQ(map->Conectadas("chisinau", "honkon") , false);

	Ciudad city = Ciudad(*map);

	delete map;
	delete as1;
	delete as2;
	delete r1;
	delete r2;
}


void test_ciudad(){
	String rest1 = "(X & Y)";
	String rest2 = "(X | Y)";
	String rest3 = "X";

	ArbolSintactico* a1 = ArbolSintactico::LeerDeString(rest1);
	ArbolSintactico* a2 = ArbolSintactico::LeerDeString(rest2);
	ArbolSintactico* a3 = ArbolSintactico::LeerDeString(rest3);

	RestriccionTP* r1 = arbolARestriccion(a1);
	RestriccionTP* r2 = arbolARestriccion(a2);
	RestriccionTP* r3 = arbolARestriccion(a3);


	Mapa* m = new Mapa();
	m->Agregar("A");
	m->Agregar("B");
	m->Agregar("C");
	m->Agregar("D");

	m->Conectar("A", "B", *r1);
	m->Conectar("B", "C", *r2);
	m->Conectar("C", "D", *r3);

	ConjString* c1 = new ConjString();
	ConjString* c2 = new ConjString();
	ConjString* c3 = new ConjString();

	c1->Agregar("X");
	c1->Agregar("Y");

	c2->Agregar("Y");
	c3->Agregar("X");

	Ciudad caba(*m);
	caba.Entrar(*c1, "A"); // RUR 0
	caba.Entrar(*c2, "B");   // RUR 1
	caba.Entrar(*c3, "C"); // RUR 2

	ASSERT_EQ(caba.Estacion_(0), "A");
	ASSERT_EQ(caba.Estacion_(1), "B");
	ASSERT_EQ(caba.Estacion_(2), "C");

	ASSERT_EQ(caba.Infracciones(0), 0);
	ASSERT_EQ(caba.Infracciones(1), 0);
	ASSERT_EQ(caba.Infracciones(2), 0);

	ASSERT(caba.Tags(0).Pertenece("X"));
	ASSERT(caba.Tags(0).Pertenece("Y"));
	ASSERT(caba.Tags(1).Pertenece("Y"));
	ASSERT(caba.Tags(2).Pertenece("X"));

	Ciudad::const_ItVectorNat it = caba.Robots();

	ASSERT(it.HayMas());
	ASSERT_EQ(it.Proximo(), 0);
	ASSERT_EQ(it.Proximo(), 1);
	ASSERT_EQ(it.Proximo(), 2);
	ASSERT(!(it.HayMas()));


	caba.Mover(0, "B");

	ASSERT_EQ(caba.Estacion_(0), "B");
	ASSERT_EQ(caba.Estacion_(1), "B");
	ASSERT_EQ(caba.Estacion_(2), "C");

	ASSERT_EQ(caba.Infracciones(0), 0);
	ASSERT_EQ(caba.Infracciones(1), 0);
	ASSERT_EQ(caba.Infracciones(2), 0);

	caba.Mover(1, "A");

	ASSERT_EQ(caba.Estacion_(0), "B");
	ASSERT_EQ(caba.Estacion_(1), "A");
	ASSERT_EQ(caba.Estacion_(2), "C");

	ASSERT_EQ(caba.Infracciones(0), 0);
	ASSERT_EQ(caba.Infracciones(1), 1);
	ASSERT_EQ(caba.Infracciones(2), 0);

	caba.Mover(1, "B");

	ASSERT_EQ(caba.Estacion_(0), "B");
	ASSERT_EQ(caba.Estacion_(1), "B");
	ASSERT_EQ(caba.Estacion_(2), "C");

	ASSERT_EQ(caba.Infracciones(0), 0);
	ASSERT_EQ(caba.Infracciones(1), 2);
	ASSERT_EQ(caba.Infracciones(2), 0);

	it = caba.Robots();
	ASSERT(it.HayMas());
	ASSERT_EQ(it.Proximo(), 0);
	ASSERT_EQ(it.Proximo(), 1);
	ASSERT_EQ(it.Proximo(), 2);
	ASSERT(!(it.HayMas()));

	caba.Mover(2, "B");

	ASSERT_EQ(caba.Estacion_(0), "B");
	ASSERT_EQ(caba.Estacion_(1), "B");
	ASSERT_EQ(caba.Estacion_(2), "B");

	ASSERT_EQ(caba.Infracciones(0), 0);
	ASSERT_EQ(caba.Infracciones(1), 2);
	ASSERT_EQ(caba.Infracciones(2), 0);

	caba.Mover(2, "A");

	ASSERT_EQ(caba.Estacion_(0), "B");
	ASSERT_EQ(caba.Infracciones(2), 1);
	ASSERT_EQ(caba.Estacion_(1), "B");
	ASSERT_EQ(caba.Estacion_(2), "A");

	ASSERT_EQ(caba.Infracciones(0), 0);
	ASSERT_EQ(caba.Infracciones(1), 2);
	ASSERT_EQ(caba.Infracciones(2), 1);

	caba.Mover(2, "B");

	ASSERT_EQ(caba.Estacion_(0), "B");
	ASSERT_EQ(caba.Estacion_(1), "B");
	ASSERT_EQ(caba.Estacion_(2), "B");

	ASSERT_EQ(caba.Infracciones(0), 0);
	ASSERT_EQ(caba.Infracciones(1), 2);
	ASSERT_EQ(caba.Infracciones(2), 2);

	caba.Inspeccion("B");
	ASSERT_EQ(caba.Estacion_(0), "B");
	ASSERT_EQ(caba.Estacion_(1), "B");

	ASSERT_EQ(caba.Infracciones(0), 0);
	ASSERT_EQ(caba.Infracciones(1), 2);

	delete m;


	delete a1;
	delete a2;
	delete a3;
	delete r1;
	delete r2;
	delete r3;
	delete c1;
	delete c2;
	delete c3;
	//delete m; "ya lo borra ciudad"
};

void test_driver_completo() {
	Conj<Estacion> estaciones;
	estaciones.Agregar("Belgrano"); 
	estaciones.Agregar("Retiro");
	estaciones.Agregar("Martinez");
	estaciones.Agregar("Relejos");
	estaciones.Agregar("Laboca");

	Driver caba(estaciones);

	caba.AgregarSenda("Belgrano", "Retiro", "(trenDePasajeros | trenDeCarga) & !trenDeLaAlegria");
	caba.AgregarSenda("Martinez", "Retiro", "trenDeLaAlegria");
	caba.AgregarSenda("Martinez", "Belgrano", "trenDePasajeros");
	caba.AgregarSenda("Relejos", "Belgrano","trenDeCarga & !trenDePasajeros" );
	caba.AgregarSenda("Retiro", "Laboca", "trenDeAutos | trenDePasajeros");
	caba.AgregarSenda("Laboca", "Martinez","trenDeAutos & !trenDeLaAlegria");

	Conj<Caracteristica> r1, r2, r3, r4;
	r1.Agregar("trenDePasajeros"); 
	r2.Agregar("trenDeCarga"); 
	r3.Agregar("trenDeLaAlegria"); 
	r4.Agregar("trenDeAutos");
	
	caba.Entrar(r1,"Belgrano"); // RUR 0 
	caba.Entrar(r2,"Retiro"); // RUR 1 
	caba.Entrar(r3,"Martinez"); // RUR 2 
	caba.Entrar(r4,"Laboca"); // RUR 3

	ASSERT_EQ(caba.IesimoRobotActivo(2),2);
	ASSERT_EQ(caba.EstacionActualIesimoRobotActivo(2),"Martinez");
	ASSERT_EQ(caba.CantidadEstaciones(), 5);
	ASSERT_EQ(caba.CaracteristicasIesimoRobotActivo(1) == r2, true);
	ASSERT_EQ(caba.CantidadRobotsActivos(), 4);
	ASSERT_EQ(caba.CantInfraccionesIesimoRobotActivo(3), 0);
	ASSERT_EQ(caba.CantidadDeSendasParaEstacion("Belgrano"), 3);
	ASSERT_EQ(caba.IesimaEstacion(4), "Laboca");
	ASSERT_EQ(caba.IesimaEstacionDeSenda("Relejos", 0), "Belgrano"); //revisar

	caba.Mover(0,"Relejos"); // RUR 0: 1 infracciones 
	caba.Mover(0,"Belgrano"); // RUR 0: 2 infracciones 
	caba.Mover(0,"Retiro"); // RUR 0: 2 infracciones 
	caba.Mover(0,"Martinez"); // RUR 0: 3 infracciones

	caba.Mover(1,"Belgrano"); // RUR 1: 0 infracciones 
	caba.Mover(1,"Martinez"); // RUR 1: 1 infracciones

	caba.Mover(2,"Belgrano"); // RUR 2: 1 infracciones 
	caba.Mover(2,"Retiro"); // RUR 2: 2 infracciones 
	caba.Mover(2,"Belgrano"); // RUR 2: 3 infracciones

	caba.Mover(3,"Martinez"); // RUR 3: 0 infracciones 
	caba.Mover(3,"Belgrano"); // RUR 3: 1 infracciones 
	caba.Mover(3,"Retiro"); // RUR 3: 2 infracciones

	Dicc<RUR,Nat> infraccionesRobots;

	// Chequeo infracciones iterando los robots 
	for (Nat i = 0; i< caba.CantidadRobotsActivos(); ++i) 
		infraccionesRobots.Definir(caba.IesimoRobotActivo(i),caba.CantInfraccionesIesimoRobotActivo(i));

	ASSERT_EQ(infraccionesRobots.Significado(0), 3);
	ASSERT_EQ(infraccionesRobots.Significado(1), 1);
	ASSERT_EQ(infraccionesRobots.Significado(2), 3);
	ASSERT_EQ(infraccionesRobots.Significado(3), 2);

	ASSERT_EQ(caba.ElMasInfractor(),0);

	// Sacamos un robot 
	caba.Inspeccion("Martinez");
	// inspeccionamos una estacion que no tiene mas robots
	caba.Inspeccion("Martinez");

	ASSERT_EQ(caba.ElMasInfractor(),2);
	ASSERT_EQ(caba.CantidadRobotsActivos(), 2);
	ASSERT_EQ(caba.IesimoRobotActivo(0),2);
	ASSERT_EQ(caba.EstacionActualIesimoRobotActivo(0),"Belgrano");
	ASSERT_EQ(caba.CantInfraccionesIesimoRobotActivo(0), 3);

}

void test_driver_hiper_completo() {
	Conj<Estacion> estaciones;
	estaciones.Agregar("Sol"); 
	estaciones.Agregar("Mercurio"); 
	estaciones.Agregar("Venus");
	estaciones.Agregar("Tierra");
	estaciones.Agregar("Luna");
	estaciones.Agregar("Marte");
	estaciones.Agregar("Jupiter");
	estaciones.Agregar("Saturno");
	estaciones.Agregar("Urano");
	estaciones.Agregar("Neptuno");

	Driver sistemaSolar(estaciones);

	sistemaSolar.AgregarSenda("Marte", "Sol", "RecolectorDePlasma");
	sistemaSolar.AgregarSenda("Marte", "Mercurio", "RecolectorDePlasma");
	sistemaSolar.AgregarSenda("Mercurio", "Sol", "RecolectorDePlasma");

	sistemaSolar.AgregarSenda("Tierra", "Marte", "Escavadora | NaveDeCarga | ModuloHabitable | NaveDePasajeros");
	sistemaSolar.AgregarSenda("Tierra", "Luna", "ModuloHabitable & NaveDePasajeros");
	sistemaSolar.AgregarSenda("Marte", "Luna", "ModuloHabitable | NaveDePasajeros");

	sistemaSolar.AgregarSenda("Tierra", "Neptuno", "NaveReactorNuclear & !NaveDePasajeros");
	sistemaSolar.AgregarSenda("Tierra", "Saturno", "NaveReactorNuclear & !NaveDePasajeros");
	sistemaSolar.AgregarSenda("Tierra", "Urano", "NaveReactorNuclear & !NaveDePasajeros");
	sistemaSolar.AgregarSenda("Neptuno", "Urano", "NaveReactorNuclear & !NaveDePasajeros");
	sistemaSolar.AgregarSenda("Saturno", "Urano", "NaveReactorNuclear & !NaveDePasajeros");
	sistemaSolar.AgregarSenda("Saturno", "Neptuno", "NaveReactorNuclear & !NaveDePasajeros");
	sistemaSolar.AgregarSenda("Saturno", "Jupiter", "NaveReactorNuclear & !NaveDePasajeros");
	sistemaSolar.AgregarSenda("Urano", "Venus", "NaveReactorNuclear & !NaveDePasajeros");
	sistemaSolar.AgregarSenda("Neptuno", "Jupiter", "NaveReactorNuclear & !NaveDePasajeros");

	sistemaSolar.AgregarSenda("Marte", "Venus", "NaveDePasajeros");
	sistemaSolar.AgregarSenda("Venus", "Jupiter", "NaveRebelde");
	sistemaSolar.AgregarSenda("Tierra", "Venus", "NaveDeGuerra");
	sistemaSolar.AgregarSenda("Tierra", "Jupiter", "NaveDeGuerra");


	Conj<Caracteristica> r1, r2, r3, r4, r5, r6, r7, r8, r9;
	r1.Agregar("NaveDePasajeros"); 

	r2.Agregar("NaveDePasajeros"); 
	r2.Agregar("ModuloHabitable"); 

	r3.Agregar("Escavadora"); 

	r4.Agregar("NaveDeCarga");
	r4.Agregar("NaveDePasajeros");

	r5.Agregar("NaveRebelde");
	r5.Agregar("NaveDePasajeros");	

	r6.Agregar("NaveRebelde");
	r6.Agregar("NaveDePasajeros");
	r6.Agregar("ModuloHabitable");

	r7.Agregar("RecolectorDePlasma"); 

	r8.Agregar("NaveReactorNuclear");

	r9.Agregar("NaveRebelde");
	r9.Agregar("NaveDeGuerra");
	r9.Agregar("NaveReactorNuclear");

	sistemaSolar.Entrar(r1,"Tierra"); // RUR 0 
	sistemaSolar.Entrar(r2,"Luna"); // RUR 1 
	sistemaSolar.Entrar(r3,"Marte"); // RUR 2 
	sistemaSolar.Entrar(r4,"Marte"); // RUR 3

	ASSERT_EQ(sistemaSolar.IesimoRobotActivo(2),2);
	ASSERT_EQ(sistemaSolar.EstacionActualIesimoRobotActivo(2),"Marte");
	ASSERT_EQ(sistemaSolar.CantidadEstaciones(), 10);
	ASSERT_EQ(sistemaSolar.CaracteristicasIesimoRobotActivo(3) == r4, true);
	ASSERT_EQ(sistemaSolar.CantidadRobotsActivos(), 4);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(0), 0);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(1), 0);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(2), 0);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(3), 0);
	ASSERT_EQ(sistemaSolar.CantidadDeSendasParaEstacion("Venus"), 4);
	ASSERT_EQ(sistemaSolar.CantidadDeSendasParaEstacion("Tierra"), 7);

	// r1 lleva pasajeros a la Luna, pero no es un modulo habitable
	sistemaSolar.Mover(0,"Luna"); // RUR 0: 1 infracciones 
	// r1 lleva pasajeros a Marte
	sistemaSolar.Mover(0,"Marte"); // RUR 0: 0 infracciones 
	// r1 vuelve a la Luna
	sistemaSolar.Mover(0,"Luna"); // RUR 0: 0 infracciones 
	// r1 vuelve a la Tierra
	sistemaSolar.Mover(0,"Tierra"); // RUR 0: 2 infracciones 

	// r2 va a la Tierra a buscar pasajeros
	sistemaSolar.Mover(1,"Tierra"); // RUR 1: 0 infracciones 
	// r2 va a Marte a colonizar
	sistemaSolar.Mover(1,"Marte"); // RUR 1: 0 infracciones

	// inspeccion en Marte !
	sistemaSolar.Inspeccion("Marte");
	// todo en orden por acá:
	ASSERT_EQ(sistemaSolar.CantidadRobotsActivos(), 4);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(0), 2);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(1), 0);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(2), 0);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(3), 0);

	// r3 se mueve a escavar en Venus
	sistemaSolar.Mover(2,"Venus"); // RUR 2: 1 infracciones 

	sistemaSolar.Entrar(r5,"Venus"); // RUR 4
	// r5 se mueve a jupiter
	sistemaSolar.Mover(4,"Jupiter"); // RUR 4: 0 infracciones 
	// r5 decide explorar Saturno y Urano en busca de material radioactivo
	sistemaSolar.Mover(4,"Saturno"); // RUR 4: 1 infracciones 
	sistemaSolar.Mover(4,"Urano"); // RUR 4: 2 infracciones 
	// r5 vuelve a Venus
	sistemaSolar.Mover(4,"Venus"); // RUR 4: 3 infracciones 

	// inspeccion en Venus !
	sistemaSolar.Inspeccion("Venus");
	// r5 es eliminado del sistema
	ASSERT_EQ(sistemaSolar.CantidadRobotsActivos(), 4);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(0), 2);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(1), 0);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(2), 1);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(3), 0);

	// entras las naves restantes al sistema
	sistemaSolar.Entrar(r6,"Jupiter"); // RUR 5
	sistemaSolar.Entrar(r7,"Marte"); // RUR 6
	sistemaSolar.Entrar(r8,"Saturno"); // RUR 7
	sistemaSolar.Entrar(r9,"Jupiter"); // RUR 8

	// r8 recorre todos los planetas radioactivos
	sistemaSolar.Mover(7,"Urano"); // RUR 8: 0 infracciones 
	sistemaSolar.Mover(7,"Neptuno"); // RUR 8: 0 infracciones 
	sistemaSolar.Mover(7,"Urano"); // RUR 8: 0 infracciones 
	sistemaSolar.Mover(7,"Saturno"); // RUR 8: 0 infracciones 
	sistemaSolar.Mover(7,"Neptuno"); // RUR 8: 0 infracciones 
	sistemaSolar.Mover(7,"Saturno"); // RUR 8: 0 infracciones 

	// r9 se mueve a Saturno a buscar material radioactivo, escoltado por r6
	sistemaSolar.Mover(8,"Saturno"); // RUR 8: 1 infracciones 
	sistemaSolar.Mover(5,"Saturno"); // RUR 8: 1 infracciones 

	// encuentran a r8 y lo capturan, llevandolo a Neptuno y luego volviendo a Jupiter
	sistemaSolar.Mover(7,"Neptuno"); // RUR 7: 0 infracciones 
	sistemaSolar.Mover(8,"Neptuno"); // RUR 8: 2 infracciones 
	sistemaSolar.Mover(5,"Neptuno"); // RUR 5: 2 infracciones 
	sistemaSolar.Mover(7,"Jupiter"); // RUR 7: 1 infracciones 
	sistemaSolar.Mover(8,"Jupiter"); // RUR 8: 2 infracciones 
	sistemaSolar.Mover(5,"Jupiter"); // RUR 5: 2 infracciones 

	// patrulla de inspeccion en Jupiter !
	sistemaSolar.Inspeccion("Jupiter");
	sistemaSolar.Inspeccion("Jupiter");

	ASSERT_EQ(sistemaSolar.CantidadRobotsActivos(), 7);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(0), 2);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(1), 0);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(2), 1);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(3), 0);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(4), 0);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(5), 0);
	ASSERT_EQ(sistemaSolar.CantInfraccionesIesimoRobotActivo(6), 0);

	ASSERT_EQ(sistemaSolar.ElMasInfractor(), 0);
	ASSERT_EQ(sistemaSolar.EstacionActualIesimoRobotActivo(0),"Tierra");
	ASSERT_EQ(sistemaSolar.EstacionActualIesimoRobotActivo(1),"Marte");
	ASSERT_EQ(sistemaSolar.EstacionActualIesimoRobotActivo(2),"Venus");
	ASSERT_EQ(sistemaSolar.EstacionActualIesimoRobotActivo(3),"Marte");
	ASSERT_EQ(sistemaSolar.EstacionActualIesimoRobotActivo(4),"Marte");
	ASSERT_EQ(sistemaSolar.EstacionActualIesimoRobotActivo(5),"Jupiter");
	ASSERT_EQ(sistemaSolar.EstacionActualIesimoRobotActivo(6),"Jupiter");
}


int main(int argc, char **argv)
{
    RUN_TEST(test_ciudad_simple);

	/******************************************************************
	 * TODO: escribir casos de test exhaustivos para todas            *
	 * las funcionalidades del módulo.                                *
     * La interacción con el TAD Ciudad se debe hacer exclusivamente  *
	 * a través de la interfaz del driver.                            *
	 ******************************************************************/

	 RUN_TEST(test_diccString);
	 RUN_TEST(test_conjString);

	 RUN_TEST(restriccion_crear);
	 RUN_TEST(restriccion_not);
	 RUN_TEST(restriccion_and);
	 RUN_TEST(restriccion_or);
	 RUN_TEST(restriccion_copiar_simple);
	 RUN_TEST(restriccion_de_arbol_sintactico);

	 RUN_TEST(cola_prior_simple);
	 RUN_TEST(cola_prior_encolar_desencolar);
	 RUN_TEST(cola_prior_iterador);

	 RUN_TEST(mapa_agregar);
	 RUN_TEST(mapa_conectar);
	 RUN_TEST(mapa_restriccion);

	 RUN_TEST(test_ciudad_capo);
	 RUN_TEST(test_ciudad);
	 
	 RUN_TEST(test_driver_completo);
	 RUN_TEST(test_driver_hiper_completo);
	 
	return 0;
}
