#ifndef GRAPH_H
#define GRAPH_H

#include "Vector.h"
#include <iostream>
#include <string>

// forward
template<class T> class Node;
template<class T> class Edge;

template<class _N, class _E>
class Traits {
public:
    typedef _N N;
    typedef _E E;
    typedef Traits<_N, _E> self;
    typedef Node<self> node;
    typedef Edge<self> edge;
};

template<class T>
class Edge {
public:
    typedef typename T::E E;
    typedef typename T::node node;
    E data;
    node* nodes[2];
    
    Edge(E _data);
};

template<class T>
class Node {
public:
    typedef typename T::N N;
    typedef typename T::edge edge;
    N data;
    Vector<edge*> adjacency;
    
    Node(N _data);
};

template<class T>
class Graph {
public:
    typedef typename T::node Node;
    typedef typename T::N N;
    typedef typename T::E E;
    typedef typename T::edge edge;
    
    Vector<Node*> vertices;
    
    void add_node(N _data);
    bool find(N val, int& pos);
    void add_edge(N u, N v, E w);
    void print_graph();
    int num_vertices() const;
    int num_edges() const;
    ~Graph();
};

//instanciaciones
extern template class Graph<Traits<int, int>>;
extern template class Graph<Traits<std::string, int>>;
extern template class Graph<Traits<double, float>>;

#endif
