#include "Stack.h"

template<class T>
Stack<T>::Stack() {
    deque = new Deque<T>();
}

template<class T>
Stack<T>::~Stack() {
    delete deque;
}

template<class T>
T Stack<T>::top() {
    return deque->getEnd();
}

template<class T>
void Stack<T>::pop() {
    deque->removeFromEnd();
}

template<class T>
void Stack<T>::push(T d) {
    deque->insertAtEnd(d);
}

template<class T>
bool Stack<T>::empty() {
    return deque->isEmpty();
}

//debug
template<class T>
void Stack<T>::print() {
    std::cout << "[Stack] ";
    deque->print();
}

template class Stack<int>;
template class Stack<double>;
template class Stack<std::string>;