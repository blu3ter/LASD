
#ifndef MAPPABLE_HPP
#define MAPPABLE_HPP

/* ************************************************************************** */

#include <functional>

/* ************************************************************************** */

#include "traversable.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

template <typename Data>
class MappableContainer : virtual public TraversableContainer<Data> {
  // Must extend TraversableContainer<Data>

private:

  // ...

protected:

  // ...
  

public:

  // Destructor
  virtual ~MappableContainer() = default;
  // ~MappableContainer() specifiers

  /* ************************************************************************ */

  // Copy assignment
  // type operator=(argument); // Copy assignment of abstract types is not possible.
  MappableContainer & operator=(const MappableContainer &) = delete;

  // Move assignment
  // type operator=(argument); // Move assignment of abstract types is not possible.   
   MappableContainer & operator=(MappableContainer &&) = delete;
  
  /* ************************************************************************ */

  // Comparison operators
  // type operator==(argument) specifiers; // Comparison of abstract types is not possible.
  bool operator==(const MappableContainer &) const noexcept = delete;
  // type operator!=(argument) specifiers; // Comparison of abstract types is not possible.
  bool operator!=(const MappableContainer &) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member function

  using MapFun = std::function<void(Data &)>;

  // type Map(argument) specifiers;
  virtual void Map(MapFun) = 0;

};

/* ************************************************************************** */

template <typename Data>
class PreOrderMappableContainer : virtual public MappableContainer<Data>, 
                                virtual public PreOrderTraversableContainer<Data> {
  // Must extend MappableContainer<Data>,
  //             PreOrderTraversableContainer<Data>

private:

  // ...

protected:

  // ...
  

public: 

  // Destructor
  // ~PreOrderMappableContainer() specifiers
  virtual ~PreOrderMappableContainer() = default;
  /* ************************************************************************ */

  // Copy assignment
  // type operator=(argument); // Copy assignment of abstract types is not possible.
  PreOrderMappableContainer & operator=(const PreOrderMappableContainer &) = delete;
  // Move assignment
  // type operator=(argument); // Move assignment of abstract types is not possible.
  PreOrderMappableContainer & operator=(PreOrderMappableContainer &&) = delete;

  /* ************************************************************************ */

  // Comparison operators
  // type operator==(argument) specifiers; // Comparison of abstract types is not possible.
  bool operator==(const PreOrderMappableContainer &) const noexcept = delete;

  // type operator!=(argument) specifiers; // Comparison of abstract types is not possible.
  bool operator!=(const PreOrderMappableContainer &) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member function

   using typename MappableContainer<Data>::MapFun;

  // type PreOrderMap(argument) specifiers; 
  virtual void PreOrderMap(MapFun) = 0;

  /* ************************************************************************ */

  // Specific member function (inherited from MappableContainer)

  // type Map(argument) specifiers; // Override MappableContainer member
  virtual void Map(MapFun) override;

};

/* ************************************************************************** */

template <typename Data>
class PostOrderMappableContainer :virtual public MappableContainer<Data>,
                                 virtual public PostOrderTraversableContainer<Data> {
  // Must extend MappableContainer<Data>,
  //             PostOrderTraversableContainer<Data>

private:

  // ...

protected:

  // ...

public:

  // Destructor
  // ~PostOrderMappableContainer() specifiers
  virtual ~PostOrderMappableContainer() = default;

  /* ************************************************************************ */

  // Copy assignment
  // type operator=(argument); // Copy assignment of abstract types is not possible.
  PostOrderMappableContainer & operator=(const PostOrderMappableContainer &) = delete;

  // Move assignment
  // type operator=(argument); // Move assignment of abstract types is not possible.
  PostOrderMappableContainer & operator=(PostOrderMappableContainer &&) = delete;

  /* ************************************************************************ */

  // Comparison operators
  // type operator==(argument) specifiers; // Comparison of abstract types is not possible.
  bool operator==(const PostOrderMappableContainer &) const noexcept = delete;
  // type operator!=(argument) specifiers; // Comparison of abstract types is not possible.
  bool operator!=(const PostOrderMappableContainer &) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member function

   using typename MappableContainer<Data>::MapFun;

  // type PostOrderMap(argument) specifiers;
  virtual void PostOrderMap(Mapfun) = 0;

  /* ************************************************************************ */

  // Specific member function (inherited from MappableContainer)

  // type Map(argument) specifiers; // Override MappableContainer member
 virtual void Map(MapFun) const override;
  

};

/* ************************************************************************** */

}

#include "mappable.cpp"

#endif
