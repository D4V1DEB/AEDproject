#ifndef DEQUE_H
#define DEQUE_H

#include <iostream>
#include <stdexcept>

template <class T>
class Node {
public:
    T m_data;
    Node<T>* m_next;
    Node<T>* m_prev;
    
    Node(T data);
    ~Node();
};

template <class T>
class Deque {
private:
    Node<T>* head;
    Node<T>* tail;
    
public:
    Deque();
    ~Deque();
    
    bool isEmpty();
    void insertAtBegin(T d);
    void insertAtEnd(T d);
    void removeFromBegin();
    void removeFromEnd();
    T getBegin();
    T getEnd();
    void print();
};

// instanciaciones explicitas
extern template class Deque<int>;
extern template class Deque<double>;
extern template class Deque<std::string>;

#endif