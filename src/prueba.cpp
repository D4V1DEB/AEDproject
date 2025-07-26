#include "Graph.h"
#include <iostream>
#include <iomanip>

// g++ -std=c++11 Deque.cpp Vector.cpp Queue.cpp Stack.cpp Graph.cpp PriorityQueue.cpp HeapTree.cpp prueba_updated.cpp -o graph_test

void test_basic_with_coordinates() {
    std::cout << "=== BASIC TEST WITH COORDINATES ===\n";
    Graph<Traits<int, int>> simple_graph(false, EUCLIDEAN);
    
    // nodos con coordenadas
    simple_graph.add_node(1, 0.0, 0.0);    // Origen
    simple_graph.add_node(2, 3.0, 4.0);    // Punto (3,4)
    simple_graph.add_node(3, 6.0, 8.0);    // Punto (6,8)
    simple_graph.add_node(4, 2.0, 1.0);    // Punto (2,1)
    
    // Aaristas
    simple_graph.add_edge(1, 2, 10);
    simple_graph.add_edge(2, 3, 20);
    simple_graph.add_edge(3, 4, 30);
    simple_graph.add_edge(4, 1, 40);
    simple_graph.add_edge(1, 3, 50);  // Conexion directa dx
    
    std::cout << "Grafo con coordenadas:\n";
    simple_graph.print_graph();
    
    std::cout << "\n--- Comparación de algoritmos (1 -> 3) ---\n";
    std::cout << "BFS: ";
    simple_graph.Breadth_First_Search(1, 3);
    std::cout << "\n";
    
    std::cout << "DFS: ";
    simple_graph.Depth_First_Search(1, 3);
    std::cout << "\n";
    
    std::cout << "Best First Search (Euclidiana): ";
    simple_graph.Best_First_Search(1, 3);
    std::cout << "\n";
}

void test_connected_graph_with_coordinates() {
    std::cout << "\n=== TESTING CONNECTED GRAPH WITH COORDINATES ===\n";
    Graph<Traits<int, int>> graph(false, EUCLIDEAN);  // no dirigido con heuritica euclidiana
    
    // 8 nodos con coordenadas 
    graph.add_node(1, 0.0, 0.0);   // esquina inferior izquierda
    graph.add_node(2, 2.0, 0.0);   // 
    graph.add_node(3, 0.0, 2.0);   // 
    graph.add_node(4, 4.0, 0.0);   // 
    graph.add_node(5, 0.0, 4.0);   // 
    graph.add_node(6, 4.0, 2.0);   // 
    graph.add_node(7, 2.0, 4.0);   // 
    graph.add_node(8, 4.0, 4.0);   // equina superior derecha
    
    // aristas para grafo conexo
    graph.add_edge(1, 2, 5);
    graph.add_edge(1, 3, 3);
    graph.add_edge(2, 4, 2);
    graph.add_edge(3, 5, 7);
    graph.add_edge(4, 6, 1);
    graph.add_edge(5, 6, 8);
    graph.add_edge(6, 7, 4);
    graph.add_edge(7, 8, 6);
    graph.add_edge(5, 8, 9);
    graph.add_edge(2, 6, 3);  // Conexin adicional 
    
    std::cout << "Grafo conexo con coordenadas:\n";
    graph.print_graph();
    
    std::cout << "\n--- Búsqueda desde esquina inferior izquierda (1) hasta superior derecha (8) ---\n";
    
    std::cout << "BFS (1 -> 8): ";
    graph.Breadth_First_Search(1, 8);
    std::cout << "\n";
    
    std::cout << "DFS (1 -> 8): ";
    graph.Depth_First_Search(1, 8);
    std::cout << "\n";
    
    std::cout << "Best First Search (1 -> 8): ";
    graph.Best_First_Search(1, 8);
    
    std::cout << "\n\nEstadísticas:\n";
    std::cout << "Vértices: " << graph.num_vertices() 
              << "\nAristas: " << graph.num_edges() << "\n";
}

