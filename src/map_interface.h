#ifndef MAP_INTERFACE_H
#define MAP_INTERFACE_H

#include <QObject>
#include <QDebug>
#include <QWebChannel>
#include <QString>
#include <QRectF> // For bounding box (rectangle)
#include "data_types.h" // Include your common data types

class MapInterface : public QObject {
    Q_OBJECT

public:
    explicit MapInterface(QObject *parent = nullptr) : QObject(parent) {
        qDebug() << "MapInterface C++ object created.";
    }

public slots:
    // Called from JS when Leaflet map is fully loaded and QWebChannel is ready
    Q_INVOKABLE void onMapLoaded() {
        qDebug() << "FROM JAVASCRIPT: Map fully loaded and QWebChannel ready.";
        emit mapReady();
    }

    // Called from JS when user clicks on an existing node marker on the map
    Q_INVOKABLE void onNodeClick(int nodeId) {
        qDebug() << "FROM JAVASCRIPT: Node clicked: ID =" << nodeId;
        emit nodeSelectionRequested(nodeId);
    }

    // Called from JS when user draws a single marker obstacle
    Q_INVOKABLE void onObstacleMarkerDrawn(double lat, double lon) {
        qDebug() << "FROM JAVASCRIPT: Obstacle marker drawn at Lat:" << lat << ", Lon:" << lon;
        emit obstacleMarkerDrawn(LatLon(lat, lon));
    }

    // Called from JS when user draws a rectangular or polygonal obstacle area
    Q_INVOKABLE void onObstacleAreaDrawn(double minLat, double minLon, double maxLat, double maxLon) {
        qDebug() << "FROM JAVASCRIPT: Obstacle area drawn (bbox): "
                 << "minLat=" << minLat << ", minLon=" << minLon
                 << ", maxLat=" << maxLat << ", maxLon=" << maxLon;
        // QRectF uses (left, top, width, height). For geographic, it's (minLon, maxLat, (maxLon-minLon), (minLat-maxLat))
        // Or you can just pass the 4 doubles. Let's pass the 4 doubles directly for clarity.
        emit obstacleAreaDrawn(minLat, minLon, maxLat, maxLon);
    }

    // A general log function from JS for debugging
    Q_INVOKABLE void logFromJs(const QString &message) {
        qDebug() << "FROM JAVASCRIPT (Log):" << message;
    }

signals:
    // Signals to communicate map events to other C++ backend components
    void mapReady();
    void nodeSelectionRequested(int nodeId);
    void obstacleMarkerDrawn(const LatLon& coords);
    void obstacleAreaDrawn(double minLat, double minLon, double maxLat, double maxLon);
    // You might add signals for clearing obstacles, editing drawn shapes etc.
};

#endif // MAP_INTERFACE_H
