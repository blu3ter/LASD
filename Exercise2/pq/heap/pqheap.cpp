#include <stdexcept>

namespace lasd {

/* ************************************************************************** */

// Specific constructors

// Constructor from TraversableContainer
template <typename Data>
PQHeap<Data>::PQHeap(const TraversableContainer<Data>& container) : HeapVec<Data>(container) {
    // HeapVec constructor already calls Heapify(), so we have a valid heap
    capacity = size; // Initial capacity equals size
}

// Constructor from MappableContainer (move)
template <typename Data>
PQHeap<Data>::PQHeap(MappableContainer<Data>&& container) : HeapVec<Data>(std::move(container)) {
    // HeapVec constructor already calls Heapify(), so we have a valid heap
    capacity = size; // Initial capacity equals size
}

/* ************************************************************************** */

// Copy constructor
template <typename Data>
PQHeap<Data>::PQHeap(const PQHeap<Data>& other) : HeapVec<Data>(other) {
    // HeapVec copy constructor preserves heap property
    capacity = other.capacity;
}

// Move constructor
template <typename Data>
PQHeap<Data>::PQHeap(PQHeap<Data>&& other) noexcept : HeapVec<Data>(std::move(other)) {
    // HeapVec move constructor preserves heap property
    capacity = other.capacity;
    other.capacity = 0;
}

/* ************************************************************************** */

// Copy assignment
template <typename Data>
PQHeap<Data>& PQHeap<Data>::operator=(const PQHeap<Data>& other) {
    HeapVec<Data>::operator=(other);
    capacity = other.capacity;
    return *this;
}

// Move assignment
template <typename Data>
PQHeap<Data>& PQHeap<Data>::operator=(PQHeap<Data>&& other) noexcept {
    HeapVec<Data>::operator=(std::move(other));
    capacity = other.capacity;
    other.capacity = 0;
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
    
    // Reduce size (but keep capacity for future insertions)
    Container::size = size - 1;
    
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
    // Initialize capacity if this is the first insertion
    if (capacity == 0) {
        InitializeCapacity();
    }
    
    // Ensure we have enough capacity for one more element
    EnsureCapacity(size + 1);
    
    // Increase size by one (but don't reallocate since we have capacity)
    if (size + 1 <= capacity) {
        // Just increment size without reallocating
        Container::size = size + 1;
    } else {
        // This should not happen due to EnsureCapacity, but safety fallback
        Resize(size + 1);
    }
    
    // Insert new element at the end
    Elements[size - 1] = element;
    
    // Restore heap property by moving the new element up
    HeapifyUp(size - 1);
}

// Insert function - inserts a new element (move version)
template <typename Data>
void PQHeap<Data>::Insert(Data&& element) {
    // Initialize capacity if this is the first insertion
    if (capacity == 0) {
        InitializeCapacity();
    }
    
    // Ensure we have enough capacity for one more element
    EnsureCapacity(size + 1);
    
    // Increase size by one (but don't reallocate since we have capacity)
    if (size + 1 <= capacity) {
        // Just increment size without reallocating
        Container::size = size + 1;
    } else {
        // This should not happen due to EnsureCapacity, but safety fallback
        Resize(size + 1);
    }
    
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

// Capacity management functions

// Ensures that the heap has at least the required capacity
template <typename Data>
void PQHeap<Data>::EnsureCapacity(ulong requiredSize) {
    if (requiredSize > capacity) {
        // Calculate new capacity: double the current capacity or use DEFAULT_CAPACITY
        ulong newCapacity = (capacity == 0) ? DEFAULT_CAPACITY : capacity;
        while (newCapacity < requiredSize) {
            newCapacity *= 2;
        }
        
        // Store current size
        ulong currentSize = size;
        
        // Resize to new capacity (this will change size to newCapacity)
        Resize(newCapacity);
        
        // Restore original size but keep the larger capacity
        Container::size = currentSize;
        capacity = newCapacity;
    }
}

// Initialize capacity for default constructor
template <typename Data>
void PQHeap<Data>::InitializeCapacity() {
    if (capacity == 0 && size == 0) {
        // Allocate initial capacity but keep size at 0
        Resize(DEFAULT_CAPACITY);
        Container::size = 0;
        capacity = DEFAULT_CAPACITY;
    }
}

/* ************************************************************************** */

}
