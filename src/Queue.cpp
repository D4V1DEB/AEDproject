#include "Queue.h"

template<class T>
Queue<T>::Queue() {
    deque = new Deque<T>();
}

template<class T>
Queue<T>::~Queue() {
    delete deque;
}

template<class T>
T Queue<T>::front() {
    return deque->getBegin();
}

template<class T>
void Queue<T>::pop() {
    deque->removeFromBegin();
}

template<class T>
void Queue<T>::push(T d) {
    deque->insertAtEnd(d);
}

template<class T>
bool Queue<T>::empty() {
    return deque->isEmpty();
}

//debug
template<class T>
void Queue<T>::print() {
    std::cout << "[Queue] ";
    deque->print();
}

template class Queue<int>;
template class Queue<double>;
template class Queue<std::string>;