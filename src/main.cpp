#include <QApplication>
#include <QMainWindow>
#include <QWebEngineView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFile>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QActionGroup>
#include <QMenu>
#include <QMenuBar>

// Include your custom classes
#include "app_controller.h"
#include "graph_manager.h"
#include "route_finder.h"
#include "map_interface.h" // Even though MapInterface is connected by AppController,
                            // main might need to interact with it directly for channel setup.

// Global utility for haversineDistance - required by GraphManager and RouteFinder
// Best practice would be to put this in a dedicated `utils.h/cpp` file.
const double EARTH_RADIUS_KM = 6371.0;
double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    lat1 = lat1 * M_PI / 180.0;
    lat2 = lat2 * M_PI / 180.0;

    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dLon / 2) * std::sin(dLon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return EARTH_RADIUS_KM * c;
}


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("Street Graph Router");
    window.resize(1200, 800); // Larger window for map and controls

    QWidget *centralWidget = new QWidget(&window);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget); // Main vertical layout

    // --- Top Control Panel ---
    QHBoxLayout *controlLayout = new QHBoxLayout();
    mainLayout->addLayout(controlLayout);

    QLabel *statusLabel = new QLabel("Status: Ready to load graph.");
    controlLayout->addWidget(statusLabel);
    controlLayout->addStretch(1); // Push status to left

    // --- Map View ---
    QWebEngineView *mapView = new QWebEngineView(centralWidget);
    mainLayout->addWidget(mapView);

    // --- Backend Instances ---
    GraphManager graphManager;
    RouteFinder routeFinder;

    // --- QWebChannel Setup (Crucial for JS-C++ bridge) ---
    QWebChannel *channel = new QWebChannel(mapView->page());
    MapInterface mapInterface; // MapInterface instance
    channel->registerObject("mapInterface", &mapInterface); // Expose to JS
    mapView->page()->setWebChannel(channel);

    // --- AppController ---
    AppController appController(mapView, &graphManager, &routeFinder, &window);
    // Connect AppController's status messages to the UI status label
    QObject::connect(&appController, &AppController::statusMessage, statusLabel, &QLabel::setText);

    // --- Load map.html into QWebEngineView ---
    QString htmlFilePath = QCoreApplication::applicationDirPath() + "/web/map.html";
    if (!QFile::exists(htmlFilePath)) {
        qCritical() << "ERROR: map.html not found at:" << htmlFilePath;
        qCritical() << "Please ensure 'web/map.html' is in the same directory as your executable.";
        return -1;
    }
    mapView->setUrl(QUrl::fromLocalFile(htmlFilePath));
    qDebug() << "Loading HTML from:" << htmlFilePath;

    // --- UI Buttons and Actions ---
    QMenuBar *menuBar = window.menuBar();
    QMenu *fileMenu = menuBar->addMenu("&File");
    QAction *loadNodesAction = fileMenu->addAction("&Load Nodes from CSV...");
    QObject::connect(loadNodesAction, &QAction::triggered, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&window, "Open Node CSV File", "", "CSV Files (*.csv);;All Files (*)");
        if (!fileName.isEmpty()) {
            appController.loadGraphData(fileName);
        }
    });

    QMenu *selectionMenu = menuBar->addMenu("&Select Mode");
    QActionGroup *selectionGroup = new QActionGroup(&window); // Ensures only one is checked
    selectionGroup->setExclusive(true);

    QAction *selectOriginAction = selectionMenu->addAction("Select &Origin");
    selectOriginAction->setCheckable(true);
    selectionGroup->addAction(selectOriginAction);
    QObject::connect(selectOriginAction, &QAction::toggled, [&](bool checked){
        if (checked) appController.currentSelectionMode_ = RouteSelectionMode::Origin;
        else if (selectionGroup->checkedAction() == nullptr) appController.currentSelectionMode_ = RouteSelectionMode::None;
        appController.updateMapJsDisplay(); // Update map to reflect mode
        emit statusLabel->setText("Mode: Select Origin. Click on map node.");
    });


    QAction *selectDestinationAction = selectionMenu->addAction("Select &Destination");
    selectDestinationAction->setCheckable(true);
    selectionGroup->addAction(selectDestinationAction);
    QObject::connect(selectDestinationAction, &QAction::toggled, [&](bool checked){
        if (checked) appController.currentSelectionMode_ = RouteSelectionMode::Destination;
        else if (selectionGroup->checkedAction() == nullptr) appController.currentSelectionMode_ = RouteSelectionMode::None;
        appController.updateMapJsDisplay();
        emit statusLabel->setText("Mode: Select Destination. Click on map node.");
    });

    QAction *selectObstacleAction = selectionMenu->addAction("Select &Obstacle");
    selectObstacleAction->setCheckable(true);
    selectionGroup->addAction(selectObstacleAction);
    QObject::connect(selectObstacleAction, &QAction::toggled, [&](bool checked){
        if (checked) appController.currentSelectionMode_ = RouteSelectionMode::Obstacle;
        else if (selectionGroup->checkedAction() == nullptr) appController.currentSelectionMode_ = RouteSelectionMode::None;
        appController.updateMapJsDisplay();
        emit statusLabel->setText("Mode: Select Obstacle. Click node or use Leaflet.Draw.");
    });

    QAction *clearObstaclesAction = selectionMenu->addAction("&Clear All Obstacles");
    QObject::connect(clearObstaclesAction, &QAction::triggered, &appController, &AppController::clearObstacles);

    QMenu *routeMenu = menuBar->addMenu("&Route");
    QAction *findRouteAction = routeMenu->addAction("&Find Route");
    QObject::connect(findRouteAction, &QAction::triggered, &appController, &AppController::findRoute);


    window.setCentralWidget(centralWidget);
    window.show();

    return app.exec();
}
