#include "Driver.h"
#include "mini_test.h"
#include "Lista.h"
#include "Conj.h"
#include "Dicc.h"

#include <string>
#include <iostream>

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

struct infoRobots {
    Dicc<RUR,Nat> infracciones;
    Dicc<RUR,Estacion> estaciones;
    Dicc<RUR,Conj<Caracteristica> > caracteristicas;
};

infoRobots getDatosRobotsActivos(const Driver& d)
{
    infoRobots info;

    for (Nat i = 0;  i< d.CantidadRobotsActivos(); ++i)
        info.infracciones.Definir(d.IesimoRobotActivo(i),d.CantInfraccionesIesimoRobotActivo(i));

    for (Nat i = 0;  i< d.CantidadRobotsActivos(); ++i)
        info.estaciones.Definir(d.IesimoRobotActivo(i),d.EstacionActualIesimoRobotActivo(i));

    for (Nat i = 0;  i< d.CantidadRobotsActivos(); ++i)
        info.caracteristicas.Definir(d.IesimoRobotActivo(i),d.CaracteristicasIesimoRobotActivo(i));

    return info;
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

    infoRobots info = getDatosRobotsActivos(caba);

    ASSERT_EQ(info.infracciones.Significado(0), 1);
    ASSERT_EQ(info.infracciones.Significado(1), 1);
    ASSERT_EQ(info.infracciones.Significado(2), 2);

    ASSERT_EQ(caba.ElMasInfractor(),2);

    // Vuela un robot
    caba.Inspeccion("Retiro");
    ASSERT_EQ(caba.CantidadRobotsActivos(),2);

}

void test_mapa() {
    Conj<Estacion> es;
    es.Agregar("Belgrano");
    es.Agregar("Retiro");
    es.Agregar("Olivos");
    es.Agregar("Martinez");
    Driver d(es);
    d.AgregarSenda("Belgrano","Retiro","ca");
    d.AgregarSenda("Retiro","Olivos","cb");
    d.AgregarSenda("Retiro","Martinez","cc");
    d.AgregarSenda("Belgrano","Martinez","cc");

    // Verifica # estaciones
    ASSERT_EQ(d.CantidadEstaciones(),4);

    // Verifica # sendas x estacion
    ASSERT_EQ(d.CantidadDeSendasParaEstacion("Retiro"),3);
    ASSERT_EQ(d.CantidadDeSendasParaEstacion("Belgrano"),2);
    ASSERT_EQ(d.CantidadDeSendasParaEstacion("Olivos"),1);
    ASSERT_EQ(d.CantidadDeSendasParaEstacion("Martinez"),2);
}

// Entrar robots
void test_entrar() {
    Conj<Estacion> es;
    es.Agregar("Belgrano");
    es.Agregar("Retiro");
    Driver d(es);

    Conj<Caracteristica> cs1,cs2,cs3;
    cs1.Agregar("ca");
    cs2.Agregar("ca");
    cs2.Agregar("cb");
    cs3.Agregar("cc");
    d.Entrar(cs1,"Belgrano");
    d.Entrar(cs2,"Retiro");
    d.Entrar(cs3,"Retiro");
    ASSERT_EQ(d.CantidadRobotsActivos(),3);

    infoRobots info = getDatosRobotsActivos(d);
    ASSERT_EQ(info.estaciones.Significado(0),"Belgrano");
    ASSERT_EQ(info.estaciones.Significado(1),"Retiro");
    ASSERT_EQ(info.estaciones.Significado(2),"Retiro");
}

// Mover robots
void test_mover_robots() {
    Conj<Estacion> es;
    es.Agregar("Belgrano");
    es.Agregar("Retiro");
    Driver d(es);
    d.AgregarSenda("Belgrano","Retiro","ca");

    Conj<Caracteristica> cs1,cs2;
    cs1.Agregar("ca");
    cs2.Agregar("cb");
    d.Entrar(cs1,"Belgrano");
    d.Entrar(cs2,"Retiro");
    d.Mover(0,"Retiro");
    d.Mover(1,"Belgrano");
    infoRobots info = getDatosRobotsActivos(d);
    ASSERT_EQ(info.estaciones.Significado(0),"Retiro");
    ASSERT_EQ(info.estaciones.Significado(1),"Belgrano");
}

void test_sumar_infracciones() {
    Conj<Estacion> es;
    es.Agregar("Belgrano");
    es.Agregar("Retiro");
    Driver d(es);
    d.AgregarSenda("Belgrano","Retiro","ca");

    Conj<Caracteristica> cs1;
    cs1.Agregar("cb");
    d.Entrar(cs1,"Belgrano");
    // Hago varios mover ida y vuelta: debe sumar 10 infracciones
    for (int i = 0; i < 10; ++i)
        d.Mover(0, i%2 == 0 ? "Retiro" : "Belgrano");

    infoRobots info = getDatosRobotsActivos(d);
    ASSERT_EQ(info.estaciones.Significado(0),"Belgrano");
    ASSERT_EQ(info.infracciones.Significado(0),10);
}

