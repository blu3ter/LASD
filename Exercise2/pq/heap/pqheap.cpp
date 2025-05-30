#include <stdexcept>

namespace lasd {

/* ************************************************************************** */

// Specific constructors

// Constructor from TraversableContainer
template <typename Data>
PQHeap<Data>::PQHeap(const TraversableContainer<Data>& container) : HeapVec<Data>(container) {
    // HeapVec constructor already calls Heapify(), so we have a valid heap
}

// Constructor from MappableContainer (move)
template <typename Data>
PQHeap<Data>::PQHeap(MappableContainer<Data>&& container) : HeapVec<Data>(std::move(container)) {
    // HeapVec constructor already calls Heapify(), so we have a valid heap
}

/* ************************************************************************** */

// Copy constructor
template <typename Data>
PQHeap<Data>::PQHeap(const PQHeap<Data>& other) : HeapVec<Data>(other) {
    // HeapVec copy constructor preserves heap property
}

// Move constructor
template <typename Data>
PQHeap<Data>::PQHeap(PQHeap<Data>&& other) noexcept : HeapVec<Data>(std::move(other)) {
    // HeapVec move constructor preserves heap property
}

/* ************************************************************************** */

// Copy assignment
template <typename Data>
PQHeap<Data>& PQHeap<Data>::operator=(const PQHeap<Data>& other) {
    HeapVec<Data>::operator=(other);
    return *this;
}

// Move assignment
template <typename Data>
PQHeap<Data>& PQHeap<Data>::operator=(PQHeap<Data>&& other) noexcept {
    HeapVec<Data>::operator=(std::move(other));
    return *this;
}

/* ************************************************************************** */

// Specific member functions (inherited from PQ)

// Tip function - returns the maximum element (root) without removing it
template <typename Data>
const Data& PQHeap<Data>::Tip() const {
    if (size == 0) {
        throw std::length_error("Priority queue is empty");
    }
    return Elements[0]; // Root element has maximum priority
}

// RemoveTip function - removes the maximum element
template <typename Data>
void PQHeap<Data>::RemoveTip() {
    if (size == 0) {
        throw std::length_error("Priority queue is empty");
    }
    
    // Move last element to root position
    Elements[0] = Elements[size - 1];
    
    // Reduce size
    Resize(size - 1);
    
    // Restore heap property by moving the new root down
    if (size > 0) {
        HeapifyDown(0);
    }
}

// TipNRemove function - returns and removes the maximum element
template <typename Data>
Data PQHeap<Data>::TipNRemove() {
    if (size == 0) {
        throw std::length_error("Priority queue is empty");
    }
    
    // Store the maximum element to return
    Data maxElement = Elements[0];
    
    // Remove the tip using RemoveTip
    RemoveTip();
    
    return maxElement;
}

// Insert function - inserts a new element (copy version)
template <typename Data>
void PQHeap<Data>::Insert(const Data& element) {
    // Increase size by one
    Resize(size + 1);
    
    // Insert new element at the end
    Elements[size - 1] = element;
    
    // Restore heap property by moving the new element up
    HeapifyUp(size - 1);
}

// Insert function - inserts a new element (move version)
template <typename Data>
void PQHeap<Data>::Insert(Data&& element) {
    // Increase size by one
    Resize(size + 1);
    
    // Move new element to the end
    Elements[size - 1] = std::move(element);
    
    // Restore heap property by moving the new element up
    HeapifyUp(size - 1);
}

// Change function - changes element at given index (copy version)
template <typename Data>
void PQHeap<Data>::Change(ulong index, const Data& newValue) {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    
    Data oldValue = Elements[index];
    Elements[index] = newValue;
    
    // Restore heap property - we need to check both directions
    if (newValue > oldValue) {
        // If new value is greater, it might need to go up
        HeapifyUp(index);
    } else if (newValue < oldValue) {
        // If new value is smaller, it might need to go down
        HeapifyDown(index);
    }
    // If newValue == oldValue, no heap property restoration needed
}

// Change function - changes element at given index (move version)
template <typename Data>
void PQHeap<Data>::Change(ulong index, Data&& newValue) {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    
    Data oldValue = Elements[index];
    Elements[index] = std::move(newValue);
    
    // Restore heap property - we need to check both directions
    if (Elements[index] > oldValue) {
        // If new value is greater, it might need to go up
        HeapifyUp(index);
    } else if (Elements[index] < oldValue) {
        // If new value is smaller, it might need to go down
        HeapifyDown(index);
    }
    // If newValue == oldValue, no heap property restoration needed
}

/* ************************************************************************** */

}
