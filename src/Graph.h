#ifndef GRAPH_H
#define GRAPH_H

#include "Vector.h"
#include <iostream>
#include <string>

// forward
template<class T> class GraphNode;
template<class T> class Edge;

template<class _N, class _E>
class Traits {
public:
    typedef _N N;
    typedef _E E;
    typedef Traits<_N, _E> self;
    typedef GraphNode<self> node;
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
class GraphNode {
public:
    typedef typename T::N N;
    typedef typename T::edge edge;
    N data;
    Vector<edge*> adjacency;
    
    GraphNode(N _data);
    
    
    ~GraphNode() {
    }
    
    // Const-qualified method to get adjacency size
    int adjacency_size() const {
        return adjacency.size();
    }
};

template<class T>
class Graph {
public:
    typedef typename T::node GraphNode;
    typedef typename T::N N;
    typedef typename T::E E;
    typedef typename T::edge edge;
    
    Vector<GraphNode*> vertices;
    Vector<Vector<E>> matrix;
    Vector<bool> visited;

    bool directed;
    
    //auxiliar
    int index_of(N data);
    void init_visited();
    void validation(int u, int v);
    void resize_matrix(); //optimizado

    Graph(bool _directed = false);
    void add_node(N _data);
    bool find(N val, int& pos);
    void add_edge(N u, N v, E w);
    void add_edge_by_index(int u, int v, E w);
    void print_graph(); //debug
    void print_matrix(); //debug
    int num_vertices() const;
    int num_edges() const;

    //Busquedas
    void Depth_First_Search(N s, N t);
    void Breadth_First_Search(N s, N t);

    ~Graph();
};

//instanciaciones
extern template class Graph<Traits<int, int>>;
extern template class Graph<Traits<std::string, int>>;
extern template class Graph<Traits<double, float>>;

#endif
