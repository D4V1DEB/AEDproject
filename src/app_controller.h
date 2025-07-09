#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include <QObject>
#include <QString>
#include <QWebEngineView> // Needs to interact with the map view
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtConcurrent/QtConcurrent> // For running heavy tasks in a separate thread

#include "map_interface.h"
#include "graph_manager.h"
#include "route_finder.h"
#include "data_types.h"
#include "nlohmann/json/json.hpp" // For C++ JSON manipulation

class AppController : public QObject {
    Q_OBJECT

public:
    explicit AppController(QWebEngineView* mapView, GraphManager* graphManager, RouteFinder* routeFinder, QObject *parent = nullptr);

    // Methods called by UI elements (e.g., buttons)
    void loadGraphData(const QString& filePath);
    void findRoute();
    void clearObstacles();

public slots:
    // Slots to receive signals from MapInterface (JavaScript events)
    void handleMapReady();
    void handleNodeSelectionRequested(int nodeId);
    void handleObstacleMarkerDrawn(const LatLon& coords);
    void handleObstacleAreaDrawn(double minLat, double minLon, double maxLat, double maxLon);

private:
    QWebEngineView* mapView_;
    GraphManager* graphManager_;
    RouteFinder* routeFinder_;

    int originNodeId_ = -1;
    int destinationNodeId_ = -1;
    RouteSelectionMode currentSelectionMode_ = RouteSelectionMode::None;

    // Helper to send data to JS
    void updateMapJsDisplay();

signals:
    // Signals to update the UI (e.g., status messages, enable/disable buttons)
    void statusMessage(const QString& message);
    void routeFound(bool success);
};

#endif // APP_CONTROLLER_H
