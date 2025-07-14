#include "Vector.h"
#include "Graph.h"  // incluir Node, Edge, and Traits types
#include <iostream>

template<class T>
Vector<T>::Vector(int initialCapacity) {
    capacity = initialCapacity > 0 ? initialCapacity : 10;
    index = 0;
    data = new T[capacity];
}

template<class T>
Vector<T>::Vector(const Vector<T>& other) {
    capacity = other.capacity;
    index = other.index;
    data = new T[capacity];
    for (int i = 0; i < index; ++i) {
        data[i] = other.data[i];
    }
}

template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
    if (this != &other) {
        delete[] data;
        capacity = other.capacity;
        index = other.index;
        data = new T[capacity];
        for (int i = 0; i < index; ++i) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

template <class T>
Vector<T>::~Vector() {
    if (data != nullptr) {
        delete[] data;
        data = nullptr;
    }
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
const T& Vector<T>::get(int i) const {
    if (i < 0 || i >= index) {
        std::cout << "Error: Index out of bounds" << std::endl;
        exit(1);
    }
    return data[i];
}

template<class T>
T& Vector<T>::get(int i) {
    if (i < 0 || i >= index) {
        std::cout << "Error: Index out of bounds" << std::endl;
        exit(1);
    }
    return data[i];
}

template<class T>
void Vector<T>::set(int i, T item) {
    if (i < 0 || i >= index) {
        return;
    }
    data[i] = item;
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

template<class T>
void Vector<T>::resize_to_size(int new_size) {
    if (new_size > capacity) {
        resize(new_size);
    }
    
    while (index < new_size) {
        data[index++] = T();
    }
    
    if (new_size < index) {
        index = new_size;
    }
}

// Instantiations
template<class T>
bool Vector<T>::operator==(const Vector<T>& other) const {
    if (index != other.index) return false;
    for (int i = 0; i < index; ++i) {
        if (!(data[i] == other.data[i])) return false;
    }
    return true;
}

template<class T>
bool Vector<T>::operator<(const Vector<T>& other) const {
    int minSize = std::min(index, other.index);
    for (int i = 0; i < minSize; ++i) {
        if (data[i] < other.data[i]) return true;
        if (other.data[i] < data[i]) return false;
    }
    return index < other.index;
}

// Basic types
template class Vector<bool>;
template class Vector<int>;
template class Vector<float>;
template class Vector<double>;
template class Vector<Vector<int>>;
template class Vector<Vector<float>>;
template class Vector<Vector<double>>;

// Nested vectors - only instantiate specific methods to avoid issues with operators
// Vector<Vector<int>> specializations

// GraphNode specializations
template class Vector<GraphNode<Traits<int, int>>*>;
template class Vector<GraphNode<Traits<std::string, int>>*>;
template class Vector<GraphNode<Traits<double, float>>*>;

// Edge specializations
template class Vector<Edge<Traits<int, int>>*>;
template class Vector<Edge<Traits<std::string, int>>*>;
template class Vector<Edge<Traits<double, float>>*>;

//other 
template class Vector<GraphNode<Traits<std::string, float>>*>;
template class Vector<Edge<Traits<std::string, float>>*>;
template class Vector<GraphNode<Traits<int, double>>*>;
template class Vector<Edge<Traits<int, double>>*>;

// Implementation of operator<< (declared in Vector.h)
template<class T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec) {
    os << "[ ";
    for (int i = 0; i < vec.size(); ++i) {
        os << vec.get(i) << " ";
    }
    os << "]";
    return os;
}

// Specialized operator<< for Vector<Vector<int>>
std::ostream& operator<<(std::ostream& os, const Vector<Vector<int>>& vec) {
    os << "[ ";
    for (int i = 0; i < vec.size(); ++i) {
        os << "[ ";
        const Vector<int>& inner = vec.get(i);
        for (int j = 0; j < inner.size(); ++j) {
            os << inner.get(j) << " ";
        }
        os << "] ";
    }
    os << "]";
    return os;
}

// Specialized operator<< for Vector<Vector<float>>
std::ostream& operator<<(std::ostream& os, const Vector<Vector<float>>& vec) {
    os << "[ ";
    for (int i = 0; i < vec.size(); ++i) {
        os << "[ ";
        const Vector<float>& inner = vec.get(i);
        for (int j = 0; j < inner.size(); ++j) {
            os << inner.get(j) << " ";
        }
        os << "] ";
    }
    os << "]";
    return os;
}

// Specialized operator<< for Vector<Vector<double>>
std::ostream& operator<<(std::ostream& os, const Vector<Vector<double>>& vec) {
    os << "[ ";
    for (int i = 0; i < vec.size(); ++i) {
        os << "[ ";
        const Vector<double>& inner = vec.get(i);
        for (int j = 0; j < inner.size(); ++j) {
            os << inner.get(j) << " ";
        }
        os << "] ";
    }
    os << "]";
    return os;
}

// Explicit instantiations of operator<< for the types we need
template std::ostream& operator<<(std::ostream& os, const Vector<int>& vec);
template std::ostream& operator<<(std::ostream& os, const Vector<float>& vec);
template std::ostream& operator<<(std::ostream& os, const Vector<double>& vec);
template std::ostream& operator<<(std::ostream& os, const Vector<bool>& vec);
template std::ostream& operator<<(std::ostream& os, const Vector<GraphNode<Traits<int, int>>*>& vec);
template std::ostream& operator<<(std::ostream& os, const Vector<GraphNode<Traits<std::string, int>>*>& vec);
template std::ostream& operator<<(std::ostream& os, const Vector<GraphNode<Traits<double, float>>*>& vec);
template std::ostream& operator<<(std::ostream& os, const Vector<Edge<Traits<int, int>>*>& vec);
template std::ostream& operator<<(std::ostream& os, const Vector<Edge<Traits<std::string, int>>*>& vec);
template std::ostream& operator<<(std::ostream& os, const Vector<Edge<Traits<double, float>>*>& vec);
