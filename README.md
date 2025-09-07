# mind-net

Mind Net is a mind map storage written in the C++ programming language:

Requirements:
* Linux
* Windows

Used technologies:
* C++17
* SQLite
* Crow
* CMake

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
# - add target_compile_definitions(sqlite3 PUBLIC SQLITE_ENABLE_FTS5) to 

$<INSTALL_INTERFACE:include/>)
 
+target_compile_definitions(sqlite3 PUBLIC SQLITE_ENABLE_FTS5)
+
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
- 🐛 **Bug** – Issues, crashes, or bugs to fix
- ✨ **Feature** – New functionality or enhancements
- 🔧 **Improvement** – Improvements to existing code
- 📝 **Documentation** – Docs, guides, README updates
- ✅ **Testing** – Writing or updating tests
- 📌 **Task** – General task or chore
- ❓ **Question / Discussion** – Questions, decisions, or discussions
- ⚡ **Performance** – Performance optimization
- 🔒 **Security** – Security-related changes
- ⚰️ **Deprecation / Removal** – Removing old or deprecated code
- 

## BACKLOG

- [ ] BUG Update of boolean values in SQLite is not working.
- [ ] BUG Action list sometimes fails - AND is missing in the generated SQL statement.
- [ ] FEATURE New entity Flag
- [ ] FEATURE New entity SystemConfig
- [ ] IMPROVEMENT Add logging to files
- [ ] FEATURE Support for export to static HTML files
- [ ] FEATURE Support for PostgresSQL storage
- [ ] FEATURE User authentication (JWT or session token or ?)
- [ ] FEATURE User authorization via Validators
- [ ] FEATURE Implement Complex Filtering in REST API
- [ ] FEATURE New table comment_reaction
- [ ] FEATURE New table discussion_read_status

### Task details

#### Implement Complex Filtering in REST API

```aiignore






## express complex filtering logic (like AND, OR, etc.) in REST APIs — with both parenthetical syntax and JSON-based format.
🔧 Common Logical and Comparison Operators
Operator
	
Meaning
	
Parenthetical Format
	
JSON Format
AND
	
Logical conjunction
	
(A AND B)
	
{ "and": [A, B] }
OR
	
Logical disjunction
	
(A OR B)
	
{ "or": [A, B] }
NOT
	
Logical negation
	
NOT (A)
	
{ "not": A }
= / ==
	
Equality
	
field = value
	
{ "field": { "eq": value } }
!=
	
Inequality
	
field != value
	
{ "field": { "neq": value } }
< / >
	
Less than / Greater than
	
field < value
	
{ "field": { "lt": value } }
<= / >=
	
Less than or equal / Greater or equal
	
field <= value
	
{ "field": { "lte": value } }
IN
	
Value is in a list
	
field IN (a, b, c)
	
{ "field": { "in": [a, b, c] } }
LIKE
	
Pattern match (e.g. substring)
	
field LIKE '%abc%'
	
{ "field": { "like": "%abc%" } }
IS NULL
	
Field is null
	
field IS NULL
	
{ "field": { "is_null": true } }
EXISTS
	
Subquery or presence check
	
EXISTS (subquery)
	
{ "exists": { ... } }
🧠 Example: Complex Filter
🔹 Parenthetical Format

((owner = 123 AND visibility = 'public') OR shared = true) AND NOT deleted

🔹 JSON Format

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

#### New entity SystemConfig

- bool guest_users_are_allowed
- bool user_can_self_register
  * if user_can_self_register==false then only admin can create new users,

  
#### New table comment_reaction

CREATE TABLE comment_reaction (
comment_id INTEGER,
user_id INTEGER NOT NULL,
type TEXT NOT NULL, -- např. 'like', 'heart', 'laugh'
FOREIGN KEY(comment_id) REFERENCES comment(id),
FOREIGN KEY(user_id) REFERENCES user(id)
);


#### New table discussion_read_status

CREATE TABLE discussion_read_status (
user_id INTEGER NOT NULL,
discussion_id INTEGER NOT NULL,
last_read_at DATETIME DEFAULT CURRENT_TIMESTAMP,
PRIMARY KEY(user_id, discussion_id),
FOREIGN KEY(user_id) REFERENCES user(id),
FOREIGN KEY(discussion_id) REFERENCES discussion(id)
);


## Done




## Other
### SM-2 (SuperMemo 2)

User defines the quality of the knowledge (0-5).

Input values:
- Quality (0-5)
- Repetition - how many times the note was successfully reviewed
  - Starts at 0
  - Increases with each successful repetition (quality ≥ 3)
  - Helps determine if the interval should be extended 
- Interval - current interval in days
  - Number of days until next repetition
  - E.g. 1 → 6 → 15 → 37 → 90 ...
  - Calculated based on EF and number of repetitions
- Ef - effectivity factor (initial value 2.5)
  - Starts at 2.5
  - Changes based on answer quality
  - Determines how quickly the interval extends (e.g. 10 days × EF)

Output values:
- Last quality
  - Date (timestamp) when the note should be shown again
  - Used for selecting notes for review
- Repetitions
- Interval
- Ef
- Next review
  - Date (timestamp) when the note should be shown again
  - Used for selecting notes for review
- Last review


#### 📊 How to Identify Articles with Weak Knowledge

With your current SQLite schema, the application can easily detect which articles (or topics) a user is struggling with. Here are several strategies:

##### 1. 🔍 Select Questions with Weak SM-2 State

Use the `state` table to find questions that have:

- Low **EF** (easiness factor)  
- Few **repetitions**  
- `next_review` in the past (**overdue**)  

```sql
SELECT q.*
FROM question q
JOIN state s ON q.id = s.question_id
WHERE s.user_id = ?
  AND (s.EF < 2.0 OR s.repetitions < 3 OR s.next_review <= CURRENT_TIMESTAMP)
  AND q.active = 1;
