cmake_minimum_required(VERSION 3.10)
project(GraphRoutingProject CXX)

set(CMAKE_CXX_STANDARD 17) # Ensure C++17 features are available
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Prefer standard-compliant code

# --- Find Qt5 Modules ---
# Required for GUI, embedded web browser, and C++ <-> JS communication
find_package(Qt5 COMPONENTS Core Widgets WebEngineWidgets WebChannel REQUIRED)

# --- Find OpenCV ---
# Required for triangulation (e.g., cv::Subdiv2D)
find_package(OpenCV REQUIRED)

# --- Configure OpenMP ---
# Required for parallel processing in C++ algorithms
find_package(OpenMP REQUIRED)

# --- Define the Executable Target ---
# List all your C++ source files that form the executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/app_controller.cpp
    src/map_interface.cpp
    src/graph_manager.cpp
    src/route_finder.cpp
    # Add other .cpp files here as you create them, e.g., src/utils.cpp
)

# --- Include Directories ---
# Point to where your header-only libraries (like nlohmann/json) are located
# This path is relative to your project root.
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/libs/nlohmann_json
    ${PROJECT_SOURCE_DIR}/src # Allow src files to include each other (e.g., "map_interface.h")
)

# --- Apply Compiler Options and Link Libraries to the Executable ---

# OpenMP compiler flags (must be applied after add_executable)
if (OPENMP_FOUND)
    message(STATUS "OpenMP found. Compiling with OpenMP support.")
    target_compile_options(${PROJECT_NAME} PRIVATE ${OpenMP_C_FLAGS} ${OpenMP_CXX_FLAGS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenMP_CXX_LIBRARIES})
else()
    message(WARNING "OpenMP not found. Code will run sequentially where OpenMP directives are used.")
endif()

# Link Qt libraries
# CMAKE_AUTOMOC, CMAKE_AUTOUIC, CMAKE_AUTORCC are crucial for Qt.
# They enable Qt's meta-object system, UI file compilation, and resource embedding.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Widgets
    Qt5::WebEngineWidgets
    Qt5::WebChannel
)

# Link OpenCV libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OpenCV_LIBS}
)

# --- Copy Web Assets to Build Directory ---
# Ensures that map.html and any other web resources are available next to your executable.
# You can extend this to copy whole directories if needed (e.g., 'web/css/', 'web/js/')
file(COPY ${PROJECT_SOURCE_DIR}/web/map.html DESTINATION ${CMAKE_BINARY_DIR}/web/)
# If you add css/js/img subdirectories, you might need:
# file(COPY ${PROJECT_SOURCE_DIR}/web/css DESTINATION ${CMAKE_BINARY_DIR}/web/)
# file(COPY ${PROJECT_SOURCE_DIR}/web/js DESTINATION ${CMAKE_BINARY_DIR}/web/)
# file(COPY ${PROJECT_SOURCE_DIR}/web/img DESTINATION ${CMAKE_BINARY_DIR}/web/)
