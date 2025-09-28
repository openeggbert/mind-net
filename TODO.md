# TODO
## TODO – Migrate to C++20/23 Modules

### Why?

C++ modules are the modern way to organize code, improve clarity, and dramatically speed up compilation. Since C++20 they are officially part of the language, and they are a big step forward compared to traditional header files.

### 🧩 What are C++ Modules?

A module is a unit of code that:

* **Exports** selected types, functions, classes, etc.
* **Isolates** non-exported parts from the rest of the application.
* **Replaces** the traditional `#include` mechanism, reducing duplication and slow compile times.

#### Two parts of a module:

* **Interface module** – contains `export` declarations visible outside.
* **Implementation module** – contains definitions that are not exported.

**Example – Interface:**

```cpp
export module Math;

export int add(int a, int b);
```

**Example – Implementation:**

```cpp
module Math;

int add(int a, int b) {
    return a + b;
}
```

**Usage:**

```cpp
import Math;

int main() {
    int result = add(3, 4);
}
```

---

#### 🛠 Migration Plan

1. **Identify logical components**
   Example split:

    * `Math` – math utilities
    * `IO` – input/output
    * `Graphics` – rendering
    * `Utils` – helpers

2. **Create interface modules**
   Each logical unit gets its own `.ixx` file with `export module` + declarations.

3. **Create implementation modules**
   Store definitions (`module Name;`) separately, no `export`.

4. **Replace `#include` with `import`**

    * Use `import ModuleName;` instead of headers.
    * For STL: prefer `import std;` or `import std.vector;` (if supported).

5. **Adapt build system**
   Update **CMake** (or Visual Studio/MSVC project) to handle module compilation:

   ```cmake
   add_library(Math MODULE src/Math/math.ixx src/Math/math_impl.cpp)
   add_library(Utils MODULE src/Utils/utils.ixx src/Utils/utils_impl.cpp)

   add_executable(MyApp src/main.cpp)
   target_link_libraries(MyApp PRIVATE Math Utils)
   ```

   ⚠️ CMake support for modules is still evolving – may require flags like `/std:c++20 /experimental:module` (MSVC) or `-fmodules-ts` (Clang).

---

### ✅ Benefits

* 🚀 **Faster compilation** – compiler processes binary module interface, not repetitive `#include`.
* 🔒 **Better encapsulation** – non-exported code is hidden.
* 🧼 **Cleaner dependencies** – fewer macros, less preprocessor noise.

---

### 📁 Suggested Project Structure

```
MyProject/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── Math/
│   │   ├── math.ixx         // Interface module
│   │   └── math_impl.cpp    // Implementation module
│   ├── Utils/
│   │   ├── utils.ixx
│   │   └── utils_impl.cpp
```

* **`.ixx` files** = interface modules (`export module`, declarations).
* **Implementation files** = module body (`module Name;`, no export).

---

### ⚠️ About `#include`

* **Ideal case (pure modules):** no `#include` at all.
* **In practice:**

    * You still need `#include` for most 3rd-party libraries (Boost, SDL, OpenCV, etc.).
    * Legacy code may need gradual migration.
    * Templates/macros sometimes still rely on headers.

👉 Strategy: **write new code in modules, refactor old code gradually.**

---

📌 **Next action item:**
Pick one existing header file in the project and convert it into a C++20 module (`.ixx + .cpp`).

Here’s a draft for your `todo.md` entry, written in English, that captures the approach we discussed:

---

## TODO – Implement SQLite Full Database Rebuild with Custom DSL

### Goal

Implement support for advanced schema changes in SQLite that are not natively supported by `ALTER TABLE` (e.g. adding/removing **foreign keys**, **check constraints**, **NOT NULL**, **UNIQUE**, column type changes).
These migrations will use a **SQL-like DSL** instead of raw SQL. The DSL commands will be interpreted and executed by C++ code. At the end, the system will perform a **full database rebuild** (dump & restore) to apply all changes safely.

---

### Steps

1. **Design SQL-like DSL commands**

    * Example commands:

      ```sql
      ADD CHECK users age "age >= 0";
      DROP FOREIGN KEY orders fk_customer;
      ALTER COLUMN users email NOT NULL;
      ADD UNIQUE users email;
      DROP COLUMN users temp_field;
      ```
    * Keep syntax close to SQL so it is intuitive, but map each command internally to a C++ handler.

2. **Command parsing**

    * Implement a lightweight parser that detects whether a migration statement is:

        * pure SQL (`CREATE TABLE`, `INSERT INTO`, etc.) → execute directly, OR
        * DSL command (`ADD CHECK`, `DROP FOREIGN KEY`, etc.) → enqueue as `MigrationCommand` object.

3. **Migration execution flow**

    * Run all pure SQL migrations immediately.
    * Collect all DSL commands.
    * When the first DSL command is encountered, mark that a **rebuild is required**.
    * Continue processing migrations and enqueue further DSL commands.
    * At the end, perform a single **rebuild of the entire database**, applying all collected DSL changes.