void test_no_sumar_infracciones() {
    Conj<Estacion> es;
    es.Agregar("Belgrano");
    es.Agregar("Retiro");
    Driver d(es);
    d.AgregarSenda("Belgrano","Retiro","ca");

    Conj<Caracteristica> cs1;
    cs1.Agregar("ca");
    d.Entrar(cs1,"Belgrano");
    // Hago varios mover ida y vuelta: no debe sumar infracciones
    for (int i = 0; i < 10; ++i)
        d.Mover(0, i%2 == 0 ? "Retiro" : "Belgrano");

    infoRobots info = getDatosRobotsActivos(d);
    ASSERT_EQ(info.estaciones.Significado(0),"Belgrano");
    ASSERT_EQ(info.infracciones.Significado(0),0);
}

void test_restriccion_tautologia_contradiccion() {
    Conj<Estacion> es;
    es.Agregar("Belgrano");
    es.Agregar("Retiro");
    es.Agregar("Martinez");
    Driver d(es);
    d.AgregarSenda("Belgrano","Retiro","ser|!ser");
    d.AgregarSenda("Retiro","Martinez","casa&!casa");

    Conj<Caracteristica> cs1;
    cs1.Agregar("cb");
    d.Entrar(cs1,"Belgrano");

    d.Mover(0,"Retiro");
    // El mover no sumo
    ASSERT_EQ(d.CantInfraccionesIesimoRobotActivo(0),0);
    d.Mover(0,"Martinez");
    // El mover si sumo
    ASSERT_EQ(d.CantInfraccionesIesimoRobotActivo(0),1);
}

void test_restriccion_compleja() {
    Conj<Estacion> es;
    es.Agregar("Belgrano");
    es.Agregar("Retiro");
    es.Agregar("Martinez");
    Driver d(es);
    d.AgregarSenda("Belgrano","Retiro","(ca&!cb) | (!cc | (unaCaracteristicaDeCasiCasiCasiCincuentaYNueveCaracteres & ce))");

    Conj<Caracteristica> cs1,cs2,cs3,cs4;
    cs1.Agregar("ca");
    d.Entrar(cs1,"Belgrano"); //

    cs2.Agregar("ca");
    cs2.Agregar("cb");
    cs2.Agregar("cc");
    d.Entrar(cs2,"Belgrano");

    cs3.Agregar("ca");
    cs3.Agregar("cb");
    cs3.Agregar("cc");
    cs3.Agregar("unaCaracteristicaDeCasiCasiCasiCincuentaYNueveCaracteres");
    cs3.Agregar("ce");
    d.Entrar(cs3,"Belgrano");

    cs4.Agregar("ca");
    cs4.Agregar("cb");
    cs4.Agregar("cc");
    cs4.Agregar("unaCaracteristicaDeCasiCasiCasiCincuentaYNueveCaracteres");
    d.Entrar(cs4,"Belgrano");

    for (RUR i=0; i < 4; ++i)
    d.Mover(i,"Retiro");

    infoRobots info = getDatosRobotsActivos(d);
    ASSERT_EQ(info.infracciones.Significado(0),0);
    ASSERT_EQ(info.infracciones.Significado(1),1);
    ASSERT_EQ(info.infracciones.Significado(2),0);
    ASSERT_EQ(info.infracciones.Significado(3),1);
}

void test_prefijos() {
    Conj<Estacion> es;
    es.Agregar("Belgrano");
    es.Agregar("Retiro");
    es.Agregar("Martinez");
    Driver d(es);
    d.AgregarSenda("Belgrano","Retiro","a|ab|abcd|abcdef");
    d.AgregarSenda("Martinez","Retiro","abcd");

    Conj<Caracteristica> cs1,cs2,cs3,cs4,cs5,cs6;
    cs1.Agregar("a");
    d.Entrar(cs1,"Belgrano"); //

    cs2.Agregar("abc");
    d.Entrar(cs2,"Belgrano");

    cs3.Agregar("abcdef");
    d.Entrar(cs3,"Belgrano");

    cs4.Agregar("abcdefg");
    d.Entrar(cs4,"Belgrano");

    cs5.Agregar("a");
    cs5.Agregar("ab");
    cs5.Agregar("abc");
    cs5.Agregar("abcde");
    cs5.Agregar("abcdef");
    cs5.Agregar("abcdefg");
    d.Entrar(cs5,"Retiro");

    cs6.Agregar("a");
    cs6.Agregar("abcd");
    cs6.Agregar("abcdefg");
    d.Entrar(cs6,"Retiro");

    for (RUR i=0; i < 4; ++i)
        d.Mover(i,"Retiro");

    d.Mover(4,"Martinez");
    d.Mover(5,"Martinez");

    infoRobots info = getDatosRobotsActivos(d);
    ASSERT_EQ(info.infracciones.Significado(0),0);
    ASSERT_EQ(info.infracciones.Significado(1),1);
    ASSERT_EQ(info.infracciones.Significado(2),0);
    ASSERT_EQ(info.infracciones.Significado(3),1);
    ASSERT_EQ(info.infracciones.Significado(4),1);
    ASSERT_EQ(info.infracciones.Significado(5),0);
}

