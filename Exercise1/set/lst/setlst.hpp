
#ifndef SETLST_HPP
#define SETLST_HPP

/* ************************************************************************** */

#include "../set.hpp"
#include "../../list/list.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

template <typename Data>
class SetLst : virtual public Set<Data>,
                virtual public List<Data> {
  // Must extend Set<Data>,
  //             List<Data>

private:

  // ...

protected:

  using Container::size;
  // Nota: SetLst eredita da List<Data>, quindi i membri di List (head, tail, size, ecc.)
  // sono accessibili direttamente o tramite this->. Non è necessario un m_list separato.

protected:
  // Auxiliary member functions

  // Simulates a "search" to find an element or its correct insertion point.
  // Returns: current node (or where it should be if not found).
  // Sets:
  //   found: true if val is found, false otherwise.
  //   prev_node: node before current_node (nullptr if current_node is head or list is empty/val is new head).
  //   current_node_ptr: pointer to the node containing val, or where val should be inserted after prev_node.
  // This is a linear scan, as binary search is not efficient for linked lists.
  // The name BinarySearch in the original hpp is kept for consistency with the request,
  // but the actual helper will be FindNode for clarity.
  

public:

  // Default constructor
  // SetLst() specifiers;
  SetLst() = default;

  /* ************************************************************************ */

  // Specific constructors
  // SetLst(argument) specifiers; // A set obtained from a TraversableContainer
  SetLst(const TraversableContainer<Data>&); 
  // SetLst(argument) specifiers; // A set obtained from a MappableContainer
  SetLst(MappableContainer<Data>&&);

  /* ************************************************************************ */

  // Copy constructor
  // SetLst(argument) specifiers;
  SetLst(const SetLst<Data>&);

  // Move constructor
  // SetLst(argument) specifiers;
  SetLst(SetLst<Data>&&) noexcept;

  /* ************************************************************************ */

  // Destructor
  // ~SetLst() specifiers;
  virtual ~SetLst() = default;

  /* ************************************************************************ */

  // Copy assignment
  // type operator=(argument) specifiers;
  SetLst<Data>& operator=(const SetLst<Data>&) = default;

  // Move assignment
  // type operator=(argument) specifiers;
  SetLst<Data>& operator=(SetLst<Data>&&) noexcept;

  /* ************************************************************************ */

  // Comparison operators
  // type operator==(argument) specifiers;
  bool operator==(const SetLst<Data>&) const noexcept;
  // type operator!=(argument) specifiers;
  bool operator!=(const SetLst<Data>&) const noexcept;

  /* ************************************************************************ */

  // Specific member functions (inherited from OrderedDictionaryContainer)

  // type Min(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)
  virtual const Data& Min() const override;
  // type MinNRemove(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)
  virtual Data MinNRemove() override;
  // type RemoveMin(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)
  virtual void RemoveMin() override;

  // type Max(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)
  virtual const Data& Max() const override;
  // type MaxNRemove(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)
  virtual Data MaxNRemove() override;
  // type RemoveMax(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)
  virtual void RemoveMax() override;

  // type Predecessor(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
  virtual const Data& Predecessor(const Data&) const override;  //TODO forse va const prima di Data&
  // type PredecessorNRemove(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
  virtual Data PredecessorNRemove(const Data&) override;
  // type RemovePredecessor(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
  virtual void RemovePredecessor(const Data&) override;

  // type Successor(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
  virtual const Data& Successor(const Data&) const override;  //TODO forse va const prima di Data&
  // type SuccessorNRemove(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
  virtual Data SuccessorNRemove(const Data&) override;
  // type RemoveSuccessor(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
  virtual void RemoveSuccessor(const Data&) override;

  /* ************************************************************************ */

  // Specific member functions (inherited from DictionaryContainer)

  // type Insert(argument) specifiers; // Override DictionaryContainer member (copy of the value)
  virtual bool Insert(const Data&) override;
  // type Insert(argument) specifiers; // Override DictionaryContainer member (move of the value)
  virtual bool Insert(Data&&) noexcept override;
  // type Remove(argument) specifiers; // Override DictionaryContainer member
  virtual bool Remove(const Data&) override;

  /* ************************************************************************ */

  // Specific member functions (inherited from LinearContainer)

  // type operator[](argument) specifiers; // Override LinearContainer member (must throw std::out_of_range when out of range)
  virtual Data& operator[](unsigned long) override;
  virtual const Data& operator[](unsigned long) const override;

  // type Front() specifiers; // Override LinearContainer member (must throw std::length_error when empty)
  virtual Data& Front() override;
  virtual const Data& Front() const override;

  // type Back() specifiers; // Override LinearContainer member (must throw std::length_error when empty)
  virtual Data& Back() override;
  virtual const Data& Back() const override;

  /* ************************************************************************** */

  // Specific member function (inherited from TestableContainer)

  // type Exists(argument) specifiers; // Override TestableContainer member
  virtual bool Exists(const Data&) const noexcept override;

  /* ************************************************************************ */

  // Specific member function (inherited from ClearableContainer)

  // type Clear() specifiers; // Override ClearableContainer member
  virtual void Clear() noexcept override;

protected:

  // Auxiliary functions, if necessary!
  void FindNode(const Data& val, bool& found, typename List<Data>::Node*& prev_node, typename List<Data>::Node*& current_node) const;


};

/* ************************************************************************** */

}

#include "setlst.cpp"

#endif