4. **Rebuild process**

    1. Open old database in `READONLY`.
    2. Create new database file (or in-memory for speed).
    3. Extract schema from `sqlite_master`:

       ```sql
       SELECT type, name, sql 
       FROM sqlite_master 
       WHERE type IN ('table','index','trigger','view') 
         AND name NOT LIKE 'sqlite_%';
       ```
    4. Modify `CREATE TABLE` statements according to DSL commands (add/remove constraints, columns, keys, etc.).
    5. Execute new schema in new database.
    6. Copy data:

       ```sql
       ATTACH 'old.db' AS old;
       INSERT INTO new_table(col1, col2, ...) 
       SELECT col1, col2, ... FROM old.old_table;
       DETACH old;
       ```
    7. Recreate indexes, triggers, and views.
    8. Enable foreign keys (`PRAGMA foreign_keys=ON;`).
    9. Replace old DB file with rebuilt DB file.

5. **Optimizations**

    * **Batching DSL commands**: multiple rebuild-required commands in one migration should only trigger a single rebuild at the end.
    * **In-memory rebuild**: use `:memory:` database as the rebuild target for smaller DBs, then back it up to disk → much faster.
    * **Hybrid approach**: if a rebuild command appears far away from others, rebuild earlier; otherwise group consecutive ones.

---

### Benefits

* Developer writes **simple SQL-like commands** instead of complex manual rebuild scripts.
* System ensures the database is rebuilt consistently and safely.
* Same DSL can be reused for PostgreSQL implementation, but mapped directly to native `ALTER TABLE` commands (no rebuild needed there).

---

👉 Next Action:

* Define DSL grammar and implement parser → `MigrationCommand`.
* Implement `DatabaseRebuilder` class that executes full dump & restore based on DSL commands.
* Integrate into existing migration pipeline so that SQL and DSL can coexist.

# TODO – Database Abstraction and Multi-DB Support

## Goal
Isolate all database access through generic interfaces so the application can support multiple backends (SQLite, PostgreSQL, MariaDB/MySQL, etc.) without changing application logic.

---

## Step 1: Define Interfaces
Create generic interfaces in `mindnet/db/api/`:

- **IDatabase**  
  - `prepare(sql) -> IStatement`  
  - `exec(sql)`  
  - `getLastInsertId()`

- **IStatement**  
  - `bind(index, string|int64)`  
  - `executeStep()`  
  - `exec()`  
  - `getString(column)`  
  - `getInt(column)`

- **IMigration**  
  - `validate()`  
  - `migrate()`

These abstract away any concrete SQL library.

---

## Step 2: Implement SQLite Adapter
Wrap the existing `SQLiteCpp` code:

- `SqliteDatabase : IDatabase`  
- `SqliteStatement : IStatement`  
- `SqliteMigration : IMigration`

Ensure the existing CRUD (`create_model`, `read_model`, `update_model`, `delete_model`, `list_models`) only depend on the interfaces, **not** directly on `SQLiteCpp`.

---

## Step 3: Refactor Application
- Update `RepositoryImplSqlite` to use `IDatabase` instead of `SQLite::Database`.  
- Remove all SQLite-specific pragmas, queries, and error handling from higher layers.  
- Only the adapter (`SqliteDatabase`) should know about SQLite.

At this point the app should still run exactly as before, but all database code will be isolated.

---

## Step 4: Add PostgreSQL Support
- Implement `PostgresDatabase`, `PostgresStatement`, `PostgresMigration` using `libpqxx`.  
- Adapt differences in SQL syntax if needed (e.g., autoincrement, last insert ID).  
- Introduce `RepositoryImplPostgres` if queries must differ.  

---

## Step 5: Add Other Databases (Optional)
- **MariaDB/MySQL** → `MysqlDatabase` using MySQL/MariaDB C API or `mysql++`.  
- **Oracle, MS SQL, …** → similar adapters wrapping their C/C++ APIs.  

Each database driver only implements the common interface. Application logic stays unchanged.

---

## Step 6: Repository Factory
Introduce a single entry point:

```cpp
enum class DatabaseType { Sqlite, Postgres, Mysql };

class RepositoryFactory {
    DatabaseType type;
public:
    explicit RepositoryFactory(DatabaseType t);
    std::shared_ptr<IRepository> create(const ModelDefinition&);
};
````

Select the database type at runtime via configuration (`config.json`, env var `DB_TYPE`, etc.).

---

## Step 7: Migration System

* Move the migration logic behind `IMigration`.
* Implement `SqliteMigration`, `PostgresMigration`, etc.
* All share the same principle (track applied migrations in `migration` table, apply pending ones), but SQL may differ slightly.

---

## Step 8: Test Incrementally

1. Convert a single function (e.g., `create_model`) to use the interface.
2. Run with SQLite → verify same behavior.
3. Convert the remaining functions.
4. Add PostgreSQL adapter and test again.

---

## Benefits

* Clean separation between application and database.
* Easy to add new database backends.
* Simplifies testing (could add `InMemoryDatabase` for unit tests).
* Long-term maintainability.
