#ifndef STACK_H
#define STACK_H

#include "Deque.h"

template<class T>
class Stack {
private:
    Deque<T>* deque;
    
public:
    Stack();
    ~Stack();
    
    T top();
    void pop();
    void push(T d);
    bool empty();
    void print();
};

extern template class Stack<int>;
extern template class Stack<double>;
extern template class Stack<std::string>;

#endif