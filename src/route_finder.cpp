#include "route_finder.h"
#include <algorithm> // For std::reverse
#include <cmath>     // For std::sqrt, std::pow in heuristic
#include <omp.h>     // For OpenMP

// Re-declare haversineDistance or make it a global utility function if needed
// For now, let's assume GraphManager's haversineDistance is accessible or copy it here.
// Or even better, make it a static method in a new utils.h/cpp file.
// For this example, let's assume it's copied or globally available.
double haversineDistance(double lat1, double lon1, double lat2, double lon2); // Forward declaration

double RouteFinder::calculateHeuristic(const Node& current, const Node& goal) const {
    // Using Haversine distance as heuristic for geographical coordinates
    return haversineDistance(current.coords.lat, current.coords.lon,
                             goal.coords.lat, goal.coords.lon);
}


std::vector<int> RouteFinder::findRoute(const GraphManager& graph_manager, int origin_id, int dest_id) {
    qDebug() << "RouteFinder: Searching route from" << origin_id << "to" << dest_id;

    // Check if origin or destination are obstacles
    if (graph_manager.isObstacle(origin_id)) {
        qWarning() << "Origin node" << origin_id << "is an obstacle. Cannot find route.";
        return {};
    }
    if (graph_manager.isObstacle(dest_id)) {
        qWarning() << "Destination node" << dest_id << "is an obstacle. Cannot find route.";
        return {};
    }

    const auto& all_nodes = graph_manager.getAllNodes();
    std::unordered_map<int, double> g_score; // Actual cost from origin to current node
    std::unordered_map<int, int> came_from; // Parent node in optimal path
    std::priority_queue<NodeScore, std::vector<NodeScore>, std::greater<NodeScore>> open_set;

    // Initialize scores with infinity
    for (const auto& node : all_nodes) {
        g_score[node.id] = std::numeric_limits<double>::infinity();
    }

    g_score[origin_id] = 0;
    double h_score_origin = calculateHeuristic(graph_manager.getNode(origin_id), graph_manager.getNode(dest_id));
    open_set.push({origin_id, h_score_origin}); // f_score = g_score + h_score

    while (!open_set.empty()) {
        int current_id = open_set.top().node_id;
        open_set.pop();

        if (current_id == dest_id) {
            // Reconstruct path
            std::vector<int> path;
            int temp_id = current_id;
            while (temp_id != origin_id) {
                path.push_back(temp_id);
                temp_id = came_from[temp_id];
            }
            path.push_back(origin_id);
            std::reverse(path.begin(), path.end());
            qDebug() << "RouteFinder: Route found with" << path.size() << "nodes.";
            return path;
        }

        const Node& current_node = graph_manager.getNode(current_id);
        
        // Parallelize neighbor processing
        #pragma omp parallel for
        for (size_t i = 0; i < current_node.neighbors.size(); ++i) {
            int neighbor_id = current_node.neighbors[i];
            const Node& neighbor_node = graph_manager.getNode(neighbor_id);

            // Skip obstacle nodes
            if (graph_manager.isObstacle(neighbor_id)) {
                continue;
            }

            // Calculate tentative_g_score (cost from origin to neighbor via current)
            double dist = haversineDistance(current_node.coords.lat, current_node.coords.lon,
                                            neighbor_node.coords.lat, neighbor_node.coords.lon);
            double tentative_g_score = g_score[current_id] + dist;

            // Using critical section to protect shared g_score and came_from maps
            #pragma omp critical(astar_update)
            {
                if (tentative_g_score < g_score[neighbor_id]) {
                    came_from[neighbor_id] = current_id;
                    g_score[neighbor_id] = tentative_g_score;
                    double f_score = tentative_g_score + calculateHeuristic(neighbor_node, graph_manager.getNode(dest_id));
                    open_set.push({neighbor_id, f_score});
                }
            }
        }
    }

    qWarning() << "RouteFinder: No route found from" << origin_id << "to" << dest_id;
    return {}; // No path found
}
