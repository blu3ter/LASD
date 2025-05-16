#ifndef LINEARCONTAINER_CPP
#define LINEARCONTAINER_CPP

#include "linear.hpp"

namespace lasd {

/* ************************************************************************** */

// LinearContainer

template<typename Data>
inline const Data& LinearContainer<Data>::Front() const {
    return operator[](0);
}

template<typename Data>
inline const Data& LinearContainer<Data>::Back() const {
    return operator[](size - 1);
}

template<typename Data>
void LinearContainer<Data>::Traverse(const typename TraversableContainer<Data>::TraverseFun fun) const {
    PreOrderTraverse(fun);
}

template<typename Data>
void LinearContainer<Data>::PreOrderTraverse(const typename TraversableContainer<Data>::TraverseFun fun) const {
    for(ulong i = 0; i < size; ++i) {
        fun(operator[](i));
    }
}

template<typename Data>
void LinearContainer<Data>::PostOrderTraverse(const typename TraversableContainer<Data>::TraverseFun fun) const {
    for(ulong i = size; i > 0; --i) {
        fun(operator[](i-1));
    }
}

template <typename Data>
bool LinearContainer<Data>::operator==(const LinearContainer<Data>& other) const noexcept {
  if (this->size != other.size) {
    return false;
  }
  for (unsigned long i = 0; i < this->size; ++i) {
    if (this->operator[](i) != other.operator[](i)) { 
      return false;
    }
  }
  return true;
}

template <typename Data>
bool LinearContainer<Data>::operator!=(const LinearContainer<Data>& other) const noexcept {
  return !(*this == other);
}

/* ************************************************************************** */

// MutableLinearContainer

template<typename Data>
inline Data& MutableLinearContainer<Data>::Front() {
    return operator[](0);
}

template<typename Data>
inline Data& MutableLinearContainer<Data>::Back() {
    return operator[](size - 1);
}

template<typename Data>
void MutableLinearContainer<Data>::Map(const typename MappableContainer<Data>::MapFun fun) {
    PreOrderMap(fun);
}

template<typename Data>
void MutableLinearContainer<Data>::PreOrderMap(const typename MappableContainer<Data>::MapFun fun) {
    for(ulong i = 0; i < size; ++i) {
        fun(operator[](i));
    }
}

template<typename Data>
void MutableLinearContainer<Data>::PostOrderMap(const typename MappableContainer<Data>::MapFun fun) {
    for(ulong i = size; i > 0; --i) {
        fun(operator[](i-1));
    }
}

/* ************************************************************************** */

// SortableLinearContainer

template<typename Data>
void SortableLinearContainer<Data>::Sort() noexcept{
    InsertionSort();
}

template<typename Data>
void SortableLinearContainer<Data>::InsertionSort() noexcept{
    for(ulong i = 1; i < this->size; ++i) { 
        Data temp = this->operator[](i);
        ulong j = i;
        while (j > 0 && this->operator[](j - 1) > temp) { 
            this->operator[](j) = this->operator[](j - 1); 
            --j;
        }
        this->operator[](j) = temp; 
    }
}


} // namespace lasd

#endif
