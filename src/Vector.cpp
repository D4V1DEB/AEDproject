#include "Vector.h"
#include "Graph.h"  // incluir Node, Edge, and Traits types
#include <iostream>

template<class T>
Vector<T>::Vector(int initialCapacity) {
    if (initialCapacity <= 0) {
        initialCapacity = 10;
    }
    capacity = initialCapacity;
    data = new T[capacity];
    index = 0;
}

template<class T>
Vector<T>::~Vector() {
    delete[] data;
}

template<class T>
void Vector<T>::resize(int newCapacity) {
    T* newData = new T[newCapacity];
    for (int i = 0; i < index; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

template<class T>
void Vector<T>::add_item_end(T item) {
    if (index >= capacity) {
        resize(static_cast<int>(capacity * GROWTH_FACTOR));
    }
    data[index++] = item;
}

template<class T>
void Vector<T>::add_item(T item) {
    if (index >= capacity) {
        resize(static_cast<int>(capacity * GROWTH_FACTOR));
    }

    int i = index - 1;
    while (i >= 0 && item < data[i]) {
        data[i + 1] = data[i];
        i--;
    }
    data[i + 1] = item;
    index++;
}

template<class T>
void Vector<T>::remove_index_swap(int i) {
    if (i < 0 || i >= index) {
        std::cout << "Error: Index out of bounds" << std::endl;
        return;
    }

    data[i] = data[index - 1];
    index--;

    if (index < capacity * SHRINK_THRESHOLD && capacity > 10) {
        resize(capacity / 2);
    }
}

template<class T>
void Vector<T>::remove_index(int i) {
    if (i < 0 || i >= index) {
        std::cout << "Error: Index out of bounds" << std::endl;
        return;
    }

    for (int j = i; j < index - 1; j++) {
        data[j] = data[j + 1];
    }
    index--;

    if (index < capacity * SHRINK_THRESHOLD && capacity > 10) {
        resize(capacity / 2);
    }
}

template<class T>
int Vector<T>::search(T item) const {
    for (int i = 0; i < index; i++) {
        if (data[i] == item) {
            return i;
        }
    }
    return -1;
}

template<class T>
T Vector<T>::get(int i) const {
    if (i < 0 || i >= index) {
        std::cout << "Error: Index out of bounds" << std::endl;
        return T();
    }
    return data[i];
}

template<class T>
int Vector<T>::size() const {
    return index;
}

template<class T>
int Vector<T>::get_capacity() const {
    return capacity;
}

template<class T>
bool Vector<T>::is_empty() const {
    return index == 0;
}

template<class T>
void Vector<T>::clear() {
    index = 0;
}

template<class T>
void Vector<T>::print() const {
    std::cout << "Vector contents: ";
    if (index == 0) {
        std::cout << "(empty)";
    } else {
        for (int i = 0; i < index; i++) {
            std::cout << data[i] << " ";
        }
    }
    std::cout << std::endl;
    std::cout << "Size: " << index << ", Capacity: " << capacity << std::endl;
}

// nstanciaciones
template class Vector<Node<Traits<int, int>>*>;
template class Vector<Edge<Traits<int, int>>*>;
template class Vector<Node<Traits<std::string, int>>*>;
template class Vector<Edge<Traits<std::string, int>>*>;
template class Vector<Node<Traits<double, float>>*>;
template class Vector<Edge<Traits<double, float>>*>;
