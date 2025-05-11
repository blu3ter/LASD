
#ifndef TESTABLE_HPP
#define TESTABLE_HPP

/* ************************************************************************** */

#include "container.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

template <typename Data>
class TestableContainer : virtual public Container {
  // Must extend Container

private:

  // ...

protected:

  // ...

public:

  // Destructor
virtual ~TestableContainer() = default;

  // ~TestableContainer() specifiers

  /* ************************************************************************ */

  // Copy assignment
  // type operator=(argument); // Copy assignment of abstract types is not possible.
  TeTestableContainer & operator=(const TestableContainer&) = delete

  // Move assignment
  // type operator=(argument); // Move assignment of abstract types is not possible.
  TestableContainer & operator=(TestableContainer&&) = delete;


  /* ************************************************************************ */

  // Comparison operators
  // type operator==(argument) specifiers; // Comparison of abstract types is not possible.
  bool operator==(const TestableContrainer&) const noexcept =  delete;
  // type operator!=(argument) specifiers; // Comparison of abstract types is not possible.
  bool operator!=(const TestableContainer&) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member function

  // type Exists(argument) specifiers; // (concrete function should not throw exceptions)
  virtual bool Exists(const Data&) const noexcept = 0;

};

/* ************************************************************************** */

}

#endif
