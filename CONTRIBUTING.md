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
- [ ] FEATURE New entity Flag
- [ ] FEATURE New entity Task (related to notes) + Markdown content of notes will be parsed for tasks - like in Zim Desktop Wiki + sending e-mail messages, web browser notification, Android toast
- [ ] Use SM-18, new entity review_session
- [ ] Add hash sums for sql migrations - new column in table migration
- [ ] Bool SQLite columns should start with is_
- [ ] Fix validators and move authorization into the correct methods
- [ ] Reorder columns of tables, if needed
- [ ] Unix time columns should have SQLite type DATETIME
- [ ] FEATURE User authentication
    * via JWT token /login, which is valid 1 hour (can be configured) ... https://github.com/njligames/crow-jwt-auth
    * refresh token /refresh-token is valid 7 days (can be configured)
    * when the access token expires, the client (e.g. frontend) sends the refresh token and obtains a new access token —
      without requiring re-authentication.
- [ ] Slip Box component
- [ ] Super Memo component
- [ ] BUG Update of boolean values in SQLite is not working.
- [ ] BUG Action list sometimes fails - AND is missing in the generated SQL statement.
- [ ] FEATURE Log logging in, registration, logout, password changes
- [ ] TASK Check operator== implementations for all models
- [ ] Validators - reorder methods in cpp files
- [ ] /logout endpoint
```
CROW_ROUTE(app, "/logout")([](const crow::request& req){
auto session = req.get_session();
session.clear(); // logout
return "Logged out";
});
  ```
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
- [ ] Implementing adding reason for changes (history.reason)
- [ ] Add Logging level to configuration
- [ ] Frontend should not show actions, for which user is not authorized

### Experimental
- [ ] Chat component - Slack-like
- [ ] FEATURE Support for PostgresSQL storage
- [ ] FEATURE Implement complex Filtering in REST API
- [ ] Add support for Docker

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

