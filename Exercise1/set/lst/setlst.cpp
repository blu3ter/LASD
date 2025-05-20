#include <stdexcept>

// Per evitare problemi con l'inclusione di setlst.hpp che include setlst.cpp alla fine
#ifndef SETLST_HPP
#include "setlst.hpp"
#endif

namespace lasd {

/* ************************************************************************** */

// Specific constructors
template <typename Data>
SetLst<Data>::SetLst(const TraversableContainer<Data>& container) : List<Data>() {
    container.Traverse(
        [this](const Data& dat) {
            this->Insert(dat);
        }
    );
}

template <typename Data>
SetLst<Data>::SetLst(MappableContainer<Data>&& container) : List<Data>() {
    container.Map(
        [this](Data& dat) {
            this->Insert(std::move(dat));
        }
    );
}

// Copy constructor
template <typename Data>
SetLst<Data>::SetLst(const SetLst<Data>& other) : List<Data>(other) {
    // L'implementazione di List<Data>(other) dovrebbe fare una copia profonda.
    // Se SetLst dovesse gestire i nodi e List<Data> fosse solo un'interfaccia,
    // la copia manuale sarebbe:
    // if (other.head != nullptr) {
    //   Node* tmp = other.head;
    //   while(tmp != nullptr) {
    //     this->Insert(tmp->value); // Insert gestisce ordine e unicità
    //     tmp = tmp->next;
    //   }
    // }
}

// Move constructor
template <typename Data>
SetLst<Data>::SetLst(SetLst<Data>&& other) noexcept : List<Data>(std::move(other)) {
    // L'implementazione di List<Data>(std::move(other)) dovrebbe trasferire le risorse.
}

/* ************************************************************************ */

// Copy assignment
template <typename Data>
SetLst<Data>& SetLst<Data>::operator=(const SetLst<Data>& other) {
    List<Data>::operator=(other);
    return *this;
}

// Move assignment
template <typename Data>
SetLst<Data>& SetLst<Data>::operator=(SetLst<Data>&& other) noexcept {
    List<Data>::operator=(std::move(other));
    return *this;
}

/* ************************************************************************ */

// Comparison operators
template <typename Data>
bool SetLst<Data>::operator==(const SetLst<Data>& other) const noexcept {
    // Assumendo che List<Data>::operator== confronti dimensione e poi valuei in ordine.
    return List<Data>::operator==(other);
}

template <typename Data>
inline bool SetLst<Data>::operator!=(const SetLst<Data>& other) const noexcept {
    return !(*this == other);
}

/* ************************************************************************ */

// Specific member functions (inherited from DictionaryContainer)

template <typename Data>
bool SetLst<Data>::Insert(const Data& value) {
    typename List<Data>::Node* curr = this->head;
    typename List<Data>::Node* prev = nullptr;

    while (curr != nullptr && curr->value < value) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != nullptr && curr->value == value) {
        return false; // valueo già presente
    }

    if (prev == nullptr) { // Inserimento in testa
        List<Data>::InsertAtFront(value);
    } else { // Inserimento dopo prev
        // Questo blocco assume che List<Data> sia una lista singolarmente concatenata
        // e che SetLst possa/debba manipolare i nodi direttamente se InsertAtFront/Back non bastano.
        // Se prev == this->tail, allora è un InsertAtBack.
        if (prev == this->tail) {
            List<Data>::InsertAtBack(value);
        } else { // Inserimento in mezzo
            typename List<Data>::Node* newNode = new typename List<Data>::Node(value);
            newNode->next = prev->next; // curr
            prev->next = newNode;
            this->size++; // Deve essere gestito correttamente da List o qui
        }
    }
    return true;
}