void test_geographic_coordinates() {
    std::cout << "\n=== TESTING GEOGRAPHIC COORDINATES (AREQUIPA) ===\n";
    Graph<Traits<std::string, float>> graph(false, HAVERSINE);  // Haversine para coordenadas geograficas
    
    // Lugares de Arequipa con coordenadas reales (latitud, longitud)
    graph.add_node("Plaza_Armas", -16.3989, -71.5369);
    graph.add_node("Santa_Catalina", -16.3947, -71.5369);
    graph.add_node("Yanahuara", -16.3969, -71.5447);
    graph.add_node("San_Agustin", -16.4067, -71.5294);
    graph.add_node("San_Camilo", -16.3997, -71.5339);
    graph.add_node("Cerro_Colorado", -16.4139, -71.5408);
    graph.add_node("Sachaca", -16.4236, -71.5669);
    
    // Conexiones (distancias aproximadas en metros)
    graph.add_edge("Plaza_Armas", "Santa_Catalina", 500.0f);
    graph.add_edge("Plaza_Armas", "San_Camilo", 300.0f);
    graph.add_edge("Santa_Catalina", "Yanahuara", 700.0f);
    graph.add_edge("San_Camilo", "San_Agustin", 1200.0f);
    graph.add_edge("Yanahuara", "San_Agustin", 1500.0f);
    graph.add_edge("San_Agustin", "Cerro_Colorado", 2000.0f);
    graph.add_edge("Cerro_Colorado", "Sachaca", 1800.0f);
    graph.add_edge("Plaza_Armas", "Yanahuara", 800.0f);
    graph.add_edge("San_Camilo", "Cerro_Colorado", 2500.0f);
    
    std::cout << "Mapa de Arequipa:\n";
    graph.print_graph();
    
    std::cout << "\n--- Ruta desde Plaza de Armas hasta Sachaca ---\n";
    
    std::cout << "BFS: ";
    graph.Breadth_First_Search("Plaza_Armas", "Sachaca");
    std::cout << "\n";
    
    std::cout << "DFS: ";
    graph.Depth_First_Search("Plaza_Armas", "Sachaca");
    std::cout << "\n";
    
    std::cout << "Best First Search (Haversine): ";
    graph.Best_First_Search("Plaza_Armas", "Sachaca");
    
    std::cout << "\n\nEstadísticas del mapa:\n";
    std::cout << "Lugares: " << graph.num_vertices() 
              << "\nConexiones: " << graph.num_edges() << "\n";
}

void test_disconnected_graph_with_coordinates() {
    std::cout << "\n=== TESTING DISCONNECTED GRAPH WITH COORDINATES ===\n";
    Graph<Traits<std::string, float>> graph(false, EUCLIDEAN);  // grafo no dirigido
    
    // Componente conexa 1 (Cluster A)
    graph.add_node("A", 0.0, 0.0);
    graph.add_node("B", 1.0, 1.0);
    graph.add_node("C", 2.0, 0.0);
    graph.add_node("D", 1.0, -1.0);
    
    // Componente conexa 2 (Cluster X) - alejado del primero
    graph.add_node("X", 10.0, 10.0);
    graph.add_node("Y", 11.0, 11.0);
    graph.add_node("Z", 12.0, 10.0);
    
    // Conexiones dentro del Cluster A
    graph.add_edge("A", "B", 1.5f);
    graph.add_edge("B", "C", 2.3f);
    graph.add_edge("C", "D", 0.8f);
    graph.add_edge("D", "A", 1.2f);
    
    // Conexiones dentro del Cluster X
    graph.add_edge("X", "Y", 3.1f);
    graph.add_edge("Y", "Z", 1.2f);
    graph.add_edge("Z", "X", 2.8f);
    
    std::cout << "Grafo desconectado con coordenadas:\n";
    graph.print_graph();
    
    std::cout << "\n--- Búsquedas dentro del Cluster A (A -> C) ---\n";
    std::cout << "BFS: ";
    graph.Breadth_First_Search("A", "C");
    std::cout << "\n";
    
    std::cout << "Best First Search: ";
    graph.Best_First_Search("A", "C");
    
    std::cout << "\n\n--- Búsquedas dentro del Cluster X (X -> Z) ---\n";
    std::cout << "DFS: ";
    graph.Depth_First_Search("X", "Z");
    std::cout << "\n";
    
    std::cout << "Best First Search: ";
    graph.Best_First_Search("X", "Z");
    
    std::cout << "\n\n--- Intento de búsqueda entre clusters desconectados (A -> X) ---\n";
    std::cout << "BFS (debería fallar): ";
    graph.Breadth_First_Search("A", "X");
    std::cout << "\n";
    
    std::cout << "Best First Search (debería fallar): ";
    graph.Best_First_Search("A", "X");
    
    std::cout << "\n\nEstadísticas:\n";
    std::cout << "Vértices: " << graph.num_vertices() 
              << "\nAristas: " << graph.num_edges() << "\n";
}

