#ifndef CIUDAD_H
#define CIUDAD_H

#include <iostream>
#include <assert.h>
#include "aed2.h"
#include "diccString.h"
#include "conjString.h"
#include "ColaPrior.h"
#include "Mapa.h"
#include "Tipos.h"

using namespace aed2;

class Ciudad {
	
	public:
		friend class Driver;
		class const_ItVectorNat;

		Ciudad();
		Ciudad(Mapa&);
		~Ciudad();
		void Entrar(const ConjString&, const String&);
		void Mover(const Nat&, const String&);
		void Inspeccion(const String&);
		Nat Infracciones(const Nat&) const;
		String Estacion_(const Nat&) const;
		Lista<Estacion>::const_Iterador Estaciones() const;
		ConjString Tags(const Nat&) const;
		const_ItVectorNat Robots() const;

		class const_ItVectorNat
	    {
		    public:

		      const_ItVectorNat();

		      const_ItVectorNat(const Ciudad::const_ItVectorNat& otro);
		    
		      bool HayMas() const;

		      Nat Proximo();

			private:

		      const Vector<bool>* elementos_;

		      Nat actual_;


		      /// El constructor es privado, necesitamos el friend.
		      const_ItVectorNat(const Vector<bool>*);

		      friend Ciudad::const_ItVectorNat Ciudad::Robots() const;

	  	};

	private:
		struct Pair{//hola soy una struct con un nombre de mierda
			int infracciones, rur;
		};
		friend bool operator==(const Pair& p1, const Pair& p2){
			return (p1.infracciones == p2.infracciones && p1.rur == p2.rur);
		}
		friend bool operator>(const Pair& p1, const Pair& p2){
			if (p1.infracciones == p2.infracciones){
				return p1.rur > p2.rur;
			}
			return p1.infracciones > p2.infracciones;
		}
		friend bool operator<(const Pair& p1, const Pair& p2){
			return !(p1 == p2) && !(p1 > p2);
		}

		Nat cantRobots;
		Vector<Nat> cantInfracciones;
		Vector<String> estaciones;
		Vector<ConjString> tags;
		Vector<bool> vivos;
		Vector<DiccString<DiccString<bool> > > sendasPermitidas;
		DiccString<ColaPrior<Pair > > robotsEnEstacion;
		Vector<ColaPrior<Pair>::Iterador> sacarRobotDeEstacion;
	    Mapa map;


};

#endif //CIUDAD_H
