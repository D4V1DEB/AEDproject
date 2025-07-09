#include "graph_manager.h"
#include <fstream>
#include <sstream>
#include <algorithm> // For std::find, std::min, std::max
#include <limits>    // For std::numeric_limits
#include <omp.h>     // For OpenMP

// Haversine distance function (approximation, consider using a more precise one if needed)
double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    // Radius of Earth in kilometers
    const double R = 6371.0;

    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    lat1 = lat1 * M_PI / 180.0;
    lat2 = lat2 * M_PI / 180.0;

    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dLon / 2) * std::sin(dLon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return R * c; // Distance in kilometers
}


bool GraphManager::loadNodesFromFile(const std::string& filepath) {
    qDebug() << "GraphManager: Loading nodes from" << QString::fromStdString(filepath);
    std::ifstream file(filepath);
    if (!file.is_open()) {
        qCritical() << "Error: Could not open node file:" << QString::fromStdString(filepath);
        return false;
    }

    nodes_.clear();
    node_id_to_index_map_.clear();
    node_coords_to_id_map_.clear(); // Clear for triangulation mapping

    std::string line;
    // Skip header if any
    std::getline(file, line); // Assuming first line is header: "id,lat,lon"

    int count = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string segment;
        int id;
        double lat, lon;

        if (std::getline(ss, segment, ',')) {
            id = std::stoi(segment);
        } else continue;
        if (std::getline(ss, segment, ',')) {
            lat = std::stod(segment);
        } else continue;
        if (std::getline(ss, segment, ',')) {
            lon = std::stod(segment);
        } else continue;

        nodes_.emplace_back(id, lat, lon);
        node_id_to_index_map_[id] = nodes_.size() - 1;
        node_coords_to_id_map_[cv::Point2f(static_cast<float>(lon), static_cast<float>(lat))] = id;
        count++;
    }
    qDebug() << "GraphManager: Loaded" << count << "nodes.";

    // Optional: Parallelize initial adjacency list resizing
    adj_list_.resize(nodes_.size());

    // Signal that the graph has been loaded
    emit graphUpdated();
    return true;
}

void GraphManager::performTriangulation() {
    qDebug() << "GraphManager: Performing Delaunay triangulation...";
    edges_.clear();
    for (auto& node : nodes_) {
        node.neighbors.clear(); // Clear existing neighbors from previous runs
    }

    if (nodes_.empty()) {
        qWarning() << "GraphManager: No nodes loaded for triangulation.";
        return;
    }

    std::vector<cv::Point2f> cv_points;
    double min_lat = std::numeric_limits<double>::max();
    double max_lat = std::numeric_limits<double>::lowest();
    double min_lon = std::numeric_limits<double>::max();
    double max_lon = std::numeric_limits<double>::lowest();

    // Collect points and determine bounding box for Subdiv2D
    for (const auto& node : nodes_) {
        cv_points.emplace_back(static_cast<float>(node.coords.lon), static_cast<float>(node.coords.lat));
        min_lat = std::min(min_lat, node.coords.lat);
        max_lat = std::max(max_lat, node.coords.lat);
        min_lon = std::min(min_lon, node.coords.lon);
        max_lon = std::max(max_lon, node.coords.lon);
    }

    // Create a bounding rectangle slightly larger than the min/max coordinates
    // This helps prevent issues at the edges of the triangulation
    double padding = 0.01; // Small padding in degrees
    cv::Rect2f rect(min_lon - padding, min_lat - padding,
                    (max_lon - min_lon) + 2 * padding, (max_lat - min_lat) + 2 * padding);

    cv::Subdiv2D subdiv(rect);
    subdiv.insert(cv_points); // Insert all points

    std::vector<cv::Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);

    // Using a set to avoid duplicate edges (an edge might appear in multiple triangles)
    std::unordered_set<std::pair<int, int>, std::hash<std::pair<int, int>>> unique_edges;

    // Parallelize processing of triangles to build edges
    // Using a critical section for shared data (unique_edges, edges_, adj_list_)
    #pragma omp parallel for
    for (size_t i = 0; i < triangleList.size(); ++i) {
        cv::Vec6f t = triangleList[i];
        cv::Point2f p1(t[0], t[1]);
        cv::Point2f p2(t[2], t[3]);
        cv::Point2f p3(t[4], t[5]);

        // Only process valid triangles (vertices should correspond to original points)
        if (node_coords_to_id_map_.count(p1) &&
            node_coords_to_id_map_.count(p2) &&
            node_coords_to_id_map_.count(p3))
        {
            int id1 = node_coords_to_id_map_[p1];
            int id2 = node_coords_to_id_map_[p2];
            int id3 = node_coords_to_id_map_[p3];

            // Helper to add edge if it doesn't already exist
            auto add_edge_if_new = [&](int u, int v) {
                if (u == v) return; // No self-loops
                // Ensure canonical order for unique_edges set
                int min_id = std::min(u, v);
                int max_id = std::max(u, v);
                if (unique_edges.find({min_id, max_id}) == unique_edges.end()) {
                    #pragma omp critical
                    {
                        if (unique_edges.find({min_id, max_id}) == unique_edges.end()) { // Double check
                            unique_edges.insert({min_id, max_id});
                            const Node& node_u = getNode(u);
                            const Node& node_v = getNode(v);
                            double weight = haversineDistance(node_u.coords.lat, node_u.coords.lon,
                                                              node_v.coords.lat, node_v.coords.lon);
                            edges_.emplace_back(u, v, weight);

                            // Add to adjacency list (index-based)
                            nodes_[node_id_to_index_map_[u]].neighbors.push_back(v);
                            nodes_[node_id_to_index_map_[v]].neighbors.push_back(u); // Undirected graph
                        }
                    }
                }
            };

            add_edge_if_new(id1, id2);
            add_edge_if_new(id2, id3);
            add_edge_if_new(id3, id1);
        }
    }

    qDebug() << "GraphManager: Triangulation complete. Found" << edges_.size() << "edges.";
    emit graphUpdated();
}

