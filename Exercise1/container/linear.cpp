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
void SortableLinearContainer<Data>::Sort(){
    // Insertion sort
      for (ulong i = 1; i < size; i++) {
        Data key = std::move(operator[](i));
        long j = i - 1;
        while (j >= 0 && operator[](j) > key) {
            operator[](j + 1) = std::move(operator[](j));
            j--;
        }
        operator[](j + 1) = std::move(key);
    }
}

}

#endif
