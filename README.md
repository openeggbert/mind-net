# mind-net

Mind Net is a note taking application written in the C++ programming language:

Requirements:
* Linux
* Windows

Used technologies:
* C++17
* SQLite
* Crow
* CMake

Components:
* C++ Crow backend
* Pure HTML, JavaScript, CSS frontend

## Screenshots

### List nodes

![List nodes](screenshots/screen_list_nodes.jpg "List nodes")

### Read node

![Read node](screenshots/screen_read_node.jpg "Read node")

### Create node

![Create node](screenshots/screen_create_node.jpg "Create node")

### Delete node

![Delete node](screenshots/screen_delete_node.jpg "Delete node")

### List properties

![List properties](screenshots/screen_list_properties.jpg "List properties")

### Graph demo

![Graph demo](screenshots/screen_graph_demo.jpg "Graph demo")

### Calling get list for node

![Calling get list for node](screenshots/screen_calling_get_list_for_node.jpg "Calling get list for node")

## How to build

### How to build on Linux

These are the instructions for Debian 13. 
* For other Linux distributions , you may need to install different packages.


```aiignore
# Install dependencies
apt install build-essential libboost-all-dev cmake g++ libcurl4-openssl-dev

# Install git
apt install git

# Clone the repository
git clone https://github.com/openeggbert/mind-net/

# Init git submodules
git submodule update --init --recursive

# Go to the repository
cd mind-net

# Enable FTS5 feature of SQLite : edit third_party/sqlite/CMakeLists.txt
# Add target_compile_definitions(sqlite3 PUBLIC SQLITE_ENABLE_FTS5) 
#to: 

$<INSTALL_INTERFACE:include/>)
target_compile_definitions(sqlite3 PUBLIC SQLITE_ENABLE_FTS5)
if (SQLITE_ENABLE_COLUMN_METADATA)

# Switch to the develop branch
git checkout develop

# Create build directory
mkdir build

# Go to build directory
cd build

# Run cmake
cmake -B . -S ..

# Build
cmake --build .

#Generate JWT Secret
openssl rand -base64 32

#Export JWT Secret
export JWT_SECRET={jwt_secret}

# Run the application
./mind_net start --port 8888 -s /home/johndoe/Desktop/mindnet/frontend
```

# Project TODO / Roadmap

## Emoji Legend
- **BUG** – Issues, crashes, or bugs to fix
- **FEATURE** – New functionality or enhancements
- **IMPROVEMENT** – Improvements to existing code
- **DOCUMENTATION** – Docs, guides, README updates
- **Testing** – Writing or updating tests
- **Task** – General task or chore
- **Question / Discussion** – Questions, decisions, or discussions
- **Performance** – Performance optimization
- **Security** – Security-related changes
- **Deprecation / Removal** – Removing old or deprecated code

## BACKLOG

- [ ] BUG Update of boolean values in SQLite is not working.
- [ ] BUG Action list sometimes fails - AND is missing in the generated SQL statement.
- [ ] FEATURE New entity Flag
- [ ] IMPROVEMENT Add logging to files
- [ ] FEATURE Support for export to static HTML files
- [ ] FEATURE Support for PostgresSQL storage
- [ ] FEATURE User authentication (JWT or session token or ?)
- [ ] FEATURE User authorization via Validators
- [ ] FEATURE Implement Complex Filtering in REST API
- [ ] FEATURE New table comment_reaction
- [ ] FEATURE New table discussion_read_status
- [ ] FEATURE Triggers
- [ ] FEATURE Log logging in, registration, logout, password changes
- [ ] FEATURE new endpoint /info - shows some configuration entries (not all) + other information

### Triggers

```
//Trigger makes operations in datatabase
enum TriggerType {
before, 
after, 
instead_of, //Trigger is executed instead of the operation 
around //like before and after together
}

class Trigger {
Crudl crudl;
string table_name;
TriggerType type
condition (order>400 ...);
bool validation_passed:
int priority;
string name;
string description;
}
```

Přidej parametr validation_passed: true/false do kontextu triggeru



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


  
### New table comment_reaction

CREATE TABLE comment_reaction (
comment_id INTEGER,
user_id INTEGER NOT NULL,
type TEXT NOT NULL, -- např. 'like', 'heart', 'laugh'
FOREIGN KEY(comment_id) REFERENCES comment(id),
FOREIGN KEY(user_id) REFERENCES user(id)
);

### New table discussion_read_status

CREATE TABLE discussion_read_status (
user_id INTEGER NOT NULL,
discussion_id INTEGER NOT NULL,
last_read_at DATETIME DEFAULT CURRENT_TIMESTAMP,
PRIMARY KEY(user_id, discussion_id),
FOREIGN KEY(user_id) REFERENCES user(id),
FOREIGN KEY(discussion_id) REFERENCES discussion(id)
);

## Done


