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

# TODO – Constraint Emulation with Triggers in SQLite

SQLite’s native `ALTER TABLE` is limited. To add constraints later (e.g. `FOREIGN KEY`, `NOT NULL`, `UNIQUE`, `DEFAULT`), we can emulate them with triggers.
Below are **trigger templates** you can reuse in migrations.

trigger_constraint__{table}__{constraint_type}__{column(s)}[__{ref_table}__{ref_column(s)}]

```aiignore
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>

// utility - split by delimiter
std::vector<std::string> split(const std::string& s, const std::string& delim = "__") {
    std::vector<std::string> parts;
    size_t start = 0, end;
    while ((end = s.find(delim, start)) != std::string::npos) {
        parts.push_back(s.substr(start, end - start));
        start = end + delim.length();
    }
    parts.push_back(s.substr(start));
    return parts;
}

// TODO: implement decode_hex/decode_base64 if you will use encoding
// for simplicity we'll just return the raw string here
std::string decode_hex(const std::string& hex) {
    // Simple dummy version - actually convert hex to string
    return "[DECODED_CHECK_EXPR:" + hex + "]";
}

// main function
std::string parseTriggerConstraint(const std::string& triggerName) {
    auto parts = split(triggerName);

    if (parts.size() < 4 || parts[0] != "trigger_constraint") {
        throw std::runtime_error("Not a valid constraint trigger name: " + triggerName);
    }

    std::string table = parts[1];
    std::string type  = parts[2];

    if (type == "foreign_key") {
        if (parts.size() != 6) {
            throw std::runtime_error("Invalid foreign key trigger format: " + triggerName);
        }
        std::string column    = parts[3];
        std::string refTable  = parts[4];
        std::string refColumn = parts[5];

        std::ostringstream oss;
        oss << "FOREIGN KEY(" << column << ") REFERENCES "
            << refTable << "(" << refColumn << ")";
        return oss.str();
    }
    else if (type == "unique") {
        // parts[3] may contain multiple columns separated by '_'
        std::string cols = parts[3];
        std::replace(cols.begin(), cols.end(), '_', ','); 

        std::ostringstream oss;
        oss << "UNIQUE(" << cols << ")";
        return oss.str();
    }
    else if (type == "check") {
        if (parts.size() < 5) {
            throw std::runtime_error("Invalid check trigger format: " + triggerName);
        }
        std::string column = parts[3];
        std::string exprEncoded = parts[4];
        std::string expr = exprEncoded;

        // pokud je prefix "hex_" nebo "b64_", dekódovat
        if (expr.rfind("hex_", 0) == 0) {
            expr = decode_hex(expr.substr(4));
        } else if (expr.rfind("b64_", 0) == 0) {
            expr = "[DECODED_BASE64:" + expr.substr(4) + "]";
        }

        std::ostringstream oss;
        oss << "CHECK(" << expr << ")";
        return oss.str();
    }

    throw std::runtime_error("Unknown constraint type: " + type);
}

// demo
int main() {
    std::vector<std::string> triggers = {
        "trigger_constraint__child__foreign_key__parent_id__parent__id",
        "trigger_constraint__user__unique__username_email",
        "trigger_constraint__task__check__due_date__hex_434845434b286475655f64617465203e3d20637265617465645f617429"
    };

    for (auto& t : triggers) {
        try {
            std::cout << t << "\n -> " << parseTriggerConstraint(t) << "\n\n";
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << "\n";
        }
    }
}
```

```aiignore
trigger_constraint__child__foreign_key__parent_id__parent__id
 -> FOREIGN KEY(parent_id) REFERENCES parent(id)

trigger_constraint__user__unique__username_email
 -> UNIQUE(username,email)

trigger_constraint__task__check__due_date__hex_434845434b286475655f64617465203e3d20637265617465645f617429
 -> CHECK([DECODED_CHECK_EXPR:434845434b286475655f64617465203e3d20637265617465645f617429])

```


