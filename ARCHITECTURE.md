# Hive Architecture

Hive is built around one central idea: **ModelDefinition is the shared contract.**

A plugin defines models and behavior once. Hive then uses the same metadata to generate REST endpoints, drive persistence behavior, run validation and trigger pipelines, and render generic frontend screens.

Instead of writing separate controllers, DTO mappings, CRUD pages and database glue for every entity, Hive pushes the common behavior into reusable infrastructure and lets plugins provide domain-specific models, validators, triggers, jobs, migrations and queries.

---

## High-Level Architecture

```text
+--------------------------------------------------------------------------------+
|                                USER / CLIENT                                   |
|--------------------------------------------------------------------------------|
|  Web browser                                                                   |
|  API client                                                                    |
|  Admin / developer                                                             |
+----------------------------------------+---------------------------------------+
                                         |
                                         | HTTP / JSON
                                         v
+--------------------------------------------------------------------------------+
|                                FRONTEND LAYER                                  |
|--------------------------------------------------------------------------------|
|  frontend/                                                                     |
|                                                                                |
|  Vanilla JavaScript UI                                                         |
|  - API client                                                                  |
|  - authentication handling                                                     |
|  - token refresh and retry                                                     |
|  - schema-driven CRUD screens                                                  |
|  - domain-specific screens                                                     |
|                                                                                |
|  Reads model metadata from:                                                    |
|  /api/v1/model_definition                                                      |
+----------------------------------------+---------------------------------------+
                                         |
                                         | REST calls
                                         v
+--------------------------------------------------------------------------------+
|                              HTTP / TRANSPORT LAYER                            |
|--------------------------------------------------------------------------------|
|  src/hive-http                                                                 |
|                                                                                |
|  Crow-based HTTP server integration                                            |
|                                                                                |
|  Endpoint generators:                                                          |
|  - WebEndpointsGenerator                                                       |
|  - AuthEndpointsGenerator                                                      |
|  - ModelDefinitionEndpointsGenerator                                           |
|  - ModelEndpointGenerator                                                      |
|  - InfoHealthEndpointsGenerator                                                |
|  - SuperAdminEndpointsGenerator                                                |
|                                                                                |
|  Main generated routes:                                                        |
|  - /web/*                                                                      |
|  - /health                                                                     |
|  - /info                                                                       |
|  - /api/v1/auth/*                                                              |
|  - /api/v1/model_definition                                                    |
|  - /api/v1/<model>                                                             |
+----------------------------------------+---------------------------------------+
                                         |
                                         | request dispatch
                                         v
+--------------------------------------------------------------------------------+
|                           SERVICE ORCHESTRATION LAYER                          |
|--------------------------------------------------------------------------------|
|  src/hive-essential                                                            |
|                                                                                |
|  Central runtime flow for generated CRUD operations:                           |
|                                                                                |
|  1. Resolve target model                                                       |
|  2. Resolve authentication context                                             |
|  3. Validate requested operation                                               |
|  4. Run validators                                                             |
|  5. Run trigger pipeline                                                       |
|  6. Call repository                                                            |
|  7. Update / invalidate cache                                                  |
|  8. Serialize response                                                         |
|                                                                                |
|  Extension points used here:                                                   |
|  - validators                                                                  |
|  - triggers                                                                    |
|  - jobs                                                                        |
|  - custom queries                                                              |
+----------------------+--------------------------+------------------------------+
                       |                          |
                       | uses                     | uses
                       v                          v
+-----------------------------------+     +---------------------------------------+
|          METADATA LAYER           |     |        PLUGIN / DOMAIN LAYER          |
|-----------------------------------|     |---------------------------------------|
|  src/hive-model                   |     |  src/hive-plugin-*                    |
|  src/hive-orm                     |     |                                       |
|                                   |     |  Enabled plugin domains:              |
|  Core contracts:                  |     |  - core                               |
|  - ModelDefinition                |     |  - dictionary                         |
|  - BaseModel                      |     |  - slipbox                            |
|  - column metadata                |     |  - repetition                         |
|  - operation metadata             |     |                                       |
|  - UI metadata                    |     |  Plugins can register:                |
|                                   |     |  - models                             |
|  Metadata drives:                 |     |  - migrations                         |
|  - REST API generation            |     |  - validators                         |
|  - persistence behavior           |     |  - triggers                           |
|  - validation rules               |     |  - jobs                               |
|  - frontend generation            |     |  - custom queries                     |
+----------------------+------------+     +------------------+--------------------+
                       |                                     |
                       | model contracts                     | domain behavior
                       +------------------+------------------+
                                          |
                                          v
+--------------------------------------------------------------------------------+
|                              PERSISTENCE LAYER                                 |
|--------------------------------------------------------------------------------|
|  src/hive-db-sqlite                                                            |
|                                                                                |
|  SQLite persistence implementation                                             |
|                                                                                |
|  Main responsibilities:                                                        |
|  - RepositoryImplSqlite                                                        |
|  - SqliteRepositoryFactory                                                     |
|  - SqliteDatabaseMigration                                                     |
|  - repository utilities                                                        |
|  - transactional CRUD operations                                               |
|  - migration execution                                                         |
|  - migration integrity                                                         |
|  - model-scoped caching                                                        |
+----------------------------------------+---------------------------------------+
                                         |
                                         | SQL
                                         v
+--------------------------------------------------------------------------------+
|                                  DATABASE                                      |
|--------------------------------------------------------------------------------|
|  SQLite                                                                        |
|                                                                                |
|  Stores:                                                                       |
|  - core auth/session/logging models                                            |
|  - plugin domain models                                                        |
|  - job registry and job runs                                                   |
|  - migration state                                                             |
|  - generated CRUD entity data                                                  |
+--------------------------------------------------------------------------------+
```

