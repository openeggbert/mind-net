# 👩‍💻 Hive Developer Guide

This document is the technical reference for Hive internals.

- For setup and user-level usage, see [README.md](README.md).
- For contribution rules, see [CONTRIBUTING.md](CONTRIBUTING.md).

---

## 1) Architecture Deep Dive

Hive is a modular C++ system with clear boundaries between metadata, transport, orchestration, persistence, and domain extensions.

### Module boundaries and responsibilities

| Module | Responsibility |
|---|---|
| `src/hive-app` | Process startup, command parsing, plugin factory registration, migration bootstrap, HTTP server start |
| `src/hive-http` | Endpoint generators, routing glue, web/static serving, auth and metadata endpoints |
| `src/hive-essential` | Service orchestration: CRUD flow, validators, triggers, jobs, query dispatch |
| `src/hive-model` | Core metadata types (`ModelDefinition`) and model contracts |
| `src/hive-orm` | ORM entities + column constants (`BaseColumns`-style definitions) |
| `src/hive-db-sqlite` | SQLite persistence, repositories, migration executor, cache-backed data access |
| `src/hive-plugin-*` | Feature domains packaged as plugins (models + behavior + migrations + automation) |
| `frontend/` | Metadata-driven CRUD UI (vanilla JS), auth/session handling, API client |

### Layer interaction

```text
HTTP (Crow)
  -> Endpoint generators
  -> Service orchestration
      -> validators / triggers / query dispatch
      -> persistence repositories
          -> SQLite
```

The architectural intent is to keep policy and domain behavior in extension points (plugins, validators, triggers, jobs), not in ad-hoc controller code.

---

## 2) ModelDefinition System

`ModelDefinition` is the core architectural contract.

### Why it exists

Traditional CRUD systems duplicate schema intent across:

- controller routes
- DTO validation
- persistence mapping
- frontend form/table definitions

Hive centralizes that intent in model metadata, reducing divergence across layers.

### What it describes

For each model, metadata includes:

- model identity and plugin ownership
- column definitions and visibility/behavior flags
- allowed REST operations
- UI-relevant metadata (`title_column`, grouping)
- optional custom actions

### How it drives the system

- **DB/persistence:** repository selection and field-level handling align to model metadata.
- **API:** model routes are generated under `/api/v1/<model>`.
- **UI:** frontend reads `/api/v1/model_definition` and renders generic CRUD views.

This is the central reason Hive can onboard new entities quickly with consistent behavior.

---

## 3) BaseModel and ORM Layer

### BaseModel purpose

`BaseModel` and concrete model classes provide typed access to entity fields and serialization boundaries between:

- C++ domain objects
- JSON payloads
- DB records (`entity_fields`)

### ORM/column abstractions

Column-definition types (`BaseColumns` pattern and per-model column structs) centralize naming and constraints, reducing stringly-typed SQL/application mismatches.

### Design intent

- Keep model contracts explicit and reusable.
- Keep transport and persistence concerns interoperable but separated.
- Enable generic service logic over heterogeneous domain models.

---

## 4) Request Lifecycle

End-to-end flow for generated model endpoints:

1. Request enters Crow route (generated from `ModelDefinition`).
2. Auth context is resolved (bearer access token verification where required).
3. Endpoint validates request shape and operation-level constraints.
4. Service selects target model behavior from metadata.
5. Validators execute (authorization/business rules).
6. Trigger pipeline executes around operation:
   - `Before`
   - `InsteadOf` (optional override)
   - `After`
7. Persistence/repository layer performs DB operation.
8. Cache invalidation/update is applied for affected model lists/records.
9. Response is serialized to JSON and returned.

For auth endpoints, lifecycle includes token/session persistence and audit logging models.

---

## 5) Plugin System (Deep Dive)

Plugins are first-class extension units, not passive config files.

### Plugin lifecycle

1. Plugin factories are registered at startup.
2. `PluginRegistry` registers plugin instances.
3. Dependencies are sorted topologically (`get_plugin_names_sorted_by_dependencies`).
4. Migrations are applied in dependency order.
5. Service registries are built from plugin contributions.
6. Model endpoints are generated for plugin model definitions.

