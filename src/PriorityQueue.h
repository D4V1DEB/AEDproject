#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#include "heapTree.h"

template<class T>
class PriorityQueue {
private:
    HeapTree<T> heap;

public:
    void push(const T& elem) { heap.insert(elem); }
    T pop(){ return heap.extract_min(); }

    T top() const { return heap.peek_min(); }
    bool empty() const { return heap.empty(); }
};

// PriorityQueue
template class PriorityQueue<NodeDistance<int, int>>;
template class PriorityQueue<NodeDistance<int, float>>;
template class PriorityQueue<NodeDistance<int, double>>;
template class PriorityQueue<NodeDistance<std::string, int>>;
template class PriorityQueue<NodeDistance<double, float>>;

#endif //PRIORITYQUEUE_H