```aiignore
#include <string>
#include <vector>
#include <iostream>

// Adds constraints to CREATE TABLE SQL
std::string addConstraintsToCreate(const std::string& createSQL,
                                   const std::vector<std::string>& constraints) {
    // Find last ')'
    auto pos = createSQL.find_last_of(')');
    if (pos == std::string::npos) {
        throw std::runtime_error("Invalid CREATE TABLE SQL");
    }

    std::string before = createSQL.substr(0, pos);
    std::string after = createSQL.substr(pos); // Usually ")"

    std::string result = before;

    // Add comma before constraints if table already has columns
    if (before.find('(') != std::string::npos && before.back() != '(') {
        result += ",";
    }
    result += "\n  ";

    // Join constraints with commas
    for (size_t i = 0; i < constraints.size(); i++) {
        result += constraints[i];
        if (i + 1 < constraints.size()) {
            result += ",\n  ";
        }
    }

    result += after; // Add ")"
    return result;
}

// Demo
int main() {
    std::string sql = "CREATE TABLE task (\n"
                      "  id INTEGER PRIMARY KEY,\n"
                      "  title TEXT,\n"
                      "  created_at DATETIME,\n"
                      "  due_date DATETIME\n"
                      ")";
    std::vector<std::string> constraints = {
        "FOREIGN KEY(parent_id) REFERENCES parent(id)",
        "CHECK(due_date >= created_at)"
    };

    std::string newSQL = addConstraintsToCreate(sql, constraints);
    std::cout << newSQL << "\n";
}

```


```
---

# ✅ Rebuild Engine – Checklist

### 🔹 Preparation

* [ ] **Adopt a unified trigger naming convention** with prefix `trigger_constraint__...`

    * FOREIGN KEY:
      `trigger_constraint__child__foreign_key__parent_id__parent__id`
    * UNIQUE:
      `trigger_constraint__user__unique__username_email`
    * CHECK:
      `trigger_constraint__task__check__due_date__hex_{encoded}`
* [ ] Decide on **encoding** for CHECK expressions:

    * `hex` (simple, safe)
    * or `base64url` without padding (shorter names, safe characters)
* [ ] Create a **seed test database** with a few tables (`task`, `user`, `child`/`parent`)
* [ ] Add sample trigger names to your `todo.md` for reference

---

### 🔹 Implementation

* [ ] Implement **C++ parser for trigger names** → function `parseTriggerConstraint`

    * Returns SQL snippet:

        * `FOREIGN KEY(...) REFERENCES ...`
        * `UNIQUE(...)`
        * `CHECK(...)` (decoded expression)
* [ ] Implement **function to insert constraints into CREATE TABLE**

    * Locate the last `)` and inject constraints before it
* [ ] Implement **rebuild procedure for a single table**:

    1. `ALTER TABLE t RENAME TO t_old`
    2. `CREATE TABLE t (... + constraints...)`
    3. `INSERT INTO t SELECT * FROM t_old`
    4. `DROP TABLE t_old`
    5. `DROP TRIGGER ...` (only constraint-triggers)
* [ ] Test with seed database:

    * Valid data → rebuild succeeds
    * Invalid data → rebuild fails (as expected)

---

### 🔹 Stabilization

* [ ] Extend rebuild to **iterate over all tables** from `sqlite_master`

    * Collect all `trigger_constraint__...` triggers
    * Reconstruct CREATE TABLE definitions with native constraints
* [ ] Add **logging**: list all tables and constraints added during rebuild
* [ ] Add **unit tests**: simulate multiple migrations, run rebuild, verify results
* [ ] Add **performance benchmarks**: compare INSERT speed with FK vs. trigger-based checks
```

---

## 1. Emulate **NOT NULL**

```sql
-- Prevent NULL on insert
CREATE TRIGGER nn_{table}_{col}_insert
BEFORE INSERT ON {table}
FOR EACH ROW
WHEN NEW.{col} IS NULL
BEGIN
  SELECT RAISE(ABORT, '{table}.{col} cannot be NULL');
END;

-- Prevent NULL on update
CREATE TRIGGER nn_{table}_{col}_update
BEFORE UPDATE OF {col} ON {table}
FOR EACH ROW
WHEN NEW.{col} IS NULL
BEGIN
  SELECT RAISE(ABORT, '{table}.{col} cannot be NULL');
END;
```

---

## 2. Emulate **UNIQUE**

```sql
-- Prevent duplicate values on insert
CREATE TRIGGER uq_{table}_{col}_insert
BEFORE INSERT ON {table}
FOR EACH ROW
WHEN NEW.{col} IS NOT NULL
  AND EXISTS (SELECT 1 FROM {table} WHERE {col} = NEW.{col})
BEGIN
  SELECT RAISE(ABORT, 'duplicate value in {table}.{col}');
END;

-- Prevent duplicate values on update
CREATE TRIGGER uq_{table}_{col}_update
BEFORE UPDATE OF {col} ON {table}
FOR EACH ROW
WHEN NEW.{col} IS NOT NULL
  AND EXISTS (
    SELECT 1 FROM {table}
    WHERE {col} = NEW.{col}
      AND id != OLD.id
  )
BEGIN
  SELECT RAISE(ABORT, 'duplicate value in {table}.{col}');
END;
```