### Extension points a plugin can provide

- model registrations (`ModelDefinition` + model types)
- SQL migrations
- custom SQL queries
- validators
- triggers
- jobs (scheduler-integrated)

### Why this is powerful

A single plugin can ship its own:

- schema
- data lifecycle rules
- automation
- API surface (via generated endpoints)
- frontend visibility (via model metadata)

This enables coherent vertical slices instead of cross-cutting changes across many core files.

### Core vs non-core plugins

- **Core plugin:** platform-critical auth, session/token, logging, error, and job models + maintenance behavior.
- **Domain plugins:**
  - `slipbox` (knowledge graph/content)
  - `repetition` (spaced repetition domain)
  - `dictionary` (lexicon/search domain)

Domain plugins are best treated as extension examples for your own domain plugin design.

### Plugin best practices

- Keep plugin scope cohesive (one domain, clear boundaries).
- Avoid direct coupling to unrelated plugin internals.
- Declare dependencies explicitly.
- Ship migrations with plugin code changes.
- Put cross-entity policy in validators/triggers, not endpoint forks.

---

## 6) Creating your first plugin (practical)

This section is a practical, minimal path to adding a production-safe plugin.

### Step 1: Create plugin module skeleton

Create a new module folder following existing naming:

```text
src/hive-plugin-<your-domain>/
├── CMakeLists.txt
├── ... plugin source files ...
└── ... headers ...
```

Use existing plugin modules (`dictionary`, `slip-box`, `repetition`) as templates for structure and registration style.

### Step 2: Define plugin scope and dependencies

- Keep one clear domain boundary per plugin.
- Declare required plugin dependencies explicitly (for startup ordering and migration ordering).
- Avoid coupling to unrelated plugin internals.

### Step 3: Register plugin contributions

Implement plugin factory/registration so the plugin can contribute:

- model definitions (`ModelDefinition` + model types)
- migrations
- optional custom queries
- optional validators
- optional triggers
- optional scheduled jobs

If you can avoid custom behavior at first, start with models + migrations only, then add validators/triggers/jobs iteratively.

### Step 4: Wire build and startup

- Add the plugin subdirectory in top-level `CMakeLists.txt`.
- Link plugin library into `hive_app` in `src/hive-app/CMakeLists.txt` (typically behind a feature flag).
- Register plugin factory in app startup composition (`Main.cpp`) with existing plugin registration pattern.

### Step 5: Verify end-to-end behavior

After startup, verify in this order:

1. Plugin migrations are applied successfully.
2. `/api/v1/model_definition` includes new models.
3. Generated `/api/v1/<model>` endpoints are accessible.
4. Frontend renders CRUD screens for new models.
5. If present, plugin validators/triggers/jobs behave as expected.

### Step 6: Hardening checklist before merge

- Metadata complete (`title_column`, operation config, grouping).
- Migrations incremental and reversible in practice (rollback-safe behavior).
- No cross-plugin implicit assumptions.
- Operational logs remain readable for plugin actions.

---

## 7) Triggers and Validators

### Validators

Validators enforce operation rules before persistence, typically for:

- authorization checks
- domain invariants
- payload consistency

### Trigger phases

- **`Before`**: run before standard CRUD behavior.
- **`InsteadOf`**: replace standard behavior when custom execution is required.
- **`After`**: run after operation completion.

### Typical trigger use cases

- denormalized view maintenance
- history/audit side effects
- fulltext/search index updates
- cross-model synchronization logic

### Practical guidance

- Prefer validators for “allow/deny + invariant” logic.
- Prefer triggers for side effects and orchestration around persistence.
- Use `InsteadOf` sparingly and document expected behavior clearly.

---

## 8) ORM / Database / Migrations

### Persistence model

Hive uses repository-based persistence over SQLite, aligned with model metadata.

Key characteristics:

- model-scoped CRUD/list behavior
- SQL query abstraction with plugin-provided custom queries
- cache-aware read/list operations with invalidation on writes

### Migration model

Migrations are applied at startup with integrity controls:

