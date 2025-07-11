#ifndef QUEUE_H
#define QUEUE_H

#include "Deque.h"

template<class T>
class Queue {
private:
    Deque<T>* deque;
    
public:
    Queue();
    ~Queue();
    
    T front();
    void pop();
    void push(T d);
    bool empty();
    void print();
};

extern template class Queue<int>;
extern template class Queue<double>;
extern template class Queue<std::string>;

#endif