---

## Generated CRUD Request Flow

```text
Client
  |
  | POST /api/v1/<model>
  | GET  /api/v1/<model>
  | PUT  /api/v1/<model>/<id>
  | DELETE /api/v1/<model>/<id>
  v
+--------------------------------------------------------------------------------+
| ModelEndpointGenerator                                                         |
|--------------------------------------------------------------------------------|
| Receives HTTP request through generated Crow route                             |
| Parses path, method, query parameters and JSON body                            |
+----------------------------------------+---------------------------------------+
                                         |
                                         v
+--------------------------------------------------------------------------------+
| Authentication Context                                                         |
|--------------------------------------------------------------------------------|
| Reads bearer token if required                                                 |
| Resolves user/session context                                                  |
| Can write auth/API/superadmin/error logs                                       |
+----------------------------------------+---------------------------------------+
                                         |
                                         v
+--------------------------------------------------------------------------------+
| ModelDefinition Lookup                                                         |
|--------------------------------------------------------------------------------|
| Finds metadata for target model                                                |
| Checks plugin ownership                                                        |
| Checks enabled CRUD/list/read/create/update/delete operations                  |
| Provides column and UI metadata                                                |
+----------------------------------------+---------------------------------------+
                                         |
                                         v
+--------------------------------------------------------------------------------+
| Validation Pipeline                                                            |
|--------------------------------------------------------------------------------|
| Runs validators registered by core and domain plugins                          |
| Examples:                                                                      |
| - required fields                                                              |
| - business rules                                                               |
| - ownership checks                                                             |
| - model-specific constraints                                                   |
+----------------------------------------+---------------------------------------+
                                         |
                                         v
+--------------------------------------------------------------------------------+
| Trigger Pipeline                                                               |
|--------------------------------------------------------------------------------|
| Trigger phases:                                                                |
|                                                                                |
|  Before                                                                        |
|  - inspect request                                                             |
|  - modify data                                                                 |
|  - reject operation                                                            |
|                                                                                |
|  InsteadOf                                                                     |
|  - optionally replace default CRUD behavior                                    |
|  - useful for search, derived reads, special list operations                   |
|                                                                                |
|  After                                                                         |
|  - write history                                                               |
|  - update derived data                                                         |
|  - schedule follow-up behavior                                                 |
+----------------------------------------+---------------------------------------+
                                         |
                                         v
+--------------------------------------------------------------------------------+
| Repository Layer                                                               |
|--------------------------------------------------------------------------------|
| RepositoryImplSqlite                                                           |
| Converts model operation to SQLite operation                                   |
| Uses metadata and ORM/column definitions                                       |
| Executes SQL in controlled persistence layer                                   |
+----------------------------------------+---------------------------------------+
                                         |
                                         v
+--------------------------------------------------------------------------------+
| SQLite                                                                         |
|--------------------------------------------------------------------------------|
| Reads or writes persisted model data                                           |
| Stores core data, plugin data, jobs, logs and migration state                  |
+----------------------------------------+---------------------------------------+
                                         |
                                         v
+--------------------------------------------------------------------------------+
| Response Serialization                                                         |
|--------------------------------------------------------------------------------|
| Converts result to JSON                                                        |
| Applies model metadata conventions                                             |
| Returns HTTP response to client                                                |
+--------------------------------------------------------------------------------+
```

---

## Application Startup and Plugin Bootstrap