template <typename Data>
bool SetLst<Data>::Insert(Data&& value) {
    typename List<Data>::Node* curr = this->head;
    typename List<Data>::Node* prev = nullptr;
    
    // Per il confronto, usiamo una copia temporanea o un riferimento const
    const Data& val_to_compare = value;

    while (curr != nullptr && curr->value < val_to_compare) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != nullptr && curr->value == val_to_compare) {
        return false; // valueo già presente
    }

    if (prev == nullptr) {
        List<Data>::InsertAtFront(std::move(value));
    } else {
        if (prev == this->tail) {
            List<Data>::InsertAtBack(std::move(value));
        } else {
            typename List<Data>::Node* newNode = new typename List<Data>::Node(std::move(value));
            newNode->next = prev->next;
            prev->next = newNode;
            this->size++;
        }
    }
    return true;
}

template <typename Data>
bool SetLst<Data>::Remove(const Data& value) {
    if (this->head == nullptr) {
        return false;
    }

    typename List<Data>::Node* curr = this->head;
    typename List<Data>::Node* prev = nullptr;

    while (curr != nullptr && curr->value < value) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != nullptr && curr->value == value) {
        if (prev == nullptr) { // Rimuovi dalla testa
            List<Data>::RemoveFromFront();
        } else {
            // Se curr è la coda (tail)
            if (curr == this->tail) {
                this->tail = prev;
                prev->next = nullptr;
            } else { // Rimuovi in mezzo
                prev->next = curr->next;
            }
            delete curr;
            this->size--;
        }
        return true;
    }
    return false; // valueo non trovato
}

/* ************************************************************************ */
// Specific member function (inherited from TestableContainer)
template <typename Data>
bool SetLst<Data>::Exists(const Data& value) const noexcept {
    // Poiché la lista è ordinata, possiamo usare BinarySearch.
    // Tuttavia, BinarySearch su una lista concatenata è O(N log N) o O(N) se operator[] è O(1) ma la ricerca è O(N).
    // Una ricerca lineare semplice è O(N).
    // Per rispettare la richiesta di BinarySearch, la usiamo.
    // Se BinarySearch non è ancora implementata o si preferisce una ricerca lineare diretta:
    // typename List<Data>::Node* curr = this->head;
    // while (curr != nullptr) {
    //   if (curr->value == value) { return true; }
    //   if (curr->value > value) { return false; } // Ottimizzazione per lista ordinata
    //   curr = curr->next;
    // }
    // return false;
    return BinarySearch(value);
}

/* ************************************************************************** */
// Specific member functions (inherited from OrderedDictionaryContainer)

template <typename Data>
const Data& SetLst<Data>::Min() const {
    if (this->Empty()) {
        throw std::length_error("Set is empty in Min()");
    }
    return List<Data>::Front();
}

template <typename Data>
Data SetLst<Data>::MinNRemove() {
    if (this->Empty()) {
        throw std::length_error("Set is empty in MinNRemove()");
    }
    Data minVal = List<Data>::Front();
    List<Data>::RemoveFromFront();
    return minVal;
}

template <typename Data>
void SetLst<Data>::RemoveMin() {
    if (this->Empty()) {
        throw std::length_error("Set is empty in RemoveMin()");
    }
    List<Data>::RemoveFromFront();
}

template <typename Data>
const Data& SetLst<Data>::Max() const {
    if (this->Empty()) {
        throw std::length_error("Set is empty in Max()");
    }
    return List<Data>::Back();
}

template <typename Data>
Data SetLst<Data>::MaxNRemove() {
    if (this->Empty()) {
        throw std::length_error("Set is empty in MaxNRemove()");
    }
    Data maxVal = List<Data>::Back();
    // List<Data>::RemoveFromBack() sarebbe ideale. Se non c'è:
    if (this->size == 1) {
        List<Data>::Clear();
    } else {
        typename List<Data>::Node* curr = this->head;
        while (curr->next != this->tail) {
            curr = curr->next;
        }
        delete this->tail;
        this->tail = curr;
        curr->next = nullptr;
        this->size--;
    }
    return maxVal;
}

