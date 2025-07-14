#include "Graph.h"
#include "Stack.h"
#include "Queue.h"
#include <iostream>

#define NO_DIRECTION -1000

template<class T>
Edge<T>::Edge(typename T::E _data) : data(_data) {
    nodes[0] = nodes[1] = nullptr;
}

template<class T>
GraphNode<T>::GraphNode(typename T::N _data) : data(_data) {}

template<class T>
int Graph<T>::index_of(N data) {
    for (int i=0; i < vertices.size(); ++i) {
        if (vertices.get(i)->data == data) {
            return i;
        }
    }
    return -1;
}

template<class T>
void Graph<T>::init_visited() {
    visited.clear();
    for (int i = 0; i<vertices.size(); ++i) {
        visited.add_item_end(false);
    }
}

template<class T>
void Graph<T>::validation(int u, int v) {
    if (u<0 || u >= vertices.size() || v < 0 || v >= vertices.size()) {
        return; //out of range
    }
}

inline int min_value(int a, int b) {
    return (a < b) ? a : b;
}

inline int abs_value(int x) {
    return (x < 0) ? -x : x;
}

template<class T>
void Graph<T>::resize_matrix() {
    int new_size = vertices.size();
    int current_size = matrix.size();
    
    if (new_size == current_size) {
        return;
    }
    
    int min_size = min_value(current_size, new_size);
    for (int i = 0; i < min_size; ++i) {
        matrix.get(i).resize_to_size(new_size);
    }
    
    if (new_size > current_size) {
        for (int i = current_size; i < new_size; ++i) {
            Vector<E> new_row;
            new_row.resize_to_size(new_size);
            matrix.add_item_end(new_row);
        }
    }
    
    else if (new_size < current_size) {
        matrix.resize_to_size(new_size);
    }
}

template<class T>
Graph<T>::Graph(bool _directed) : directed(_directed) {}

template<class T>
void Graph<T>::add_node(typename T::N _data) {
    GraphNode* n = new GraphNode(_data);
    vertices.add_item_end(n);
    resize_matrix();
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
    int pos_u, pos_v;

    if (find(u, pos_u) && find(v, pos_v)) {
        validation(pos_u, pos_v);
        
        matrix.get(pos_u).get(pos_v) = w;
        if (!directed) {
            matrix.get(pos_v).get(pos_u) = w;
        }
        
        edge* e = new edge(w);
        e->nodes[0] = vertices.get(pos_u);
        e->nodes[1] = vertices.get(pos_v);
        vertices.get(pos_u)->adjacency.add_item_end(e);
        
        if (!directed) {
            edge* e2 = new edge(w);
            e2->nodes[0] = vertices.get(pos_v);
            e2->nodes[1] = vertices.get(pos_u);
            vertices.get(pos_v)->adjacency.add_item_end(e2);
        }
    }
}

template<class T>
void Graph<T>::add_edge_by_index(int u, int v, E w) {
    validation(u, v);
    
    matrix.get(u).get(v) = w;
    if (!directed) {
        matrix.get(v).get(u) = w;
    }
    
    edge* e = new edge(w);
    e->nodes[0] = vertices.get(u);
    e->nodes[1] = vertices.get(v);
    vertices.get(u)->adjacency.add_item_end(e);
    
    if (!directed) {
        edge* e2 = new edge(w);
        e2->nodes[0] = vertices.get(v);
        e2->nodes[1] = vertices.get(u);
        vertices.get(v)->adjacency.add_item_end(e2);
    }
}

template<class T>
void Graph<T>::print_graph() {
    for (int i = 0; i < vertices.size(); ++i) {
        GraphNode* n = vertices.get(i);
        std::cout << n->data << ": ";
        for (int j = 0; j < n->adjacency.size(); ++j) {
            edge* e = n->adjacency.get(j);
            GraphNode* neighbor = (e->nodes[0] == n) ? e->nodes[1] : e->nodes[0];
            std::cout << neighbor->data << "(" << e->data << ") ";
        }
        std::cout << "\n";
    }
}

