#ifndef AED2_COLAPRIOR_H_
#define AED2_COLAPRIOR_H_

#include <ostream>
#include "aed2.h"

using namespace aed2;

template <typename T>
class ColaPrior
{
private:

  struct Nodo
  {
    Nodo(const T& d) : dato(d), padre(NULL), izquierda(NULL), derecha(NULL) {};
    Nodo(const Nodo& otro);

    T dato;
    Nodo* padre;
    Nodo* izquierda;
    Nodo* derecha;
  };

  Nodo* cabeza_;
  Nat longitud_;

  Nodo* damePadre(int);

  void swapConPadre(Nodo* nodo);

  void levantar(Nodo* nodo, bool forceLevantar);

  Nodo* agregarAtras(const T& dato);


  public:
  // forward declarations
  // Las estructuras estan definidas mas abajo
  class Iterador;
  class const_Iterador;

  ColaPrior();

  ~ColaPrior();

  /// Opreaciones básicas
  bool EsVacia() const;
  T& Maximo();
  void Desencolar();
  Iterador Encolar(const T& elem);

  /// Creación del Iterador
  Iterador CrearIt();

  /// Creación del const_Iterador
  const_Iterador CrearIt() const;

 

  class Iterador
  {
    public:

      Iterador();

      Iterador(const typename ColaPrior<T>::Iterador& otro);

      Iterador& operator = (const typename ColaPrior<T>::Iterador& otro);

      bool HayMas() const;
    
      T& Actual() const;

      void Eliminar();

      bool operator == (const typename ColaPrior<T>::Iterador& otro) const;

  private:

      ColaPrior<T>* cola_;

      typename ColaPrior<T>::Nodo* siguiente_;

      /// El constructor es privado, necesitamos el friend.
      Iterador(ColaPrior<T>* colaPrior, typename ColaPrior<T>::Nodo* proximo);

      friend typename ColaPrior<T>::Iterador ColaPrior<T>::CrearIt();
      friend typename ColaPrior<T>::Iterador ColaPrior<T>::Encolar(const T& elem);
      friend class ColaPrior<T>::const_Iterador;

  };


  class const_Iterador
  {
    public:

      const_Iterador();

      const_Iterador(const typename ColaPrior<T>::const_Iterador& otro);

      const_Iterador(const typename ColaPrior<T>::Iterador& otro);

      const_Iterador& operator = (const typename ColaPrior<T>::const_Iterador& otro);
    
      bool HayMas() const;

      T& Actual() const;

      bool operator == (const typename ColaPrior<T>::const_Iterador& otro) const;

  private:

      const ColaPrior<T>* cola_;

      const typename ColaPrior<T>::Nodo* siguiente_;

      /// El constructor es privado, necesitamos el friend.
      const_Iterador(const ColaPrior<T>* colaPrior, const typename ColaPrior<T>::Nodo* proximo);

      friend typename ColaPrior<T>::const_Iterador ColaPrior<T>::CrearIt() const;

  };


};

template <typename T>
ColaPrior<T>::ColaPrior()
  : cabeza_(NULL), longitud_(0)
{}

template <typename T>
ColaPrior<T>::~ColaPrior()
{
   while(!EsVacia()) {
     Desencolar();
   }
}

template <typename T>
bool ColaPrior<T>::EsVacia() const
{
  return cabeza_ == NULL;
}

template <typename T>
void ColaPrior<T>::Desencolar()
{
  #ifdef DEBUG
  assert( not EsVacia() );
  #endif

  Nodo* padreUltimo = NULL;
  Nodo* ultimo = NULL;
  Nodo* primerIzquierda = NULL;
  Nodo* primerDerecha = NULL;
  Nodo* izq = NULL;
  Nodo* der = NULL;

  T maximo, dato;
  if(longitud_ > 1){
    padreUltimo = damePadre(longitud_ -1);
    if(padreUltimo->derecha == NULL){
        ultimo = padreUltimo->izquierda;
        padreUltimo->izquierda = NULL;
    }else{
        ultimo = padreUltimo->derecha;
        padreUltimo->derecha = NULL;
    }
    ultimo->padre = NULL;
    primerIzquierda = cabeza_->izquierda;
    ultimo->izquierda = primerIzquierda;
    if(primerIzquierda != NULL){
        primerIzquierda->padre = ultimo;
    }
    primerDerecha = cabeza_->derecha;
    ultimo->derecha = primerDerecha;
    if(primerDerecha != NULL){
        primerDerecha->padre = ultimo;
    }
    delete cabeza_;
    cabeza_ = ultimo;
    bool inPlace = false;
    dato = ultimo->dato;
    while(!inPlace){
        izq = ultimo->izquierda;
        der = ultimo->derecha;
        if(der != NULL){
            maximo = std::max(izq->dato, der->dato);
            if(maximo < dato){
                inPlace = true;
            }else{
                if(maximo == (izq->dato)){
                    swapConPadre(izq);
                }else{
                    swapConPadre(der);
                }
            }
        }else{
            if(izq == NULL){
                inPlace = true;
            }else{
                if(izq->dato > dato){
                    swapConPadre(izq);
                }else{
                    inPlace = true;
                }
            }
        }
    }
  }else{
	delete cabeza_;
    cabeza_ = NULL;
  }
  longitud_ = longitud_ - 1;
}

