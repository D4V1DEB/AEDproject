# Street Graph Router

## Project Description

The Street Graph Router is a C++ application designed to visualize and navigate a graph
representation of street intersections. It leverages **Qt** for its graphical user interface, **Qt
WebEngine** to embed an interactive **Leaflet** map with **OpenStreetMap** data, and a C++
backend for complex graph operations. Key features include graph triangulation, user-defined
origin/destination selection, obstacle (no-pass zones) identification, and efficient route
finding (A* algorithm) for millions of nodes using **OpenMP** for parallel processing.

## Features

```
● Large Graph Handling: Designed to manage graphs with millions of nodes (street
intersections).
● Graph Construction:
○ Loads node data (ID, Latitude, Longitude) from CSV files.
```

```
○ Performs Delaunay triangulation (using OpenCV) to generate graph edges
automatically.
● Interactive Map Interface:
○ Embedded Leaflet map powered by OpenStreetMap tiles.
○ Visualizes nodes, edges, routes, and obstacles directly on the map.
○ QWebChannel enables seamless C++ to JavaScript communication for dynamic map
updates.
● Route Selection:
○ User-friendly selection of origin and destination nodes by clicking on the map.
● Obstacle Definition:
○ Mark individual nodes as obstacles via map clicks.
○ Define area-based obstacles using drawing tools (rectangles, polygons, circles)
directly on the map.
○ Clear all active obstacles.
● Efficient Pathfinding:
○ Implements the A* search algorithm for route calculation.
○ OpenMP is integrated to parallelize computationally intensive tasks for enhanced
performance.
● Modular Architecture: Structured with clear separation of concerns (UI, map interface,
graph management, route finding) for collaborative development.
```
## Technologies Used

```
● C++17: Core application logic and algorithms.
● Qt 5 (Core, Widgets, WebEngineWidgets, WebChannel): Desktop GUI framework,
embedded browser, and C++ to JavaScript communication.
● OpenCV 4.x: Used for Delaunay triangulation in graph construction.
● Leaflet.js: Open-source JavaScript library for interactive maps.
● Leaflet.Draw: Plugin for drawing shapes on the map (used for defining obstacle areas).
● OpenStreetMap: Provides the base map data tiles.
● nlohmann/json: Header-only C++ library for JSON parsing and generation (for C++ to
JavaScript data transfer).
● CMake: Cross-platform build system.
● OpenMP: Compiler directive-based parallelization for C++ code.
```
## Getting Started

These instructions will get you a copy of the project up and running on your local machine for


development and testing purposes.

### Prerequisites

```
● C++ Compiler: G++ (GCC) 9 or newer (Ubuntu: sudo apt install build-essential)
● CMake: 3.10 or newer (Ubuntu: sudo apt install cmake)
● Qt 5 Development Libraries:
Bash
sudo apt install qtbase5-dev qtwebengine5-dev libqt5webchannel5-dev
● OpenCV Development Libraries:
Bash
sudo apt install libopencv-dev
● nlohmann/json: This is a header-only library. Download the json.hpp file and place it in
the libs/nlohmann_ json/ directory of your project.
Bash
mkdir -p my_graph_project/libs/nlohmann_ json
wget -O my_graph_project/libs/nlohmann_ json/json.hpp
https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
# Verify the path is correct
ls my_graph_project/libs/nlohmann_ json/json.hpp
(Note: Check nlohmann/json GitHub for the latest stable release version if 3.11.2 is
outdated.)
```
### Building the Project

1. **Clone the repository:**
    Bash
    git clone https://github.com/your-username/my_graph_project.git # Replace with your repo
    URL
    cd my_graph_project
2. **Create a build directory:**
    Bash
    mkdir build
    cd build


3. **Configure the project with CMake:**
    Bash
    cmake ..
    You should see messages confirming that Qt5, OpenCV, and OpenMP are found.
4. **Compile the project:**
    Bash
    make -j$(nproc) # Uses all available CPU cores for faster compilation

### Running the Application

After a successful build, the executable GraphRoutingProject (or whatever you named it in
CMakeLists.txt) will be located in the build/ directory.
Bash
cd build
./GraphRoutingProject

## Usage

