#ifndef GRAPH_H
#define GRAPH_H

#include "Vector.h"
#include "PriorityQueue.h"
#include <iostream>
#include <string>
#include <cmath>

//Struct coordinate
struct Coordinate {
    double x, y;  // uso x e y para cartesiana o lat, log geograficas

    Coordinate() : x(0.0), y(0.0) {}
    Coordinate(double _x, double _y) : x(_x), y(_y) {}

    double euclidean_distance(const Coordinate& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return sqrt(dx * dx + dy * dy);
    }

    double haversine_distance(const Coordinate& other) const {
        const double R = 6371000.0; // radio m 
        const double PI = 3.14159265358979323846;
        
        double lat1_rad = x * PI / 180.0;
        double lon1_rad = y * PI / 180.0;
        double lat2_rad = other.x * PI / 180.0;
        double lon2_rad = other.y * PI / 180.0;
        
        double dlat = lat2_rad - lat1_rad;
        double dlon = lon2_rad - lon1_rad;
        
        double a = sin(dlat/2) * sin(dlat/2) + cos(lat1_rad) * cos(lat2_rad) * sin(dlon/2) * sin(dlon/2);
        double c = 2 * atan2(sqrt(a), sqrt(1-a));
        
        return R * c;
    }
};

enum HeuristicType {
    EUCLIDEAN,
    HAVERSINE
};

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
    Coordinate coords;  //coordenadas cart o geog
    
    //GraphNode(N _data);
    GraphNode(N _data, double x, double y);
    
    ~GraphNode() {
    }
    
    // Const-qualified method to get adjacency size
    int adjacency_size() const {
        return adjacency.size();
    }

    void set_coordinates(double x, double y) {
        coords.x = x;
        coords.y = y;
    }
};

//struct prioridad best first search
template<class T>
struct PriorityNode {
    typedef typename T::N N;
    N data;
    double priority;
    
    PriorityNode() : priority(0.0) {}
    PriorityNode(N _data, double _priority) : data(_data), priority(_priority) {}
    
    // comparacion sobrecarga
    bool operator<(const PriorityNode& other) const {
        return priority < other.priority;
    }
    
    bool operator>(const PriorityNode& other) const {
        return priority > other.priority;
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
    HeuristicType heuristic_type;
    
    //auxiliar
    int index_of(N data);
    void init_visited();
    void validation(int u, int v);
    void resize_matrix(); //optimizado

    Graph(bool _directed = false, HeuristicType _heuristic = EUCLIDEAN);

    //void add_node(N _data);
    void add_node(N _data, double x, double y);
    bool find(N val, int& pos);
    void add_edge(N u, N v, E w);
    void add_edge_by_index(int u, int v, E w);

    //debug
    void print_graph(); //debug
    void print_matrix(); //debug
    int num_vertices() const;
    int num_edges() const;

    //Busquedas
    void Depth_First_Search(N s, N t);
    void Breadth_First_Search(N s, N t);
    void Dijkstra(N s, N t);
    void Best_First_Search(N s, N t);

    //metodo auxiliar heuristica
    double calculate_heuristic(int from_idx, int to_idx);

    ~Graph();
};

//instanciaciones
extern template class Graph<Traits<int, int>>;
extern template class Graph<Traits<std::string, int>>;
extern template class Graph<Traits<double, float>>;
extern template class Graph<Traits<std::string, float>>;
extern template class Graph<Traits<int, double>>;

#endif
