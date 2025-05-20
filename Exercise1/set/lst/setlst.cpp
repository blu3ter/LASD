#include <stdexcept>
#include <utility> // Per std::move

// Nota: SetLst eredita da lasd::List<Data>.
// Le operazioni di inserimento devono mantenere l'ordine e l'unicità degli elementi.

namespace lasd {

/* ************************************************************************** */
// Funzione Ausiliaria Protetta: FindNode
// Scorre la lista per trovare 'val' o il punto di inserimento corretto.
// Imposta 'found' a true se 'val' è presente.
// 'prev_node' punterà al nodo precedente a 'current_node'.
// 'current_node' punterà al nodo contenente 'val' o al nodo dopo il quale 'val' dovrebbe essere inserito,
// o nullptr se 'val' dovrebbe essere la nuova coda o la lista è vuota.
template <typename Data>
void SetLst<Data>::FindNode(const Data& val, bool& found, typename List<Data>::Node*& prev_node, typename List<Data>::Node*& current_node) const {
    prev_node = nullptr;
    current_node = this->head; // this->head è ereditato da List<Data>
    found = false;

    while (current_node != nullptr) {
        if (current_node->value == val) {
            found = true;
            return;
        } else if (current_node->value < val) {
            prev_node = current_node;
            current_node = current_node->next;
        } else { // current_node->value > val, punto di inserimento trovato prima di current_node
            return;
        }
    }
    // Se il loop finisce, val non è stata trovata e dovrebbe essere inserita alla fine (o la lista è vuota)
}


/* ************************************************************************** */
// Costruttori Specifici

template <typename Data>
SetLst<Data>::SetLst(const TraversableContainer<Data>& container) {
    // this->size è gestito da List<Data>
    // this->Clear(); // Assicura stato pulito, List::Clear() sarà chiamato
    container.Traverse(
        [this](const Data& dat) {
            this->Insert(dat); // Insert si occuperà di mantenere ordine e unicità
        }
    );
}

template <typename Data>
SetLst<Data>::SetLst(MappableContainer<Data>&& container) {
    // this->Clear();
    container.Map(
        [this](Data& dat) {
            this->Insert(std::move(dat));
        }
    );
}

/* ************************************************************************ */
// Copy constructor
template <typename Data>
SetLst<Data>::SetLst(const SetLst<Data>& other) : List<Data>(other) {
    // Il costruttore di copia di List<Data> dovrebbe gestire la copia profonda.
    // this->size è copiato da List<Data>'s copy constructor.
}

// Move constructor
template <typename Data>
SetLst<Data>::SetLst(SetLst<Data>&& other) noexcept : List<Data>(std::move(other)) {
    // Il costruttore di movimento di List<Data> dovrebbe gestire il trasferimento delle risorse.
    // this->size è spostato da List<Data>'s move constructor.
}

/* ************************************************************************ */
// Move assignment
template <typename Data>
SetLst<Data>& SetLst<Data>::operator=(SetLst<Data>&& other) noexcept {
    if (this != &other) {
        List<Data>::operator=(std::move(other)); // Call base class move assignment
    }
    return *this;
}
/* ************************************************************************ */
// Gli operatori di assegnazione sono =default nell'header, il che va bene
// se List<Data> ha operatori di assegnazione ben definiti.

/* ************************************************************************ */
// Comparison operators
template <typename Data>
bool SetLst<Data>::operator==(const SetLst<Data>& other) const noexcept {
    if (this->size != other.size) {
        return false;
    }
    typename List<Data>::Node* temp_this = this->head;
    typename List<Data>::Node* temp_other = other.head;
    while (temp_this != nullptr) { // implies temp_other is also not nullptr due to size check
        if (temp_this->value != temp_other->value) {
            return false;
        }
        temp_this = temp_this->next;
        temp_other = temp_other->next;
    }
    return true;
}

template <typename Data>
bool SetLst<Data>::operator!=(const SetLst<Data>& other) const noexcept {
    return !(*this == other);
}

/* ************************************************************************ */
// Specific member functions (inherited from OrderedDictionaryContainer)

template <typename Data>
const Data& SetLst<Data>::Min() const {
    if (this->size == 0) {
        throw std::length_error("Set is empty");
    }
    return this->Front(); // List::Front()
}

template <typename Data>
Data SetLst<Data>::MinNRemove() {
    if (this->size == 0) {
        throw std::length_error("Set is empty");
    }
    Data min_val = this->Front();
    this->RemoveFromFront(); // List::RemoveFromFront()
    return min_val;
}

template <typename Data>
void SetLst<Data>::RemoveMin() {
    if (this->size == 0) {
        throw std::length_error("Set is empty");
    }
    this->RemoveFromFront();
}

template <typename Data>
const Data& SetLst<Data>::Max() const {
    if (this->size == 0) {
        throw std::length_error("Set is empty");
    }
    return this->Back(); // List::Back()
}

template <typename Data>
Data SetLst<Data>::MaxNRemove() {
    if (this->size == 0) {
        throw std::length_error("Set is empty");
    }
    // List non ha RemoveFromBack() standard, ma SetLst eredita da List,
    
    // quindi se List ha questa funzionalità, può essere usata.
    // Altrimenti, dobbiamo implementare la rimozione dell'ultimo elemento.
    // Assumendo che List::Remove(value) o una navigazione manuale sia possibile.
    // Per semplicità, se List non ha RemoveFromBack, si può fare:
    Data max_val = this->Back();
    this->Remove(max_val); // Rimuove l'UNICA istanza di max_val, che è l'ultima.
                           // Questo funziona perché SetLst non ha duplicati.
                           // Se List::Remove(value) non è efficiente o non esiste,
                           // si dovrebbe navigare fino al penultimo nodo.
    return max_val;
}

template <typename Data>
void SetLst<Data>::RemoveMax() {
    if (this->size == 0) {
        throw std::length_error("Set is empty");
    }
    // Come MaxNRemove, dipende da List.
    this->Remove(this->Back()); // Rimuove l'UNICA istanza di max_val.
}


template <typename Data>
const Data& SetLst<Data>::Predecessor(const Data& val) const {
  if (this->size == 0) { throw std::length_error("Set is empty"); }
  bool found = false;
    typename List<Data>::Node* prev_node = nullptr;
    typename List<Data>::Node* current_node = nullptr;
    FindNode(val, found, prev_node, current_node);

    if (found) { // val è nell'insieme
        if (prev_node == nullptr) { // val è il minimo
            throw std::length_error("values the minimum, no predecessor");
        }
        return prev_node->value;
    } else { // val non è nell'insieme, current_node è dove val sarebbe inserita (o nullptr)
             // prev_node è il nodo prima di current_node (o l'ultimo se current_node è nullptr)
        if (prev_node == nullptr) { // val sarebbe il nuovo minimo, nessun predecessore esistente
             throw std::length_error("No element is a predecessor to the given value (value would be new minimum)");
        }
        return prev_node->value;
    }
}

template <typename Data>
Data SetLst<Data>::PredecessorNRemove(const Data& val) {
    if (this->size == 0) { throw std::length_error("Set is empty"); }
    bool found = false;
    typename List<Data>::Node* prev_node_of_val = nullptr;
    typename List<Data>::Node* node_of_val = nullptr;
    FindNode(val, found, prev_node_of_val, node_of_val);

    typename List<Data>::Node* pred_node_target = nullptr;

    if (found) {
        if (prev_node_of_val == nullptr) { throw std::length_error("values the minimum, no predecessor"); }
        pred_node_target = prev_node_of_val;
    } else {
        if (prev_node_of_val == nullptr) { throw std::length_error("No element is a predecessor to the given value"); }
        pred_node_target = prev_node_of_val;
    }
    
    Data pred_data = pred_node_target->value;
    // Per rimuovere pred_node_target, abbiamo bisogno del suo predecessore.
    // Dobbiamo fare una nuova ricerca o modificare FindNode per restituire anche il nonno.
    // Per semplicità, usiamo List::Remove(data) se disponibile e affidabile per liste ordinate senza duplicati.
    this->Remove(pred_data); // Rimuove l'unica istanza di pred_data
    return pred_data;
}

template <typename Data>
void SetLst<Data>::RemovePredecessor(const Data& val) {
    if (this->size == 0) { throw std::length_error("Set is empty"); }
    // Simile a PredecessorNRemove
    bool found = false;
    typename List<Data>::Node* prev_node_of_val = nullptr;
    typename List<Data>::Node* node_of_val = nullptr;
    FindNode(val, found, prev_node_of_val, node_of_val);

    typename List<Data>::Node* pred_node_target = nullptr;

    if (found) {
        if (prev_node_of_val == nullptr) { throw std::length_error("values the minimum, no predecessor"); }
        pred_node_target = prev_node_of_val;
    } else {
        if (prev_node_of_val == nullptr) { throw std::length_error("No element is a predecessor to the given value"); }
        pred_node_target = prev_node_of_val;
    }
    this->Remove(pred_node_target->value);
}

template <typename Data>
const Data& SetLst<Data>::Successor(const Data& val) const {
  if (this->size == 0) { throw std::length_error("Set is empty"); }
  bool found = false;
    typename List<Data>::Node* prev_node = nullptr;
    typename List<Data>::Node* current_node = nullptr; // Nodo di val se found, o dove val sarebbe inserita
    FindNode(val, found, prev_node, current_node);

    if (found) {
        if (current_node->next == nullptr) { // val è il massimo
            throw std::length_error("values the maximum, no successor");
        }
        return current_node->next->value;
    } else { // val non è nell'insieme
        if (current_node == nullptr) { // val sarebbe il nuovo massimo
            throw std::length_error("No element is a successor to the given value (value would be new maximum)");
        }
        return current_node->value; // current_node è il primo elemento > val
    }
}

template <typename Data>
Data SetLst<Data>::SuccessorNRemove(const Data& val) {
    if (this->size == 0) { throw std::length_error("Set is empty"); }
    bool found = false;
    typename List<Data>::Node* prev_node_of_val = nullptr;
    typename List<Data>::Node* node_of_val = nullptr;
    FindNode(val, found, prev_node_of_val, node_of_val);

    typename List<Data>::Node* succ_node_target = nullptr;

    if (found) {
        if (node_of_val->next == nullptr) { throw std::length_error("values the maximum, no successor"); }
        succ_node_target = node_of_val->next;
    } else {
        if (node_of_val == nullptr) { throw std::length_error("No element is a successor to the given value"); }
        succ_node_target = node_of_val;
    }
    Data succ_data = succ_node_target->value;
    this->Remove(succ_data);
    return succ_data;
}

template <typename Data>
void SetLst<Data>::RemoveSuccessor(const Data& val) {
    if (this->size == 0) { throw std::length_error("Set is empty"); }
    // Simile a SuccessorNRemove
    bool found = false;
    typename List<Data>::Node* prev_node_of_val = nullptr;
    typename List<Data>::Node* node_of_val = nullptr;
    FindNode(val, found, prev_node_of_val, node_of_val);
    
    typename List<Data>::Node* succ_node_target = nullptr;

    if (found) {
        if (node_of_val->next == nullptr) { throw std::length_error("values the maximum, no successor"); }
        succ_node_target = node_of_val->next;
    } else {
        if (node_of_val == nullptr) { throw std::length_error("No element is a successor to the given value"); }
        succ_node_target = node_of_val;
    }
    this->Remove(succ_node_target->value);
}


/* ************************************************************************ */
// Specific member functions (inherited from DictionaryContainer)

template <typename Data>
bool SetLst<Data>::Insert(const Data& val) {
    bool found = false;
    typename List<Data>::Node* prev_node = nullptr;
    typename List<Data>::Node* current_node = nullptr;
    this->FindNode(val, found, prev_node, current_node);

    if (found) {
        return false; // Elemento già presente
    } else {
        typename List<Data>::Node* newNode = new typename List<Data>::Node(val); // Copia di val
        if (prev_node == nullptr) {
            // Inserimento in testa (o lista vuota)
            newNode->next = this->head;
            this->head = newNode;
            if (this->tail == nullptr) { // La lista era vuota
                this->tail = newNode;
            }
        } else {
            // Inserimento dopo prev_node
            newNode->next = prev_node->next; // prev_node->next è current_node
            prev_node->next = newNode;
            if (newNode->next == nullptr) { // Se newNode è diventato l'ultimo nodo
                this->tail = newNode;
            }
        }
        this->size++;
        return true;
    }
}

template <typename Data>
bool SetLst<Data>::Insert(Data&& val) noexcept {
    bool found = false;
    typename List<Data>::Node* prev_node = nullptr;
    typename List<Data>::Node* current_node = nullptr;
    this->FindNode(val, found, prev_node, current_node); // FindNode prende const Data&, ok per val rvalue

    if (found) {
        return false; // Elemento già presente
    } else {
        typename List<Data>::Node* newNode = new typename List<Data>::Node(std::move(val));
        if (prev_node == nullptr) {
            // Inserimento in testa (o lista vuota)
            newNode->next = this->head;
            this->head = newNode;
            if (this->tail == nullptr) { // La lista era vuota
                this->tail = newNode;
            }
        } else {
            // Inserimento dopo prev_node
            newNode->next = prev_node->next; // prev_node->next è current_node
            prev_node->next = newNode;
            if (newNode->next == nullptr) { // Se newNode è diventato l'ultimo nodo
                this->tail = newNode;
            }
        }
        this->size++;
        return true;
    }
}

template <typename Data>
bool SetLst<Data>::Remove(const Data& val) {
    bool found = false;
    typename List<Data>::Node* prev_node = nullptr;
    typename List<Data>::Node* current_node = nullptr;
    FindNode(val, found, prev_node, current_node);

    if (found) {
        // List<Data>::Remove(val) potrebbe non essere l'ideale se non è ottimizzata
        // per liste ordinate o se rimuove solo la prima occorrenza.
        // Dato che SetLst non ha duplicati e FindNode ci dà il nodo,
        // sarebbe meglio avere una List::RemoveNode(prev, current) se esistesse.
        // Se List<Data>::Remove(const Data&) è garantita per rimuovere l'elemento
        // corretto in una lista ordinata senza duplicati, allora va bene.
        // Altrimenti, si dovrebbe implementare una rimozione più mirata:
        if (prev_node == nullptr) { // Rimuovi dalla testa (current_node è head)
            this->List<Data>::RemoveFromFront(); // Metodo valido di List
        } else {
            // Rimuovi current_node, il cui predecessore è prev_node.
            // current_node è garantito non essere nullptr perché found è true.
            // prev_node è garantito non essere nullptr per questa clausola else.
            
            prev_node->next = current_node->next; // Bypass current_node
            
            if (current_node == this->tail) { // Se stavamo rimuovendo la coda
                this->tail = prev_node;     // prev_node diventa la nuova coda
            }
            // Se current_node->next era nullptr (cioè current_node era tail),
            // allora prev_node->next è ora nullptr, e tail è correttamente prev_node.

            delete current_node;
            this->size--;
        }
        return true;
    } else {
        return false; // Elemento non trovato
    }
}
/* ************************************************************************ */

// Specific member functions (inherited from LinearContainer)

// operator[] (mutable)
template <typename Data>
Data& SetLst<Data>::operator[](unsigned long index) {
    return List<Data>::operator[](index);
}

// operator[] (non-mutable)
template <typename Data>
const Data& SetLst<Data>::operator[](unsigned long index) const {
    return List<Data>::operator[](index);
}

// Front (mutable)
template <typename Data>
Data& SetLst<Data>::Front() {
    return List<Data>::Front();
}

// Front (non-mutable)
template <typename Data>
const Data& SetLst<Data>::Front() const {
    return List<Data>::Front();
}

// Back (mutable)
template <typename Data>
Data& SetLst<Data>::Back() {
    return List<Data>::Back();
}

// Back (non-mutable)
template <typename Data>
const Data& SetLst<Data>::Back() const {
    return List<Data>::Back();
}

/* ************************************************************************** */

// Specific member function (inherited from TestableContainer)
template <typename Data>
bool SetLst<Data>::Exists(const Data& val) const noexcept {
    if (this->size == 0) {
        return false;
    }
    bool found = false;
    typename List<Data>::Node* prev_node = nullptr;
    typename List<Data>::Node* current_node = nullptr;
    FindNode(val, found, prev_node, current_node);
    return found;
}

/* ************************************************************************ */

// Specific member function (inherited from ClearableContainer)
template <typename Data>
void SetLst<Data>::Clear() noexcept {
    List<Data>::Clear(); // Chiama Clear della classe base List
}

/* ************************************************************************** */

} // namespace lasd
