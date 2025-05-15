
#ifndef LIST_HPP
#define LIST_HPP

/* ************************************************************************** */

#include "../container/linear.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

template <typename Data>
class List : virtual public MutableLinearContainer<Data>,
             virtual public ClearableContainer {
  // Must extend MutableLinearContainer<Data>,
  //             ClearableContainer

private:

  // ...

protected:

  using Container::size;

  struct Node {

    // Data
    Data value;
    Node* next = nullptr;
    // ...

    /* ********************************************************************** */

    // Specific constructors
    Node() = default;
    Node(const Data& dat) : value(dat) {};
    Node(Data&& dat) noexcept : value(std::move(dat)) {};
    // ...

    /* ********************************************************************** */

    // Copy constructor
    Node(const Node& nod) : value(nod.value){}; 
    // ...

    // Move constructor
    Node(Node&&) noexcept;
    // ...

    /* ********************************************************************** */

    // Destructor
    virtual ~Node() noexcept = default;
    // ...

    /* ********************************************************************** */

    // Comparison operators
    bool operator == (const Node&) const noexcept = delete;
    bool operator != (const Node&) const noexcept = delete;
    // ...

    /* ********************************************************************** */

    // Specific member functions

    // ...

  };

    Node * head = nullptr;
    Node * tail = nullptr;
  // ...

public:

  // Default constructor
  // List() specifiers;
  List() = default;

  /* ************************************************************************ */

  // Specific constructor
  // List(argument) specifiers; // A list obtained from a TraversableContainer
  List(const TraversableContainer<Data>&);
  // List(argument) specifiers; // A list obtained from a MappableContainer
  List(MappableContainer<Data>&&);

  /* ************************************************************************ */

  // Copy constructor
  // List(argument) specifiers;
  List(const List<Data>&);

  // Move constructor
  // List(argument) specifiers;
  List(List<Data>&&) noexcept;

  /* ************************************************************************ */

  // Destructor
  // ~List() specifiers;
  virtual ~List() noexcept;

  /* ************************************************************************ */

  // Copy assignment
  // type operator=(argument) specifiers;
  List<Data>& operator=(const List<Data>&);

  // Move assignment
  // type operator=(argument) specifiers;
  List<Data>& operator=(List<Data>&&) noexcept;

  /* ************************************************************************ */

  // Comparison operators
  // type operator==(argument) specifiers;
  bool operator==(const List<Data>&) const noexcept;
  // type operator!=(argument) specifiers;
  bool operator!=(const List<Data>&) const noexcept;

  /* ************************************************************************ */

  // Specific member functions

  // type InsertAtFront(argument) specifier; // Copy of the value
  virtual void InsertAtFront(const Data&);
  // type InsertAtFront(argument) specifier; // Move of the value
  virtual void InsertAtFront(Data&&);
  // type RemoveFromFront() specifier; // (must throw std::length_error when empty)
  virtual void RemoveFromFront();
  // type FrontNRemove() specifier; // (must throw std::length_error when empty)
  virtual Data FrontNRemove();

  // type InsertAtBack(argument) specifier; // Copy of the value
  virtual void InsertAtBack(const Data&);
  // type InsertAtBack(argument) specifier; // Move of the value
  virtual void InsertAtBack(Data&&);
  // type RemoveFromBack() specifier; // (must throw std::length_error when empty)
  virtual void RemoveFromBack();
  // type BackNRemove() specifier; // (must throw std::length_error when empty)
  virtual Data BackNRemove();

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

  /* ************************************************************************ */

  // Specific member function (inherited from TraversableContainer)

   using typename TraversableContainer<Data>::TraverseFun;

  // type Traverse(arguments) specifiers; // Override TraversableContainer member
  virtual void Traverse(TraverseFun) const override;

  /* ************************************************************************ */

  // Specific member function (inherited from PreOrderTraversableContainer)

  // type PreOrderTraverse(arguments) specifiers; // Override PreOrderTraversableContainer member
  virtual void PreOrderTraverse(TraverseFun) const override;

  /* ************************************************************************ */

  // Specific member function (inherited from PostOrderTraversableContainer)

  // type PostOrderTraverse(arguments) specifiers; // Override PostOrderTraversableContainer member
  virtual void PostOrderTraverse(TraverseFun) const override;

  /* ************************************************************************ */

  // Specific member function (inherited from ClearableContainer)

  // type Clear() specifiers; // Override ClearableContainer member
  void Clear() override;
protected:

  // Auxiliary functions, if necessary!

};

/* ************************************************************************** */

}

#include "list.cpp"

#endif