void test_inspeccion_simple() {
    Conj<Estacion> es;
    es.Agregar("Belgrano");
    es.Agregar("Retiro");
    Driver d(es);
    d.AgregarSenda("Belgrano","Retiro","ca");

    Conj<Caracteristica> cs1;
    cs1.Agregar("cb");
    d.Entrar(cs1,"Belgrano");
    d.Entrar(cs1,"Retiro");
    ASSERT_EQ(d.CantidadRobotsActivos(),2);
    d.Mover(0, "Retiro");
    d.Inspeccion("Retiro"); // debe volar el RUR 0

    ASSERT_EQ(d.CantidadRobotsActivos(),1);

    infoRobots info = getDatosRobotsActivos(d);
    // Chequeo que el que queda no fue modificado
    ASSERT_EQ(info.estaciones.Significado(1),"Retiro");
    ASSERT_EQ(info.infracciones.Significado(1),0);
}

void test_inspeccion_con_empate() {
    Conj<Estacion> es;
    es.Agregar("Belgrano");
    es.Agregar("Retiro");
    Driver d(es);
    d.AgregarSenda("Belgrano","Retiro","ca");

    Conj<Caracteristica> cs1;
    cs1.Agregar("cb");
    d.Entrar(cs1,"Belgrano");
    d.Entrar(cs1,"Belgrano");
    d.Entrar(cs1,"Belgrano");
    d.Entrar(cs1,"Retiro");
    ASSERT_EQ(d.CantidadRobotsActivos(),4);
    d.Mover(0, "Retiro");
    d.Mover(1, "Retiro");
    d.Mover(2, "Retiro");
    d.Inspeccion("Retiro"); // debe volar un solo RUR del conj {0,1,2}

    ASSERT_EQ(d.CantidadRobotsActivos(),3);

    infoRobots info = getDatosRobotsActivos(d);
    // Chequeo que los que quedan están ok
    // Alguno de los primeros RURs tuvo que desaparecer
    ASSERT(!(info.estaciones.Definido(0) && info.estaciones.Definido(1) && info.estaciones.Definido(2)));
    ASSERT_EQ(info.estaciones.Significado(3),"Retiro");
    ASSERT_EQ(info.infracciones.Significado(3),0);
}

void test_inspeccion_orden_prioridades() {
    Conj<Estacion> es;
    es.Agregar("Belgrano");
    es.Agregar("Retiro");
    Driver d(es);
    d.AgregarSenda("Belgrano","Retiro","ca");

    Conj<Caracteristica> cs1;
    cs1.Agregar("cb");
    d.Entrar(cs1,"Belgrano");
    d.Entrar(cs1,"Retiro");
    d.Entrar(cs1,"Belgrano");
    d.Entrar(cs1,"Retiro");
    d.Entrar(cs1,"Retiro");

    d.Mover(2, "Retiro");
    d.Mover(2, "Belgrano");
    d.Mover(2, "Retiro");
    d.Mover(1, "Belgrano");
    d.Mover(1, "Retiro");
    d.Mover(0, "Retiro");

    ASSERT_EQ(d.CantidadRobotsActivos(),5);

    d.Inspeccion("Retiro"); // debe volar RUR 2
    ASSERT_EQ(d.CantidadRobotsActivos(),4);
    ASSERT(!getDatosRobotsActivos(d).infracciones.Definido(2));
    d.Inspeccion("Retiro"); // debe volar RUR 1
    ASSERT_EQ(d.CantidadRobotsActivos(),3);
    ASSERT(!getDatosRobotsActivos(d).infracciones.Definido(1));
    d.Inspeccion("Retiro"); // debe volar RUR 0
    ASSERT_EQ(d.CantidadRobotsActivos(),2);
    ASSERT(!getDatosRobotsActivos(d).infracciones.Definido(0));
    d.Inspeccion("Retiro"); // no debe volar nadie
    ASSERT_EQ(d.CantidadRobotsActivos(),2);
    ASSERT(getDatosRobotsActivos(d).infracciones.Definido(3));
}

