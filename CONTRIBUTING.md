# Contributing to Mind-Net

We welcome contributions of all kinds: code, documentation, tests, or feature ideas.  
Please follow the [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/) and existing project style.

For a **detailed technical description of how Mind-Net works internally**, see  
👉 [DEVELOPERS.md](./DEVELOPERS.md)

## How to contribute

1. Fork the repo and create a feature branch (`feature/my-feature`).
2. Follow **C++ Core Guidelines** and existing coding style.
3. Add tests where possible.
4. Open a Pull Request with a clear description of your changes.

Please report bugs or request features in [GitHub Issues](https://github.com/openeggbert/mind-net/issues).

## Project TODO / Roadmap

- **BUG** – Issues, crashes, or bugs to fix
- **FEATURE** – New functionality or enhancements
- **IMPROVEMENT** – Improvements to existing code
- **DOCUMENTATION** – Docs, guides, README updates
- **TESTING** – Writing or updating tests
- **TASK** – General task or chore
- **QUESTION / DISCUSSION** – Questions, decisions, or discussions
- **PERFORMANCE** – Performance optimization
- **SECURITY** – Security-related changes
- **DEPRECATION / REMOVAL** – Removing old or deprecated code

## BACKLOG

### Critical
- [ ] Use crow_app.concurrency() and add new configuration entry
- [ ] New table alert - notification for user
- [ ] FEATURE New table Flag
- [ ] FEATURE New table Task (related to notes) + Markdown content of notes will be parsed for tasks - like in Zim Desktop Wiki + sending e-mail messages, web browser notification, Android toast
- [ ] New table pinned_note
- [ ] note.alias_for_note_id
- [ ] Use SM-18, new entity review_session
- [ ] Add hash sums for sql migrations - new column in table migration
- [ ] Bool SQLite columns should start with is_
- [ ] Fix validators and move authorization into the correct methods
- [ ] Reorder columns of tables, if needed
- [ ] Unix time columns should have SQLite type DATETIME
- [ ] Slip Box component
- [ ] Super Memo component
- [ ] BUG Update of boolean values in SQLite is not working.
- [ ] BUG Action list sometimes fails - AND is missing in the generated SQL statement.
- [ ] TASK Check operator== implementations for all models
- [ ] FEATURE Support for export to static HTML files

### Extending
- [ ] Move some parts of enum header files to cpp files
- [ ] IMPROVEMENT QueryParam - add filter(complex json filtering) and query (like '%_%')
- [ ] TASK Duplication in read_model and list_models - Both functions have nearly identical logic for reading data — consider refactoring into a shared utility.
- [ ] Improve documentation
- [ ] IMPROVEMENT Add logging to files
- [ ] FEATURE Create OpenAPI specification for the REST API
- [ ] bool custom_action.expand
- [ ] new entity File
- [ ] Frontend : sort and order is missing
- [ ] Tree view: via vis.js, clicking on node opens the node in a new tab
- [ ] Implement adding reason for changes (history.reason)
- [ ] Frontend should not show actions, for which user is not authorized
- [ ] Validators - reorder methods in cpp files
- [ ] Add support for missing column types (JSON, BLOB, etc.)
- [ ] Integrate AI into mind-net
- [ ] Add tests

### Experimental
- [ ] Chat component - Slack-like
- [ ] Mail component - E-mail like
- [ ] FEATURE Support for PostgresSQL storage
- [ ] FEATURE Implement complex Filtering in REST API
- [ ] Add support for Docker
- [ ] Rewrite enums using EnumUtils.h

### Implement Complex Filtering in REST API

#### Operators and JSON Format

| Operator    | Meaning                               | JSON Format                        |
| ----------- | ------------------------------------- | ---------------------------------- |
| **AND**     | Logical conjunction                   | `{ "and": [A, B] }`                |
| **OR**      | Logical disjunction                   | `{ "or": [A, B] }`                 |
| **NOT**     | Logical negation                      | `{ "not": A }`                     |
| **= / ==**  | Equality                              | `{ "field": { "eq": value } }`     |
| **!=**      | Inequality                            | `{ "field": { "neq": value } }`    |
| **< / >**   | Less than / Greater than              | `{ "field": { "lt": value } }`     |
| **<= / >=** | Less than or equal / Greater or equal | `{ "field": { "lte": value } }`    |
| **IN**      | Value is in a list                    | `{ "field": { "in": [a, b, c] } }` |
| **LIKE**    | Pattern match (substring)             | `{ "field": { "like": "%abc%" } }` |
| **IS NULL** | Field is null                         | `{ "field": { "is_null": true } }` |
| **EXISTS**  | Subquery or presence check            | `{ "exists": { ... } }`            |

---

#### Example: Complex Filter

```json
{
  "and": [
    {
      "or": [
        {
          "and": [
            { "owner": { "eq": 123 } },
            { "visibility": { "eq": "public" } }
          ]
        },
        { "shared": { "eq": true } }
      ]
    },
    { "deleted": { "eq": false } }
  ]
}
```

---

## Done

- [x] IMPROVEMENT Enums will be PascalCase, not all uppercase
- [x] IMPROVEMENT Refactor struct Configuration
- [x] FEATURE new endpoints /info and /health - shows some configuration entries (not all) + other information
- [x] FEATURE Triggers - also add adding operations (as json) to history table
- [x] ModelDefinition - add title_column
- [x] IMPROVEMENT Paging - add First and Last buttons
- [x] New table concept : title, disambiguation, note_id
- [x] New table source: type:book/web, title, author, year, page_number, url, map_id
- [x] FEATURE User authorization via Validators
- [x] New table idea: string title, string content, bool important, bool public
- [x] New table api_log
- [x] New entity wanted_note : note_title, ...
- [x] Add source_id to table note
- [x] New table access_token : name, description, expiration_date, string allowed_operations
- [x] New table refresh_token
- [x] New table login_session
- [x] FEATURE User authentication
- [x] /logout endpoint
- [x] Add Logging level to configuration
- [x] New table auth_log : logging registration, logout, password changes



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