```text
hive_app
  |
  v
+--------------------------------------------------------------------------------+
| Application Bootstrap                                                          |
|--------------------------------------------------------------------------------|
| src/hive-app                                                                   |
|                                                                                |
| Responsibilities:                                                              |
| - parse command line                                                           |
| - load configuration                                                           |
| - create app services                                                          |
| - register plugin factories                                                    |
| - start migrations                                                             |
| - start HTTP server                                                            |
+----------------------------------------+---------------------------------------+
                                         |
                                         v
+--------------------------------------------------------------------------------+
| Plugin Factory Registration                                                    |
|--------------------------------------------------------------------------------|
| Plugin factories create plugin instances                                       |
|                                                                                |
| Typical plugins:                                                               |
| - CorePlugin                                                                   |
| - DictionaryPlugin                                                             |
| - SlipBoxPlugin                                                                |
| - RepetitionPlugin                                                             |
+----------------------------------------+---------------------------------------+
                                         |
                                         v
+--------------------------------------------------------------------------------+
| Dependency Resolution                                                          |
|--------------------------------------------------------------------------------|
| Startup orders plugins according to declared dependencies                      |
| Core plugin must be available before domain plugins that depend on it          |
+----------------------------------------+---------------------------------------+
                                         |
                                         v
+--------------------------------------------------------------------------------+
| Migration Bootstrap                                                            |
|--------------------------------------------------------------------------------|
| Each plugin can contribute migration scripts                                   |
|                                                                                |
| Migration system:                                                              |
| - applies migrations in dependency order                                       |
| - tracks migration state                                                       |
| - verifies migration integrity                                                 |
| - prepares tables for core and domain models                                   |
+----------------------------------------+---------------------------------------+
                                         |
                                         v
+--------------------------------------------------------------------------------+
| Runtime Registry Build                                                         |
|--------------------------------------------------------------------------------|
| Plugins register runtime extension points:                                     |
|                                                                                |
| - ModelDefinition entries                                                      |
| - validators                                                                   |
| - triggers                                                                     |
| - jobs                                                                         |
| - custom queries                                                               |
| - persistence methods                                                          |
+----------------------------------------+---------------------------------------+
                                         |
                                         v
+--------------------------------------------------------------------------------+
| HTTP Route Generation                                                          |
|--------------------------------------------------------------------------------|
| ModelEndpointGenerator creates routes from ModelDefinition                     |
| ModelDefinitionEndpointsGenerator exposes metadata to frontend                 |
| AuthEndpointsGenerator exposes authentication lifecycle                        |
| WebEndpointsGenerator serves frontend files                                    |
+----------------------------------------+---------------------------------------+
                                         |
                                         v
+--------------------------------------------------------------------------------+
| Running Server                                                                 |
|--------------------------------------------------------------------------------|
| Frontend and API clients can now use generated endpoints                       |
| Scheduler can run registered jobs                                              |
| Trigger and validator pipelines are active                                     |
+--------------------------------------------------------------------------------+
```

---

## Schema-Driven Frontend Flow

```text
Browser
  |
  v
+--------------------------------------------------------------------------------+
| frontend/                                                                      |
|--------------------------------------------------------------------------------|
| Vanilla JavaScript frontend                                                    |
| No frontend build chain required                                               |
+----------------------------------------+---------------------------------------+
                                         |
                                         | GET /api/v1/model_definition
                                         v
+--------------------------------------------------------------------------------+
| ModelDefinition Endpoint                                                       |
|--------------------------------------------------------------------------------|
| Returns metadata for available models                                          |
|                                                                                |
| Metadata includes:                                                             |
| - model name                                                                   |
| - plugin ownership                                                             |
| - columns                                                                      |
| - visible fields                                                               |
| - allowed operations                                                           |
| - title column                                                                 |
| - grouping / UI hints                                                          |
+----------------------------------------+---------------------------------------+
                                         |
                                         v
+--------------------------------------------------------------------------------+
| Generic UI Rendering                                                           |
|--------------------------------------------------------------------------------|
| Frontend uses metadata to render:                                              |
| - list views                                                                   |
| - read views                                                                   |
| - create forms                                                                 |
| - update forms                                                                 |
| - delete actions                                                               |
| - navigation                                                                   |
+----------------------------------------+---------------------------------------+
                                         |
                                         | CRUD calls
                                         v
+--------------------------------------------------------------------------------+
| Generated REST API                                                             |
|--------------------------------------------------------------------------------|
| /api/v1/<model>                                                                |
|                                                                                |
| Same backend metadata controls API behavior and frontend behavior              |
+--------------------------------------------------------------------------------+
```

---

## Automation Flow - Jobs and Triggers

```text
+--------------------------------------------------------------------------------+
| Trigger-based automation                                                       |
|--------------------------------------------------------------------------------|
| CRUD operation                                                                 |
|   -> Before trigger                                                            |
|   -> InsteadOf trigger, optional                                               |
|   -> Repository operation                                                      |
|   -> After trigger                                                             |
|                                                                                |
| Used for:                                                                      |
| - history records                                                              |
| - derived values                                                               |
| - search/fulltext behavior                                                     |
| - domain-specific side effects                                                 |
+--------------------------------------------------------------------------------+

+--------------------------------------------------------------------------------+
| Scheduled automation                                                           |
|--------------------------------------------------------------------------------|
| CronScheduler                                                                  |
|   -> reads registered jobs                                                     |
|   -> checks schedule                                                           |
|   -> runs job                                                                  |
|   -> persists JobRun                                                           |
|                                                                                |
| Example job types:                                                             |
| - cleanup jobs                                                                 |
| - vacuum job                                                                   |
| - HTML export jobs                                                             |
| - plugin-specific automation                                                   |
+--------------------------------------------------------------------------------+
```
