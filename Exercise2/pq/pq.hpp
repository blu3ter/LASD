
#ifndef PQ_HPP
#define PQ_HPP

/* ************************************************************************** */

#include "../container/linear.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

template <typename Data>
class PQ : virtual public LinearContainer<Data>,
           virtual public ClearableContainer {
  // Must extend LinearContainer<Data>,
  //             ClearableContainer

private:

protected:

public:

  // Destructor
  // ~PQ() specifiers
  virtual ~PQ() = default;

  /* ************************************************************************ */

  // Copy assignment
  // type operator=(argument); // Copy assignment of abstract types is not possible.
  PQ& operator=(const PQ&) = delete;

  // Move assignment
  // type operator=(argument); // Move assignment of abstract types is not possible.
  PQ& operator=(PQ&&) noexcept = delete;

  /* ************************************************************************ */

  // Specific member functions

  // type Tip(argument) specifiers; // (concrete function must throw std::length_error when empty)
  virtual const Data& Tip() const = 0;
  // type RemoveTip(argument) specifiers; // (concrete function must throw std::length_error when empty)
  virtual void RemoveTip() = 0;
  // type TipNRemove(argument) specifiers; // (concrete function must throw std::length_error when empty)
  virtual Data TipNRemove() = 0;

  // type Insert(argument) specifiers; // Copy of the value
  virtual void Insert(const Data&) = 0;
  // type Insert(argument) specifiers; // Move of the value
  virtual void Insert(Data&&) = 0;

  // type Change(argument) specifiers; // Copy of the value
  virtual void Change(ulong, const Data&) = 0;
  // type Change(argument) specifiers; // Move of the value
  virtual void Change(ulong, Data&&) = 0;

};

/* ************************************************************************** */

}

#endif
