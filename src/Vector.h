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
    Vector(const Vector<T>& other);
    Vector<T>& operator=(const Vector<T>& other);
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

    bool operator==(const Vector<T>& other) const;
    bool operator<(const Vector<T>& other) const;

    //funciones nuevas
    void resize_to_size(int new_size); 
};


template<class T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec);

std::ostream& operator<<(std::ostream& os, const Vector<Vector<int>>& vec);
std::ostream& operator<<(std::ostream& os, const Vector<Vector<float>>& vec);

template<typename K, typename V>
class Traits;

template<typename T>
class GraphNode;

template<typename T>
class Edge;

//instanciaciones
extern template class Vector<bool>;
extern template class Vector<int>;
extern template class Vector<float>;
extern template class Vector<Vector<int>>;
extern template class Vector<Vector<float>>;

extern template class Vector<GraphNode<Traits<int, int>>*>;
extern template class Vector<Edge<Traits<int, int>>*>;
extern template class Vector<GraphNode<Traits<std::string, int>>*>;
extern template class Vector<Edge<Traits<std::string, int>>*>;
extern template class Vector<GraphNode<Traits<double, float>>*>;
extern template class Vector<Edge<Traits<double, float>>*>;

#endif
