

#include <stdexcept>
#include <string>
#include <iostream> 


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
      Elements = new Data[size] {};
      
    }
  } else {
    Elements = nullptr; //if size = 0
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
 
}

/* ************************************************************************** */

// Copy assignment
template<typename Data>
Vector<Data>& Vector<Data>::operator=(const Vector<Data>& vec) {
  if (this != &vec) { 
    Vector<Data> temp(vec);

    std::swap(size, temp.size);
    std::swap(Elements, temp.Elements);
  }
  return *this;
}

// Move assignment
template<typename Data>
Vector<Data>& Vector<Data>::operator=(Vector<Data>&& vec) noexcept {
  if (this != &vec) { 
    std::swap(size, vec.size);
    std::swap(Elements, vec.Elements);
  }
  return *this;
}

/* ************************************************************************** */

// Comparison operators
template<typename Data>
bool Vector<Data>::operator==(const Vector<Data>& vec) const noexcept {
  if(size != vec.size) { 
    return false;
  }
  if (Elements == vec.Elements) { 
      return true;
  }
  if (Elements == nullptr || vec.Elements == nullptr) { 
      return false; //if size > 0 and one of the vectors is null
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
    if (Elements == nullptr) {
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
    if (Elements == nullptr) { 
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
    if (Elements == nullptr) {
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
    if (Elements == nullptr) { 
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
    if (Elements == nullptr) { 
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
    if (Elements == nullptr) {
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
  if (newsize == size) return; 

  if (newsize == 0) {
    Clear();
  } else {
    Data* TmpElements = new Data[newsize] {}; 
    if (size > 0 && Elements != nullptr) {
      ulong minsize = (size < newsize) ? size : newsize;
      for (ulong index = 0; index < minsize; ++index) {
      
        TmpElements[index] = std::move(Elements[index]); 
      }
    }
    delete[] Elements;
    Elements = TmpElements;
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
SortableVector<Data>::SortableVector(MappableContainer<Data>&& con) : Vector<Data>(std::move(con)) {} 

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