template <typename T>
T& ColaPrior<T>::Maximo()
{
  return this->cabeza_->dato;
}

template <typename T>
typename ColaPrior<T>::Iterador ColaPrior<T>::Encolar(const T& dato)
{
  Nodo* nodo = agregarAtras(dato);
  levantar(nodo,false);

  Iterador iter = CrearIt();
  iter.siguiente_ = nodo;
  return iter;
}

template <typename T>
typename ColaPrior<T>::Nodo* ColaPrior<T>::agregarAtras(const T& dato)
{
    Nodo* nuevo = new Nodo(dato);
    if(longitud_ == 0){
      cabeza_ = nuevo;
    }else{
      Nodo* actual = damePadre(longitud_);
      nuevo->padre = actual;
      if(longitud_ % 2 == 0){
            actual->derecha = nuevo;
        }else{
            actual->izquierda = nuevo;
        }
    }

    // if(longitud_ == 0){
    //     actual = NULL;
    // }else{
    //     actual = damePadre(longitud_);
    //     nuevo->dato = dato;
    //     nuevo->padre = actual;    
    // }
    // if(longitud_ == 0){
    //     cabeza_ = nuevo;
    // }else{
    //     if(longitud_ % 2 == 0){
    //         actual->derecha = nuevo;
    //     }else{
    //         actual->izquierda = nuevo;
    //     }
    // }
    longitud_ = longitud_ + 1;
    return nuevo;
}

template <typename T>
typename ColaPrior<T>::Nodo* ColaPrior<T>::damePadre(int pos)
{
    int arraySize = 0;
    int tam = pos;
    while(tam > 0){
        arraySize++;
        tam = (tam - 1)/2;
    }
    int array[arraySize];
    int cant = 0;
    while(cant < arraySize){
        array[cant] = -1;
        cant++;
    } 
    int index = arraySize -1;
    tam = pos;

    while(tam > 0){
        array[index] = tam % 2;
        index--;
        tam = (tam - 1)/2;
    }

    index = 0;
    Nodo* actual = cabeza_;

    while(index < arraySize -1){
        if(array[index] == 0){
            actual = actual->derecha;
        }else{
            actual = actual->izquierda;
        }
        index++;
    }
    return actual;
}

template <typename T>
void ColaPrior<T>::levantar(Nodo* nodo, bool forceLevantar)
{
    while(nodo->padre != NULL && (nodo->dato > nodo->padre->dato || forceLevantar)){
        swapConPadre(nodo);
    }
}

template <typename T>
void ColaPrior<T>::swapConPadre(Nodo* nodo)
{
    Nodo* padreObj = nodo->padre;
    Nodo* abuelo = padreObj->padre;

    bool hermanoADer = (padreObj->izquierda->dato == nodo->dato);
    Nodo* hermano;
    if(hermanoADer){
        hermano = padreObj->derecha;
    }else{
        hermano = padreObj->izquierda;
    }

    Nodo* hijoIzq = nodo->izquierda;
    Nodo* hijoDer = nodo->derecha;

    if(abuelo != NULL){
        if(abuelo->izquierda->dato == padreObj->dato){
            abuelo->izquierda = nodo;
        }else{
            abuelo->derecha = nodo;
        }
    }

    if(hijoIzq != NULL){
        hijoIzq->padre = padreObj;
    }

    if(hijoDer != NULL){
        hijoDer->padre = padreObj;
    }

    padreObj->izquierda = hijoIzq;
    padreObj->derecha = hijoDer;
    padreObj->padre = nodo;

    if(hermano != NULL){
        hermano->padre = nodo;
    }

    nodo->padre = abuelo;

    if(hermanoADer){
        nodo->derecha = hermano;
        nodo->izquierda = padreObj;
    }else{
        nodo->izquierda = hermano;
        nodo->derecha = padreObj;
    }

    if(nodo->padre == NULL){
        cabeza_ = nodo;
    }

}


template <typename T>
typename ColaPrior<T>::Iterador ColaPrior<T>::CrearIt()
{
  return Iterador(this, NULL);
}



