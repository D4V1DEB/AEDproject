#include "Deque.h"

// Node
template <class T>
Node<T>::Node(T data) {
    m_data = data;
    m_next = m_prev = nullptr;
}

template <class T>
Node<T>::~Node() {}

// Deque
template <class T>
Deque<T>::Deque() {
    head = nullptr;
    tail = nullptr;
}

template <class T>
Deque<T>::~Deque() {
    while (!isEmpty()) {
        removeFromBegin();
    }
}

template <class T>
bool Deque<T>::isEmpty() {
    return head == nullptr;
}

template <class T>
void Deque<T>::insertAtBegin(T d) {
    Node<T>* node = new Node<T>(d);
    if (head == nullptr) {
        head = node;
        tail = node;
    } else {
        node->m_next = head;
        head->m_prev = node;
        head = node;
    }
}

template <class T>
void Deque<T>::insertAtEnd(T d) {
    Node<T>* node = new Node<T>(d);
    if (isEmpty()) {
        head = node;
        tail = node;
    } else {
        tail->m_next = node;
        node->m_prev = tail;
        tail = node;
    }
}

template <class T>
void Deque<T>::removeFromBegin() {
    if (isEmpty()) {
        return;
    }
    
    Node<T>* temp = head;
    if (head == tail) {
        head = nullptr;
        tail = nullptr;
    } else {
        head = head->m_next;
        head->m_prev = nullptr;
    }
    delete temp;
}

template <class T>
void Deque<T>::removeFromEnd() {
    if (isEmpty()) {
        return;
    }
    
    Node<T>* temp = tail;
    if (head == tail) {
        head = nullptr;
        tail = nullptr;
    } else {
        tail = tail->m_prev;
        tail->m_next = nullptr;
    }
    delete temp;
}

template <class T>
T Deque<T>::getBegin() {
    if (isEmpty()) {
        throw std::runtime_error("Deque is empty: cannot get front element");
    }
    return head->m_data;
}

template <class T>
T Deque<T>::getEnd() {
    if (isEmpty()) {
        throw std::runtime_error("Deque is empty: cannot get back element");
    }
    return tail->m_data;
}

//debug
template<class T>
void Deque<T>::print() {
    Node<T>* current = head;
    while (current) {
        std::cout << current->m_data << " ";
        current = current->m_next;
    }
    std::cout << "\n";
}


//instanciaciones explicitas
template class Deque<int>;
template class Deque<double>;
template class Deque<std::string>;