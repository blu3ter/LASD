#include <stdexcept>
#include <algorithm> // Per std::sort, std::unique, std::move, std::lower_bound

#include "setvec.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

// Protected Auxiliary Functions

template <typename Data>
long SetVec<Data>::BinarySearch(const Data& val) const {
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
    this->size = 0; // Imposta Container::size a 0. Vector::Elements (ereditato) è nullptr.
                    // buffer.size è DEFAULT_CAPACITY, buffer.Elements è allocato.
}

template <typename Data>
SetVec<Data>::SetVec(const TraversableContainer<Data>& container) : buffer(container.Size() > 0 ? container.Size() : DEFAULT_CAPACITY) {
    this->size = 0; // Inizializza la size del SetVec
    // head e tail sono 0 di default
    container.Traverse(
        [this](const Data& data) {
            this->Insert(data); // Insert incrementerà this->size e gestirà head/tail/buffer
        }
    );
}

template <typename Data>
SetVec<Data>::SetVec(MappableContainer<Data>&& container) : buffer(container.Size() > 0 ? container.Size() : DEFAULT_CAPACITY) {
    this->size = 0; // Inizializza la size del SetVec
    // head e tail sono 0 di default
    container.Map(
        [this](Data& data) {
            this->Insert(std::move(data)); // Insert incrementerà this->size e gestirà head/tail/buffer
        }
    );
}

// Copy constructor
template <typename Data>
SetVec<Data>::SetVec(const SetVec<Data>& other) 
  : Vector<Data>(other),       // Copia la parte base Vector. Questo imposta this->size (ereditato)
                               // e this->Elements (ereditato) basandosi sulla parte base di 'other'.
                               // Il costruttore di copia di Vector è stato reso robusto.
    buffer(other.buffer),     // Copia il membro 'buffer' (Vector)
    head(other.head),
    tail(other.tail)
    // this->size = other.size; // Ridondante: this->size (Container::size) è già stato impostato
                               // correttamente da Vector<Data>(other).
{
    // Se Vector<Data>(other) imposta this->size correttamente, non serve altro.
    // Assicuriamoci che this->size rifletta la size logica del SetVec.
    // Dato che Vector<Data>(other) copia la size della *parte base* di other,
    // e SetVec usa this->size per la sua dimensione logica, questo dovrebbe essere corretto.
}

// Move constructor
template <typename Data>
SetVec<Data>::SetVec(SetVec<Data>&& other) noexcept 
  : Vector<Data>(std::move(other)) // Sposta la parte base Vector. Questo è responsabile per
                                   // lo spostamento di this->size (ereditato) e this->Elements (ereditato).
{
    std::swap(this->buffer, other.buffer);
    std::swap(this->head, other.head);
    std::swap(this->tail, other.tail);
    // NON fare std::swap(this->size, other.size);
    // this->size è già stato correttamente scambiato da Vector<Data>(std::move(other)).
    // 'other' è ora in uno stato valido ma non specificato (tipico dopo std::move).
    // Il suo distruttore (~SetVec) sarà chiamato e gestirà le risorse.
}


// Copy assignment
template <typename Data>
SetVec<Data>& SetVec<Data>::operator=(const SetVec<Data>& other) {
    if (this != &other) {
        Vector<Data>::operator=(other); // Assegna la parte base Vector. Questo imposta this->size.
        this->buffer = other.buffer;   // Assegna il membro buffer.
        this->head = other.head;
        this->tail = other.tail;
        // this->size = other.size; // Ridondante: this->size è già stato impostato da Vector<Data>::operator=(other).
    }
    return *this;
}

// Move assignment
template <typename Data>
SetVec<Data>& SetVec<Data>::operator=(SetVec<Data>&& other) noexcept {
    if (this != &other) {
        Vector<Data>::operator=(std::move(other)); // Sposta la parte base Vector. Gestisce this->size.
        
        std::swap(this->buffer, other.buffer);
        std::swap(this->head, other.head);
        std::swap(this->tail, other.tail);
        // NON fare std::swap(this->size, other.size);
        // this->size è già stato correttamente scambiato da Vector<Data>::operator=(std::move(other)).
    }
    return *this;
}

