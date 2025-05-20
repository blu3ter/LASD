

#ifndef SETVEC_HPP
#define SETVEC_HPP

/* ************************************************************************** */

#include "../set.hpp"
#include <stdexcept> // Per std::length_error, std::out_of_range

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

template <typename Data>
class SetVec : virtual public lasd::Set<Data>,
               virtual public lasd::ResizableContainer {

private:
  Data* elements = nullptr;
  ulong
   currentSize = 0;
  ulong
   bufferSize = 4; // Dimensione fisica iniziale del buffer
  ulong
   head = 0;       // Indice fisico del primo elemento logico

protected:
  // using Container::size; // Già ereditato e implementato come Size()

public:
  // Default constructor
  SetVec();

  /* ************************************************************************ */

  // Specific constructors
  SetVec(const TraversableContainer<Data>&); // Costruttore da un TraversableContainer
  SetVec(MappableContainer<Data>&&);      // Costruttore da un MappableContainer (move)

  /* ************************************************************************ */

  // Copy constructor
  SetVec(const SetVec<Data>&);

  // Move constructor
  SetVec(SetVec<Data>&&) noexcept;

  /* ************************************************************************ */

  // Destructor
  virtual ~SetVec();

  /* ************************************************************************ */

  // Copy assignment
  SetVec<Data>& operator=(const SetVec<Data>&);

  // Move assignment
  SetVec<Data>& operator=(SetVec<Data>&&) noexcept;

  /* ************************************************************************ */

  // Comparison operators
  bool operator==(const SetVec<Data>&) const noexcept;
  inline bool operator!=(const SetVec<Data>&) const noexcept;

  /* ************************************************************************ */

  // Specific member functions (inherited from Set/DictionaryContainer)

  virtual bool Insert(const Data&) override;
  virtual bool Insert(Data&&) override;     
  virtual bool Remove(const Data&) override;

  /* ************************************************************************ */

  // Specific member functions (inherited from OrderedDictionaryContainer)

  virtual const Data& Min() const override; // Lancia std::length_error se vuoto.
  virtual Data MinNRemove() override;       // Lancia std::length_error se vuoto.
  virtual void RemoveMin() override; // Lancia std::length_error se vuoto.

  virtual const Data& Max() const override; // Lancia std::length_error se vuoto.
  virtual Data MaxNRemove() override;       // Lancia std::length_error se vuoto.
  virtual void RemoveMax() override; // Lancia std::length_error se vuoto.

  virtual const Data& Predecessor(const Data&) const override; // Lancia std::length_error se non trovato o se l'elemento è il minimo.
  virtual Data PredecessorNRemove(const Data&) override;       // Lancia std::length_error se non trovato o se l'elemento è il minimo.
  virtual void RemovePredecessor(const Data&) override; // Lancia std::length_error se non trovato o se l'elemento è il minimo.

  virtual const Data& Successor(const Data&) const override; // Lancia std::length_error se non trovato o se l'elemento è il massimo.
  virtual Data SuccessorNRemove(const Data&) override;       // Lancia std::length_error se non trovato o se l'elemento è il massimo.
  virtual void RemoveSuccessor(const Data&) override; // Lancia std::length_error se non trovato o se l'elemento è il massimo.

  /* ************************************************************************ */

  // Specific member function (inherited from TestableContainer)
  virtual bool Exists(const Data&) const noexcept override; // Verifica se un elemento è presente.

  /* ************************************************************************ */

  // Specific member function (inherited from ResizableContainer)
  virtual void Resize(ulong
     newsize) override; // Ridimensiona il buffer. Può perdere dati se newsize < currentSize.
                                                       // Per un Set, Resize dovrebbe idealmente solo cambiare la capacità del buffer,
                                                       // non la dimensione logica (numero di elementi), a meno che non sia per troncare.
                                                       // La Resize di ResizableContainer è più generica.
                                                       // Qui la interpretiamo come cambio di capacità del buffer interno.

  /* ************************************************************************ */
  // Specific member function (inherited from ClearableContainer)
  virtual void Clear() override; // Rimuove tutti gli elementi.

  // Specific member functions (inherited from Container)
  virtual inline ulong
   Size() const noexcept override { return currentSize; }
  virtual inline bool Empty() const noexcept override { return (currentSize == 0); }

  /* ************************************************************************ */

  // Specific member functions (inherited from Set/LinearContainer)
  virtual Data& Front() override; // Lancia std::length_error se vuoto
  virtual const Data& Front() const override; // Lancia std::length_error se vuoto

  virtual Data& Back() override; // Lancia std::length_error se vuoto
  virtual const Data& Back() const override; // Lancia std::length_error se vuoto

  virtual Data& operator[](ulong
  ) override; // Lancia std::out_of_range se l'indice non è valido
  virtual const Data& operator[](ulong
  ) const override; // Lancia std::out_of_range se l'indice non è valido

protected:
  // Auxiliary functions

  // Cerca l'elemento.
  // Restituisce l'indice se trovato.
  // Se non trovato, restituisce l'indice dove dovrebbe essere inserito per mantenere l'ordine.
  // Un valore negativo (es. -(insertion_point + 1)) potrebbe indicare "non trovato ma inserisci qui".
  // Semplifichiamo: restituisce un `long` per l'indice, e un booleano per `found`.
  // Oppure, un `std::pair<long, bool> BinarySearch(const Data& val) const;`
  // O ancora, restituisce l'indice. Se `elements[index] == val`, allora trovato.
  // Se `index == currentSize` o `elements[index] != val`, allora non trovato, e `index` è il punto di inserimento.
  long FindIndexOf(const Data& val) const; // Usato da Exists, Remove, Insert per trovare l'elemento o la sua posizione.

  // Funzione ausiliaria per l'inserimento che gestisce il ridimensionamento e lo shift.
  bool InsertAt(ulong
     index, const Data& val);
  bool InsertAt(ulong
     index, Data&& val);

  // Funzione ausiliaria per la rimozione che gestisce lo shift e il ridimensionamento.
  void RemoveAt(ulong
     index);

  // Funzioni per la gestione della capacità del buffer interno.
  void EnsureCapacity(ulong
     minCapacity); // Assicura che bufferSize sia almeno minCapacity.
  void ShrinkToFit(); // Riduce bufferSize a currentSize (o a un minimo ragionevole).

  // La ricerca binaria vera e propria.
  // Cerca `val` nell'intervallo `[low, high]` dell'array `elements`.
  // Restituisce l'indice se `val` è trovata.
  // Altrimenti, restituisce l'indice `i` tale che tutti gli elementi `elements[j < i]` sono `< val`
  // e tutti gli elementi `elements[k >= i]` sono `> val`.
  // Questo è l'indice dove `val` dovrebbe essere inserita per mantenere l'ordine.
  long BinarySearch(const Data& val, long low, long high) const;

};

/* ************************************************************************** */

}

#include "setvec.cpp"

#endif
