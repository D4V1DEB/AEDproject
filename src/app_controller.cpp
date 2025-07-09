#include "app_controller.h"

AppController::AppController(QWebEngineView* mapView, GraphManager* graphManager, RouteFinder* routeFinder, QObject *parent)
    : QObject(parent),
      mapView_(mapView),
      graphManager_(graphManager),
      routeFinder_(routeFinder)
{
    qDebug() << "AppController created.";

    // Connect signals from MapInterface to AppController's slots
    MapInterface* mapInterface = qobject_cast<MapInterface*>(mapView_->page()->webChannel()->objects().value("mapInterface"));
    if (mapInterface) {
        connect(mapInterface, &MapInterface::mapReady, this, &AppController::handleMapReady);
        connect(mapInterface, &MapInterface::nodeSelectionRequested, this, &AppController::handleNodeSelectionRequested);
        connect(mapInterface, &MapInterface::obstacleMarkerDrawn, this, &AppController::handleObstacleMarkerDrawn);
        connect(mapInterface, &MapInterface::obstacleAreaDrawn, this, &AppController::handleObstacleAreaDrawn);
    } else {
        qCritical() << "AppController: MapInterface object not found via QWebChannel.";
    }

    // Connect graph manager updates to map display updates
    connect(graphManager_, &GraphManager::graphUpdated, this, &AppController::updateMapJsDisplay);
}

void AppController::loadGraphData(const QString& filePath) {
    emit statusMessage("Loading graph data...");
    // Run graph loading and triangulation in a separate thread using QtConcurrent
    // This prevents the UI from freezing during heavy computation
    QtConcurrent::run([=]() {
        if (graphManager_->loadNodesFromFile(filePath.toStdString())) {
            graphManager_->performTriangulation();
            emit statusMessage("Graph loaded and triangulated successfully. "
                               "Total nodes: " + QString::number(graphManager_->getAllNodes().size()) +
                               ", Total edges: " + QString::number(graphManager_->getAllEdges().size()));
            // The graphUpdated signal from GraphManager will trigger updateMapJsDisplay()
        } else {
            emit statusMessage("Failed to load graph data.");
        }
    });
}

void AppController::findRoute() {
    if (originNodeId_ == -1 || destinationNodeId_ == -1) {
        emit statusMessage("Please select both origin and destination nodes.");
        emit routeFound(false);
        return;
    }

    emit statusMessage("Finding route...");
    QtConcurrent::run([=]() {
        std::vector<int> path = routeFinder_->findRoute(*graphManager_, originNodeId_, destinationNodeId_);
        if (!path.empty()) {
            emit statusMessage("Route found!");
            // Update the map display with the new route
            // This is done via updateMapJsDisplay which will serialize current state
        } else {
            emit statusMessage("No route found between selected nodes.");
        }
        updateMapJsDisplay(); // Force update after route search
        emit routeFound(!path.empty());
    });
}

void AppController::clearObstacles() {
    graphManager_->clearAllObstacles();
    emit statusMessage("All obstacles cleared.");
    // graphUpdated signal from GraphManager will trigger updateMapJsDisplay()
}

// --- Handlers for JavaScript events (from MapInterface) ---
void AppController::handleMapReady() {
    emit statusMessage("Map is ready. Please load graph data.");
    // Optionally load a default graph here
    // loadGraphData("path/to/your/default_nodes.csv");
}