---

## 3. Emulate **FOREIGN KEY**

```sql
-- Check parent existence on insert
CREATE TRIGGER fk_{child}_{col}_insert
BEFORE INSERT ON {child}
FOR EACH ROW
WHEN NEW.{col} IS NOT NULL
  AND (SELECT {pcol} FROM {parent} WHERE {pcol} = NEW.{col}) IS NULL
BEGIN
  SELECT RAISE(ABORT, 'FK violation: {child}.{col} → {parent}.{pcol}');
END;

-- Check parent existence on update
CREATE TRIGGER fk_{child}_{col}_update
BEFORE UPDATE OF {col} ON {child}
FOR EACH ROW
WHEN NEW.{col} IS NOT NULL
  AND (SELECT {pcol} FROM {parent} WHERE {pcol} = NEW.{col}) IS NULL
BEGIN
  SELECT RAISE(ABORT, 'FK violation: {child}.{col} → {parent}.{pcol}');
END;

-- Emulate ON DELETE CASCADE
CREATE TRIGGER fk_{parent}_{pcol}_delete
AFTER DELETE ON {parent}
FOR EACH ROW
BEGIN
  DELETE FROM {child} WHERE {col} = OLD.{pcol};
END;
```

*(replace DELETE with `UPDATE ... SET {col}=NULL` to emulate `ON DELETE SET NULL`)*

---

## 4. Emulate **DEFAULT**

```sql
-- AFTER INSERT (because SQLite does not allow changing NEW directly)
CREATE TRIGGER def_{table}_{col}
AFTER INSERT ON {table}
FOR EACH ROW
WHEN NEW.{col} IS NULL
BEGIN
  UPDATE {table}
    SET {col} = {default_expr}
    WHERE id = NEW.id;
END;
```

Example:

```sql
-- Default current timestamp
CREATE TRIGGER def_user_created_at
AFTER INSERT ON user
FOR EACH ROW
WHEN NEW.created_at IS NULL
BEGIN
  UPDATE user
    SET created_at = strftime('%Y-%m-%d %H:%M:%S', 'now')
    WHERE id = NEW.id;
END;
```

---

## Notes

* Replace placeholders: `{table}`, `{col}`, `{child}`, `{parent}`, `{pcol}`, `{default_expr}`.
* For consistency, consider a **global switch** table:

  ```sql
  CREATE TABLE settings (id INTEGER PRIMARY KEY CHECK (id=1), enforce_constraints INTEGER NOT NULL DEFAULT 1);
  INSERT INTO settings (id, enforce_constraints) VALUES (1, 1);
  ```

  Then wrap triggers with:

  ```sql
  WHEN (SELECT enforce_constraints FROM settings WHERE id=1) = 1 AND ...
  ```

  → Allows enabling/disabling all constraint triggers at once.

---

✅ This setup gives you a lightweight way to **simulate constraints in SQLite** and later replace them with native constraints if you move to PostgreSQL.


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

## TODO: Future Improvements for Personal C++ Knowledge System

This list contains critical observations and enhancement suggestions based on current UI, architecture, and user experience. These are **not urgent** and can be revisited in the future when time allows.

keyboard shortcuts, clickable tags, searchable field to the header

---

### 🧠 UX & Usability


- [ ] Add tooltips and inline help for all buttons and metadata fields.
- [ ] Provide visual feedback for actions (e.g. save, delete, copy) via toasts or highlights.
- [ ] Add a breadcrumb or tree navigation for parent/child note structure.
- [ ] Implement a read-only mode to prevent accidental edits during review.

---


### 🚀 Onboarding & Accessibility

- [ ] Create a first-time user guide with screenshots.
- [ ] Add sample notes or templates for Zettelkasten and SuperMemo.
- [ ] Include inline explanations for metadata fields (e.g. what is "Order"?).
- [ ] Add a welcome screen or setup wizard for new users.

---

### 🔧 Architecture & Extensibility

- [ ] Add rate limiting to REST API for security.
- [ ] Consider CI/CD pipeline for plugin testing and integration.
- [ ] Explore export formats (Markdown, JSON) for notes.
- [ ] Document plugin development process for future contributors.

---

### 🧪 Validation & Safety

- [ ] Add syntax validation for metadata blocks.
- [ ] Highlight invalid or incomplete key-value entries.
- [ ] Add confirmation dialogs for destructive actions (e.g. delete).