template<class T>
void Graph<T>::print_matrix() {
    for (int i = 0; i < vertices.size(); i++) {
        for (int j = 0; j < vertices.size(); j++) {
            std::cout << matrix.get(i).get(j) << " ";
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
        const GraphNode* node = vertices.get(i);
        count += node->adjacency_size();
    }
    return directed ? count : count / 2;
}

//Busquedas

template<class T>
void Graph<T>::Depth_First_Search(typename T::N s, typename T::N t) {
    int start_idx = index_of(s);
    int end_idx = index_of(t);
    if (start_idx == -1 || end_idx == -1) return;

    init_visited();
    Stack<N> stack;
    stack.push(s);
    visited.set(start_idx, true);

    while(!stack.empty()) {
        N current = stack.top();
        stack.pop();

        std::cout << current << " ";
        if (current == t) break;

        int idx = index_of(current);
        if (idx == -1) continue;

        GraphNode* n = vertices.get(idx);
        for (int i = 0; i < n->adjacency.size(); ++i) {
            edge* e = n->adjacency.get(i);
            GraphNode* neighbor = (e->nodes[0] == n) ? e->nodes[1] : e->nodes[0];

            int neighbor_idx = index_of(neighbor->data);
            if (neighbor_idx != -1 && !visited.get(neighbor_idx)) {
                visited.set(neighbor_idx, true);
                stack.push(neighbor->data);
            }
        }
    }
}

template<class T>
void Graph<T>::Breadth_First_Search(typename T::N s, typename T::N t) {
    int start_idx = index_of(s);
    int end_idx = index_of(t);
    if (start_idx == -1 || end_idx == -1) return;

    init_visited();
    Queue<N> queue;
    queue.push(s);
    visited.set(start_idx, true);
    
    while (!queue.empty()) {
        N current = queue.front();
        queue.pop();

        std::cout << current << " ";
        if (current == t) break;
        
        int idx = index_of(current);
        if (idx == -1 ) continue;

        GraphNode* n = vertices.get(idx);
        for (int i = 0; i < n->adjacency.size(); ++i) {
            edge* e = n->adjacency.get(i);
            GraphNode* neighbor = (e->nodes[0] == n) ? e->nodes[1] : e->nodes[0];

            int neighbor_idx = index_of(neighbor->data);
            if (neighbor_idx != -1 && !visited.get(neighbor_idx)) {
                visited.set(neighbor_idx, true);
                queue.push(neighbor->data);
            }
        }
    }
}


template<class T>
Graph<T>::~Graph() {
    Vector<edge*> deleted_edges;
    
    for (int i = 0; i < vertices.size(); ++i) {
        GraphNode* node = vertices.get(i);
        for (int j = 0; j < node->adjacency.size(); ++j) {
            edge* e = node->adjacency.get(j);
            
            bool already_deleted = false;
            for (int k = 0; k < deleted_edges.size(); ++k) {
                if (deleted_edges.get(k) == e) {
                    already_deleted = true;
                    break;
                }
            }
            
            if (!already_deleted) {
                deleted_edges.add_item_end(e);
                delete e;
            }
        }
    }
    
    for (int i = 0; i < vertices.size(); ++i) {
        delete vertices.get(i);
    }
}

// Instanciacion
template class Edge<Traits<int, int>>;
template class Edge<Traits<std::string, int>>;
template class Edge<Traits<double, float>>;
template class Edge<Traits<std::string, float>>;
template class Edge<Traits<int, double>>;

template class GraphNode<Traits<int, int>>;
template class GraphNode<Traits<std::string, int>>;
template class GraphNode<Traits<double, float>>;
template class GraphNode<Traits<std::string, float>>;
template class GraphNode<Traits<int, double>>;

template class Graph<Traits<int, int>>;
template class Graph<Traits<std::string, int>>;
template class Graph<Traits<double, float>>;
template class Graph<Traits<std::string, float>>;
template class Graph<Traits<int, double>>;
//... mas de ser necesario