void AppController::handleNodeSelectionRequested(int nodeId) {
    // This function will handle what happens when a node is clicked on the map
    // based on the current selection mode of your UI.
    // You'll need to manage a QActionGroup or similar in your main window
    // to set currentSelectionMode_.

    switch (currentSelectionMode_) {
        case RouteSelectionMode::Origin:
            originNodeId_ = nodeId;
            emit statusMessage("Origin node selected: " + QString::number(nodeId));
            break;
        case RouteSelectionMode::Destination:
            destinationNodeId_ = nodeId;
            emit statusMessage("Destination node selected: " + QString::number(nodeId));
            break;
        case RouteSelectionMode::Obstacle:
            graphManager_->toggleObstacleNode(nodeId);
            emit statusMessage("Node " + QString::number(nodeId) + " obstacle status toggled.");
            break;
        case RouteSelectionMode::ClearObstacle:
            graphManager_->toggleObstacleNode(nodeId); // Same toggle clears it
            emit statusMessage("Node " + QString::number(nodeId) + " obstacle status toggled (cleared).");
            break;
        case RouteSelectionMode::None:
        default:
            emit statusMessage("No selection mode active. Click a UI button first.");
            break;
    }
    updateMapJsDisplay(); // Redraw map to show highlights/obstacles
}

void AppController::handleObstacleMarkerDrawn(const LatLon& coords) {
    int closestNodeId = graphManager_->getClosestNodeId(coords.lat, coords.lon);
    if (closestNodeId != -1) {
        graphManager_->toggleObstacleNode(closestNodeId); // Mark closest node as obstacle
        emit statusMessage("Obstacle added at node: " + QString::number(closestNodeId));
    } else {
        emit statusMessage("No nearby node found for obstacle marker.");
    }
    updateMapJsDisplay();
}

void AppController::handleObstacleAreaDrawn(double minLat, double minLon, double maxLat, double maxLon) {
    // The order of lat/lon from Leaflet.Draw bounds can be a bit tricky,
    // ensure min/max are correct for your GraphManager's expectation.
    graphManager_->setObstacleArea(minLat, minLon, maxLat, maxLon);
    emit statusMessage("Obstacle area set. Nodes within area marked.");
    updateMapJsDisplay();
}

// Helper to push current graph state to JS for display
void AppController::updateMapJsDisplay() {
    nlohmann::json jsonData;
    jsonData["nodes"] = nlohmann::json::array();
    for (const auto& node : graphManager_->getAllNodes()) {
        jsonData["nodes"].push_back({
            {"id", node.id},
            {"lat", node.coords.lat},
            {"lon", node.coords.lon}
        });
    }

    jsonData["edges"] = nlohmann::json::array();
    for (const auto& edge : graphManager_->getAllEdges()) {
        const Node& u = graphManager_->getNode(edge.u_id);
        const Node& v = graphManager_->getNode(edge.v_id);
        jsonData["edges"].push_back({
            {"startLat", u.coords.lat},
            {"startLon", u.coords.lon},
            {"endLat", v.coords.lat},
            {"endLon", v.coords.lon}
        });
    }

    // Include the current route if available
    std::vector<int> currentPath; // Get this from RouteFinder or store it
    // For now, let's re-run findRoute if origin/dest are set (not ideal for performance, but good for demo)
    if (originNodeId_ != -1 && destinationNodeId_ != -1) {
         currentPath = routeFinder_->findRoute(*graphManager_, originNodeId_, destinationNodeId_);
    }

    jsonData["route"] = nlohmann::json::array();
    for (int nodeId : currentPath) {
        const Node& node = graphManager_->getNode(nodeId);
        jsonData["route"].push_back({{"id", node.id}, {"lat", node.coords.lat}, {"lon", node.coords.lon}});
    }

    jsonData["originNodeId"] = originNodeId_;
    jsonData["destinationNodeId"] = destinationNodeId_;

    // Add obstacle node IDs for JS to highlight
    jsonData["obstacleNodeIds"] = nlohmann::json::array();
    for (int obsId : graphManager_->getObstacleNodeIds()) {
        jsonData["obstacleNodeIds"].push_back(obsId);
    }

    std::string jsonString = jsonData.dump();
    QString jsCommand = QString("updateMapDisplay(%1);").arg(QString::fromStdString(jsonString));
    mapView_->page()->runJavaScript(jsCommand);
}
