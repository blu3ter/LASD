#ifndef VECTOR_CPP
#define VECTOR_CPP

#include <stdexcept>
#include <string>
#include <iostream> // Per std::cerr (debug)
#include "./vector.hpp"

namespace lasd {

/* ************************************************************************** */

// Specific constructors
template<typename Data>
Vector<Data>::Vector(const ulong newsize) {
  size = newsize;
  if (newsize > 0) {
    Elements = new Data[size] {};
  } else {
    Elements = nullptr;
  }
}

template<typename Data>
Vector<Data>::Vector(const TraversableContainer<Data>& con) : Vector(con.Size()) {
  if (Elements != nullptr) { // Assicura che Elements sia allocato se con.Size() > 0
    ulong index = 0;
    con.Traverse(
      [this, &index](const Data& dat) {
        Elements[index++] = dat;
      }
    );
  }
}

template<typename Data>
Vector<Data>::Vector(MappableContainer<Data>&& con) : Vector(con.Size()) {
  if (Elements != nullptr) { // Assicura che Elements sia allocato se con.Size() > 0
    ulong index = 0;
    con.Map(
      [this, &index](Data& dat) {
        Elements[index++] = std::move(dat);
      }
    );
  }
}

/* ************************************************************************** */

// Copy constructor
template<typename Data>
Vector<Data>::Vector(const Vector<Data>& vec) {
  size = vec.size;
  if (size > 0) {
    if (vec.Elements != nullptr) {
      Elements = new Data[size];
      std::copy(vec.Elements, vec.Elements + size, Elements);
    } else {
      // Questo caso indica uno stato anomalo per 'vec' se size > 0,
      // o è la parte base di SetVec che non usa il suo 'Elements' ereditato.
      // Allochiamo e inizializziamo a default per evitare crash,
      // ma i dati non vengono copiati perché la sorgente è nullptr.
      Elements = new Data[size] {};
      // std::cerr << "Warning: Vector copy constructor called with vec.Elements == nullptr and vec.size > 0. Allocating default initialized array for *this." << std::endl;
    }
  } else {
    Elements = nullptr; // Se size è 0, Elements deve essere nullptr.
  }
}

// Move constructor
template<typename Data>
Vector<Data>::Vector(Vector<Data>&& vec) noexcept {
  std::swap(size, vec.size);
  std::swap(Elements, vec.Elements);
}

/* ************************************************************************** */

// Destructor
template<typename Data>
Vector<Data>::~Vector() noexcept {
  delete[] Elements;
  // Elements = nullptr; // Non strettamente necessario qui, ma buona pratica se l'oggetto potesse essere riutilizzato (non il caso per il distruttore)
}

/* ************************************************************************** */

// Copy assignment
template<typename Data>
Vector<Data>& Vector<Data>::operator=(const Vector<Data>& vec) {
  if (this != &vec) { // Protezione auto-assegnazione
    // Crea una copia temporanea usando il costruttore di copia (ora più robusto)
    Vector<Data> temp(vec);
    // Scambia i membri con la copia temporanea
    std::swap(size, temp.size);
    std::swap(Elements, temp.Elements);
  }
  // La vecchia memoria di *this è ora in temp e sarà deallocata quando temp esce dallo scope.
  return *this;
}

// Move assignment
template<typename Data>
Vector<Data>& Vector<Data>::operator=(Vector<Data>&& vec) noexcept {
  if (this != &vec) { // Protezione auto-assegnazione (anche se meno critica per move)
    std::swap(size, vec.size);
    std::swap(Elements, vec.Elements);
    // vec è ora nello stato "svuotato", i suoi vecchi membri sono in *this.
    // Il distruttore di vec gestirà la deallocazione dei suoi (ora ex-this) membri se necessario.
  }
  return *this;
}

/* ************************************************************************** */

// Comparison operators
template<typename Data>
bool Vector<Data>::operator==(const Vector<Data>& vec) const noexcept {
  if(size != vec.size) { // Confronta prima le size
    return false;
  }
  if (Elements == vec.Elements) { // Se puntano allo stesso array (o entrambi nullptr)
      return true;
  }
  if (Elements == nullptr || vec.Elements == nullptr) { // Se uno è nullptr e l'altro no (e size > 0), sono diversi
      return false; // Questo caso è coperto da size != vec.size se size > 0 per uno e 0 per l'altro
                    // Ma se size è uguale e >0, e uno è nullptr, è un errore di stato o sono diversi.
  }
  for(ulong index = 0; index < size; ++index) {
    if(Elements[index] != vec.Elements[index]) {
      return false;
    }
  }
  return true;
}

template<typename Data>
bool Vector<Data>::operator!=(const Vector<Data>& vec) const noexcept {
  return !(*this == vec);
}

/* ************************************************************************** */

// Specific member functions (inherited from LinearContainer)
template<typename Data>
const Data& Vector<Data>::operator[]( ulong index) const {
  if(index < size) {
    if (Elements == nullptr) { // Controllo di sicurezza
        throw std::logic_error("Access to vector with null elements but size > 0.");
    }
    return Elements[index];
  } else {
    throw std::out_of_range("Access at index " + std::to_string(index) + "; vector size " + std::to_string(size) + ".");
  }
}

template<typename Data>
Data& Vector<Data>::operator[](ulong index) {
  if(index < size) {
    if (Elements == nullptr) { // Controllo di sicurezza
        throw std::logic_error("Access to vector with null elements but size > 0.");
    }
    return Elements[index];
  } else {
    throw std::out_of_range("Access at index " + std::to_string(index) + "; vector size " + std::to_string(size) + ".");
  }
}

template<typename Data>
const Data& Vector<Data>::Front() const {
  if(size != 0) {
    if (Elements == nullptr) { // Controllo di sicurezza
        throw std::logic_error("Access to Front() of vector with null elements but size > 0.");
    }
    return Elements[0];
  } else {
    throw std::length_error("Access to an empty vector.");
  }
}

template<typename Data>
Data& Vector<Data>::Front() {
  if(size != 0) {
    if (Elements == nullptr) { // Controllo di sicurezza
        throw std::logic_error("Access to Front() of vector with null elements but size > 0.");
    }
    return Elements[0];
  } else {
    throw std::length_error("Access to an empty vector.");
  }
}

template<typename Data>
const Data& Vector<Data>::Back() const {
  if(size != 0) {
    if (Elements == nullptr) { // Controllo di sicurezza
        throw std::logic_error("Access to Back() of vector with null elements but size > 0.");
    }
    return Elements[size - 1];
  } else {
    throw std::length_error("Access to an empty vector.");
  }
}

template<typename Data>
Data& Vector<Data>::Back() {
  if(size != 0) {
    if (Elements == nullptr) { // Controllo di sicurezza
        throw std::logic_error("Access to Back() of vector with null elements but size > 0.");
    }
    return Elements[size - 1];
  } else {
    throw std::length_error("Access to an empty vector.");
  }
}

/* ************************************************************************** */

// Specific member functions (inherited from ResizableContainer)
template<typename Data>
void Vector<Data>::Resize(const ulong newsize) {
  if (newsize == size) return; // Nessun cambiamento necessario

  if (newsize == 0) {
    Clear();
  } else {
    Data* TmpElements = new Data[newsize] {}; // Alloca e inizializza a default
    if (size > 0 && Elements != nullptr) { // Solo se ci sono elementi da copiare
      ulong minsize = (size < newsize) ? size : newsize;
      for (ulong index = 0; index < minsize; ++index) {
        // std::swap(Elements[index], TmpElements[index]); // Questo sposta gli elementi vecchi in TmpElements
                                                        // e i nuovi (default) in Elements. Non è quello che vogliamo.
        TmpElements[index] = std::move(Elements[index]); // Sposta i vecchi elementi nel nuovo buffer
      }
    }
    delete[] Elements; // Dealloca il vecchio buffer
    Elements = TmpElements; // Elements ora punta al nuovo buffer
    size = newsize;
  }
}

/* ************************************************************************** */

// Specific member functions (inherited from ClearableContainer)
template<typename Data>
void Vector<Data>::Clear() {
  delete[] Elements;
  Elements = nullptr;
  size = 0;
}

/* ************************************************************************** */

// Specific constructors (SortableVector)
template<typename Data>
SortableVector<Data>::SortableVector(const ulong newsize) : Vector<Data>(newsize) {}

template<typename Data>
SortableVector<Data>::SortableVector(const TraversableContainer<Data>& con) : Vector<Data>(con) {}

template<typename Data>
SortableVector<Data>::SortableVector(MappableContainer<Data>&& con) : Vector<Data>(std::move(con)) {} // Qui dovrebbe essere Vector<Data>(con) e poi Map.
                                                                                                    // O Vector<Data>(con.Size()) e poi Map.
                                                                                                    // La versione attuale di Vector(MappableContainer&&) è corretta.

/* ************************************************************************** */

// Copy constructor (SortableVector)
template<typename Data>
SortableVector<Data>::SortableVector(const SortableVector<Data>& vec) : Vector<Data>(vec) {}

// Move constructor (SortableVector)
template<typename Data>
SortableVector<Data>::SortableVector(SortableVector<Data>&& vec) noexcept : Vector<Data>(std::move(vec)) {}

/* ************************************************************************** */

// Destructor (SortableVector)
template<typename Data>
SortableVector<Data>::~SortableVector() noexcept = default;

/* ************************************************************************** */

// Copy assignment (SortableVector)
template<typename Data>
SortableVector<Data>& SortableVector<Data>::operator=(const SortableVector<Data>& vec) {
  Vector<Data>::operator=(vec);
  return *this;
}

// Move assignment (SortableVector)
template<typename Data>
SortableVector<Data>& SortableVector<Data>::operator=(SortableVector<Data>&& vec) noexcept {
  Vector<Data>::operator=(std::move(vec));
  return *this;
}

/* ************************************************************************** */

}

#endif
