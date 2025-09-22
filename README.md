# 🧠 Mind-Net

![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)
![C++23](https://img.shields.io/badge/C%2B%2B-23-blue)

**Mind-Net** is an open-source software for **note-taking, knowledge mapping, and Zettelkasten-style linking**.  
Its goal is to provide a **fast, extensible, and sustainable** tool for everyday knowledge management.

With its flexible design, Mind-Net can be extended beyond note-taking—adapting to your personal workflows, research, or entirely new use cases.

## ✨ Features

- 📌 Manage notes, maps, and links (`note`, `map`, `node_link`, …)  
- 🔗 Automatic creation and linking of entities  
- 🧩 Modular architecture – new entities are defined via **ModelDefinition**  
- 🔄 Auto-generated REST API (`/api/v1/...`) from model definitions  
- 🌐 Web frontend (vanilla JS + HTML + CSS) with CRUD operations  
- 🕵️ Audit logging and API request logging  
- ⚡ Modern **C++20/23 backend** (no heavy frameworks)  
- 🗃️ SQLite database with automatic migrations  

## Components:

 1. C++ Crow backend
 2. Pure HTML, JavaScript, CSS frontend

## 📦 Installation & Usage

### 1. Requirements
- **C++23 compiler** (tested on GCC 14)  
- **CMake** 
- **SQLite3** 
- **Crow**

### 2. Build the backend

These are the instructions for Debian 13.
 * Other operating systems may have different instructions

```bash
# Install dependencies
apt install build-essential libboost-all-dev cmake g++ libcurl4-openssl-dev

# Install git
apt install git

# Clone the repository
git clone https://github.com/openeggbert/mind-net.git/

# Init git submodules
git submodule update --init --recursive

# Go to the repository
cd mind-net

# Enable FTS5 feature of SQLite : edit third_party/sqlite/CMakeLists.txt
# Add target_compile_definitions(sqlite3 PUBLIC SQLITE_ENABLE_FTS5) 
#to: 

$<INSTALL_INTERFACE:include/>)
target_compile_definitions(sqlite3 PUBLIC SQLITE_ENABLE_FTS5)
if (SQLITE_ENABLE_COLUMN_METADATA)

# Switch to the develop branch
git checkout develop

# Create build directory
mkdir build

# Go to build directory
cd build

# Run cmake
cmake -B . -S ..

# Build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=TRUE ..
cmake --build . --config Release -j$(nproc)
cd src/mindnet/app
strip --strip-all mind_net_app
upx --best --lzma mind_net_app

#Generate JWT Secret
openssl rand -base64 32

#Configure values in mindnet.properties
```

### 3. Run the server

```bash
./mind-net  start --port 8888 -s /home/johndoe/Desktop/mindnet/frontend
```

The backend runs at `http://localhost:8888`.

### 4. Frontend

Simply open:

```
http://localhost:8888/web/
```

The frontend is plain JavaScript – no build step required.

## 🔌 REST API

* Versioned under: `/api/v1/...`
* Endpoints are generated automatically based on **ModelDefinition**.
* Special endpoints:

  * `/api/v1/model_definition` – JSON description of all models/entities
  * `/health` – application health check
  * `/info` – build information / metadata

## 🗂️ Project Structure

```
mind-net/
├── backend/                # C++ server (Crow + custom framework)
│   ├── model/              # BaseModel, BaseColumns, ModelDefinition
│   ├── migrations/         # SQL migrations
│   └── plugins/            # Plugins (core, note, map, ...)
├── frontend/               # Web UI (vanilla JS, HTML, CSS)
│   ├── crud.js             # CRUD logic
│   ├── navigation.js       # Navigation
│   └── explore.js          # Map visualization (vis.js)
├── CMakeLists.txt          # Build configuration
└── LICENSE                 # GPLv3
```

## 🤝 Contributing

See [CONTRIBUTING.md](./CONTRIBUTING.md) for guidelines and [DEVELOPERS.md](./DEVELOPERS.md) for internal architecture.

## 📜 License

This project is licensed under **GNU GPL v3**.
See [LICENSE](LICENSE) for details.

## 🌟 Project Goals

* **Sustainability** – easy to extend with new entities and features.
* **Performance** – lightweight C++ backend, simple frontend.
* **Longevity** – designed as a **knowledge management system**.

## Screenshots

### CRUD operations

#### List nodes
![List nodes](screenshots/screen_list_nodes.jpg "List nodes")

#### Read node

![Read node](screenshots/screen_read_node.jpg "Read node")

#### Create node

![Create node](screenshots/screen_create_node.jpg "Create node")

#### Delete node

![Delete node](screenshots/screen_delete_node.jpg "Delete node")

#### List properties

![List properties](screenshots/screen_list_properties.jpg "List properties")

### Graph exploration

![Graph demo](screenshots/screen_graph_demo.jpg "Graph demo")

### API demo

![Calling get list for node](screenshots/screen_calling_get_list_for_node.jpg "Calling get list for node")