````

---

##### 2. 📚 Aggregate Weak Questions by Article

If each question is linked to an `article_id`, you can group weak questions by article:

```sql
SELECT q.article_id, COUNT(*) AS weak_questions
FROM question q
JOIN state s ON q.id = s.question_id
WHERE s.user_id = ?
  AND (s.EF < 2.0 OR s.repetitions < 3 OR s.next_review <= CURRENT_TIMESTAMP)
GROUP BY q.article_id
ORDER BY weak_questions DESC;
```

This gives you a ranked list of articles with the most weak questions for a given user.

---

##### 3. 🧠 Alternatively: Use `review.grade`

If you prefer to assess knowledge based on recent review performance:

```sql
SELECT q.article_id, AVG(r.grade) AS avg_grade
FROM question q
JOIN review r ON q.id = r.question_id
WHERE r.user_id = ?
GROUP BY q.article_id
HAVING avg_grade < 3
ORDER BY avg_grade ASC;
```

This highlights questions where the user consistently scores poorly.




#### New action Review - only for questions and nodes

**Scenario:**

- Frontend shows
  * title of the question.
  * buttons: "Show answer", "Skip"
- Once the user clicks "Show answer"
  * The application shows: How well do you understand this? (0-5):
  * User enters a number (0-5)
  * Application updates REVIEW_IN_X_DAYS, EASINESS_FACTOR, REPETITION, EXPIRES_AT
  * Continues to the next node

#### Refactor script.js

The huge file script.js can  be refactored and split into several modules. In its current state, it is *
*monolithic** - containing everything from API, cache, schema build, CRUD renderers, navigation, network map to
initialization and menu. Splitting it into modules will improve readability, maintenance and testability.

I propose the following **modular approach**:

---

### 1️⃣ `api.js` - API and cache

* `API_BASE`, `MODEL_DEFINITION_URL`, `CACHE_KEY`, `CACHE_TTL_MS`
* `loadModelDefinition()`
* `apiFetch()`
* `resolveForeignKeyValue()`

---

### 2️⃣ `schemas.js` - Entity schema build

* `buildEntitySchemas()`
* `buildGlobals()`
* `mapColumnType()`
* `findTitleField()`
* `filterColumnsForForm()`
* `toLabel()`
* `capitalize()`

---

### 3️⃣ `state.js` - Global state

* `entities`, `entityLabels`, `actions`, `entitySchemas`
* `selectedEntity`, `selectedAction`, `selectedActionId`
* `currentPage`, `pageSize`, `totalPages`
* `mainEntities`, `linkEntities`, `reviewEntities`, `collaborationEntities`, `suggestionEntities`, `notMainEntities`
* `actionLabels`

---

### 4️⃣ `dom.js` - DOM references and basic UI helpers

* `entityNav`, `crudMenu`, `entityTitle`, `contentArea`
* `showError()`, `getQueryParams()`, `formatDateTime()`, `parseDateTimeToUnix()`
* `getHiddenColumns()`, `setHiddenColumns()`, `isColumnHidden()`

---

### 5️⃣ `crud.js` - CRUD render functions

* `renderEntityForm()`
* `renderEntityRead()`
* `renderEntityList()`
* `renderColumnSelector()`
* `executeCustomAction()`
* Event handlers for form submit, pageSize changes, applyColumns

---

### 6️⃣ `explore.js` - Map/Network visualization

* `renderMapExplore()`
* `loadChildren()`
* `loadTestNodes()`
* `add_node_and_edges()`
* `drawNetwork()`
* `replaceSpacesWithUnderscores()`
* `currentCenterNodeId`, `parentStack`, `network`

---

### 7️⃣ `navigation.js` - Navigation and menu

* `renderEntityNav()`
* `renderCrudMenu()`
* `updateActiveMenu()`
* `selectEntity()`
* `selectAction()`
* `changePage()`

---

### 8️⃣ `actions.js` - Global CRUD helpers

* `readEntity()`, `editEntity()`, `deleteEntity()`

---

### 9️⃣ `init.js` - Initialization

* `initializeFromURL()`
* Mobile menu button handler
* Launching: `initializeFromURL();`

---

✅ **Benefits:**

* Individual JS parts have a clear purpose.
* Reduced dependency on global variables (you can gradually transition to ES6 module `import/export`).
* Easier testing and maintenance.
* Future extensions e.g. additional entities, custom actions, or new visualizations will be simpler.
* `initializeFromURL()`
* Mobile menu button handler
* Launching: `initializeFromURL();`

---

✅ **Benefits:**

* Individual JS parts have a clear purpose.
* Reduced dependency on global variables (you can gradually transition to ES6 module `import/export`).
* Easier testing and maintenance.
* Future extensions e.g. additional entities, custom actions, or new visualizations will be simpler.

