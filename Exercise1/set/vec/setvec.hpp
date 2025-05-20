

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

  virtual const Data& Min() const override; 
  virtual Data MinNRemove() override;      
  virtual void RemoveMin() override;

  virtual const Data& Max() const override; 
  virtual Data MaxNRemove() override;    
  virtual void RemoveMax() override; 

  virtual const Data& Predecessor(const Data&) const override; 
  virtual Data PredecessorNRemove(const Data&) override;    
  virtual void RemovePredecessor(const Data&) override; 

  virtual const Data& Successor(const Data&) const override;
  virtual Data SuccessorNRemove(const Data&) override;      
  virtual void RemoveSuccessor(const Data&) override; 
  /* ************************************************************************ */

  // Specific member function (inherited from TestableContainer)
  virtual bool Exists(const Data&) const noexcept override; 

  /* ************************************************************************ */

  // Specific member function (inherited from ResizableContainer)
  virtual void Resize(ulong) override; 

  /* ************************************************************************ */
  // Specific member function (inherited from ClearableContainer)
  virtual void Clear() override; 

  // Specific member functions (inherited from Container)
  virtual inline ulong Size() const noexcept override { return currentSize; }
  virtual inline bool Empty() const noexcept override { return (currentSize == 0); }

  /* ************************************************************************ */

  // Specific member functions (inherited from LinearContainer)
  virtual Data& Front() override;
  virtual const Data& Front() const override

  virtual Data& Back() override; 
  virtual const Data& Back() const override; 

  virtual Data& operator[](ulong
  ) override; 
  virtual const Data& operator[](ulong
  ) const override; 

protected:
  // Auxiliary functions

  long FindIndexOf(const Data& val) const; 

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

  long BinarySearch(const Data& val, long low, long high) const;

};

/* ************************************************************************** */

}

#include "setvec.cpp"

#endif
