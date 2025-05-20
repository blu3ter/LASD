#include <stdexcept>
#include <algorithm> // Per std::sort, std::unique, std::move, std::lower_bound

#include "setvec.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

// Protected Auxiliary Functions

template <typename Data>
long SetVec<Data>::BinarySearch(const Data& val) const {
    // Questa implementazione assume che gli elementi siano ordinati nel buffer circolare.
    // Restituisce l'indice logico dell'elemento se trovato, altrimenti -1.

    if (this->Empty()) { 
        return -1;
    }

    ulong low = 0;
    ulong high = this->size; 

    while (low < high) {
        ulong mid_logical = low + (high - low) / 2;
        ulong mid_physical = (this->head + mid_logical) % this->buffer.Size(); 
        
        if (this->buffer[mid_physical] == val) {
            return mid_logical; 
        } else if (this->buffer[mid_physical] < val) {
            low = mid_logical + 1;
        } else {
            high = mid_logical;
        }
    }
    return -1; 
}


// Specific constructors

template <typename Data>
SetVec<Data>::SetVec() : buffer(DEFAULT_CAPACITY) {
    this->size = 0;
}

template <typename Data>
SetVec<Data>::SetVec(const TraversableContainer<Data>& container) : buffer(container.Size() > 0 ? container.Size() : DEFAULT_CAPACITY) {
    this->size = 0;
    container.Traverse(
        [this](const Data& data) {
            this->Insert(data);
        }
    );
}

template <typename Data>
SetVec<Data>::SetVec(MappableContainer<Data>&& container) : buffer(container.Size() > 0 ? container.Size() : DEFAULT_CAPACITY) {
    this->size = 0;
    container.Map(
        [this](Data& data) {
            this->Insert(std::move(data));
        }
    );
}

// Copy constructor
template <typename Data>
SetVec<Data>::SetVec(const SetVec<Data>& other) : Vector<Data>(other) {
    this->buffer = other.buffer;
    this->head = other.head;
    this->tail = other.tail;
    this->size = other.size; 
}

// Move constructor
template <typename Data>
SetVec<Data>::SetVec(SetVec<Data>&& other) noexcept : Vector<Data>(std::move(other)) {
    std::swap(this->buffer, other.buffer);
    std::swap(this->head, other.head);
    std::swap(this->tail, other.tail);
    std::swap(this->size, other.size);
}


// Copy assignment
template <typename Data>
SetVec<Data>& SetVec<Data>::operator=(const SetVec<Data>& other) {
    if (this != &other) {
        Vector<Data>::operator=(other);
        this->buffer = other.buffer;
        this->head = other.head;
        this->tail = other.tail;
        this->size = other.size;
    }
    return *this;
}

// Move assignment
template <typename Data>
SetVec<Data>& SetVec<Data>::operator=(SetVec<Data>&& other) noexcept {
    if (this != &other) {
        Vector<Data>::operator=(std::move(other));
        std::swap(this->buffer, other.buffer);
        std::swap(this->head, other.head);
        std::swap(this->tail, other.tail);
        std::swap(this->size, other.size);
    }
    return *this;
}

// Comparison operators
template <typename Data>
bool SetVec<Data>::operator==(const SetVec<Data>& other) const noexcept {
    if (this->size != other.size) {
        return false;
    }
    if (this->Empty()) {
        return true;
    }
    for (ulong i = 0; i < this->size; ++i) {
        if (this->operator[](i) != other.operator[](i)) { 
            return false;
        }
    }
    return true;
}

template <typename Data>
bool SetVec<Data>::operator!=(const SetVec<Data>& other) const noexcept {
    return !(*this == other);
}


// Specific member functions (inherited from DictionaryContainer)

template <typename Data>
bool SetVec<Data>::Insert(const Data& val) {
    if (Exists(val)) { 
        return false; 
    }

    if (this->size == this->buffer.Size()) { 
        Resize( (this->buffer.Size() == 0) ? DEFAULT_CAPACITY : this->buffer.Size() * 2 );
    }

    ulong insert_pos_logical = 0;
    while(insert_pos_logical < this->size && this->operator[](insert_pos_logical) < val) {
        insert_pos_logical++;
    }
    ulong insert_pos_physical = (this->head + insert_pos_logical) % this->buffer.Size();

    if (insert_pos_logical == this->size) {
        this->buffer[this->tail] = val;
        this->tail = (this->tail + 1) % this->buffer.Size();
    } else {
        ulong current_logical = this->size;
        while(current_logical > insert_pos_logical) {
            ulong physical_dest = (this->head + current_logical) % this->buffer.Size();
            ulong physical_src = (this->head + current_logical - 1) % this->buffer.Size();
            this->buffer[physical_dest] = this->buffer[physical_src];
            current_logical--;
        }
        this->buffer[insert_pos_physical] = val;
        this->tail = (this->tail + 1) % this->buffer.Size();
    }
    
    this->size++;
    return true;
}


