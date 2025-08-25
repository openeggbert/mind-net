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
# Run the application
./mind_net
```

## Entities

## TODO

### Support for export to static HTML files

### Support for PostgresSQL storage

### New entity SystemConfig

- bool guest_users_are_allowed
- bool user_can_self_register
   * if user_can_self_register==false then only admin can create new users,

###  Multi-user support

### New entity Flag

- starred

### New entity Question

### New entity Review

### New entity SM2Result

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