int GraphManager::getClosestNodeId(double lat, double lon) const {
    if (nodes_.empty()) return -1; // No nodes to search

    int closestId = -1;
    double minDistance = std::numeric_limits<double>::max();

    // Use OpenMP for parallel search of closest node
    // Note: Reduction for minDistance and closestId needs careful handling or
    // a custom reduction, or simply run sequentially for this relatively small loop.
    // For 1M nodes, a k-d tree or similar spatial index is much better.
    // For simplicity of this example, we'll use a sequential brute force.

    // A simple sequential scan is easier and often fast enough for this specific task
    // unless you call it thousands of times per second.
    for (const auto& node : nodes_) {
        double dist = haversineDistance(lat, lon, node.coords.lat, node.coords.lon);
        if (dist < minDistance) {
            minDistance = dist;
            closestId = node.id;
        }
    }

    qDebug() << "GraphManager: Closest node to (" << lat << "," << lon << ") is ID" << closestId
             << "with distance" << minDistance << "km.";
    return closestId;
}

void GraphManager::toggleObstacleNode(int nodeId) {
    auto it = node_id_to_index_map_.find(nodeId);
    if (it != node_id_to_index_map_.end()) {
        size_t index = it->second;
        nodes_[index].is_obstacle = !nodes_[index].is_obstacle; // Toggle status
        if (nodes_[index].is_obstacle) {
            obstacle_node_ids_.insert(nodeId);
            qDebug() << "GraphManager: Node" << nodeId << "set as obstacle.";
        } else {
            obstacle_node_ids_.erase(nodeId);
            qDebug() << "GraphManager: Node" << nodeId << "cleared as obstacle.";
        }
        emit graphUpdated();
    } else {
        qWarning() << "GraphManager: Node ID" << nodeId << "not found.";
    }
}

void GraphManager::setObstacleArea(double minLat, double minLon, double maxLat, double maxLon) {
    qDebug() << "GraphManager: Setting obstacle area from (" << minLat << "," << minLon << ") to (" << maxLat << "," << maxLon << ")";
    int count = 0;
    #pragma omp parallel for reduction(+:count) // Parallelize the loop over nodes
    for (size_t i = 0; i < nodes_.size(); ++i) {
        Node& node = nodes_[i]; // Reference to modify directly
        if (node.coords.lat >= minLat && node.coords.lat <= maxLat &&
            node.coords.lon >= minLon && node.coords.lon <= maxLon)
        {
            if (!node.is_obstacle) { // Only change if not already an obstacle
                node.is_obstacle = true;
                #pragma omp critical(obstacle_set_update) // Protect access to shared set
                {
                    obstacle_node_ids_.insert(node.id);
                }
                count++;
            }
        }
    }
    qDebug() << "GraphManager: Set" << count << "nodes as obstacles in the area.";
    emit graphUpdated();
}

void GraphManager::clearAllObstacles() {
    qDebug() << "GraphManager: Clearing all obstacles.";
    #pragma omp parallel for
    for (size_t i = 0; i < nodes_.size(); ++i) {
        nodes_[i].is_obstacle = false; // Reset flag
    }
    obstacle_node_ids_.clear(); // Clear the set
    emit graphUpdated();
}

Node GraphManager::getNode(int nodeId) const {
    auto it = node_id_to_index_map_.find(nodeId);
    if (it != node_id_to_index_map_.end()) {
        return nodes_[it->second];
    }
    qWarning() << "GraphManager: getNode() - Node ID" << nodeId << "not found.";
    return Node(-1, 0, 0); // Return an invalid node
}

// Custom hash for cv::Point2f, needed for unordered_map<cv::Point2f, int>
namespace cv {
    bool operator==(const Point2f& a, const Point2f& b) {
        return (std::abs(a.x - b.x) < std::numeric_limits<float>::epsilon() * 100.0f) &&
               (std::abs(a.y - b.y) < std::numeric_limits<float>::epsilon() * 100.0f);
    }
} // namespace cv
