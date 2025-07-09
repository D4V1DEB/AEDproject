#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <vector>
#include <string>
#include <limits> // For std::numeric_limits

// Struct to represent a geographic point
struct LatLon {
    double lat;
    double lon;

    // Default constructor
    LatLon() : lat(0.0), lon(0.0) {}

    // Parameterized constructor
    LatLon(double lat, double lon) : lat(lat), lon(lon) {}

    // Equality operator (useful for comparisons)
    bool operator==(const LatLon& other) const {
        // Compare with a small epsilon for floating point numbers
        return (std::abs(lat - other.lat) < std::numeric_limits<double>::epsilon() * 100.0) &&
               (std::abs(lon - other.lon) < std::numeric_limits<double>::epsilon() * 100.0);
    }
};

// Struct to represent a graph node (intersection)
struct Node {
    int id;
    LatLon coords;
    std::vector<int> neighbors; // Adjacency list
    bool is_obstacle; // Flag set by user input

    // Default constructor
    Node() : id(-1), is_obstacle(false) {}

    // Parameterized constructor
    Node(int id, double lat, double lon) : id(id), coords(lat, lon), is_obstacle(false) {}
};

// Struct to represent a graph edge
struct Edge {
    int u_id; // ID of the first node
    int v_id; // ID of the second node
    double weight; // Distance or cost of the edge

    // Default constructor
    Edge() : u_id(-1), v_id(-1), weight(0.0) {}

    // Parameterized constructor
    Edge(int u, int v, double w) : u_id(u), v_id(v), weight(w) {}
};

// Enum for different route selection modes (useful for future expansion)
enum class RouteSelectionMode {
    None,
    Origin,
    Destination,
    Obstacle,
    ClearObstacle
};

#endif // DATA_TYPES_H
