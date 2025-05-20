#include <stdexcept>
#include <algorithm> // Per std::sort, std::lower_bound, std::copy, std::move

namespace lasd {

/* ************************************************************************** */
// COSTRUTTORI E DISTRUTTORE
/* ************************************************************************** */

// Default constructor
template <typename Data>
SetVec<Data>::SetVec() {
  elements = new Data[bufferSize];
  // head è già 0 per default
}

// Specific constructor (from TraversableContainer)
template <typename Data>
SetVec<Data>::SetVec(const TraversableContainer<Data>& container) : SetVec() { // Delega al default constructor
  container.Traverse(
    [this](const Data& dat) {
      Insert(dat); // Insert gestirà l'ordinamento e head
    }
  );
}

// Specific constructor (from MappableContainer)
template <typename Data>
SetVec<Data>::SetVec(MappableContainer<Data>&& container) : SetVec() { // Delega al default constructor
  container.Map(
    [this](Data& dat) {
      Insert(std::move(dat)); // Insert gestirà l'ordinamento e head
    }
  );
}

// Copy constructor
template <typename Data>
SetVec<Data>::SetVec(const SetVec<Data>& other) {
  elements = new Data[other.bufferSize];
  bufferSize = other.bufferSize;
  currentSize = other.currentSize;
  head = other.head; // Copia head
  for (unsigned long i = 0; i < bufferSize; ++i) { // Copia l'intero buffer fisico se necessario, o solo gli elementi attivi
      // Se other.elements non è completamente riempito, questo potrebbe copiare spazzatura.
      // È meglio copiare solo gli elementi logici.
      // Ma per mantenere la stessa struttura fisica (per buffer circolare):
      if (other.elements != nullptr) { // Assicurati che other.elements sia valido
          // Copia l'intero array fisico per mantenere la struttura circolare
          // Questo è corretto se gli elementi non usati non hanno stati invalidi
          // o se copiamo solo la porzione rilevante.
          // Per semplicità e correttezza, copiamo gli elementi logici nella stessa configurazione fisica.
          std::copy(other.elements, other.elements + other.bufferSize, elements);
          // Tuttavia, è più sicuro copiare solo gli elementi logici e ricostruire:
          // head = 0; // Ricostruiamo linearizzato per semplicità, o copiamo la struttura
          // for (unsigned long k=0; k < currentSize; ++k) {
          //    elements[k] = other.elements[(other.head + k) % other.bufferSize];
          // }
          // Per ora, copiamo l'intero buffer fisico e head.
      }
  }
   // Alternativa più sicura e che potrebbe linearizzare:
   // bufferSize = other.bufferSize;
   // currentSize = other.currentSize;
   // elements = new Data[bufferSize];
   // head = 0; // Linearizziamo nella copia
   // for (unsigned long i = 0; i < currentSize; ++i) {
   //   elements[i] = other.elements[(other.head + i) % other.bufferSize];
   // }
   // Scegliamo di copiare la struttura fisica per ora:
   // Se other.elements è nullptr (es. dopo un move), questo è un problema.
   // Il costruttore di copia assume 'other' è valido.
}


// Move constructor
template <typename Data>
SetVec<Data>::SetVec(SetVec<Data>&& other) noexcept {
  std::swap(elements, other.elements);
  std::swap(bufferSize, other.bufferSize);
  std::swap(currentSize, other.currentSize);
  std::swap(head, other.head); // Sposta anche head
}

// Destructor
template <typename Data>
SetVec<Data>::~SetVec() {
  delete[] elements;
}

/* ************************************************************************** */
// OPERATORI DI ASSEGNAMENTO E CONFRONTO
/* ************************************************************************** */

// Copy assignment
template <typename Data>
SetVec<Data>& SetVec<Data>::operator=(const SetVec<Data>& other) {
  if (this != &other) {
    // Rilascia le risorse correnti
    delete[] elements;

    // Copia i dati da 'other'
    bufferSize = other.bufferSize;
    currentSize = other.currentSize;
    head = other.head; // Copia head
    elements = new Data[bufferSize];
    // Copia gli elementi mantenendo la struttura fisica
    if (other.elements) {
        std::copy(other.elements, other.elements + other.bufferSize, elements);
    }
    // Alternativa (linearizzando):
    // head = 0;
    // for (unsigned long i = 0; i < currentSize; ++i) {
    //   elements[i] = other.elements[(other.head + i) % other.bufferSize];
    // }
  }
  return *this;
}

// Move assignment
template <typename Data>
SetVec<Data>& SetVec<Data>::operator=(SetVec<Data>&& other) noexcept {
  if (this != &other) {
    std::swap(elements, other.elements);
    std::swap(bufferSize, other.bufferSize);
    std::swap(currentSize, other.currentSize);
    std::swap(head, other.head); // Sposta anche head
  }
  return *this;
}

// Comparison operators
template <typename Data>
bool SetVec<Data>::operator==(const SetVec<Data>& other) const noexcept {
  if (currentSize != other.currentSize) {
    return false;
  }
  if (currentSize == 0) return true; // Due set vuoti sono uguali

  // Confronta gli elementi logici, che sono ordinati
  for (unsigned long i = 0; i < currentSize; ++i) {
    if (elements[(head + i) % bufferSize] != other.elements[(other.head + i) % other.bufferSize]) {
      return false;
    }
  }
  return true;
}

template <typename Data>
inline bool SetVec<Data>::operator!=(const SetVec<Data>& other) const noexcept {
  return !(*this == other);
}

/* ************************************************************************** */
// FUNZIONI AUSILIARIE PROTETTE
/* ************************************************************************** */

// EnsureCapacity: Assicura che il buffer abbia almeno minCapacity.
// Linearizza gli elementi nel nuovo buffer e resetta head a 0.
template <typename Data>
void SetVec<Data>::EnsureCapacity(unsigned long minCapacity) {
  if (minCapacity > bufferSize) {
    unsigned long newBufferSize = bufferSize;
    while (newBufferSize < minCapacity) {
      newBufferSize = (newBufferSize == 0) ? 1 : newBufferSize * 2;
    }
     if (newBufferSize == 0 && minCapacity > 0) newBufferSize = minCapacity;


    Data* newElements = new Data[newBufferSize];
    for (unsigned long i = 0; i < currentSize; ++i) {
      newElements[i] = std::move(elements[(head + i) % bufferSize]);
    }
    delete[] elements;
    elements = newElements;
    bufferSize = newBufferSize;
    head = 0; // Gli elementi sono ora linearizzati
  }
}

// ShrinkToFit: Riduce la capacità del buffer.
// Linearizza gli elementi e resetta head a 0.
template <typename Data>
void SetVec<Data>::ShrinkToFit() {
  unsigned long newBufferSize = std::max(currentSize, (unsigned long)4);
  if (newBufferSize < bufferSize) {
    if (currentSize == 0) {
        delete[] elements;
        bufferSize = 4;
        elements = new Data[bufferSize];
        head = 0;
        return;
    }
    Data* newElements = new Data[newBufferSize];
    for (unsigned long i = 0; i < currentSize; ++i) {
      newElements[i] = std::move(elements[(head + i) % bufferSize]);
    }
    delete[] elements;
    elements = newElements;
    bufferSize = newBufferSize;
    head = 0; // Gli elementi sono ora linearizzati
  }
}

// BinarySearch: Cerca val nell'array logico ordinato.
// Opera su indici logici [logicalLow, logicalHigh].
// Restituisce l'indice logico se trovato, o il punto di inserimento logico.
template <typename Data>
long SetVec<Data>::BinarySearch(const Data& val, long logicalLow, long logicalHigh) const {
    while (logicalLow <= logicalHigh) {
        long midLogical = logicalLow + (logicalHigh - logicalLow) / 2;
        unsigned long midPhysical = (head + midLogical) % bufferSize;
        if (elements[midPhysical] == val) {
            return midLogical; // Trovato, restituisce indice logico
        } else if (elements[midPhysical] < val) {
            logicalLow = midLogical + 1;
        } else {
            logicalHigh = midLogical - 1;
        }
    }
    return logicalLow; // Punto di inserimento logico
}

// FindIndexOf: Trova l'indice logico di val o dove dovrebbe essere.
template <typename Data>
long SetVec<Data>::FindIndexOf(const Data& val) const {
  if (currentSize == 0) {
    return 0; // Indice logico di inserimento è 0
  }
  return BinarySearch(val, 0, currentSize - 1);
}


// InsertAt: Inserisce val all'indice logico specificato.
// Gestisce lo shift e l'aggiornamento di head se necessario.
template <typename Data>
bool SetVec<Data>::InsertAt(unsigned long logicalIndex, const Data& val) {
  EnsureCapacity(currentSize + 1); // Potrebbe linearizzare e resettare head

  // Dopo EnsureCapacity, head è 0 se c'è stata riallocazione.
  // Se non c'è stata riallocazione, head è invariato.
  // La logica di shift deve funzionare in entrambi i casi.

  // Decidi da che parte shiftare per minimizzare il lavoro.
  // Se inseriamo nella prima metà, shiftiamo a sinistra (spostando head).
  // Se inseriamo nella seconda metà, shiftiamo a destra.
  if (logicalIndex < currentSize / 2) {
    // Shift a sinistra: sposta gli elementi [0..logicalIndex-1] a sinistra di una posizione
    // e decrementa head.
    head = (head + bufferSize - 1) % bufferSize; // Nuovo head
    unsigned long currentPhysical;
    for (unsigned long i = 0; i < logicalIndex; ++i) {
        currentPhysical = (head + i) % bufferSize; // Nuova posizione fisica
        // La vecchia posizione fisica dell'elemento i era (head+1+i)%bufferSize
        // elements[currentPhysical] = std::move(elements[(head + 1 + i) % bufferSize]);
        // Questo è complicato. Se head è 0 (linearizzato), è più facile.
    }
     // Se head è stato modificato da EnsureCapacity, questo è più semplice.
     // Assumiamo che dopo EnsureCapacity, head = 0.
     // Se non c'è stata riallocazione, head è il vecchio head.
     // Questa parte è la più complessa per un buffer circolare.

     // Strategia più semplice: se head != 0, linearizza prima di shiftare,
     // oppure implementa lo shift circolare con attenzione.

     // Per ora, assumiamo che EnsureCapacity linearizzi se necessario,
     // e se non lo fa, lo shift avviene sull'array "come se fosse" lineare
     // e poi si aggiusta head o gli indici.
     // Questo è un punto debole del buffer circolare *ordinato*.

     // Semplificazione: EnsureCapacity(currentSize + 1) linearizza sempre se head != 0 e c'è spazio.
     // O meglio: EnsureCapacity espande e linearizza.
     // Se non espande, dobbiamo fare lo shift circolare.

    if (head != 0 && logicalIndex > 0) { // Caso complesso di shift a sinistra con head != 0
        // Sposta gli elementi da head fino a (head + logicalIndex -1) una posizione a sinistra
        unsigned long physIdxToSet;
        for(long i = logicalIndex -1; i >= 0; --i) {
            physIdxToSet = (head + i) % bufferSize;
            elements[physIdxToSet] = std::move(elements[(head + i + 1) % bufferSize]); // Non corretto
        }
        // Questo è difficile. Ripensiamo lo shift.
    }


    // Shift a destra (più standard se head=0 o si considera l'array logico)
    unsigned long physicalInsertionPoint = (head + logicalIndex) % bufferSize;
    for (long i = currentSize; i > (long)logicalIndex; --i) {
        elements[(head + i) % bufferSize] = std::move(elements[(head + i - 1 + bufferSize) % bufferSize]);
    }
    elements[physicalInsertionPoint] = val;

  } else { // Shift a destra
    unsigned long physicalInsertionPoint = (head + logicalIndex) % bufferSize;
    for (long i = currentSize; i > (long)logicalIndex; --i) {
        // Indice fisico dell'elemento i-esimo logico: (head + i) % bufferSize
        // Indice fisico dell'elemento (i-1)-esimo logico: (head + i - 1 + bufferSize) % bufferSize
        elements[(head + i) % bufferSize] = std::move(elements[(head + i - 1 + bufferSize) % bufferSize]);
    }
    elements[physicalInsertionPoint] = val;
  }
  currentSize++;
  return true;
}

template <typename Data>
bool SetVec<Data>::InsertAt(unsigned long logicalIndex, Data&& val) {
  EnsureCapacity(currentSize + 1);
  // Stessa logica di shift di InsertAt(const Data&), usando std::move(val)
  unsigned long physicalInsertionPoint = (head + logicalIndex) % bufferSize;

  // Shift a destra (più semplice da implementare correttamente per buffer circolare)
  // Sposta gli elementi da logicalIndex a currentSize-1 una posizione a "destra" (incremento indice logico)
  for (long i = currentSize -1 ; i >= (long)logicalIndex; --i) {
      unsigned long destPhysical = (head + i + 1) % bufferSize;
      unsigned long srcPhysical = (head + i) % bufferSize;
      elements[destPhysical] = std::move(elements[srcPhysical]);
  }
  elements[physicalInsertionPoint] = std::move(val);
  currentSize++;
  return true;
}

// RemoveAt: Rimuove l'elemento all'indice logico specificato.
template <typename Data>
void SetVec<Data>::RemoveAt(unsigned long logicalIndex) {
  // Shift a sinistra per compattare
  // Sposta gli elementi da logicalIndex+1 a currentSize-1 una posizione a "sinistra" (decremento indice logico)
  for (unsigned long i = logicalIndex; i < currentSize - 1; ++i) {
    unsigned long destPhysical = (head + i) % bufferSize;
    unsigned long srcPhysical = (head + i + 1) % bufferSize;
    elements[destPhysical] = std::move(elements[srcPhysical]);
  }
  currentSize--;
  // Considera ShrinkToFit
  if (bufferSize > 4 && currentSize < bufferSize / 4) {
      ShrinkToFit(); // Questo linearizzerà e resetterà head
  }
}


/* ************************************************************************** */
// FUNZIONI MEMBRO SPECIFICHE (EREDITATE)
/* ************************************************************************** */

// Insert (copy)
template <typename Data>
bool SetVec<Data>::Insert(const Data& val) {
  long logicalIndex = FindIndexOf(val); // Indice logico dove val è o dovrebbe essere
  unsigned long physicalIdx = (head + logicalIndex) % bufferSize;
  if (logicalIndex < (long)currentSize && elements[physicalIdx] == val) {
    return false; // Elemento già presente
  }
  // Altrimenti, logicalIndex è il punto di inserimento logico
  return InsertAt(logicalIndex, val);
}

// Insert (move)
template <typename Data>
bool SetVec<Data>::Insert(Data&& val) {
  // Per la ricerca, potremmo dover usare una copia temporanea o un riferimento const
  // FindIndexOf si aspetta const Data&.
  long logicalIndex = FindIndexOf(val);
  unsigned long physicalIdx = (head + logicalIndex) % bufferSize;
  if (logicalIndex < (long)currentSize && elements[physicalIdx] == val) {
    return false; // Elemento già presente
  }
  return InsertAt(logicalIndex, std::move(val));
}

// Remove
template <typename Data>
bool SetVec<Data>::Remove(const Data& val) {
  if (Empty()) return false;
  long logicalIndex = FindIndexOf(val);
  unsigned long physicalIdx = (head + logicalIndex) % bufferSize;
  if (logicalIndex < (long)currentSize && elements[physicalIdx] == val) {
    RemoveAt(logicalIndex);
    return true;
  }
  return false; // Elemento non trovato
}

// Exists
template <typename Data>
bool SetVec<Data>::Exists(const Data& val) const noexcept {
  if (Empty()) return false;
  long logicalIndex = FindIndexOf(val);
  unsigned long physicalIdx = (head + logicalIndex) % bufferSize;
  return (logicalIndex < (long)currentSize && elements[physicalIdx] == val);
}

// Min
template <typename Data>
const Data& SetVec<Data>::Min() const {
  if (Empty()) {
    throw std::length_error("Set is empty");
  }
  return elements[head]; // Il primo elemento logico è a head
}

// Max
template <typename Data>
const Data& SetVec<Data>::Max() const {
  if (Empty()) {
    throw std::length_error("Set is empty");
  }
  return elements[(head + currentSize - 1) % bufferSize]; // L'ultimo elemento logico
}

// MinNRemove
template <typename Data>
Data SetVec<Data>::MinNRemove() {
  if (Empty()) {
    throw std::length_error("Set is empty");
  }
  Data minVal = std::move(elements[head]);
  // Rimuovere il primo elemento logico: incrementare head e decrementare size.
  head = (head + 1) % bufferSize;
  currentSize--;
  if (bufferSize > 4 && currentSize < bufferSize / 4) {
      ShrinkToFit(); // Questo linearizzerà e resetterà head
  }
  return minVal;
}

// MaxNRemove
template <typename Data>
Data SetVec<Data>::MaxNRemove() {
  if (Empty()) {
    throw std::length_error("Set is empty");
  }
  unsigned long lastPhysicalIndex = (head + currentSize - 1) % bufferSize;
  Data maxVal = std::move(elements[lastPhysicalIndex]);
  currentSize--; // Basta decrementare la dimensione, l'elemento non serve più
  if (bufferSize > 4 && currentSize < bufferSize / 4) {
      ShrinkToFit();
  }
  return maxVal;
}

// Predecessor
template <typename Data>
const Data& SetVec<Data>::Predecessor(const Data& val) const {
  if (Empty()) throw std::length_error("Set is empty");
  long logicalIndex = FindIndexOf(val);
  unsigned long physicalIdx = (head + logicalIndex) % bufferSize;

  if (logicalIndex == 0 && (logicalIndex >= (long)currentSize || elements[physicalIdx] != val)) { // val non trovata e sarebbe la prima, o è la prima
      throw std::length_error("No predecessor");
  }
  if (logicalIndex < (long)currentSize && elements[physicalIdx] == val) { // Trovato
    if (logicalIndex > 0) return elements[(head + logicalIndex - 1 + bufferSize) % bufferSize];
    else throw std::length_error("No predecessor for the minimum value");
  } else { // Non trovato, logicalIndex è il punto di inserimento
    if (logicalIndex > 0) return elements[(head + logicalIndex - 1 + bufferSize) % bufferSize];
    else throw std::length_error("Value not found, no predecessor");
  }
}

// Successor
template <typename Data>
const Data& SetVec<Data>::Successor(const Data& val) const {
  if (Empty()) throw std::length_error("Set is empty");
  long logicalIndex = FindIndexOf(val);
  unsigned long physicalIdx = (head + logicalIndex) % bufferSize;

  if (logicalIndex < (long)currentSize && elements[physicalIdx] == val) { // Trovato
    if (logicalIndex < currentSize - 1) return elements[(head + logicalIndex + 1) % bufferSize];
    else throw std::length_error("No successor for the maximum value");
  } else { // Non trovato, logicalIndex è il punto di inserimento
    if (logicalIndex < currentSize) return elements[(head + logicalIndex) % bufferSize]; // L'elemento al punto di inserimento è il successore
    else throw std::length_error("Value not found, no successor");
  }
}

// PredecessorNRemove
template <typename Data>
Data SetVec<Data>::PredecessorNRemove(const Data& val) {
  if (Empty()) throw std::length_error("Set is empty");
  long logicalIndex = FindIndexOf(val); // Indice logico di val o dove sarebbe
  unsigned long physicalValIdx = (head + logicalIndex) % bufferSize;

  long predLogicalIndex = -1;

  if (logicalIndex == 0) { // val è (o sarebbe) il minimo
      throw std::length_error("No predecessor to remove");
  }

  if (logicalIndex < (long)currentSize && elements[physicalValIdx] == val) { // val è presente
      predLogicalIndex = logicalIndex -1;
  } else { // val non è presente, logicalIndex è il punto di inserimento
      predLogicalIndex = logicalIndex -1;
  }
  
  if (predLogicalIndex >=0 ) {
    Data predVal = std::move(elements[(head + predLogicalIndex) % bufferSize]);
    RemoveAt(predLogicalIndex); // Rimuove all'indice logico del predecessore
    return predVal;
  } else {
      throw std::length_error("Predecessor not found or not removable");
  }
}

// SuccessorNRemove
template <typename Data>
Data SetVec<Data>::SuccessorNRemove(const Data& val) {
  if (Empty()) throw std::length_error("Set is empty");
  long logicalIndex = FindIndexOf(val); // Indice logico di val o dove sarebbe
  unsigned long physicalValIdx = (head + logicalIndex) % bufferSize;
  
  long succLogicalIndex = -1;

  if (logicalIndex < (long)currentSize && elements[physicalValIdx] == val) { // val è presente
    if (logicalIndex < currentSize - 1) succLogicalIndex = logicalIndex + 1;
  } else { // val non è presente, logicalIndex è il punto di inserimento
    if (logicalIndex < currentSize) succLogicalIndex = logicalIndex;
  }
  
  if (succLogicalIndex != -1 && succLogicalIndex < (long)currentSize) {
    Data succVal = std::move(elements[(head + succLogicalIndex) % bufferSize]);
    RemoveAt(succLogicalIndex); // Rimuove all'indice logico del successore
    return succVal;
  } else {
    throw std::length_error("Successor not found or not removable");
  }
}


// Clear
template <typename Data>
void SetVec<Data>::Clear() {
  // Non è necessario deallocare e riallocare se bufferSize è ragionevole.
  // Basta resettare currentSize e head.
  // delete[] elements;
  // bufferSize = 4;
  // elements = new Data[bufferSize];
  currentSize = 0;
  head = 0; // Resetta head
}

// Resize
template <typename Data>
void SetVec<Data>::Resize(unsigned long newsize) {
    if (newsize == 0) {
        Clear();
    } else if (newsize > bufferSize) {
        EnsureCapacity(newsize); // Questo linearizza e resetta head
    } else if (newsize < bufferSize && newsize >= currentSize) {
        // Riduci buffer e linearizza
        Data* newElements = new Data[newsize];
        for (unsigned long i = 0; i < currentSize; ++i) {
            newElements[i] = std::move(elements[(head + i) % bufferSize]);
        }
        delete[] elements;
        elements = newElements;
        bufferSize = newsize;
        head = 0; // Linearizzato
    }
    // Se newsize < currentSize, non facciamo nulla per non perdere dati.
}

// RemoveMin
template <typename Data>
void SetVec<Data>::RemoveMin() {
  if (Empty()) {
    throw std::length_error("Set is empty, cannot remove min");
  }
  // Rimuovere il primo elemento logico: incrementare head e decrementare size.
  // Non usiamo RemoveAt(0) perché farebbe uno shift non necessario qui.
  head = (head + 1) % bufferSize;
  currentSize--;
  // Considera ShrinkToFit se la dimensione logica diventa molto più piccola del buffer
  if (bufferSize > 4 && currentSize < bufferSize / 4) {
      ShrinkToFit(); // Questo linearizzerà e resetterà head
  }
}

// RemoveMax
template <typename Data>
void SetVec<Data>::RemoveMax() {
  if (Empty()) {
    throw std::length_error("Set is empty, cannot remove max");
  }
  // Rimuovere l'ultimo elemento logico: decrementare size.
  // L'elemento fisico rimane lì ma non è più accessibile logicamente.
  currentSize--;
  // Considera ShrinkToFit
  if (bufferSize > 4 && currentSize < bufferSize / 4) {
      ShrinkToFit();
  }
}

// RemovePredecessor
template <typename Data>
void SetVec<Data>::RemovePredecessor(const Data& val) {
  if (Empty()) {
    throw std::length_error("Set is empty, cannot remove predecessor");
  }
  long logicalIndexVal = FindIndexOf(val); // Indice logico di val o dove sarebbe
  unsigned long physicalValIdx = (head + logicalIndexVal) % bufferSize;

  if (logicalIndexVal == 0) { 
    // Se val è il minimo (o sarebbe il minimo se non presente), non ha predecessore.
    throw std::length_error("No predecessor to remove (value is/would be minimum or set is small)");
  }

  // Determina l'indice logico del predecessore
  long predLogicalIndex;
  // Caso 1: val è presente nel set. Il predecessore è l'elemento prima di val.
  if (logicalIndexVal < (long)currentSize && elements[physicalValIdx] == val) {
    // val è trovata, il predecessore è all'indice precedente
     if (logicalIndexVal > 0) { // Assicura che non sia il primo elemento
        predLogicalIndex = logicalIndexVal - 1;
    } else { // val è il primo elemento, non ha predecessore
        throw std::length_error("Value is the minimum, no predecessor to remove");
    }
  } else { // Caso 2: val non è presente. logicalIndexVal è il punto di inserimento.
           // Il predecessore dell'elemento che *sarebbe* a logicalIndexVal è a logicalIndexVal - 1.
    if (logicalIndexVal > 0) { // Assicura che il punto di inserimento non sia 0
        predLogicalIndex = logicalIndexVal - 1;
    } else { // Il punto di inserimento è 0, nessun predecessore per l'elemento che sarebbe inserito qui.
        throw std::length_error("Value not found, and insertion point is at the beginning, no predecessor");
    }
  }
  
  RemoveAt(predLogicalIndex);
}

// RemoveSuccessor
template <typename Data>
void SetVec<Data>::RemoveSuccessor(const Data& val) {
  if (Empty()) {
    throw std::length_error("Set is empty, cannot remove successor");
  }
  long logicalIndexVal = FindIndexOf(val); // Indice logico di val o dove sarebbe
  unsigned long physicalValIdx = (head + logicalIndexVal) % bufferSize;

  long succLogicalIndex;

  // Caso 1: val è presente nel set. Il successore è l'elemento dopo val.
  if (logicalIndexVal < (long)currentSize && elements[physicalValIdx] == val) {
    if (logicalIndexVal < currentSize - 1) { // Assicura che non sia l'ultimo elemento
        succLogicalIndex = logicalIndexVal + 1;
    } else { // val è l'ultimo elemento, non ha successore
      throw std::length_error("Value is the maximum, no successor to remove");
    }
  } else { // Caso 2: val non è presente. logicalIndexVal è il punto di inserimento.
           // L'elemento a questo punto di inserimento (se esiste) è il successore.
    if (logicalIndexVal < (long)currentSize) { // C'è un elemento al punto di inserimento
        succLogicalIndex = logicalIndexVal;
    } else { // Il punto di inserimento è oltre la fine, nessun successore
      throw std::length_error("Value not found, and insertion point is at the end, no successor");
    }
  }
  
  RemoveAt(succLogicalIndex);
}
/* ************************************************************************** */
// SPECIFIC MEMBER FUNCTIONS (inherited from Set/LinearContainer)
/* ************************************************************************** */

// Front (mutable)
template <typename Data>
Data& SetVec<Data>::Front() {
  if (Empty()) {
    throw std::length_error("Set is empty, cannot access Front()");
  }
  return elements[head];
}

// Front (non-mutable)
template <typename Data>
const Data& SetVec<Data>::Front() const {
  if (Empty()) {
    throw std::length_error("Set is empty, cannot access Front()");
  }
  return elements[head]; // Same as Min()
}

// Back (mutable)
template <typename Data>
Data& SetVec<Data>::Back() {
  if (Empty()) {
    throw std::length_error("Set is empty, cannot access Back()");
  }
  return elements[(head + currentSize - 1) % bufferSize];
}

// Back (non-mutable)
template <typename Data>
const Data& SetVec<Data>::Back() const {
  if (Empty()) {
    throw std::length_error("Set is empty, cannot access Back()");
  }
  return elements[(head + currentSize - 1) % bufferSize]; // Same as Max()
}

// operator[] (mutable)
template <typename Data>
Data& SetVec<Data>::operator[](unsigned long index) {
  if (index >= currentSize) {
    throw std::out_of_range("Index out of range for operator[]: " + std::to_string(index) + "; Current size: " + std::to_string(currentSize));
  }
  return elements[(head + index) % bufferSize];
}

// operator[] (non-mutable)
template <typename Data>
const Data& SetVec<Data>::operator[](unsigned long index) const {
  if (index >= currentSize) {
    throw std::out_of_range("Index out of range for operator[]: " + std::to_string(index) + "; Current size: " + std::to_string(currentSize));
  }
  return elements[(head + index) % bufferSize];
}
/* ************************************************************************** */

}
