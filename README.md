# Street Graph Router

## Project Description

The Street Graph Router is a C++ application designed to visualize and navigate a graph representation of street intersections. It leverages **Qt** for its graphical user interface, **Qt WebEngine** to embed an interactive **Leaflet** map with **OpenStreetMap** data, and a C++ backend for complex graph operations. Key features include graph triangulation, user-defined origin/destination selection, obstacle (no-pass zones) identification, and efficient route finding (A* algorithm) for millions of nodes using **OpenMP** for parallel processing.

## Features

- **Large Graph Handling:** Designed to manage graphs with millions of nodes (street intersections).
- **Graph Construction:**
  - Loads node data (ID, Latitude, Longitude) from CSV files.
  - Performs **Delaunay triangulation (using OpenCV)** to generate graph edges automatically.
- **Interactive Map Interface:**
  - Embedded **Leaflet** map powered by **OpenStreetMap** tiles.
  - Visualizes nodes, edges, routes, and obstacles directly on the map.
  - **QWebChannel** enables seamless C++ to JavaScript communication for dynamic map updates.
- **Route Selection:**
  - User-friendly selection of origin and destination nodes by clicking on the map.
- **Obstacle Definition:**
  - Mark individual nodes as obstacles via map clicks.
  - Define **area-based obstacles** using drawing tools (rectangles, polygons, circles) directly on the map.
  - Clear all active obstacles.
- **Efficient Pathfinding:**
  - Implements the **A* search algorithm** for route calculation.
  - **OpenMP** is integrated to parallelize computationally intensive tasks for enhanced performance.
- **Modular Architecture:** Structured with clear separation of concerns (UI, map interface, graph management, route finding) for collaborative development.

## Technologies Used

- **C++17:** Core application logic and algorithms.
- **Qt 5 (Core, Widgets, WebEngineWidgets, WebChannel):** Desktop GUI framework, embedded browser, and C++ to JavaScript communication.
- **OpenCV 4.x:** Used for Delaunay triangulation in graph construction.
- **Leaflet.js:** Open-source JavaScript library for interactive maps.
- **Leaflet.Draw:** Plugin for drawing shapes on the map (used for defining obstacle areas).
- **OpenStreetMap:** Provides the base map data tiles.
- **nlohmann/json:** Header-only C++ library for JSON parsing and generation (for C++ to JavaScript data transfer).
- **CMake:** Cross-platform build system.
- **OpenMP:** Compiler directive-based parallelization for C++ code.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

- C++ Compiler: G++ (GCC) 9 or newer (Ubuntu: `sudo apt install build-essential`)
- CMake: 3.10 or newer (Ubuntu: `sudo apt install cmake`)
- Qt 5 Development Libraries:
  ```bash
  sudo apt install qtbase5-dev qtwebengine5-dev libqt5webchannel5-dev
