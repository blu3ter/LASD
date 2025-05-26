
#ifndef VECTOR_HPP
#define VECTOR_HPP

/* ************************************************************************** */

#include "../container/linear.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

template <typename Data>
class Vector : virtual public MutableLinearContainer<Data>,
                virtual public ResizableContainer {
  // Must extend MutableLinearContainer<Data>,
  //             ResizableContainer

private:

  // ...

protected:

  using Container::size;

  Data * Elements = nullptr;
  static const ulong DEFAULT_CAPACITY = 16;
  // ...
 
public:

  // Default constructor
  // Vector() specifiers;
  Vector() = default;

  /* ************************************************************************ */

  // Specific constructors
  // Vector(argument) specifiers; // A vector with a given initial dimension
  Vector (const ulong);
  // Vector(argument) specifiers; // A vector obtained from a TraversableContainer
  Vector (const TraversableContainer<Data>&);
  // Vector(argument) specifiers; // A vector obtained from a MappableContainer
  Vector (MappableContainer<Data>&&);

  /* ************************************************************************ */

  // Copy constructor
  // Vector(argument) specifiers;
  Vector (const Vector<Data>&);

  // Move constructor
  // Vector(argument) specifiers;
  Vector (Vector<Data>&&) noexcept;
  /* ************************************************************************ */

  // Destructor
  // ~Vector() specifiers;
  virtual ~Vector() noexcept;

  /* ************************************************************************ */

  // Copy assignment
  // type operator=(argument) specifiers;
  Vector<Data>& operator=(const Vector<Data>&); //Data?

  // Move assignment
  // type operator=(argument) specifiers;
  Vector<Data>& operator=(Vector<Data>&&) noexcept; //Data?

  /* ************************************************************************ */

  // Comparison operators
  // type operator==(argument) specifiers;
  bool operator==(const Vector<Data>&) const noexcept;
  // type operator!=(argument) specifiers;
  bool operator!=(const Vector<Data>&) const noexcept;

  /* ************************************************************************ */

  // Specific member functions (inherited from MutableLinearContainer)

  // type operator[](argument) specifiers; // Override MutableLinearContainer member (must throw std::out_of_range when out of range)
  virtual Data& operator[](ulong) override;

  // type Front() specifiers; // Override MutableLinearContainer member (must throw std::length_error when empty)
  virtual Data& Front() override;

  // type Back() specifiers; // Override MutableLinearContainer member (must throw std::length_error when empty)
  virtual Data& Back() override;

  /* ************************************************************************ */

  // Specific member functions (inherited from LinearContainer)

  // type operator[](argument) specifiers; // Override LinearContainer member (must throw std::out_of_range when out of range)
  virtual const Data& operator[](ulong) const override;

  // type Front() specifiers; // Override LinearContainer member (must throw std::length_error when empty)
  virtual const Data& Front() const override;

  // type Back() specifiers; // Override LinearContainer member (must throw std::length_error when empty)
  virtual const Data& Back() const override;

  /* ************************************************************************ */

  // Specific member function (inherited from ResizableContainer)

  // type Resize(argument) specifiers; // Override ResizableContainer member
  virtual void Resize(ulong) override;

  /* ************************************************************************ */

  // Specific member function (inherited from ClearableContainer)

  // type Clear() specifiers; // Override ClearableContainer member
  virtual void Clear() override;

protected:

  // Auxiliary functions, if necessary!


};

/* ************************************************************************** */

template <typename Data>
class SortableVector : virtual public Vector<Data>,
                      virtual public SortableLinearContainer<Data> {
  // Must extend Vector<Data>,
  //             SortableLinearContainer<Data>

private:

  // ...

protected:

  using Container::size;

  // ...

public:

  // Default constructor
  // SortableVector() specifiers;
  SortableVector() = default;

  /* ************************************************************************ */

  // Specific constructors
  // SortableVector(argument) specifiers; // A vector with a given initial dimension
  SortableVector(const ulong);
  // SortableVector(argument) specifiers; // A vector obtained from a TraversableContainer
  SortableVector(const TraversableContainer<Data>&);
  // SortableVector(argument) specifiers; // A vector obtained from a MappableContainer
  SortableVector(MappableContainer<Data>&&);
  /* ************************************************************************ */

  // Copy constructor
  // SortableVector(argument) specifiers;
  SortableVector(const SortableVector<Data>&);

  // Move constructor
  // SortableVector(argument) specifiers;
  SortableVector(SortableVector<Data>&&) noexcept;

  /* ************************************************************************ */

  // Destructor
  // ~SortableVector() specifiers;
  virtual ~SortableVector() noexcept;

  /* ************************************************************************ */

  // Copy assignment
  // type operator=(argument) specifiers;
  SortableVector<Data>& operator=(const SortableVector<Data>&);

  // Move assignment
  // type operator=(argument) specifiers;
  SortableVector<Data>& operator=(SortableVector<Data>&&) noexcept;

protected:

  // Auxiliary functions, if necessary!
  // sort() already implemented in SortableLinearContainer



};

/* ************************************************************************** */

}


#include "vector.cpp"

#endif




