# mind-net

Mind Net is a mind map storage written in the C++ programming language:

Requirements:
* Linux
* Windows

Used technologies:
* C++17
* SQLite
* CMake

## Screenshots

### List nodes

![List nodes](screenshots/screen_list_nodes.jpg "List nodes")

### Read node

![Read node](screenshots/screen_read_node.jpg "Read node")

### Create node

![Create node](screenshots/screen_create_node.jpg "Create node")

### Delete node

![Delete node](screenshots/screen_delete_node.jpg "Delete node")

### List properties

![List properties](screenshots/screen_list_properties.jpg "List properties")

### Graph demo

![Graph demo](screenshots/screen_graph_demo.jpg "Graph demo")

### Calling get list for node

![Calling get list for node](screenshots/screen_calling_get_list_for_node.jpg "Calling get list for node")

## How to build

### How to build on Linux

These are the instructions for Debian 13. 
* For other Linux distributions , you may need to install different packages.


```aiignore
# Install dependencies
apt install build-essential libboost-all-dev cmake g++ libcurl4-openssl-dev
# Install git
apt install git
# Clone the repository
git clone https://github.com/openeggbert/mind-net/
# Init git submodules
git submodule update --init --recursive
# Go to the repository
cd mind-net
# Enable FTS5 feature of SQLite : edit third_party/sqlite/CMakeLists.txt
# - add target_compile_definitions(sqlite3 PUBLIC SQLITE_ENABLE_FTS5) to 
# Switch to the develop branch
git checkout develop
# Create build directory
mkdir build
# Go to build directory
cd build
# Run cmake
cmake -B . -S ..
# Build
cmake --build .
# Run the application
./mind_net
```

## Entities

## TODO

### Crow web server producing REST API and serving pure html files

### Support for export to static HTML files

### Replace std::cout by custom loggers