template <typename Data>
void SetLst<Data>::RemoveMax() {
    if (this->Empty()) {
        throw std::length_error("Set is empty in RemoveMax()");
    }
    // List<Data>::RemoveFromBack() sarebbe ideale. Se non c'è:
    if (this->size == 1) {
        List<Data>::Clear();
    } else {
        typename List<Data>::Node* curr = this->head;
        while (curr->next != this->tail) {
            curr = curr->next;
        }
        delete this->tail;
        this->tail = curr;
        curr->next = nullptr;
        this->size--;
    }
}

template <typename Data>
const Data& SetLst<Data>::Predecessor(const Data& val) const {
    if (this->Empty() || val <= this->Min()) { // Se val è il minimo o minore, non ha predecessore nel set
        throw std::length_error("No predecessor found");
    }
    typename List<Data>::Node* curr = this->head;
    typename List<Data>::Node* predNode = nullptr;
    while(curr != nullptr && curr->value < val) {
        predNode = curr;
        curr = curr->next;
    }
    if (predNode == nullptr) { // Tutti gli valuei sono >= val, o lista vuota (già gestito)
        throw std::length_error("No predecessor found");
    }
    return predNode->value;
}

template <typename Data>
Data SetLst<Data>::PredecessorNRemove(const Data& val) {
    const Data& predVal = Predecessor(val); // Lancia eccezione se non trovato
    Data result = predVal; // Copia prima di rimuovere
    Remove(result); // Rimuove il predecessore trovato
    return result;
}

template <typename Data>
void SetLst<Data>::RemovePredecessor(const Data& val) {
    const Data& predVal = Predecessor(val); // Lancia eccezione se non trovato
    Remove(predVal);
}

template <typename Data>
const Data& SetLst<Data>::Successor(const Data& val) const {
     if (this->Empty() || val >= this->Max()) { // Se val è il massimo o maggiore, non ha successore
        throw std::length_error("No successor found");
    }
    typename List<Data>::Node* curr = this->head;
    while(curr != nullptr) {
        if (curr->value > val) {
            return curr->value;
        }
        curr = curr->next;
    }
    throw std::length_error("No successor found"); // Non dovrebbe arrivare qui se Max() è gestito
}

template <typename Data>
Data SetLst<Data>::SuccessorNRemove(const Data& val) {
    const Data& succVal = Successor(val); // Lancia eccezione se non trovato
    Data result = succVal; // Copia prima di rimuovere
    Remove(result); // Rimuove il successore trovato
    return result;
}

template <typename Data>
void SetLst<Data>::RemoveSuccessor(const Data& val) {
    const Data& succVal = Successor(val); // Lancia eccezione se non trovato
    Remove(succVal);
}

/* ************************************************************************ */
// Specific member functions (inherited from LinearContainer)
template <typename Data>
Data& SetLst<Data>::operator[](ulong index) {
    // Deve chiamare l'operator[] della classe base List<Data>
    return List<Data>::operator[](index);
}

/* ************************************************************************ */
// Specific member function (inherited from ClearableContainer)
template <typename Data>
void SetLst<Data>::Clear() {
    List<Data>::Clear(); // Chiama il Clear della classe base List
}

/* ************************************************************************** */
// Auxiliary functions

template <typename Data>
bool SetLst<Data>::BinarySearch(const Data& value) const {
    if (this->size == 0) {
        return false;
    }
    long low = 0;
    long high = static_cast<long>(this->size) - 1;

    while (low <= high) {
        long mid_idx = low + (high - low) / 2;
        // Assumendo che List<Data> abbia un operator[] const.
        // SetLst eredita operator[] non-const, ma List<Data> dovrebbe avere la versione const.
        const Data& mid_val = static_cast<const List<Data>*>(this)->operator[](mid_idx);

        if (mid_val == value) {
            return true;
        } else if (mid_val < value) {
            low = mid_idx + 1;
        } else {
            high = mid_idx - 1;
        }
    }
    return false;
}

/* ************************************************************************** */

}