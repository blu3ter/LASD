namespace lasd {

/* ************************************************************************** */

// Specific constructors

// Constructor from TraversableContainer
template <typename Data>
HeapVec<Data>::HeapVec(const TraversableContainer<Data>& container) : SortableVector<Data>(container) {
    Heapify();
}

// Constructor from MappableContainer (move)
template <typename Data>
HeapVec<Data>::HeapVec(MappableContainer<Data>&& container) : SortableVector<Data>(std::move(container)) {
    Heapify();
}

/* ************************************************************************** */

// Copy constructor
template <typename Data>
HeapVec<Data>::HeapVec(const HeapVec<Data>& other) : SortableVector<Data>(other) {
    // No need to heapify since other is already a heap
}

// Move constructor
template <typename Data>
HeapVec<Data>::HeapVec(HeapVec<Data>&& other) noexcept : SortableVector<Data>(std::move(other)) {
    // No need to heapify since other is already a heap
}

/* ************************************************************************** */

// Copy assignment
template <typename Data>
HeapVec<Data>& HeapVec<Data>::operator=(const HeapVec<Data>& other) {
    SortableVector<Data>::operator=(other);
    // No need to heapify since other is already a heap
    return *this;
}

// Move assignment
template <typename Data>
HeapVec<Data>& HeapVec<Data>::operator=(HeapVec<Data>&& other) noexcept {
    SortableVector<Data>::operator=(std::move(other));
    // No need to heapify since other is already a heap
    return *this;
}

/* ************************************************************************** */

// Comparison operators
template <typename Data>
bool HeapVec<Data>::operator==(const HeapVec<Data>& other) const noexcept {
    return SortableVector<Data>::operator==(other);
}

template <typename Data>
bool HeapVec<Data>::operator!=(const HeapVec<Data>& other) const noexcept {
    return !(*this == other);
}

/* ************************************************************************** */

// Specific member functions (inherited from Heap)

// IsHeap function - verifies if the current array satisfies max-heap property
template <typename Data>
bool HeapVec<Data>::IsHeap() const noexcept {
    for (ulong i = 0; i < size; ++i) {
        ulong leftChild = LeftChild(i);
        ulong rightChild = RightChild(i);
        
        // Check if left child exists and violates max-heap property
        if (leftChild < size && Elements[i] < Elements[leftChild]) {
            return false;
        }
        
        // Check if right child exists and violates max-heap property
        if (rightChild < size && Elements[i] < Elements[rightChild]) {
            return false;
        }
    }
    return true;
}

// Heapify function - transforms the array into a max-heap
template <typename Data>
void HeapVec<Data>::Heapify() {
    // Start from the last non-leaf node and heapify down
    if (size <= 1) return;
    
    // First non-leaf node is at index (size-2)/2
    for (long i = (size - 2) / 2; i >= 0; --i) {
        HeapifyDown(i);
    }
}

/* ************************************************************************** */

// Sort function - HeapSort implementation
template <typename Data>
void HeapVec<Data>::Sort() noexcept {
    // First ensure we have a valid heap
    Heapify();
    
    // Extract elements one by one from heap
    for (ulong i = size - 1; i > 0; --i) {
        // Move current root (maximum) to end
        std::swap(Elements[0], Elements[i]);
        
        // Temporarily reduce heap size and heapify root
        ulong originalSize = size;
        size = i; // Reduce heap size
        HeapifyDown(0);
        size = originalSize; // Restore original size
    }
}

/* ************************************************************************** */

// Auxiliary functions

// Calculate left child index
template <typename Data>
ulong HeapVec<Data>::LeftChild(ulong index) const noexcept {
    return 2 * index + 1;
}

// Calculate right child index
template <typename Data>
ulong HeapVec<Data>::RightChild(ulong index) const noexcept {
    return 2 * index + 2;
}

// Calculate parent index
template <typename Data>
ulong HeapVec<Data>::Parent(ulong index) const noexcept {
    return (index - 1) / 2;
}

// HeapifyUp - maintains max-heap property going upward
template <typename Data>
void HeapVec<Data>::HeapifyUp(ulong index) {
    while (index > 0) {
        ulong parentIndex = Parent(index);
        
        // If parent is already greater or equal, heap property is satisfied
        if (Elements[parentIndex] >= Elements[index]) {
            break;
        }
        
        // Swap with parent and continue upward
        std::swap(Elements[index], Elements[parentIndex]);
        index = parentIndex;
    }
}

// HeapifyDown - maintains max-heap property going downward
template <typename Data>
void HeapVec<Data>::HeapifyDown(ulong index) {
    while (true) {
        ulong largest = index;
        ulong leftChild = LeftChild(index);
        ulong rightChild = RightChild(index);
        
        // Find the largest among node and its children
        if (leftChild < size && Elements[leftChild] > Elements[largest]) {
            largest = leftChild;
        }
        
        if (rightChild < size && Elements[rightChild] > Elements[largest]) {
            largest = rightChild;
        }
        
        // If largest is not the current node, swap and continue
        if (largest != index) {
            std::swap(Elements[index], Elements[largest]);
            index = largest;
        } else {
            break; // Heap property is satisfied
        }
    }
}

/* ************************************************************************** */

}