void test_directed_graph_with_coordinates() {
    std::cout << "\n=== TESTING DIRECTED GRAPH WITH COORDINATES ===\n";
    Graph<Traits<int, double>> graph(true, EUCLIDEAN);  // Grafo dirigido
    
    // Agregar nodos en un patron circular
    graph.add_node(1, 0.0, 0.0);    // Centro
    graph.add_node(2, 3.0, 0.0);    // Este
    graph.add_node(3, 0.0, 3.0);    // Norte
    graph.add_node(4, -3.0, 0.0);   // Oeste
    graph.add_node(5, 0.0, -3.0);   // Sur
    
    // Aristas dirigidas formando un ciclo
    graph.add_edge(1, 2, 5.5);   // Centro -> Este
    graph.add_edge(2, 3, 3.2);   // Este -> Norte
    graph.add_edge(3, 4, 7.1);   // Norte -> Oeste
    graph.add_edge(4, 5, 2.8);   // Oeste -> Sur
    graph.add_edge(5, 1, 9.4);   // Sur -> Centro (completa el ciclo)
    
    // Algunos atajos adicionales
    graph.add_edge(1, 3, 4.5);   // Centro -> Norte (directo)
    graph.add_edge(2, 4, 8.0);   // Este -> Oeste (salto)
    
    std::cout << "Grafo dirigido con coordenadas (patrón circular):\n";
    graph.print_graph();
    
    std::cout << "\n--- Búsquedas en grafo dirigido ---\n";
    
    std::cout << "DFS (1 -> 4): ";
    graph.Depth_First_Search(1, 4);
    std::cout << "\n";
    
    std::cout << "Best First Search (1 -> 4): ";
    graph.Best_First_Search(1, 4);
    
    std::cout << "\n\nBFS siguiendo el ciclo completo (5 -> 1): ";
    graph.Breadth_First_Search(5, 1);
    std::cout << "\n";
    
    std::cout << "Best First Search (5 -> 1): ";
    graph.Best_First_Search(5, 1);
    
    std::cout << "\n\nEstadísticas:\n";
    std::cout << "Vértices: " << graph.num_vertices() 
              << "\nAristas: " << graph.num_edges() << "\n";
}

void test_performance_comparison() {
    std::cout << "\n=== PERFORMANCE COMPARISON ===\n";
    Graph<Traits<int, int>> graph(false, EUCLIDEAN);
    
    std::cout << "Comparacion de rendimiento...\n";
    
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            int node_id = i * 5 + j;
            graph.add_node(node_id, i * 10.0, j * 10.0);
        }
    }
    
    // Conectar nodos adyacentes (4-conectividad)
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            int current = i * 5 + j;
            
            // Conexión hacia la derecha
            if (j < 4) {
                int right = i * 5 + (j + 1);
                graph.add_edge(current, right, 10);
            }
            
            // Conexión hacia abajo
            if (i < 4) {
                int down = (i + 1) * 5 + j;
                graph.add_edge(current, down, 10);
            }
            
            // Algunas conexiones diagonales
            if (i < 4 && j < 4) {
                int diagonal = (i + 1) * 5 + (j + 1);
                graph.add_edge(current, diagonal, 14); // sqrt(10^2 + 10^2) ≈ 14
            }
        }
    }
    
    std::cout << "Grilla creada: " << graph.num_vertices() << " nodos, " 
              << graph.num_edges() << " aristas\n";
    
    // Comparar algoritmos desde esquina inferior izquierda a superior derecha
    int start = 0;   // (0,0)
    int end = 24;    // (4,4)
    
    std::cout << "\n--- Comparación de rutas desde (0,0) hasta (4,4) ---\n";
    
    std::cout << "BFS: ";
    graph.Breadth_First_Search(start, end);
    std::cout << "\n";
    
    std::cout << "DFS: ";
    graph.Depth_First_Search(start, end);
    std::cout << "\n";
    
    std::cout << "Best First Search: ";
    graph.Best_First_Search(start, end);
    
    std::cout << "\n\nNota: Best First Search debería encontrar una ruta más directa\n";
    std::cout << "hacia el objetivo debido a la heurística euclidiana.\n";
}

int main() {
    std::cout << std::fixed << std::setprecision(2);
    
    // Pruebas basicas
    test_basic_with_coordinates();
    
    // Pruebas con diferentes tipos de grafos
    test_connected_graph_with_coordinates();
    test_geographic_coordinates();
    test_disconnected_graph_with_coordinates();
    test_directed_graph_with_coordinates();
    
    // Comparacion de rendimiento
    test_performance_comparison();
    
    std::cout << "\n=== TODAS LAS PRUEBAS COMPLETADAS ===\n";
    std::cout << "El Best First Search usa heurísticas para dirigir la búsqueda\n";
    std::cout << "hacia el objetivo de manera más eficiente que BFS o DFS.\n";
    
    return 0;
}