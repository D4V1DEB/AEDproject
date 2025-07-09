#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <QDebug> // For debugging purposes within the class
#include <opencv2/core.hpp> // For OpenCV data types (Point2f, Rect)
#include <opencv2/imgproc.hpp> // For cv::Subdiv2D

#include "data_types.h" // Your common data types

// A custom hash for LatLon if you need to use it in unordered_map/set keys
// For Node, Edge, etc.
namespace std {
    template<> struct hash<LatLon> {
        size_t operator()(const LatLon& ll) const {
            return hash<double>()(ll.lat) ^ (hash<double>()(ll.lon) << 1);
        }
    };
}

class GraphManager : public QObject { // Inherit from QObject for signals/slots if needed later
    Q_OBJECT // Add Q_OBJECT if you want this class to use signals/slots itself.

public:
    explicit GraphManager(QObject *parent = nullptr) : QObject(parent) {
        qDebug() << "GraphManager created.";
    }

    // Core operations
    bool loadNodesFromFile(const std::string& filepath);
    void performTriangulation(); // Generates edges using OpenCV
    int getClosestNodeId(double lat, double lon) const; // Finds graph node from map click

    // Obstacle management
    void toggleObstacleNode(int nodeId); // Toggles obstacle status for a single node
    void setObstacleArea(double minLat, double minLon, double maxLat, double maxLon); // Sets obstacles within a bounding box
    void clearAllObstacles(); // Clears all obstacles

    // Getters for graph data (for drawing and route finding)
    const std::vector<Node>& getAllNodes() const { return nodes_; }
    const std::vector<Edge>& getAllEdges() const { return edges_; }
    const std::unordered_set<int>& getObstacleNodeIds() const { return obstacle_node_ids_; }
    Node getNode(int nodeId) const; // Throws if not found

signals:
    // Signal to notify that graph data has changed (e.g., after loading, triangulation, or obstacle change)
    void graphUpdated();

private:
    std::vector<Node> nodes_;
    std::vector<Edge> edges_; // Explicit list of edges after triangulation
    std::unordered_map<int, size_t> node_id_to_index_map_; // Maps node ID to its index in 'nodes_' vector
    std::unordered_set<int> obstacle_node_ids_; // Stores IDs of nodes currently marked as obstacles

    // Helper for triangulation: maps OpenCV points back to Node IDs
    // You'll need a way to quickly look up Node ID by LatLon or by its original index
    std::unordered_map<cv::Point2f, int, std::hash<cv::Point2f>> cv_point_to_node_id_map_;
    // Custom hash for cv::Point2f for the unordered_map (required)
    struct CvPoint2fHash {
        size_t operator()(const cv::Point2f& p) const {
            return std::hash<float>()(p.x) ^ (std::hash<float>()(p.y) << 1);
        }
    };
    std::unordered_map<cv::Point2f, int, CvPoint2fHash> node_coords_to_id_map_; // To map triangulation points to node IDs
};

#endif // GRAPH_MANAGER_H
