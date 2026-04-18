# 🧠 Mind-Net

![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)
![C++23](https://img.shields.io/badge/C%2B%2B-23-blue)

Mind-Net is a modular **C++23 universal data server** with a metadata-driven backend, auto-generated REST API, automation engine (jobs + triggers), and schema-driven web frontend.

It is designed for teams and individuals who need more than CRUD: a system that evolves through plugins, model metadata, and domain-specific automation.

Mind-Net demonstrates system-level C++ engineering: custom model abstractions, runtime extensibility, migration integrity, and API/UI generation from a shared metadata contract.

## 🚀 Overview

Mind-Net combines:

- a high-performance C++ backend (Crow + SQLite)
- a custom model/ORM stack (`ModelDefinition`, `BaseModel`, column metadata)
- automatic API generation under `/api/v1/*`
- token-based authentication and audit-capable core models
- scheduler + trigger pipelines for background and event-driven behavior
- dynamic frontend generation from backend model metadata

### Why this project stands out

- **Architecture-first design:** model metadata drives storage, API, validation, triggers, and UI.
- **Extensibility by construction:** plugins contribute models, queries, validators, triggers, jobs, and migrations.
- **Production-oriented data layer:** migration integrity checks (checksums/chain hashes), transactional execution, and caching controls.
- **Serious systems engineering:** not just a UI app, but a composable backend platform (~44k LOC).

For implementation internals and extension mechanics, see [DEVELOPERS.md](./DEVELOPERS.md).

## 🎯 When to use Mind-Net

Mind-Net is a strong fit when you need:

- a domain platform with many entities that should expose consistent CRUD APIs quickly
- backend-driven UI generation to minimize repetitive frontend form/table code
- plugin-level extensibility for independent feature domains
- built-in automation hooks (triggers + scheduled jobs)
- auditable operations and explicit migration control in SQLite-backed deployments

It is especially suitable for knowledge systems, structured content domains, internal tools, and extensible data-heavy backends.

## ✨ Key Features

### Core platform

- 🔄 **Auto-generated REST API** from model definitions (`/api/v1/<model>` CRUD/list routes)
- 🧩 **Plugin-based modular architecture** with dependency-aware startup and migrations
- 🧠 **Custom model/ORM layer** with strongly-typed entities and centralized column metadata
- 🗃️ **SQLite persistence** with repository abstraction and model-scoped caching
- 🧪 **Validation + trigger pipeline** (`Before`, `After`, `InsteadOf`) per CRUD operation
- ⏱️ **Cron scheduler** with job registry sync and persisted run history

### Security and operations

- 🔐 **Authentication lifecycle endpoints**: login, logout, refresh token, register, change password
- 🪪 **Hashed token storage** with revocation/expiration checks and session tracking
- 📜 **Operational logging models** for auth/API/superadmin/error flows
- ❤️ **Health and info endpoints**: `/health`, `/info`

### Frontend

- 🌐 **Vanilla JS frontend** (no frontend build toolchain required)
- 🧱 **Schema-driven CRUD UI** rendered from `/api/v1/model_definition`
- 🔁 **Client-side token refresh + retry** for authenticated API operations

### Extensibility in practice

- 🧩 Plugins register **models + migrations + validators + triggers + jobs + queries**
- 🔌 Domain plugins (Slipbox, Repetition, Dictionary) share one core runtime and extension model
- 🛠 New model definitions automatically surface in API metadata and generic UI screens

## 🖼️ Screenshots

| Area | Screenshot |
|---|---|
| CRUD: list nodes | ![List nodes](screenshots/screen_list_nodes.jpg "List nodes") |
| CRUD: read node | ![Read node](screenshots/screen_read_node.jpg "Read node") |
| CRUD: create node | ![Create node](screenshots/screen_create_node.jpg "Create node") |
| CRUD: delete node | ![Delete node](screenshots/screen_delete_node.jpg "Delete node") |
| CRUD: list properties | ![List properties](screenshots/screen_list_properties.jpg "List properties") |
| Graph exploration | ![Graph demo](screenshots/screen_graph_demo.jpg "Graph demo") |
| API call example | ![Calling get list for node](screenshots/screen_calling_get_list_for_node.jpg "Calling get list for node") |

## 🏗️ Architecture

### High-level modules

- `src/mind-net-app` — application bootstrap, plugin loading, command handling
- `src/mind-net-http` — endpoint generators, HTTP server integration
- `src/mind-net-essential` — service orchestration (CRUD, triggers, validators, jobs, queries)
- `src/mind-net-model` — model definitions and base model abstractions
- `src/mind-net-orm` — ORM entities and column constants
- `src/mind-net-db-sqlite` — repositories, queries, migrations, SQLite integration
- `src/mind-net-plugin-*` — domain extensions (`core`, `dictionary`, `slipbox`, `repetition`)
- `frontend/` — dynamic web UI (vanilla JS)

### Request/data flow (at runtime)

```text
Client (Web UI / API consumer)
        |
        v
Crow route (generated from ModelDefinition)
        |
        v
Service layer
  - auth context
  - validators
  - triggers (Before/InsteadOf/After)
        |
        v
Persistence/Repository
        |
        v
SQLite (+ migrations + cache invalidation)
```

### Startup flow

1. Load plugin factories.
2. Resolve plugin dependencies.
3. Apply plugin migrations in dependency order.
4. Build service registries (models, validators, triggers, jobs, queries).
5. Generate model endpoints and start HTTP server.

### Why this scales

- New entities are defined once in metadata and reused across layers.
- Domain behavior stays local to plugins rather than leaking into core service code.
- Consistent CRUD semantics reduce implementation variance and maintenance cost.

## 🔌 Plugin System

Plugins are first-class extension units. A plugin can register:

- model definitions
- custom SQL queries
- validators
- triggers
- jobs
- migrations

This is stronger than typical “plugin-like” feature flags: each plugin can ship schema, behavior, API-level validation logic, and automation as one coherent unit.

### Core plugin (foundational)

The `core` plugin provides platform-critical auth/session, logging/audit, error, and automation foundations used by all domain plugins.

### Domain plugins

- **Slipbox** — knowledge management (notes/maps/links/tags/content), rich triggers, custom queries, HTML export job
- **Repetition** — spaced repetition domain (sessions/reviews/predictions), scheduling triggers, custom selection query
- **Dictionary** — lexicon/dictionary domain (terms/aliases/sources/metrics), search/fulltext triggers, export and query support

## 🔌 API Overview

- Base path: `/api/v1/`
- Metadata endpoint: `/api/v1/model_definition`
- Auth endpoints: `/api/v1/auth/*` (`login`, `logout`, `refresh_token`, `register`, `change_password`)
- Generated model endpoints: `/api/v1/<model>`
  - operations include create/read/update/delete/list (subject to model configuration)

The metadata endpoint is the contract between backend and dynamic frontend.

## 🧰 Technology Stack

- **Language:** C++23
- **HTTP:** Crow
- **Database:** SQLite (via SQLiteCpp + custom DB layer)
- **JSON:** `nlohmann/json`
- **Build:** CMake
- **Testing:** GoogleTest / GoogleMock
- **Frontend:** HTML + CSS + Vanilla JavaScript

## ⚡ Getting Started

### 1) Prerequisites

- C++23-capable compiler (GCC 14 tested)
- CMake
- Git
- Linux build essentials

### 2) Clone and prepare

```bash
git clone https://github.com/openeggbert/mind-net.git
cd mind-net
git submodule update --init --recursive
```

### 3) Build

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --target mind_net_app
```

### 4) Run

```bash
./build/src/mind-net-app/mind_net_app start \
  --port 9000 \
  --frontend-port 9000 \
  --host http://localhost \
  -s ./frontend
```

Then open:

- Web UI: `http://localhost:9000/web/`
- API base: `http://localhost:9000/api/v1/`

### Initial administrator login

Default administrator credentials are written to `pw.txt` on first bootstrap.

1. Save credentials securely.
2. Delete `pw.txt`.
3. Change the administrator password immediately.

## 🧭 Developer Entry Points

- Start with [DEVELOPERS.md](./DEVELOPERS.md) for architecture deep dive and extension workflow.
- Inspect `src/mind-net-app/mindnet/app/Main.cpp` for startup composition.
- Inspect `src/mind-net-http/` for endpoint generators.
- Inspect `src/mind-net-essential/` for service orchestration, validators, triggers, and jobs.
- Inspect plugin factories under `src/mind-net-plugin-*/` to see real extension examples.
- Follow the practical first-plugin walkthrough in [DEVELOPERS.md](./DEVELOPERS.md#6-creating-your-first-plugin-practical).

## 📖 Usage

### Common user workflows

- Browse available apps from the frontend landing page.
- Use dynamic CRUD views generated from backend model definitions.
- Search, navigate, and manage domain data through plugin-specific entities.

### API workflows

- Read schema metadata from `/api/v1/model_definition`.
- Authenticate via `/api/v1/auth/login` and use bearer tokens.
- Use generated endpoints `/api/v1/<model>` for create/read/update/delete/list operations.

### Automation workflows

- Register plugin jobs with cron expressions.
- Track execution using persisted `job_entry` and `job_run` data.
- Add triggers to react to CRUD events (`Before`, `After`, `InsteadOf`).

## 🧠 Why Mind-Net is interesting (Engineering Perspective)

- **Metadata-driven architecture:** one model definition influences API, persistence behavior, and dynamic UI.
- **Extensible runtime:** plugins package schema + behavior + automation instead of isolated DTOs.
- **Operational discipline:** migration integrity checks and explicit startup composition support maintainability at scale.
- **Systems-level C++ design:** clear boundaries between HTTP, service orchestration, model/ORM, and persistence layers.

For deep technical internals, see [DEVELOPERS.md](./DEVELOPERS.md).

## ⚖️ Design tradeoffs

- **Metadata-driven generation vs explicit hand-written control:** much less boilerplate, but metadata quality and conventions become critical.
- **Generic CRUD pipeline vs fully custom endpoints:** consistency and speed for most entities, while advanced behavior should be implemented via validators/triggers/jobs.
- **Plugin modularity vs runtime composition complexity:** extensibility improves, but startup/dependency/migration ordering must be managed carefully.

## 🗂️ Project Structure

```text
mind-net/
├── CMakeLists.txt
├── include/
│   ├── mind-net-model/
│   ├── mind-net-orm/
│   ├── mind-net-http/
│   └── mind-net-api/
├── src/
│   ├── mind-net-app/
│   ├── mind-net-essential/
│   ├── mind-net-db-sqlite/
│   ├── mind-net-http/
│   ├── mind-net-model/
│   ├── mind-net-orm/
│   ├── mind-net-plugin-dictionary/
│   ├── mind-net-plugin-slip-box/
│   └── mind-net-plugin-repetition/
├── frontend/
├── screenshots/
├── tests/
└── TODO.md
```

## 🛣️ Roadmap

Planned/active directions reflected in project TODOs include:

- C++20/23 module migration
- wider database abstraction and multi-DB support
- trigger/constraint enhancements
- frontend cleanup and modernization

## 🤝 Contributing

See [CONTRIBUTING.md](./CONTRIBUTING.md) and [DEVELOPERS.md](./DEVELOPERS.md).

## 📜 License

This project is licensed under **MIT**. See [LICENSE](LICENSE).