// Comparison operators
template <typename Data>
bool SetVec<Data>::operator==(const SetVec<Data>& other) const noexcept {
    if (this->size != other.size) {
        return false;
    }
    if (this->Empty()) { // Se this->size è 0, e other.size è 0, sono uguali.
        return true;
    }
    // Confronta gli elementi in ordine logico
    for (ulong i = 0; i < this->size; ++i) {
        // operator[] accede agli elementi tramite this->buffer, this->head, e this->size
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
    // Usa this->operator[] che è basato su this->buffer, this->head, this->size
    while(insert_pos_logical < this->size && this->operator[](insert_pos_logical) < val) {
        insert_pos_logical++;
    }
    // this->buffer.Size() è la capacità del buffer Vector sottostante
    ulong insert_pos_physical = (this->head + insert_pos_logical) % this->buffer.Size();

    if (insert_pos_logical == this->size) { // Inserimento in coda (logica)
        this->buffer[this->tail] = val;
        this->tail = (this->tail + 1) % this->buffer.Size();
    } else { // Inserimento in mezzo o all'inizio (logico)
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

    // Sposta gli elementi a destra di found_idx_logical di una posizione a sinistra (logicamente).
    for (ulong i = static_cast<ulong>(found_idx_logical); i < this->size - 1; ++i) {
        ulong current_physical = (this->head + i) % this->buffer.Size();
        ulong next_physical = (this->head + i + 1) % this->buffer.Size();
        this->buffer[current_physical] = this->buffer[next_physical]; // o std::move se appropriato
    }

    // Aggiorna tail e size
    // Se il buffer è vuoto dopo la rimozione, head e tail dovrebbero essere 0.
    // Se size diventa 0, tail dovrebbe idealmente tornare a head (es. 0).
    if (this->size == 1) { // Stiamo per rimuovere l'ultimo elemento
        this->head = 0;
        this->tail = 0;
    } else {
        // Spostando gli elementi a sinistra, tail si sposta indietro.
        this->tail = (this->tail == 0) ? (this->buffer.Size() - 1) : (this->tail - 1);
    }
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
    // L'ultimo elemento logico è all'indice fisico (head + size - 1) % buffer.Size()
    // o, equivalentemente, l'elemento prima di tail se il buffer non è vuoto.
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
    this->buffer.Clear(); // Svuota il Vector sottostante (Elements=nullptr, size=0)
    this->buffer.Resize(DEFAULT_CAPACITY); // Riporta alla capacità di default (Elements allocato, size=DEFAULT_CAPACITY)
                                        // Questo non è corretto per la logica di SetVec.
                                        // buffer dovrebbe essere un Vector di capacità DEFAULT_CAPACITY ma size logica 0.
    // Correzione per Clear:
    // this->buffer.Clear(); // Dealloca gli elementi di buffer e imposta buffer.size = 0
    // if (this->buffer.Size() != DEFAULT_CAPACITY) { // buffer.Size() qui è la capacità
    //     this->buffer.Resize(DEFAULT_CAPACITY); // Ri-alloca buffer con capacità DEFAULT_CAPACITY
    // }
    // La chiamata a buffer.Clear() in Vector.cpp fa: delete[] Elements; Elements = nullptr; size = 0;
    // La chiamata a buffer.Resize(DEFAULT_CAPACITY) in Vector.cpp fa:
    //   delete[] Elements; Elements = new Data[DEFAULT_CAPACITY]{}; size = DEFAULT_CAPACITY;
    // Questo significa che this->buffer.size (la size del Vector membro) diventa DEFAULT_CAPACITY.
    // Ma la size logica del SetVec (this->size) deve essere 0.

    // Logica corretta per SetVec::Clear():
    Vector<Data> new_empty_buffer(DEFAULT_CAPACITY); // Crea un buffer con capacità ma size 0 (se Vector(ulong) fa questo)
                                                    // Vector(ulong) imposta size = newsize. Quindi questo crea un buffer di size DEFAULT_CAPACITY.
                                                    // Dobbiamo assicurarci che il buffer interno di SetVec abbia capacità DEFAULT_CAPACITY
                                                    // ma la *size logica* del SetVec sia 0.
    this->buffer.Clear(); // Imposta this->buffer.Elements = nullptr, this->buffer.size = 0
    if (DEFAULT_CAPACITY > 0) {
        this->buffer.Resize(DEFAULT_CAPACITY); // Alloca this->buffer.Elements con capacità DEFAULT_CAPACITY
                                            // e imposta this->buffer.size = DEFAULT_CAPACITY.
                                            // Questo non è ciò che vogliamo per la *logica* di SetVec.
                                            // SetVec usa this->size per la sua dimensione logica.
                                            // this->buffer è solo l'array di storage.
                                            // La size di this->buffer dovrebbe essere la sua capacità.
    }
    // La classe Vector non distingue tra size logica e capacità. 'size' è la dimensione logica.
    // Quindi, this->buffer.Resize(DEFAULT_CAPACITY) crea un buffer con DEFAULT_CAPACITY elementi.
    // Questo va bene per la capacità, ma la *size logica* del SetVec deve essere 0.

    // Approccio più semplice per Clear:
    this->head = 0;
    this->tail = 0;
    this->size = 0; // La size logica del SetVec è 0.
    // Il buffer può mantenere la sua capacità, o essere ridimensionato se necessario.
    // Per coerenza con il costruttore di default, potremmo voler un buffer di DEFAULT_CAPACITY.
    if (this->buffer.Size() != DEFAULT_CAPACITY) { // buffer.Size() è la size/capacità del Vector membro
        this->buffer.Resize(DEFAULT_CAPACITY); // Assicura che il buffer abbia la capacità di default.
                                               // Vector::Resize imposta anche la size del Vector membro.
    }
    // Dopo Resize, this->buffer.size è DEFAULT_CAPACITY. Questo è ok se interpretiamo
    // this->buffer.size come la capacità del buffer di SetVec.
    // La size logica di SetVec è this->size (Container::size).
}

// Specific member function (inherited from ResizableContainer)
template <typename Data>
void SetVec<Data>::Resize(ulong new_buffer_capacity) {
    // Questa funzione dovrebbe ridimensionare this->buffer, non this->size (logico).
    // Ma l'interfaccia ResizableContainer si riferisce alla size logica.
    // Qui c'è ambiguità. Assumiamo che Resize si riferisca alla capacità del buffer interno.
    if (new_buffer_capacity == this->buffer.Size()) return;
    
    // Non si può ridurre la capacità del buffer a meno degli elementi attualmente contenuti.
    if (new_buffer_capacity < this->size) new_buffer_capacity = this->size; 
    if (new_buffer_capacity == 0 && this->size > 0) new_buffer_capacity = this->size; // Non può essere 0 se ci sono elementi
    if (new_buffer_capacity == 0 && this->size == 0) new_buffer_capacity = DEFAULT_CAPACITY; // Minima capacità

    Vector<Data> new_physical_buffer(new_buffer_capacity); // Alloca un nuovo Vector con la nuova capacità
                                                        // Vector(ulong) imposta la sua size alla capacità.
    ulong current_logical_size = this->size; 
    for (ulong i = 0; i < current_logical_size; ++i) {
        new_physical_buffer[i] = this->operator[](i); // Copia gli elementi in ordine logico
    }
    
    this->buffer = std::move(new_physical_buffer); // Sposta il nuovo buffer
    this->head = 0;
    // tail è la posizione del prossimo inserimento. Se head=0, tail = size logica.
    this->tail = current_logical_size % this->buffer.Size(); 
                                                    // Se buffer.Size() (capacità) è 0, questo è un problema.
                                                    // Ma abbiamo gestito new_buffer_capacity == 0 sopra.
    this->size = current_logical_size; // La size logica del SetVec non cambia con questo tipo di resize.
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
        if (this->size == 0) { 
             throw std::length_error("Nessun successore: set vuoto nonostante idx valido (impossibile).");
        }
        if (static_cast<ulong>(idx) == this->size - 1) { 
            throw std::length_error("Nessun successore per il valore massimo.");
        }
        if (static_cast<ulong>(idx + 1) < this->size) {
            return this->operator[](static_cast<ulong>(idx + 1)); 
        } else {
            throw std::length_error("Errore logico: idx non è l'ultimo ma idx+1 è fuori range.");
        }
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