- schema metadata/history checks
- checksum + chain-hash verification
- transactional execution + rollback on failure

This design favors operational safety and deterministic schema evolution.

### Table design guidance

When introducing new tables, define explicitly:

- primary key strategy
- nullability and defaults
- unique constraints
- foreign keys and indexes
- check constraints where invariants are critical

Use incremental migrations; do not rewrite already-applied migrations.

---

## 9) Automation System (Jobs + Scheduler)

Automation is a first-class backend feature.

### Components

- plugin-defined jobs
- persisted job definitions (`job_entry`)
- persisted run telemetry (`job_run`)
- cron-based scheduling engine

### Runtime behavior

- Job definitions are synchronized into DB.
- Cron expressions are parsed and scheduled at startup.
- Execution runs via scheduler + worker threads.
- Run metadata is persisted (`last_run`, `next_run`, status).

### Usage patterns

- maintenance/cleanup tasks
- batch recomputation jobs
- periodic exports or consistency checks

---

## 10) Frontend Integration (Metadata-Driven UI)

The frontend is intentionally lightweight and generic.

### How backend drives frontend

1. Frontend loads `/api/v1/model_definition`.
2. Builds navigation and CRUD views from model groups/metadata.
3. Uses generated model endpoints for operations.
4. Resolves foreign key labels via cached lookups.
5. Applies auth tokens and refresh flow in API client.

This keeps frontend feature growth mostly metadata-driven rather than hand-coded per entity.

---

## 11) Developer Workflow

### A) Add a new model to an existing plugin

1. Define columns/constants in plugin ORM/model area.
2. Implement model type (`BaseModel`-based).
3. Register model definition with correct metadata:
   - allowed operations
   - `title_column`
   - grouping/custom actions (if needed)
4. Add migration(s) for schema changes.
5. Register validators/triggers if domain requires them.
6. Start app and verify:
   - `/api/v1/model_definition`
   - generated `/api/v1/<model>` endpoints
   - frontend CRUD visibility

### B) Add a new plugin

1. Create plugin module under `src/hive-plugin-<name>/`.
2. Implement plugin factory and registrations.
3. Declare dependencies on required plugins.
4. Add plugin migrations and domain models.
5. Register optional queries/validators/triggers/jobs.
6. Wire plugin into CMake and startup registration.
7. Validate dependency sorting + migration application at startup.

### C) Debugging checklist

- `GET /health` for runtime liveness
- `GET /info` for build/runtime metadata
- `GET /api/v1/model_definition` for metadata contract sanity
- auth/token tables for session/token lifecycle issues
- `job_entry`/`job_run` for scheduler visibility
- logs/models (`api_log`, `auth_log`, `error`, `history`) for operational traces

### D) Safe extension checklist

- Keep model metadata complete and explicit.
- Use validators/triggers instead of endpoint copy-paste.
- Keep plugin boundaries clear.
- Ship migration + model + behavior together.
- Verify UI metadata impact (`title_column`, grouping, visibility).

---

## 12) Design tradeoffs

- **Metadata-driven generation vs explicit route code:** less CRUD boilerplate and stronger consistency, but metadata correctness becomes a critical engineering discipline.
- **Plugin modularity vs startup complexity:** domain isolation improves maintainability, but dependency ordering and migration orchestration must be handled carefully.
- **Generic service pipeline vs ad-hoc custom handlers:** easier long-term maintenance and predictable behavior, but advanced cases should be modeled through triggers/validators/jobs instead of quick endpoint forks.
- **SQLite-first approach vs multi-DB abstraction complexity:** excellent operational simplicity and portability now, with future multi-database support requiring careful abstraction boundaries.

---

## 13) Design Principles

- **Metadata over duplication:** one source of truth across API/UI/persistence behavior.
- **Extension points over forks:** validators/triggers/jobs/queries instead of patching core flow.
- **Operational correctness:** migrations and auth/session handling are treated as platform concerns.
- **Pragmatic stack:** minimal dependencies with strong architectural control in C++.

---

## 14) References

- [README.md](README.md)
- [CONTRIBUTING.md](CONTRIBUTING.md)
- [TODO.md](TODO.md)