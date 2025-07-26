#ifndef HEAPTREE_H
#define HEAPTREE_H

#include "Vector.h"
#include "NodeDistance.h"

template<class T>
class HeapTree {
    Vector<T> data;

public:
    HeapTree() : data(10) {}
    void insert(T item) {
        data.add_item_end(item);
        heapify_up(data.size() - 1);
    }

    T extract_min() {
        if (data.is_empty()) std::cout<<"empty heap";
        T min = data.get(0);
        data.set(0, data.get(data.size()-1));
        data.remove_index(data.size()-1);
        heapify_down(0);
        return min;
    }

    T peek_min() const {
        return data.get(0);
    }

    bool empty() const { return data.is_empty(); }

private:
    void heapify_up(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (data.get(idx) < data.get(parent)) {
                std::swap(data.get(idx), data.get(parent));
                idx = parent;
            } else {
                break;
            }
        }
    }

    void heapify_down(int idx) {
        while (true) {
            int smallest = idx;
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;

            if (left < data.size() && data.get(left) < data.get(smallest)) smallest = left;
            if (right < data.size() && data.get(right) < data.get(smallest)) smallest = right;

            if (smallest != idx) {
                std::swap(data.get(idx), data.get(smallest));
                idx = smallest;
            } else {
                break;
            }
        }
    }
};

// HeapTree
template class HeapTree<NodeDistance<int, int>>;
template class HeapTree<NodeDistance<int, float>>;
template class HeapTree<NodeDistance<int, double>>;
template class HeapTree<NodeDistance<std::string, int>>;
template class HeapTree<NodeDistance<std::string, double>>;
template class HeapTree<NodeDistance<double, float>>;

#endif // HEAPTREE_H