template <typename Data>
bool SetVec<Data>::Insert(Data&& val) {
    if (Exists(val)) {
        return false;
    }

    if (this->size == this->buffer.Size()) {
        Resize( (this->buffer.Size() == 0) ? DEFAULT_CAPACITY : this->buffer.Size() * 2 );
    }
    
    ulong insert_pos_logical = 0;
    while(insert_pos_logical < this->size && this->operator[](insert_pos_logical) < val) {
        insert_pos_logical++;
    }
    ulong insert_pos_physical = (this->head + insert_pos_logical) % this->buffer.Size();

    if (insert_pos_logical == this->size) {
        this->buffer[this->tail] = std::move(val);
        this->tail = (this->tail + 1) % this->buffer.Size();
    } else {
        ulong current_logical = this->size;
        while(current_logical > insert_pos_logical) {
            ulong physical_dest = (this->head + current_logical) % this->buffer.Size();
            ulong physical_src = (this->head + current_logical - 1) % this->buffer.Size();
            this->buffer[physical_dest] = std::move(this->buffer[physical_src]);
            current_logical--;
        }
        this->buffer[insert_pos_physical] = std::move(val);
        this->tail = (this->tail + 1) % this->buffer.Size();
    }

    this->size++;
    return true;
}


template <typename Data>
bool SetVec<Data>::Remove(const Data& val) {
    long found_idx_logical = BinarySearch(val); 
    if (found_idx_logical == -1) {
        return false; 
    }

    for (ulong i = static_cast<ulong>(found_idx_logical); i < this->size - 1; ++i) {
        ulong current_physical = (this->head + i) % this->buffer.Size();
        ulong next_physical = (this->head + i + 1) % this->buffer.Size();
        this->buffer[current_physical] = this->buffer[next_physical];
    }

    this->tail = (this->tail == 0) ? (this->buffer.Size() - 1) : (this->tail - 1);
    this->size--;

    if (this->size < this->buffer.Size() / 4 && this->buffer.Size() > DEFAULT_CAPACITY) {
        Resize(this->buffer.Size() / 2);
    }
    return true;
}

// Specific member functions (inherited from LinearContainer)

template <typename Data>
Data& SetVec<Data>::operator[](ulong logicalIndex) {
    if (logicalIndex >= this->size) {
        throw std::out_of_range("Accesso fuori range in SetVec::operator[]");
    }
    return this->buffer[(this->head + logicalIndex) % this->buffer.Size()];
}

template <typename Data>
const Data& SetVec<Data>::operator[](ulong logicalIndex) const {
    if (logicalIndex >= this->size) {
        throw std::out_of_range("Accesso fuori range in SetVec::operator[] const");
    }
    return this->buffer[(this->head + logicalIndex) % this->buffer.Size()];
}


template <typename Data>
Data& SetVec<Data>::Front() {
    if (this->Empty()) {
        throw std::length_error("Accesso a Front() su SetVec vuoto");
    }
    return this->buffer[this->head]; 
}

template <typename Data>
const Data& SetVec<Data>::Front() const {
    if (this->Empty()) {
        throw std::length_error("Accesso a Front() const su SetVec vuoto");
    }
    return this->buffer[this->head];
}

template <typename Data>
Data& SetVec<Data>::Back() {
    if (this->Empty()) {
        throw std::length_error("Accesso a Back() su SetVec vuoto");
    }
    ulong last_physical_idx = (this->tail == 0) ? (this->buffer.Size() - 1) : (this->tail - 1);
    return this->buffer[last_physical_idx]; 
}

template <typename Data>
const Data& SetVec<Data>::Back() const {
    if (this->Empty()) {
        throw std::length_error("Accesso a Back() const su SetVec vuoto");
    }
    ulong last_physical_idx = (this->tail == 0) ? (this->buffer.Size() - 1) : (this->tail - 1);
    return this->buffer[last_physical_idx];
}


// Specific member function (inherited from TestableContainer)
template <typename Data>
bool SetVec<Data>::Exists(const Data& val) const noexcept {
    return BinarySearch(val) != -1;
}

// Specific member function (inherited from ClearableContainer)
template <typename Data>
void SetVec<Data>::Clear() {
    this->buffer.Clear(); 
    this->buffer.Resize(DEFAULT_CAPACITY); 
    this->head = 0;
    this->tail = 0;
    this->size = 0;
}

// Specific member function (inherited from ResizableContainer)
template <typename Data>
void SetVec<Data>::Resize(ulong new_capacity) {
    if (new_capacity == this->buffer.Size()) return;
    if (new_capacity < this->size) new_capacity = this->size; 
    if (new_capacity == 0) new_capacity = DEFAULT_CAPACITY;

    Vector<Data> new_buffer(new_capacity);
    ulong current_size = this->size; 
    for (ulong i = 0; i < current_size; ++i) {
        new_buffer[i] = this->operator[](i); 
    }
    
    this->buffer = std::move(new_buffer);
    this->head = 0;
    this->tail = current_size % this->buffer.Size(); 
    this->size = current_size; 
}


