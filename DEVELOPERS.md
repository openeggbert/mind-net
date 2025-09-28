# 👩‍💻 Mind-Net Developer Guide

This document explains the **internal architecture** of Mind-Net and gives practical guidelines for developers who want to extend or modify the system.
If you’re looking for installation and usage instructions, see [README.md](README.md).

---

## 🏗️ Core Concepts

### 1. ModelDefinition

At the heart of Mind-Net is the **`ModelDefinition`** system.
Every entity (e.g. `note`, `map`, `node_link`, `api_log`) is described by a `ModelDefinition`:

* Defines the **columns** (`BaseColumns` + `coldef` flags).
* Specifies **REST operations** (`crudl`: create, read, update, delete, list).
* Provides metadata such as:

    * `title_column` → which column is shown in UI / foreign key labels
    * `group` → used for organizing entities in UI
    * `customActions` → extra endpoints/actions beyond CRUD

From this single definition, the backend generates:

* SQL migrations
* REST endpoints (`/api/v1/<entity>`)
* JSON schema (`/api/v1/model_definition`)
* Frontend CRUD UI (auto-rendered tables, forms, and detail pages)

This design means: **defining an entity is usually enough** — the rest (API + UI) comes for free.

---

### 2. Backend Flow

1. **Request arrives** at Crow (`crow::request`).
2. **Router dispatches** to generic handlers (`list_model`, `read_model`, etc.).
3. **ModelDefinition** provides schema & metadata for the entity.
4. **SQL is generated** dynamically (CRUD queries, filtering, ordering, pagination).
5. **Result is serialized** into JSON and returned to the frontend.

Additional features:

* **Migrations**: SQL scripts under `backend/migrations/` are applied at startup.
* **Audit logging**: writes into `history` / `api_log` tables automatically.
* **Validators**: per-entity checks for authorization / business logic.

---

### 3. Frontend Flow

The frontend is **vanilla JS**, no build step. Its logic is generic:

1. On startup, it loads `/api/v1/model_definition`.
2. Renders **navigation** and **CRUD pages** based on entity definitions.
3. CRUD operations are thin wrappers around REST API calls.
4. **Foreign keys** are resolved via cached lookups (`resolveForeignKeyValue`).
5. Extra **custom actions** (if defined in `ModelDefinition`) appear as buttons.

So when you add a new entity, the frontend automatically supports it.

---

### 4. Database & Migrations

* Backend ships with **embedded migrations** (`add_migration("Vxx__name.sql", R"(...)")`).
* On startup, it checks applied migrations in the `schema_version` table.
* Applies pending migrations in order.
* This means schema is always kept in sync with the code.

Tables usually include:

* `id` (autoincrement)
* `created_at`, `updated_at` (filled automatically)
* Domain-specific columns
* Optional: `FOREIGN KEY` constraints

---

## 🧩 Extending Mind-Net

### Adding a New Entity

1. **Define Columns**

    * Create `.../columns/MyEntityColumns.h`
    * Use `static constexpr const char*` for column names.

2. **Define Model**

    * Create `.../models/MyEntity.h`
    * Inherit from `BaseModel`
    * Declare fields
    * Add operator== if needed

3. **Register in ModelDefinition**

    * Add `def(...)` with proper metadata
    * Set `title_column` for display in UI

4. **Add Migration**

    * Add a `Vxx__create_my_entity.sql` migration in `backend/migrations/`

5. **Test**

    * Run backend
    * Check `/api/v1/model_definition` to see your entity
    * Open frontend → CRUD UI should appear automatically

---

### Custom Actions

* Extend entities with extra actions (e.g. “Explore map”, “Export note”).
* Define in `ModelDefinition` with `.set_custom_actions(...)`.
* They will appear as buttons in the frontend automatically.

---

### Logging

* `api_log`: Stores every API request (method, endpoint, parameters, status).
* `history`: Tracks changes to entities (with triggers).
* Useful for audit trails and debugging.

---

### Authentication & Security (planned / partial)

* JWT-based login/logout planned via `/login` `/refresh-token` `/logout`.
* Authorization checks are implemented via **validators** inside the model layer.
* Roadmap includes per-operation permissions and session handling.

---

## 🔍 Key Design Choices

* **Monolithic, plugin-based**:
  All models live under `plugins/`. It’s modular in code, but still a single binary.

* **Convention over configuration**:
  If you follow naming conventions (`*_id`, `*_at`, etc.), you get automatic features.

* **Minimal dependencies**:
  No heavy ORM or web framework — only Crow + SQLite.

* **Frontend without build step**:
  Plain JS, works everywhere, no npm/webpack required.

---

## 🧭 Developer Tips

* Always set `title_column` in `ModelDefinition` for better UX.
* Use `sibling_order` with gaps (e.g. 0, 100, 200) to allow insertions.
* Keep migrations incremental (`Vxx__...sql`), never edit old ones.
* When adding enums, prefer PascalCase (e.g. `HttpMethod::Post`).
* Use `resolveForeignKeyValue` + cache for foreign key display names.
* Run `GET /info` and `GET /health` to debug builds and runtime status.

---

## 🚀 Contribution Workflow (Developers)

1. Pick an issue or feature from `CONTRIBUTING.md`.
2. Implement using the extension flow above.
3. Add/modify migrations.
4. Test both backend and frontend.
5. Open PR → base branch: `develop`.

---

## Development Guidelines

### SQL tables

Questions:
* Column Type?
* NOT NULL?
* UNIQUE?
* DEFAULT {}?
* CHECK?

```aiignore
CREATE TABLE {table name} (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

	{COLUMNS}

    {CHECK}
    
    {UNIQUE}
    
    {FOREIGN KEYS}
);

CREATE INDEX idx_{table name}_{column name} ON {table name}({column name});
```