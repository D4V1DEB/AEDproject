#ifndef VECTOR_H
#define VECTOR_H

#include <string>

template<class T>
class Vector {
private:
    T* data;
    int capacity;
    int index;

    void resize(int newCapacity);

    static constexpr double GROWTH_FACTOR = 2.0;
    static constexpr double SHRINK_THRESHOLD = 0.25;

public:
    Vector(int initialCapacity = 10);
    ~Vector();

    void add_item_end(T item);
    void add_item(T item);
    void remove_index_swap(int i);
    void remove_index(int i);
    int search(T item) const;
    T& get(int i);
    const T& get(int i) const;
    void set(int i, T item);
    int size() const;
    int get_capacity() const;
    bool is_empty() const;
    void clear();
    void print() const;
};

// frward
template<typename K, typename V>
class Traits;

template<typename T>
class Node;

template<typename T>
class Edge;

//instanciaciones
extern template class Vector<Node<Traits<int, int>>*>;
extern template class Vector<Edge<Traits<int, int>>*>;
extern template class Vector<Node<Traits<std::string, int>>*>;
extern template class Vector<Edge<Traits<std::string, int>>*>;
extern template class Vector<Node<Traits<double, float>>*>;
extern template class Vector<Edge<Traits<double, float>>*>;

#endif
