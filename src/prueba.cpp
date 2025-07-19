#include "Graph.h"
#include <iostream>

// g++ -std=c++11 Deque.cpp Vector.cpp Queue.cpp Stack.cpp Graph.cpp prueba.cpp -o graph_test

void test_connected_graph() {
    std::cout << "\n=== TESTING CONNECTED GRAPH ===\n";
    Graph<Traits<int, int>> graph(false);  // Grafo no dirigido

    // Agregar 8 nodos
    for (int i = 1; i <= 8; ++i) {
        graph.add_node(i);
    }

    // Agregar aristas para formar un grafo conexo
    graph.add_edge(1, 2, 5);
    graph.add_edge(1, 3, 3);
    graph.add_edge(2, 4, 2);
    graph.add_edge(3, 5, 7);
    graph.add_edge(4, 6, 1);
    graph.add_edge(5, 6, 8);
    graph.add_edge(6, 7, 4);
    graph.add_edge(7, 8, 6);
    graph.add_edge(5, 8, 9);

    std::cout << "Grafo conexo:\n";
    graph.print_graph();
    std::cout << "\nMatriz de adyacencia:\n";
    graph.print_matrix();

    std::cout << "\nBFS desde 1 hasta 8:\n";
    graph.Breadth_First_Search(1, 8);

    std::cout << "\n\nDFS desde 1 hasta 6:\n";
    graph.Depth_First_Search(1, 6);

    std::cout << "\n\nEstadísticas:\n";
    std::cout << "Vértices: " << graph.num_vertices() 
              << "\nAristas: " << graph.num_edges() << "\n";
}

void test_disconnected_graph() {
    std::cout << "\n=== TESTING DISCONNECTED GRAPH ===\n";
    Graph<Traits<std::string, float>> graph(false);  // Grafo no dirigido con strings

    // Agregar nodos (dos componentes conexas)
    graph.add_node("A");
    graph.add_node("B");
    graph.add_node("C");
    graph.add_node("D");
    graph.add_node("X");
    graph.add_node("Y");
    graph.add_node("Z");

    // Componente conexa 1
    graph.add_edge("A", "B", 1.5f);
    graph.add_edge("B", "C", 2.3f);
    graph.add_edge("C", "D", 0.8f);

    // Componente conexa 2
    graph.add_edge("X", "Y", 3.1f);
    graph.add_edge("Y", "Z", 1.2f);

    std::cout << "Grafo no conexo:\n";
    graph.print_graph();

    std::cout << "\nBFS desde A hasta D (conexo):\n";
    graph.Breadth_First_Search("A", "D");

    std::cout << "\n\nDFS desde X hasta Z (conexo):\n";
    graph.Depth_First_Search("X", "Z");

    std::cout << "\n\nIntento de BFS entre componentes desconectadas (A -> X):\n";
    graph.Breadth_First_Search("A", "X");  // No debería encontrar camino

    std::cout << "\n\nEstadísticas:\n";
    std::cout << "Vértices: " << graph.num_vertices() 
              << "\nAristas: " << graph.num_edges() << "\n";
    graph.Dijkstra("X", "Z");
}

void test_directed_graph() {
    std::cout << "\n=== TESTING DIRECTED GRAPH ===\n";
    Graph<Traits<int, double>> graph(true);  // Grafo dirigido

    // Agregar nodos
    for (int i = 1; i <= 5; ++i) {
        graph.add_node(i);
    }

    // Aristas dirigidas
    graph.add_edge(1, 2, 5.5);
    graph.add_edge(2, 3, 3.2);
    graph.add_edge(3, 4, 7.1);
    graph.add_edge(4, 5, 2.8);
    graph.add_edge(5, 1, 9.4);  // Ciclo

    std::cout << "Grafo dirigido:\n";
    graph.print_graph();

    std::cout << "\nDFS desde 1 hasta 5 (siguiendo direcciones):\n";
    graph.Depth_First_Search(1, 5);

    std::cout << "\n\nIntento de BFS en dirección inversa (5 -> 1):\n";
    graph.Breadth_First_Search(5, 1);  // Debería funcionar por el ciclo

    std::cout << "\n\nEstadísticas:\n";
    std::cout << "Vértices: " << graph.num_vertices() 
              << "\nAristas: " << graph.num_edges() << "\n";
    graph.Dijkstra(2, 4);
}

int main() {
    // Grafo simple del ejemplo original
    std::cout << "=== BASIC TEST ===\n";
    Graph<Traits<int, int>> simple_graph(false);
    simple_graph.add_node(1);
    simple_graph.add_node(2);
    simple_graph.add_node(3);
    simple_graph.add_node(4);
    simple_graph.add_edge(1, 2, 10);
    simple_graph.add_edge(2, 3, 20);
    simple_graph.add_edge(3, 4, 30);
    simple_graph.add_edge(4, 1, 40);
    simple_graph.print_graph();
    std::cout << "\nBFS 1->3: ";
    simple_graph.Breadth_First_Search(1, 3);

    // Tests avanzados
    test_connected_graph();
    test_disconnected_graph();
    test_directed_graph();

    return 0;
}