template <typename T>
typename ColaPrior<T>::const_Iterador ColaPrior<T>::CrearIt() const
{
  return const_Iterador(this, NULL);
}



  // Implementacion Iterador

template <typename T>
ColaPrior<T>::Iterador::Iterador()
  : cola_(NULL), siguiente_(NULL)
{}

template <typename T>
ColaPrior<T>::Iterador::Iterador(const typename ColaPrior<T>::Iterador& otro)
  : cola_(otro.cola_), siguiente_(otro.siguiente_)
{}

template <typename T>
typename ColaPrior<T>::Iterador& ColaPrior<T>::Iterador::operator= (const typename ColaPrior<T>::Iterador& otro)
{
  cola_ = otro.cola_;
  siguiente_ = otro.siguiente_;

  return *this;
}

template <typename T>
bool ColaPrior<T>::Iterador::HayMas() const
{
  return siguiente_ != NULL;
}


template <typename T>
T& ColaPrior<T>::Iterador::Actual() const
{
  #ifdef DEBUG
  assert( HayMas() );
  #endif

  return siguiente_->dato;
}


template <typename T>
T& ColaPrior<T>::const_Iterador::Actual() const
{
  #ifdef DEBUG
  assert( HayMas() );
  #endif

  return siguiente_->dato;
}



template <typename T>
void ColaPrior<T>::Iterador::Eliminar()
{
  #ifdef DEBUG
  assert(HayMas());
  #endif

  cola_->levantar(siguiente_,true);
  cola_->Desencolar();
  siguiente_ = NULL;
}

template<class T>
bool ColaPrior<T>::Iterador::operator == (const typename ColaPrior<T>::Iterador& otro) const {
  return cola_ == otro.cola_ && siguiente_ == otro.siguiente_;
}

template<class T>
ColaPrior<T>::Iterador::Iterador(ColaPrior<T>* cola, typename ColaPrior<T>::Nodo* nodo)
  : cola_(cola), siguiente_(nodo)
{};


  // Implementacion const_Iterador

template <typename T>
ColaPrior<T>::const_Iterador::const_Iterador()
  : cola_(NULL), siguiente_(NULL)
{}

template <typename T>
ColaPrior<T>::const_Iterador::const_Iterador(const typename ColaPrior<T>::const_Iterador& otro)
  : cola_(otro.cola_), siguiente_(otro.siguiente_)
{}

template <typename T>
ColaPrior<T>::const_Iterador::const_Iterador(const typename ColaPrior<T>::Iterador& otro)
  : cola_(otro.cola_), siguiente_(otro.siguiente_)
{}

template <typename T>
typename ColaPrior<T>::const_Iterador& ColaPrior<T>::const_Iterador::operator = (const typename ColaPrior<T>::const_Iterador& otro)
{
  cola_ = otro.cola_;
  siguiente_ = otro.siguiente_;

  return *this;
}

template <typename T>
bool ColaPrior<T>::const_Iterador::HayMas() const
{
  return siguiente_ != NULL;
}

template<class T>
bool ColaPrior<T>::const_Iterador::operator == (const typename ColaPrior<T>::const_Iterador& otro) const {
  return cola_ == otro.cola_ && siguiente_ == otro.siguiente_;
}

template <typename T>
ColaPrior<T>::const_Iterador::const_Iterador(const ColaPrior<T>* cola, const typename ColaPrior<T>::Nodo* proximo)
  : cola_(cola), siguiente_(proximo)
{}


  // Otros

template <typename T>
std::ostream& operator<<(std::ostream& os, const ColaPrior<T>& cola)
{
  return Mostrar(os, cola, '<', '>');
}

template <typename T>
bool operator == (const ColaPrior<T>& l1, const ColaPrior<T>& l2)
{
  typename ColaPrior<T>::const_Iterador it1 = l1.CrearIt();
  typename ColaPrior<T>::const_Iterador it2 = l2.CrearIt();

  return it1.HayMas() == it2.HayMas() && it1.Actual() == it2.Actual();
    
}

template <typename T>
ColaPrior<T>::Nodo::Nodo(const Nodo& otro) : dato(otro.dato), padre(NULL), izquierda(NULL), derecha(NULL)
{
  Nodo* izq = NULL;
  Nodo* der = NULL;
  if(otro.izquierda != NULL){
    izq = new Nodo(*(otro.izquierda));
    izq->padre = this;
    this->izquierda = izq;
  }
  if(otro.derecha != NULL){
    der = new Nodo(*(otro.derecha));
    der->padre = this;
    this->derecha = der;
  }

}

#endif // AED2_COLAPRIOR_H_INCLUDED