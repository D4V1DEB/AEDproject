#include "Graph.h"
#include <iostream>  // Needed for std::cout in print_graph()

template<class T>
Edge<T>::Edge(typename T::E _data) : data(_data) {
    nodes[0] = nodes[1] = nullptr;
}

template<class T>
Node<T>::Node(typename T::N _data) : data(_data) {}

template<class T>
void Graph<T>::add_node(typename T::N _data) {
    Node* n = new Node(_data);
    vertices.add_item_end(n);
}

template<class T>
bool Graph<T>::find(typename T::N val, int& pos) {
    for (int i = 0; i < vertices.size(); ++i) {
        if (vertices.get(i)->data == val) {
            pos = i;
            return true;
        }
    }
    return false;
}

template<class T>
void Graph<T>::add_edge(typename T::N u, typename T::N v, typename T::E w) {
    int p, q;
    if (find(u, p) && find(v, q)) {
        edge* e = new edge(w);
        e->nodes[0] = vertices.get(p);
        e->nodes[1] = vertices.get(q);
        vertices.get(p)->adjacency.add_item_end(e);
    }
}

template<class T>
void Graph<T>::print_graph() {
    for (int i = 0; i < vertices.size(); ++i) {
        Node* n = vertices.get(i);
        std::cout << n->data << ": ";
        for (int j = 0; j < n->adjacency.size(); ++j) {
            edge* e = n->adjacency.get(j);
            Node* neighbor = (e->nodes[0] == n) ? e->nodes[1] : e->nodes[0];
            std::cout << neighbor->data << "(" << e->data << ") ";
        }
        std::cout << "\n";
    }
}

template<class T>
int Graph<T>::num_vertices() const {
    return vertices.size();
}

template<class T>
int Graph<T>::num_edges() const {
    int count = 0;
    for (int i = 0; i < vertices.size(); i++) {
        count += vertices.get(i)->adjacency.size();
    }
    return count;
}

template<class T>
Graph<T>::~Graph() {
    for (int i = 0; i < vertices.size(); ++i) {
        Node* node = vertices.get(i);
        for (int j = 0; j < node->adjacency.size(); ++j) {
            delete node->adjacency.get(j);
        }
        delete node;
    }
}


// Instanciacion
template class Edge<Traits<int, int>>;
template class Edge<Traits<std::string, int>>;
template class Edge<Traits<double, float>>;

template class Node<Traits<int, int>>;
template class Node<Traits<std::string, int>>;
template class Node<Traits<double, float>>;

template class Graph<Traits<int, int>>;
template class Graph<Traits<std::string, int>>;
template class Graph<Traits<double, float>>;
//... mas de ser necesario