void test_el_mas_infractor() {
    Conj<Estacion> es;
    es.Agregar("Belgrano");
    es.Agregar("Retiro");
    Driver d(es);
    d.AgregarSenda("Belgrano","Retiro","ca");

    Conj<Caracteristica> cs1;
    cs1.Agregar("cb");
    d.Entrar(cs1,"Belgrano");
    d.Entrar(cs1,"Belgrano");
    d.Entrar(cs1,"Belgrano");
    d.Entrar(cs1,"Retiro");
    ASSERT_EQ(d.CantidadRobotsActivos(),4);
    d.Mover(0, "Retiro");
    d.Mover(1, "Retiro");
    d.Mover(2, "Retiro");
    d.Mover(0, "Belgrano");
    d.Mover(1, "Belgrano");

    Conj<RUR> peores; // {0,1} con 2 infracciones
    peores.Agregar(0);
    peores.Agregar(1);

    ASSERT(peores.Pertenece(d.ElMasInfractor()));

    d.Mover(0,"Retiro"); // Ahora el RUR 0 es el más infractor
    ASSERT_EQ(d.ElMasInfractor(),0);
}

void test_el_mas_infractor_e_inspeccion() {
    Conj<Estacion> es;
    es.Agregar("Belgrano");
    es.Agregar("Retiro");
    Driver d(es);
    d.AgregarSenda("Belgrano","Retiro","ca");

    Conj<Caracteristica> cs1;
    cs1.Agregar("cb");
    d.Entrar(cs1,"Belgrano");
    d.Entrar(cs1,"Belgrano");
    d.Entrar(cs1,"Belgrano");
    d.Entrar(cs1,"Retiro");
    ASSERT_EQ(d.CantidadRobotsActivos(),4);
    d.Mover(0, "Retiro");
    d.Mover(1, "Retiro");
    d.Mover(2, "Retiro");
    d.Mover(0, "Belgrano");
    d.Mover(1, "Belgrano");

    Conj<RUR> peores; // {0,1} con 2 infracciones
    peores.Agregar(0);
    peores.Agregar(1);

    d.Inspeccion("Retiro"); // No debe volar a ninguno de los mas infractores
    ASSERT(peores.Pertenece(d.ElMasInfractor()));

    infoRobots info = getDatosRobotsActivos(d);
    ASSERT(info.caracteristicas.Definido(0) && info.caracteristicas.Definido(1));

    d.Inspeccion("Belgrano"); // Debe volar a uno de los dos mas infractores
    ASSERT(peores.Pertenece(d.ElMasInfractor()));
    info = getDatosRobotsActivos(d);
    ASSERT(!(info.caracteristicas.Definido(0) && info.caracteristicas.Definido(1)));
}

void test_no_reusa_RURs() {
    Conj<Estacion> es;
    es.Agregar("Belgrano");
    es.Agregar("Retiro");
    Driver d(es);
    d.AgregarSenda("Belgrano","Retiro","ca");

    Conj<Caracteristica> cs1;
    cs1.Agregar("cb");
    d.Entrar(cs1,"Belgrano");
    d.Entrar(cs1,"Retiro");
    d.Mover(0, "Retiro");
    d.Inspeccion("Retiro"); // debe volar el RUR 0

    infoRobots info = getDatosRobotsActivos(d);
    ASSERT(!info.infracciones.Definido(0));

    d.Entrar(cs1,"Belgrano"); // no debe reusar RUR 0

    info = getDatosRobotsActivos(d);
    ASSERT(!info.infracciones.Definido(0)); // RUR 0 tiene que seguir inactivo
}

int main(int argc, char **argv)
{
    RUN_TEST(test_ciudad_simple);
    RUN_TEST(test_mapa);
    RUN_TEST(test_entrar);
    RUN_TEST(test_mover_robots);
    RUN_TEST(test_sumar_infracciones);
    RUN_TEST(test_no_sumar_infracciones);
    RUN_TEST(test_inspeccion_simple);
    RUN_TEST(test_el_mas_infractor);

    RUN_TEST(test_el_mas_infractor_e_inspeccion);
    RUN_TEST(test_inspeccion_con_empate);
    RUN_TEST(test_inspeccion_orden_prioridades);
    RUN_TEST(test_no_reusa_RURs);
    RUN_TEST(test_prefijos);
    RUN_TEST(test_restriccion_compleja);
    RUN_TEST(test_restriccion_tautologia_contradiccion);
	/******************************************************************
	 * TODO: escribir casos de test exhaustivos para todas            *
	 * las funcionalidades del módulo.                                *
     * La interacción con el TAD Ciudad se debe hacer exclusivamente  *
	 * a través de la interfaz del driver.                            *
	 ******************************************************************/

	return 0;
}
