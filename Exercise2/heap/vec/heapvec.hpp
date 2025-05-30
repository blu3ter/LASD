
#ifndef HEAPVEC_HPP
#define HEAPVEC_HPP

/* ************************************************************************** */

#include "../heap.hpp"
#include "../../vector/vector.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

template <typename Data>
class HeapVec : virtual public Heap<Data>,
                public SortableVector<Data>  {//TODO virtual protected?
  // Must extend Heap<Data>,
  // Could extend SortableVector<Data>

private:

  // ...

protected:

  // using Container::???;
  using Container::size;
  using SortableVector<Data>::Elements;

  // ...

public:

  // Default constructor
  // HeapVec() specifiers;
  HeapVec() = default;

  /* ************************************************************************ */

  // Specific constructors
  // HeapVec(argument) specifiers; // A heap obtained from a TraversableContainer
  HeapVec(const TraversableContainer<Data>&);
  // HeapVec(argument) specifiers; // A heap obtained from a MappableContainer
  HeapVec(MappableContainer<Data>&&);

  /* ************************************************************************ */

  // Copy constructor
  // HeapVec(argument) specifiers;
  HeapVec(const HeapVec<Data>&);

  // Move constructor
  // HeapVec(argument) specifiers;
  HeapVec(HeapVec<Data>&&) noexcept;

  /* ************************************************************************ */

  // Destructor
  // ~HeapVec() specifiers;
  virtual ~HeapVec() = default;

  /* ************************************************************************ */

  // Copy assignment
  // type operator=(argument) specifiers;
  HeapVec<Data>& operator=(const HeapVec<Data>&);

  // Move assignment
  // type operator=(argument) specifiers;
  HeapVec<Data>& operator=(HeapVec<Data>&&) noexcept;

  /* ************************************************************************ */

  // Comparison operators
  // type operator==(argument) specifiers;
  bool operator==(const HeapVec<Data>&) const noexcept;
  // type operator!=(argument) specifiers;
  bool operator!=(const HeapVec<Data>&) const noexcept;

  /* ************************************************************************ */

  // Specific member functions (inherited from Heap)

  // type IsHeap(argument) specifiers; // Override Heap member
  bool IsHeap() const noexcept override;

  // type Heapify(argument) specifiers; // Override Heap member
  void Heapify() override;

  /* ************************************************************************ */

  // Specific member function (inherited from SortableLinearContainer)

  // type Sort(argument) specifiers; // Override SortableLinearContainer member (Heapsort)
  void Sort() noexcept override;

protected:

  // Auxiliary functions, if necessary!
  ulong LeftChild(ulong) const noexcept;
  ulong RightChild(ulong) const noexcept;
  ulong Parent(ulong) const noexcept;
  void HeapifyUp(ulong);
  void HeapifyDown(ulong);

};

/* ************************************************************************** */

}

#include "heapvec.cpp"

#endif
