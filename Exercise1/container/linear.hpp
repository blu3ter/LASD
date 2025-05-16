
#ifndef LINEAR_HPP
#define LINEAR_HPP

/* ************************************************************************** */

#include "mappable.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

template <typename Data>
class LinearContainer : virtual public PreOrderTraversableContainer<Data>,
                       virtual public PostOrderTraversableContainer<Data> {
  // Must extend PreOrderTraversableContainer<Data>,
  //             PostOrderTraversableContainer<Data>

private:

  // ...

protected:

  // ...
  using Container::size;

public:

  // Destructor
  // ~LinearContainer() specifiers
  virtual ~LinearContainer() = default; 

  /* ************************************************************************ */

  // Copy assignment
  // type operator=(argument); // Copy assignment of abstract types is not possible.
  bool operator = (const LinearContainer&) = delete;

  // Move assignment
  // type operator=(argument); // Move assignment of abstract types is not possible.
  bool operator = (LinearContainer&&) = delete;

  /* ************************************************************************ */

  // Comparison operators
  // type operator==(argument) specifiers; // Comparison of abstract types is possible.
  virtual bool operator==(const LinearContainer&) const noexcept;
  // type operator!=(argument) specifiers; // Comparison of abstract types is possible.
  virtual bool operator!=(const LinearContainer&) const noexcept;

  /* ************************************************************************ */

  // Specific member functions

  // type operator[](argument) specifiers; // (non-mutable version; concrete function must throw std::out_of_range when out of range)
  virtual const Data& operator[](ulong) const = 0;

  // type Front() specifiers; // (non-mutable version; concrete function must throw std::length_error when empty)
  virtual const Data& Front() const ;

  // type Back() specifiers; // (non-mutable version; concrete function must throw std::length_error when empty)
  virtual const Data& Back() const;

  /* ************************************************************************ */

  // Specific member function (inherited from TraversableContainer)

   using typename TraversableContainer<Data>::TraverseFun;

  // type Traverse(argument) specifiers; // Override TraversableContainer member
  void Traverse(TraverseFun) const override;

  /* ************************************************************************ */

  // Specific member function (inherited from PreOrderTraversableContainer)

  // type PreOrderTraverse(argument) specifiers; // Override PreOrderTraversableContainer member
  void PreOrderTraverse(TraverseFun) const override;

  /* ************************************************************************ */

  // Specific member function (inherited from PostOrderTraversableContainer)

  // type PostOrderTraverse(argument) specifiers; // Override PostOrderTraversableContainer member
  void PostOrderTraverse(TraverseFun) const override;

};

/* ************************************************************************** */

template <typename Data>
class MutableLinearContainer : virtual public LinearContainer<Data>,
                             virtual public PostOrderMappableContainer<Data>,
                             virtual public PreOrderMappableContainer<Data> {
  // Must extend LinearContainer<Data>,
  //             PreOrderMappableContainer<Data>,
  //             PostOrderMappableContainer<Data>

private:

  // ...

protected:

  // ...
  using Container::size;

public:

  // Destructor
  // ~MutableLinearContainer() specifiers
  virtual ~MutableLinearContainer() = default;

  /* ************************************************************************ */

  // Copy assignment
  // type operator=(argument); // Copy assignment of abstract types is not possible.
  MutableLinearContainer& operator=(const MutableLinearContainer&) = delete;

  // Move assignment
  // type operator=(argument); // Move assignment of abstract types is not possible.
  MutableLinearContainer& operator=(MutableLinearContainer&&) = delete;

  /* ************************************************************************ */

  // Specific member functions

  // type operator[](argument) specifiers; // (mutable version; concrete function must throw std::out_of_range when out of range)
  virtual Data& operator[](ulong) = 0;

  // type Front() specifiers; // (mutable version; concrete function must throw std::length_error when empty)
  virtual Data& Front();

  // type Back() specifiers; // (mutable version; concrete function must throw std::length_error when empty)
  virtual Data& Back();

  /* ************************************************************************ */

  // Specific member function (inherited from MappableContainer)

  using typename MappableContainer<Data>::MapFun;

  // type Map(argument) specifiers; // Override MappableContainer member
  virtual void Map(MapFun) override;

  /* ************************************************************************ */

  // Specific member function (inherited from PreOrderMappableContainer)

  // type PreOrderMap(argument) specifiers; // Override PreOrderMappableContainer member
  virtual void PreOrderMap(MapFun) override;

  /* ************************************************************************ */

  // Specific member function (inherited from PostOrderMappableContainer)

  // type PostOrderMap(argument) specifiers; // Override PostOrderMappableContainer member
  virtual void PostOrderMap(MapFun) override;

};

template <typename Data>
class SortableLinearContainer : virtual public MutableLinearContainer<Data> {
  // Must extend MutableLinearContainer<Data>

private:

  // ...

protected:

  // ...
  using Container::size;

public:

  // Destructor
  // ~SortableLinearContainer() specifiers
  virtual ~SortableLinearContainer() = default;

  /* ************************************************************************ */

  // Copy assignment
  // type operator=(argument); // Copy assignment of abstract types is not possible.
  SortableLinearContainer& operator=(const SortableLinearContainer&) = delete;

  // Move assignment
  // type operator=(argument); // Move assignment of abstract types is not be possible.
  SortableLinearContainer& operator=(SortableLinearContainer&&) = delete;
  
  /* ************************************************************************ */

  // Specific member function

  // type Sort() specifiers;
  virtual void Sort() noexcept;

protected:

 // Auxiliary member functions, if necessary!
 virtual void InsertionSort() noexcept;
  // ...

};

/* ************************************************************************** */

}

#endif

#include "linear.cpp"


