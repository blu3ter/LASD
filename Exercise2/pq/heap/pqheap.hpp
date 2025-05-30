
#ifndef PQHEAP_HPP
#define PQHEAP_HPP

/* ************************************************************************** */

#include "../pq.hpp"
#include "../../heap/vec/heapvec.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

template <typename Data>
class PQHeap : virtual public PQ<Data>,
               virtual public HeapVec<Data> {
  // Must extend PQ<Data>,
  // Could extend HeapVec<Data>

private:

  // ...

protected:

  // using Container::???;
  using Container::size;
  using HeapVec<Data>::Elements;
  using HeapVec<Data>::HeapifyUp;
  using HeapVec<Data>::HeapifyDown;
  using HeapVec<Data>::Resize;

  // ...

public:

  // Default constructor
  // PQHeap() specifiers;
  PQHeap() = default;

  /* ************************************************************************ */

  // Specific constructors
  // PQHeap(argument) specifiers; // A priority queue obtained from a TraversableContainer
  PQHeap(const TraversableContainer<Data>&);
  // PQHeap(argument) specifiers; // A priority queue obtained from a MappableContainer
  PQHeap(MappableContainer<Data>&&);

  /* ************************************************************************ */

  // Copy constructor
  // PQHeap(argument) specifiers;
  PQHeap(const PQHeap<Data>&);

  // Move constructor
  // PQHeap(argument) specifiers;
  PQHeap(PQHeap<Data>&&) noexcept;

  /* ************************************************************************ */

  // Destructor
  // ~PQHeap() specifiers;
  virtual ~PQHeap() = default;

  /* ************************************************************************ */

  // Copy assignment
  // type operator=(argument) specifiers;
  PQHeap<Data>& operator=(const PQHeap<Data>&);

  // Move assignment
  // type operator=(argument) specifiers;
  PQHeap<Data>& operator=(PQHeap<Data>&&) noexcept;

  /* ************************************************************************ */

  // Specific member functions (inherited from PQ)

  // type Tip(argument) specifiers; // Override PQ member (must throw std::length_error when empty)
  const Data& Tip() const override;
  // type RemoveTip(argument) specifiers; // Override PQ member (must throw std::length_error when empty)
  void RemoveTip() override;
  // type TipNRemove(argument) specifiers; // Override PQ member (must throw std::length_error when empty)
  Data TipNRemove() override;

  // type Insert(argument) specifiers; // Override PQ member _(Copy of the value)
  void Insert(const Data&) override;
  // type Insert(argument) specifiers; // Override PQ member (Move of the value)
  void Insert(Data&&) override;

  // type Change(argument) specifiers; // Override PQ member (Copy of the value)
  void Change(ulong, const Data&) override;
  // type Change(argument) specifiers; // Override PQ member (Move of the value)
  void Change(ulong, Data&&) override;

protected:

  // Auxiliary functions, if necessary!

};

/* ************************************************************************** */

}

#include "pqheap.cpp"

#endif