// Specific member functions (inherited from OrderedDictionaryContainer)

template <typename Data>
const Data& SetVec<Data>::Min() const {
    if (this->Empty()) {
        throw std::length_error("Min() su SetVec vuoto.");
    }
    return this->Front(); 
}

template <typename Data>
Data SetVec<Data>::MinNRemove() {
    if (this->Empty()) {
        throw std::length_error("MinNRemove() su SetVec vuoto.");
    }
    Data min_val = this->Min();
    Remove(min_val); 
    return min_val;
}

template <typename Data>
void SetVec<Data>::RemoveMin() {
    if (this->Empty()) {
        throw std::length_error("RemoveMin() su SetVec vuoto.");
    }
    Remove(this->Min());
}

template <typename Data>
const Data& SetVec<Data>::Max() const {
    if (this->Empty()) {
        throw std::length_error("Max() su SetVec vuoto.");
    }
    return this->Back(); 
}

template <typename Data>
Data SetVec<Data>::MaxNRemove() {
    if (this->Empty()) {
        throw std::length_error("MaxNRemove() su SetVec vuoto.");
    }
    Data max_val = this->Max();
    Remove(max_val);
    return max_val;
}

template <typename Data>
void SetVec<Data>::RemoveMax() {
    if (this->Empty()) {
        throw std::length_error("RemoveMax() su SetVec vuoto.");
    }
    Remove(this->Max());
}


template <typename Data>
const Data& SetVec<Data>::Predecessor(const Data& val) const {
    if (this->Empty()) {
        throw std::length_error("Predecessor() su SetVec vuoto.");
    }
    long idx = BinarySearch(val); 
    if (idx != -1) { // val esiste
        if (idx == 0) { 
            throw std::length_error("Nessun predecessore per il valore minimo.");
        }
        return this->operator[](static_cast<ulong>(idx - 1)); 
    } else { 
        ulong low = 0;
        ulong high = this->size;
        ulong potential_idx = this->size; 

        while(low < high) {
            ulong mid_logical = low + (high - low) / 2;
            if (this->operator[](mid_logical) < val) {
                low = mid_logical + 1;
            } else {
                potential_idx = mid_logical;
                high = mid_logical;
            }
        }
        if (potential_idx == 0) { 
            throw std::length_error("Nessun predecessore trovato per il valore dato.");
        }
        return this->operator[](potential_idx - 1);
    }
}


template <typename Data>
Data SetVec<Data>::PredecessorNRemove(const Data& val) {
    const Data& pred = Predecessor(val);
    Data pred_copy = pred; 
    Remove(pred_copy); 
    return pred_copy;
}

template <typename Data>
void SetVec<Data>::RemovePredecessor(const Data& val) {
    const Data& pred = Predecessor(val);
    Remove(pred);
}


template <typename Data>
const Data& SetVec<Data>::Successor(const Data& val) const {
     if (this->Empty()) {
        throw std::length_error("Successor() su SetVec vuoto.");
    }
    long idx = BinarySearch(val); 
    if (idx != -1) { // val esiste
        if (this->size == 0) { // Controllo di sicurezza aggiuntivo
             throw std::length_error("Nessun successore: set vuoto nonostante idx valido (impossibile).");
        }
        // Confronto corretto per evitare warning signed/unsigned e underflow
        if (static_cast<ulong>(idx) == this->size - 1) { 
            throw std::length_error("Nessun successore per il valore massimo.");
        }
        // Assicura che idx + 1 sia un indice logico valido prima dell'accesso
        if (static_cast<ulong>(idx + 1) < this->size) {
            return this->operator[](static_cast<ulong>(idx + 1)); 
        } else {
            // Questo ramo non dovrebbe essere raggiunto se la logica precedente è corretta
            throw std::length_error("Errore logico: idx non è l'ultimo ma idx+1 è fuori range.");
        }
    } else { // val non esiste, cerca il successore del punto in cui val sarebbe inserito
        ulong low = 0;
        ulong high = this->size;
        ulong potential_idx = this->size; 

        while(low < high) {
            ulong mid_logical = low + (high - low) / 2;
            if (this->operator[](mid_logical) < val) { 
                low = mid_logical + 1;
            } else { 
                potential_idx = mid_logical;
                high = mid_logical;
            }
        }
        if (potential_idx == this->size) { 
            throw std::length_error("Nessun successore trovato per il valore dato.");
        }
        return this->operator[](potential_idx);
    }
}

template <typename Data>
Data SetVec<Data>::SuccessorNRemove(const Data& val) {
    const Data& succ = Successor(val);
    Data succ_copy = succ;
    Remove(succ_copy);
    return succ_copy;
}

template <typename Data>
void SetVec<Data>::RemoveSuccessor(const Data& val) {
    const Data& succ = Successor(val);
    Remove(succ);
}


/* ************************************************************************** */

}