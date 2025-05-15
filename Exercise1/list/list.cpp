namespace lasd {

/* ************************************************************************** */

// Node Move Constructor
template <typename Data>
List<Data>::Node::Node(Node&& nod) noexcept {
  std::swap(value, nod.value);
  std::swap(next, nod.next);
}

/* ************************************************************************** */
// List Specific Constructors

template <typename Data>
List<Data>::List(const TraversableContainer<Data>& con) {
  con.Traverse(
    [this](const Data& dat) {
      InsertAtBack(dat);
    }
  );
}

template <typename Data>
List<Data>::List(MappableContainer<Data>&& con) {
  con.Map(
    [this](Data& dat) {
      InsertAtBack(std::move(dat));
    }
  );
}

/* ************************************************************************** */
// List Copy Constructor
template <typename Data>
List<Data>::List(const List<Data>& lst) {
  if (lst.head == nullptr) {
    head = tail = nullptr;
    size = 0;
  } else {
    Node* tmp = lst.head;
    while(tmp != nullptr) {
      InsertAtBack(tmp->value);
      tmp = tmp->next;
    }
  }
}

// List Move Constructor
template <typename Data>
List<Data>::List(List<Data>&& lst) noexcept {
  std::swap(head, lst.head);
  std::swap(tail, lst.tail);
  std::swap(size, lst.size);
}

/* ************************************************************************** */
// List Destructor
template <typename Data>
List<Data>::~List() noexcept {
  Clear();
}

/* ************************************************************************** */
// List Copy Assignment
template <typename Data>
List<Data>& List<Data>::operator=(const List<Data>& lst) {
  List<Data>* tmpLst = new List<Data>(lst);
  std::swap(*this, *tmpLst);
  delete tmpLst;
  return *this;
}

// List Move Assignment
template <typename Data>
List<Data>& List<Data>::operator=(List<Data>&& lst) noexcept {
  std::swap(head, lst.head);
  std::swap(tail, lst.tail);
  std::swap(size, lst.size);
  return *this;
}

/* ************************************************************************** */
// List Comparison Operators
template <typename Data>
bool List<Data>::operator==(const List<Data>& lst) const noexcept {
  if (size != lst.size) {
    return false;
  }
  Node* curr = head;
  Node* currLst = lst.head;
  while (curr != nullptr) {
    if (curr->value != currLst->value) {
      return false;
    }
    curr = curr->next;
    currLst = currLst->next;
  }
  return true;
}

template <typename Data>
bool List<Data>::operator!=(const List<Data>& lst) const noexcept {
  return !(*this == lst);
}

/* ************************************************************************** */
// List Specific Member functions

template <typename Data>
void List<Data>::InsertAtFront(const Data& dat) {
  Node* newNode = new Node(dat);
  newNode->next = head;
  head = newNode;
  if (tail == nullptr) {
    tail = head;
  }
  size++;
}

template <typename Data>
void List<Data>::InsertAtFront(Data&& dat) {
  Node* newNode = new Node(std::move(dat));
  newNode->next = head;
  head = newNode;
  if (tail == nullptr) {
    tail = head;
  }
  size++;
}

template <typename Data>
void List<Data>::RemoveFromFront() {
  if (head == nullptr) {
    throw std::length_error("Access to an empty list.");
  }
  Node* temp = head;
  head = head->next;
  if (head == nullptr) { // or if (size == 1) before decrement
    tail = nullptr;
  }
  delete temp;
  size--;
}

template <typename Data>
Data List<Data>::FrontNRemove() {
  if (head == nullptr) {
    throw std::length_error("Access to an empty list.");
  }
  Data retVal = std::move(head->value); // Use move for efficiency
  RemoveFromFront();
  return retVal;
}

template <typename Data>
void List<Data>::InsertAtBack(const Data& dat) {
  Node* newNode = new Node(dat);
  if (tail == nullptr) {
    head = tail = newNode;
  } else {
    tail->next = newNode;
    tail = newNode;
  }
  size++;
}

template <typename Data>
void List<Data>::InsertAtBack(Data&& dat) {
  Node* newNode = new Node(std::move(dat));
  if (tail == nullptr) {
    head = tail = newNode;
  } else {
    tail->next = newNode;
    tail = newNode;
  }
  size++;
}

template <typename Data>
void List<Data>::RemoveFromBack() {
    if (head == nullptr) {
        throw std::length_error("Access to an empty list.");
    }
    if (head == tail) { // Only one element
        delete head;
        head = tail = nullptr;
    } else {
        Node* curr = head;
        while (curr->next != tail) {
            curr = curr->next;
        }
        delete tail;
        tail = curr;
        tail->next = nullptr;
    }
    size--;
}

template <typename Data>
Data List<Data>::BackNRemove() {
    if (tail == nullptr) { // or head == nullptr
        throw std::length_error("Access to an empty list.");
    }
    Data retVal = std::move(tail->value);
    RemoveFromBack();
    return retVal;
}

/* ************************************************************************** */
// Inherited from MutableLinearContainer

template <typename Data>
Data& List<Data>::operator[](ulong index) {
  if (index >= size) {
    throw std::out_of_range("Access at index " + std::to_string(index) + "; list size " + std::to_string(size) + ".");
  }
  Node* curr = head;
  for (ulong i = 0; i < index; ++i) {
    curr = curr->next;
  }
  return curr->value;
}

template <typename Data>
Data& List<Data>::Front() {
  if (head == nullptr) {
    throw std::length_error("Access to an empty list.");
  }
  return head->value;
}

template <typename Data>
Data& List<Data>::Back() {
  if (tail == nullptr) {
    throw std::length_error("Access to an empty list.");
  }
  return tail->value;
}

/* ************************************************************************** */
// Inherited from LinearContainer (const versions)

template <typename Data>
const Data& List<Data>::operator[](ulong index) const {
  if (index >= size) {
    throw std::out_of_range("Access at index " + std::to_string(index) + "; list size " + std::to_string(size) + ".");
  }
  Node* curr = head;
  for (ulong i = 0; i < index; ++i) {
    curr = curr->next;
  }
  return curr->value;
}

template <typename Data>
const Data& List<Data>::Front() const {
  if (head == nullptr) {
    throw std::length_error("Access to an empty list.");
  }
  return head->value;
}

template <typename Data>
const Data& List<Data>::Back() const {
  if (tail == nullptr) {
    throw std::length_error("Access to an empty list.");
  }
  return tail->value;
}

/* ************************************************************************** */
// Inherited from MappableContainer

template <typename Data>
void List<Data>::Map(MapFun fun) {
  PreOrderMap(fun);
}

/* ************************************************************************** */
// Inherited from PreOrderMappableContainer

template <typename Data>
void List<Data>::PreOrderMap(MapFun fun) {
  Node* curr = head;
  while (curr != nullptr) {
    fun(curr->value);
    curr = curr->next;
  }
}

/* ************************************************************************** */
// Inherited from PostOrderMappableContainer

template <typename Data>
void List<Data>::PostOrderMap(MapFun fun) {
    // For a singly linked list, a typical post-order map requires recursion or an auxiliary stack.
    // Implementing with recursion to match typical post-order traversal logic.
    std::function<void(Node*)> recMap = 
        [&](Node* curr) {
        if (curr != nullptr) {
            recMap(curr->next);
            fun(curr->value);
        }
    };
    recMap(head);
}

/* ************************************************************************** */
// Inherited from TraversableContainer

template <typename Data>
void List<Data>::Traverse(TraverseFun fun) const {
  PreOrderTraverse(fun);
}

/* ************************************************************************** */
// Inherited from PreOrderTraversableContainer

template <typename Data>
void List<Data>::PreOrderTraverse(TraverseFun fun) const {
  Node* curr = head;
  while (curr != nullptr) {
    fun(curr->value);
    curr = curr->next;
  }
}

/* ************************************************************************** */
// Inherited from PostOrderTraversableContainer

template <typename Data>
void List<Data>::PostOrderTraverse(TraverseFun fun) const {
    // For a singly linked list, a typical post-order traverse requires recursion or an auxiliary stack.
    // Implementing with recursion.
    std::function<void(const Node*)> recTraverse = 
        [&](const Node* curr) {
        if (curr != nullptr) {
            recTraverse(curr->next);
            fun(curr->value);
        }
    };
    recTraverse(head);
}

/* ************************************************************************** */
// Inherited from ClearableContainer

template <typename Data>
void List<Data>::Clear() {
  Node* curr = head;
  while (curr != nullptr) {
    Node* temp = curr;
    curr = curr->next;
    delete temp;
  }
  head = tail = nullptr;
  size = 0;
}

/* ************************************************************************** */

}
