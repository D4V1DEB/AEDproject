#ifndef NODEDISTANCE_H
#define NODEDISTANCE_H
#include <iostream>

template<typename N, typename E>
struct NodeDistance {
    E distance;
    N node;

    bool operator<(const NodeDistance& other) const {
        return distance < other.distance;
    }
};

template<typename N, typename E>
bool operator==(const NodeDistance<N,E>& a, const NodeDistance<N,E>& b) {
    return a.node == b.node && a.distance == b.distance;
}

// Specialized operator<< for NodeDistance<N,E>
template<typename N, typename E>
std::ostream& operator<<(std::ostream& os, const NodeDistance<N,E>& nd) {
    os << "(" << nd.node << ", " << nd.distance << ")";
    return os;
}

#endif //NODEDISTANCE_H
