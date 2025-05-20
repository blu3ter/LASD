
#ifndef SETVEC_HPP
#define SETVEC_HPP

/* ************************************************************************** */

#include "../set.hpp"
#include "../../vector/vector.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

template <typename Data>
class SetVec : virtual public Set<Data>,
                virtual public Vector<Data> {
  // Must extend Set<Data>,
  //             ResizableContainer

private:

  // ...

protected:

   using Container::size;

  // ...

public:

  // Default constructor
  // SetVec() specifiers;
  SetVec() = default;

  /* ************************************************************************ */

  // Specific constructors
  // SetVec(argument) specifiers; // A set obtained from a TraversableContainer
  SetVec(const TraversableContainer<Data>&);
  // SetVec(argument) specifiers; // A set obtained from a MappableContainer
  SetVec(MappableContainer<Data>&&);

  /* ************************************************************************ */

  // Copy constructor
  // SetVec(argument) specifiers;
  SetVec(const SetVec<Data>&);

  // Move constructor
  // SetVec(argument) specifiers;
  SetVec(SetVec<Data>&&) noexcept;

  /* ************************************************************************ */

  // Destructor
  // ~SetVec() specifiers;
  virtual ~SetVec() = default;

  /* ************************************************************************ */

  // Copy assignment
  // type operator=(argument) specifiers;
  SetVec<Data>& operator=(const SetVec<Data>&);

  // Move assignment
  // type operator=(argument) specifiers;
  SetVec<Data>& operator=(SetVec<Data>&&) noexcept;

  /* ************************************************************************ */

  // Comparison operators
  // type operator==(argument) specifiers;
  bool operator==(const SetVec<Data>&) const noexcept;
  // type operator!=(argument) specifiers;
  bool operator!=(const SetVec<Data>&) const noexcept;

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
  virtual const Data& Predecessor(const Data&) const override;
  // type PredecessorNRemove(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
  virtual Data PredecessorNRemove(const Data&) override;
  // type RemovePredecessor(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
  virtual void RemovePredecessor(const Data&) override;

  // type Successor(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
  virtual const Data& Successor(const Data&) const override;
  // type SuccessorNRemove(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
  virtual Data SuccessorNRemove(const Data&) override;
  // type RemoveSuccessor(argument) specifiers; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
  virtual void RemoveSuccessor(const Data&) override;

  /* ************************************************************************ */

  // Specific member functions (inherited from DictionaryContainer)

  // type Insert(argument) specifiers; // Override DictionaryContainer member (copy of the value)
  virtual bool Insert(const Data&) override;
  // type Insert(argument) specifiers; // Override DictionaryContainer member (move of the value)
  virtual bool Insert(Data&&) override;
  // type Remove(argument) specifiers; // Override DictionaryContainer member
  virtual bool Remove(const Data&) override;

  /* ************************************************************************ */

  // Specific member functions (inherited from LinearContainer)

  // type operator[](argument) specifiers; // Override LinearContainer member (must throw std::out_of_range when out of range)
  virtual Data& operator[](ulong) override;

  /* ************************************************************************** */

  // Specific member function (inherited from TestableContainer)

  // type Exists(argument) specifiers; // Override TestableContainer member
  virtual bool Exists(const Data&) const noexcept override;

  /* ************************************************************************ */

  // Specific member function (inherited from ClearableContainer)

  // type Clear() specifiers; // Override ClearableContainer member
  virtual void Clear() override;

protected:

  // Auxiliary functions, if necessary!

};

/* ************************************************************************** */

}

#include "setvec.cpp"

#endif