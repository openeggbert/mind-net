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

### Plugin Dictionary
- [ ] Multi-language - translate English text, which is not yet translated
- [ ] New cron job: delete terms with status Deleted
- [ ] Export to static web
- [ ] Bug: Search: Any map does not take effect
- [ ] Repetition: Under Start button: 42 cards selected: 18 due / 12 not due / 12 never
- [ ] Modify fulltext search - replace LIKE by FTS
- [ ] UI: Add: Created at and Updated at
- [ ] Bug: Browser title is not updated after map is changed.
- [ ] New window History
- [ ] TermVisitHistoryWindow.js: Add an option to show only visits for the current term
- [ ] Add new column dictionary_link.position
- [ ] Button "Add Alias" should look the same as buttons "Add Tag" and "Add Flag"
- [ ] New rule: Alias.alias must be different from term.title
- [ ] Term, Note: The Save button should switch the textarea to rendered Markdown view.
- [ ] Markdown editor: add emoji button
- [ ] Markdown renderer: fix the styles - table, pre, code, ...
- [ ] Make UI mobile friendly
- [ ] New button "Focus Mode"
- [ ] Graph, vis.js

### Backend
- [ ] New table error
- [ ] Bug: column ip_address has always the value 127.0.0.1
- [ ] Fix bugs in the cron_quartz and CronScheduler
- [ ] CronScheduler: fix ShardedModelCache, computing the shard index
- [ ] Replace hash_sha_256(password) with: Argon2id (best modern choice) or bcrypt/scrypt/PBKDF2
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
- [ ] Implement adding reason for changes (history.reason)
- [ ] Validators - reorder methods in cpp files
- [ ] Add new ColumnTypes: BLOB, REAL: std::variant<int64_t, std::string, double, std::vector<std::byte>> `static_assert(sizeof(double) == 8, "double must be 64-bit"); typedef double r64;`
- [ ] Integrate AI into mind-net
- [ ] Add tests
- [ ] ORM framework: add count, exists, empty
- [ ] To avoid model name collisions, use plugin_name consistently throughout the system. When finding a model, both
  plugin_name and model_name are required. This allows the same model_name to exist multiple times across different
  plugins.
- [ ] Use PascalCase naming convention for all enums
- [ ] Create a system user during startup if it doesn't exist. This user should not be able to login.
- [ ] Check, test and fix all validators

### Frontend
- [ ] sort and order is missing
- [ ] Tree view: via vis.js, clicking on node opens the node in a new tab
- [ ] Frontend should not show actions, for which user is not authorized to
- 
### Experimental
- [ ] Chat plugin - Slack-like
- [ ] Mail plugin - E-mail like
- [ ] Suggestion plugin - E-mail like
- [ ] FEATURE Implement complex Filtering in REST API
- [ ] New abstract class Provider
- [ ] Add support for Docker
- [ ] Rewrite enums using EnumUtils.h
- [ ] Table note - new column path9
- [ ] QT desktop application, which starts or stops the server


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
- [x] FEATURE User authorization via Validators
- [x] New table api_log
- [x] New table access_token : name, description, expiration_date, string allowed_operations
- [x] New table refresh_token
- [x] New table login_session
- [x] FEATURE User authentication
- [x] /logout endpoint
- [x] Add Logging level to configuration
- [x] New table auth_log : logging registration, logout, password changes
- [x] Unix time columns should have SQLite type DATETIME
- [x] Reorder columns of tables, if needed
- [x] Add hash sums for sql migrations - new column in table migration
- [x] BUG Update of boolean values in SQLite is not working.
- [x] BUG Action list sometimes fails - AND is missing in the generated SQL statement.
- [x] FEATURE Query - name, args, returns string - for Validators and Triggers
- [x] Fix validators and move authorization into the correct methods
- [x] CronJobs - https://www.baeldung.com/cron-expressions - also cleanup CronJobs - delete old auth_log, api_log, super_admin_log, history (READ and LIST only)
- [x] All ID fields should be migrated from integer to identification for consistency and future compatibility
- [x] All occurrences of methods to_values(), from_values() must use serialize_fields() and deserialize_fields()
- [x] ModelDefinition: readonly. ColumnDefinition: mutable
- [x] New tables: dictionary_url_type a dictionary_url
- [x] Notes: New buttons: Expand, Collapse
- [x] If map is changed, clear Search term input
- [x] dictionary_map - new column emoji
- [x] dictionary_term - new column repetition bool default 1
- [x] &map_id={}&term_id={}
- [x] OlderTerm, NewerTerm
- [x] Search Term - should also use the table dictionary_term_alias
- [x] New table dictionary_term_understanding.
- [x] New enum UnderstandingLevel: Unknown, Recognized, Understood, Applied, Internalized
- [x] New column dictionary_term_visit.source: Unknown, Created, Search, Link, Note, Random, External, Other, ...
- [x] Bug - &term_id is sometimes not present in the url
- [x] Window "Term Visit History" - add paging
- [x] Implement three-level autocomplete relevance scoring: 1) exact title > exact alias 2) prefix title > prefix alias 3) substring title > substring alias
- [x] Bug: Map emoji is not loaded, if VisitSource is External
- [x] Frontend : Do not update, if the term, note, ... was not changed.
- [x] Bug: renaming term does not remove the old title from the cache
- [x] Fix translations for: dictionary.enums.status.none
- [x] Added message "You have unsaved changes."
- [x] After a new note is created, the note should be expanded.