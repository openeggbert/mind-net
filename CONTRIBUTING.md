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
- [ ] TASK Check operator== implementations for all models
- [ ] Plugin Slipbox: fulltext search

### Extending
- [ ] Keyboard shortcuts for plugins: Slipbox and Repetition
- [ ] r_session - new column description - optional
- [ ] Path in slipbox plugin
- [ ] Previous next in slipbox plugin
- [ ] Repetition plugin - show next review
- [ ] Replace hash_sha_256(password) with: Argon2id (best modern choice) or bcrypt/scrypt/PBKDF2
- [ ] FEATURE Support for export to static HTML files
- [ ] Repetition plugin - new configuration entries - selected_items_limit, session_lifetime_in_hours
- [ ] CronJobs - https://www.baeldung.com/cron-expressions - also cleanup CronJobs -delete old auth_log, api_log, super_admin_log, history (READ and LIST only)
- [ ] FEATURE Support for PostgresSQL storage
- [ ] Move some parts of enum header files to cpp files
- [ ] IMPROVEMENT QueryParam - add filter(complex json filtering) and query (like '%_%')
- [ ] TASK Duplication in read_model and list_models - Both functions have nearly identical logic for reading data — consider refactoring into a shared utility.
- [ ] Improve documentation
- [ ] IMPROVEMENT Add logging to files
- [ ] FEATURE Create OpenAPI specification for the REST API
- [ ] bool custom_action.expand
- [ ] new table file - use content addressable file system with path structure like files/ab/cdef1234... for storing
  file content
- [ ] Frontend : sort and order is missing
- [ ] Tree view: via vis.js, clicking on node opens the node in a new tab
- [ ] Implement adding reason for changes (history.reason)
- [ ] Frontend should not show actions, for which user is not authorized
- [ ] Validators - reorder methods in cpp files
- [ ] Add support for missing column types (JSON, BLOB, etc.)
- [ ] Integrate AI into mind-net
- [ ] Add tests
- [ ] Plugin slipbox : import note as JSON and via the web interface

### Experimental
- [ ] Chat plugin - Slack-like
- [ ] Mail plugin - E-mail like
- [ ] Suggestion plugin - E-mail like
- [ ] Plugin can create new REST endpoints
- [ ] FEATURE Implement complex Filtering in REST API
- [ ] New abstract class Provider
- [ ] Add support for Docker
- [ ] Rewrite enums using EnumUtils.h
- [ ] Table note - new column path9


### Table File
```aiignore
-- V21__create_file.sql
CREATE TABLE file (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,

    -- metadata
    filename TEXT NOT NULL,
    extension TEXT NOT NULL,
    mime_type TEXT,
    size INTEGER NOT NULL,
    encrypted BOOLEAN DEFAULT 0,
    compressed BOOLEAN DEFAULT 0,
    compression_type INTEGER DEFAULT 0,

    -- integrity check
    checksum TEXT,                  -- hash value
    checksum_type TEXT DEFAULT 'SHA256', -- SHA256, MD5, BLAKE3...

    -- content
    data BLOB,                      -- for small files (e.g. < 2 MB)
    path TEXT,                      -- disk path for large files 

    -- relationships (generic)
    table_name TEXT NOT NULL,       -- e.g. 'note', 'task', 'idea', ...
    record_id INTEGER NOT NULL,     -- ID in respective table

    -- validation
    CHECK ((data IS NOT NULL AND path IS NULL) OR (data IS NULL AND path IS NOT NULL))
);

-- Indexes for fast searching
CREATE INDEX idx_file_table_record ON file(table_name, record_id);
CREATE INDEX idx_file_checksum ON file(checksum);
CREATE INDEX idx_file_filename ON file(filename);
CREATE INDEX idx_file_mime_type ON file(mime_type);

```
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
- [x] New table alert
- [x] New table flag
- [x] New table project
- [x] New table task - Markdown content of notes will be parsed for tasks - like in Zim Desktop Wiki + sending e-mail messages, web browser notification, Android toast
- [x] New table pinned_note
- [x] note.alias_for_note_id
- [x] Unix time columns should have SQLite type DATETIME
- [x] Reorder columns of tables, if needed
- [x] Add hash sums for sql migrations - new column in table migration
- [x] BUG Update of boolean values in SQLite is not working.
- [x] BUG Action list sometimes fails - AND is missing in the generated SQL statement.
- [x] Slip Box component
- [x] Super Memo component
- [x] FEATURE Query - name, args, returns string - for Validators and Triggers
- [x] Fix validators and move authorization into the correct methods
- [x] Ignore empty notes in repetition
- [x] Bug - window jumps and then can't be closed on mobile
- [x] Links will allow labels using |