1. **Launch the Application:** Run the GraphRoutingProject executable. A Qt window will
    appear with an embedded map.
2. **Load Graph Data:**
    ○ Go to File -> Load Nodes from CSV....
    ○ Select a CSV file containing your node data (e.g., data/nodes.csv). The format should
       be id,latitude,longitude per line.
    ○ The application will load the nodes and perform triangulation, then display them on
       the map.
3. **Select Origin/Destination:**
    ○ Go to Select Mode -> Select Origin.


```
○ Click on a node (marker) on the map to set it as the origin.
○ Go to Select Mode -> Select Destination.
○ Click on another node on the map to set it as the destination.
```
4. **Define Obstacles:**
    ○ Go to Select Mode -> Select Obstacle.
    ○ **Single Node:** Click on a node marker on the map to toggle its obstacle status (it will
       turn red).
    ○ **Area Obstacles:** Use the drawing tools on the left side of the map (rectangle,
       polygon, circle, marker) to draw areas or place individual markers. Nodes within the
       drawn area will be marked as obstacles.
    ○ To clear all obstacles, go to Select Mode -> Clear All Obstacles.
5. **Find Route:**
    ○ Once origin, destination, and any obstacles are set, go to Route -> Find Route.
    ○ The A* algorithm will run, and if a path is found, it will be displayed on the map as a
       red line.
    ○ Status messages at the top of the window will provide feedback.

## Project Structure (For Developers)

This section details the purpose of each file and directory to aid in collaboration and
development.
● **CMakeLists.txt** : The root CMake file controlling the entire build process, linking libraries,
and defining targets.
● **src/** : Contains all C++ source code.
○ main.cpp: The entry point of the application. Sets up the main window,
QWebEngineView, QWebChannel, and initializes the AppController, GraphManager,
and RouteFinder.
○ app_controller.h/app_controller.cpp: Acts as the orchestrator between the UI (main
window, buttons), MapInterface (JS events), GraphManager, and RouteFinder.
Handles user interactions and dispatches tasks.
○ map_interface.h/map_interface.cpp: The **QWebChannel bridge**. map_interface.h
declares Q_INVOKABLE slots (callable by JavaScript) and signals (emitted to C++
backend). map_interface.cpp contains their minimal implementations, primarily
emitting C++ signals.
○ graph_manager.h/graph_manager.cpp: Manages all graph-related data (Node, Edge
structures) and operations. This includes loading nodes from files, performing
Delaunay triangulation (using OpenCV's Subdiv2D), and handling obstacle node
management.
○ route_finder.h/route_finder.cpp: Encapsulates the pathfinding logic. Implements the
**A*** search algorithm, considering obstacles.


```
○ data_types.h: Defines common data structures used across the backend, such as
Node, Edge, LatLon, and RouteSelectionMode enum.
● web/ : Contains web assets for the QWebEngineView.
○ map.html: The HTML file embedding the Leaflet map. Contains JavaScript for map
initialization, displaying graph elements, handling user map interactions, and
managing the QWebChannel connection.
● libs/ : External header-only libraries or small uninstalled libraries.
○ nlohmann_ json/json.hpp: The header file for the nlohmann/json C++ JSON library.
● data/ : Directory for sample input data files.
○ nodes.csv: Example CSV file for node coordinates.
● build/ : (Generated automatically by CMake) Contains compiled binaries, makefiles, and
intermediate build artifacts.
● README.md : This file.
```
## Contributing

We welcome contributions! Please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature (git checkout -b feature/AmazingFeature).
3. Commit your changes (git commit -m 'Add some AmazingFeature').
4. Push to the branch (git push origin feature/AmazingFeature).
5. Open a Pull Request.

## License

This project is licensed under the MIT License - see the LICENSE file (if you add one) for
details.

## Acknowledgments

```
● OpenStreetMap: For open-source map data.
● Leaflet: For the excellent JavaScript mapping library.
● Leaflet.Draw: For map drawing capabilities.
● Qt Project: For the powerful cross-platform UI framework.
```

● OpenCV: For computer vision and geometric algorithms.
● nlohmann/json: For easy JSON handling in C++.
This README.md should give your classmates a very clear understanding of the project, how
to set it up, and where to contribute. Good luck!


