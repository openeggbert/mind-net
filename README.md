# mini-wiki

Mini Wiki is a lightweight wiki inspired by MediaWiki and written in the C++ programming language:
* No hierarchy of articles. Each article is in the root
* One article can be in more categories
* Users - now only the administrator
* Revisions – history of changes is stored, changes can be reverted

Requirements:
* Linux
* Windows

Used technologies:
* C++17
* SQLite
* CMake

Entities:
* TODO

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
git clone https://github.com/openeggbert/mini-wiki/
# Init git submodules
git submodule update --init --recursive
# Go to the repository
cd mini-wiki
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
./mini_wiki
```


## Database schema

```aiignore
CREATE TABLE content (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	content TEXT NOT NULL,
	format TEXT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE namespace (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	name TEXT NOT NULL UNIQUE,
	description TEXT
);

CREATE TABLE user (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	username TEXT NOT NULL UNIQUE,
	password_hash TEXT NOT NULL
);

CREATE TABLE role (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	name TEXT NOT NULL UNIQUE,
	description TEXT
);

CREATE TABLE user_role (
	user_id INTEGER NOT NULL,
	role_id INTEGER NOT NULL,
	PRIMARY KEY(user_id, role_id),
	FOREIGN KEY(user_id) REFERENCES user(id) ON DELETE CASCADE,
	FOREIGN KEY(role_id) REFERENCES role(id) ON DELETE CASCADE
);


CREATE TABLE revision (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	article_id INTEGER NOT NULL,
	content_id INTEGER NOT NULL,
	user_id INTEGER,
	comment TEXT,
	deleted BOOLEAN DEFAULT 0,
	minor_edit BOOLEAN DEFAULT 0,
	edited_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	reverted_from_revision_id INTEGER,

	FOREIGN KEY (article_id) REFERENCES article(id) ON DELETE CASCADE,
	FOREIGN KEY (content_id) REFERENCES content(id) ON DELETE CASCADE,
	FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE SET NULL,
	FOREIGN KEY (reverted_from_revision_id) REFERENCES revision(id) ON DELETE SET NULL
);

CREATE TABLE article (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	namespace_id INTEGER NOT NULL,
	title TEXT NOT NULL,
	revision_id INTEGER,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	is_redirect BOOLEAN DEFAULT 0,
	redirect_article_id INTEGER,
	redirect_reason TEXT,
	CHECK ( (is_redirect = 0 AND redirect_article_id IS NULL) OR (is_redirect = 1 AND redirect_article_id IS NOT NULL) ),
	UNIQUE(namespace_id, title),

	FOREIGN KEY (revision_id) REFERENCES revision(id) ON DELETE SET NULL,
	FOREIGN KEY (redirect_article_id) REFERENCES article(id) ON DELETE SET NULL,
	FOREIGN KEY (namespace_id) REFERENCES namespace(id) ON DELETE CASCADE
);

CREATE TABLE wanted_article (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	title TEXT NOT NULL,
	namespace_id INTEGER NOT NULL,
	requested_by_user_id INTEGER,
	requested_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	FOREIGN KEY(namespace_id) REFERENCES namespace(id) ON DELETE CASCADE,
	FOREIGN KEY(requested_by_user_id) REFERENCES user(id) ON DELETE SET NULL,
	UNIQUE(namespace_id, title)
);


CREATE TABLE infobox (
	article_id INTEGER PRIMARY KEY,
	data JSON NOT NULL,
	FOREIGN KEY (article_id) REFERENCES article(id) ON DELETE CASCADE
);


CREATE TABLE article_property(
	article_id INTEGER,
	key TEXT,
	value TEXT,
	FOREIGN KEY (article_id) REFERENCES article(id) ON DELETE CASCADE,
	PRIMARY KEY(article_id, key)
);

CREATE TABLE category (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	title TEXT NOT NULL UNIQUE,
	parent_id INTEGER,
	FOREIGN KEY (parent_id) REFERENCES category(id) ON DELETE SET NULL,
	CHECK (parent_id IS NULL OR parent_id <> id)
);

CREATE TABLE article_category (
	article_id INTEGER NOT NULL,
	category_id INTEGER NOT NULL,
	PRIMARY KEY (article_id, category_id),
	FOREIGN KEY (article_id) REFERENCES article(id) ON DELETE CASCADE,
	FOREIGN KEY (category_id) REFERENCES category(id) ON DELETE CASCADE
);

CREATE TABLE article_link(
	from_article_id INTEGER,
	to_article_id INTEGER,
	CHECK (from_article_id <> to_article_id),
	FOREIGN KEY (from_article_id) REFERENCES article(id) ON DELETE CASCADE,
	FOREIGN KEY (to_article_id) REFERENCES article(id) ON DELETE CASCADE,
	PRIMARY KEY(from_article_id, to_article_id)
);

CREATE TABLE category_link(
	from_article_id INTEGER,
	to_category_id INTEGER,
	FOREIGN KEY (from_article_id) REFERENCES article(id) ON DELETE CASCADE,
	FOREIGN KEY (to_category_id) REFERENCES category(id) ON DELETE CASCADE,
	PRIMARY KEY(from_article_id, to_category_id)
);

CREATE TABLE external_link(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	from_article_id INTEGER,
	to_url TEXT,
	UNIQUE(from_article_id, to_url),
	FOREIGN KEY (from_article_id) REFERENCES article(id) ON DELETE CASCADE
);

-- Fulltext for article content with join column
CREATE VIRTUAL TABLE content_fts USING fts5(
	content,
	content_id UNINDEXED
);

CREATE INDEX idx_revision_article ON revision(article_id);
CREATE INDEX idx_article_namespace_title ON article(namespace_id, title);
CREATE INDEX idx_article_category_category ON article_category(category_id);
CREATE INDEX idx_category_parent ON category(parent_id);
CREATE INDEX idx_revision_edited_at ON revision(edited_at);
CREATE INDEX idx_revision_user ON revision(user_id);
CREATE INDEX idx_article_title ON article(title);
CREATE INDEX idx_article_category_article ON article_category(article_id);

-- Trigger for automatic update of updated_at on new revision
CREATE TRIGGER trg_article_updated_at
AFTER INSERT ON revision
FOR EACH ROW
BEGIN
	UPDATE article
	SET updated_at = CURRENT_TIMESTAMP, revision_id = NEW.id
	WHERE id = NEW.article_id;
END;





CREATE TRIGGER trg_content_fts_insert
AFTER INSERT ON content
BEGIN
  INSERT INTO content_fts(rowid, content, content_id) VALUES (new.id, new.content, new.id);
END;

CREATE TRIGGER trg_content_fts_update
AFTER UPDATE ON content
BEGIN
  UPDATE content_fts SET content = new.content WHERE rowid = new.id;
END;

CREATE TRIGGER trg_content_fts_delete
AFTER DELETE ON content
BEGIN
  DELETE FROM content_fts WHERE rowid = old.id;
END;


```

## TODO

### Support for PostgreSQL storage

### Crow web server producing REST API and serving pure html files

### Support for export to static HTML files

### Review system

* Shows the title of the note.
* Shows: Press ENTER to show content, or type 's' to skip:
* When user presses ENTER, the content of the note is shown.
* Asks: How well do you understand this? (0–5):
* User enters a number (0-5)
* Application updates REVIEW_IN_X_DAYS, EASINESS_FACTOR, REPETITION, EXPIRES_AT
* Continues to the next note

#### SM-2 (SuperMemo 2)

User defines the quality of the knowledge (0-5).

Input values:
* quality (0-5)
* repetition - how many times the note was successfuly reviewed
* interval  - current interval in days
* ef - effectivity factor (initial value 2.5)

```aiignore
struct SM2Result {
    int nextInterval;
    double nextEF;
    int nextRepetition;
};

SM2Result calculateSM2(int quality, int repetition, int interval, double ef) {
    SM2Result result;

    if (quality < 3) {
        result.nextRepetition = 0;
        result.nextInterval = 1;
    } else {
        result.nextRepetition = repetition + 1;
        if (repetition == 0) {
            result.nextInterval = 1;
        } else if (repetition == 1) {
            result.nextInterval = 6;
        } else {
            result.nextInterval = static_cast<int>(interval * ef);
        }
    }

    // Update of EF based on the quality
    double newEF = ef + (0.1 - (5 - quality) * (0.08 + (5 - quality) * 0.02));
    if (newEF < 1.3) newEF = 1.3;

    result.nextEF = newEF;
    return result;
    
    //example auto result = calculateSM2(4, 2, 10, 2.5);
// result.nextInterval = 25
// result.nextEF = 2.46
// result.nextRepetition = 3
}
```

After the calculation:
```
note.REPETITION = result.nextRepetition;
note.INTERVAL = result.nextInterval;
note.EF = result.nextEF;
note.NEXT_REVIEW_AT = now + result.nextInterval * 86400; // v sekundách
note.LAST_QUALITY = quality;
```

```aiignore
 REPETITION

    Starts at 0
    Increases with each successful repetition (quality ≥ 3)
    Helps determine if the interval should be extended

EF (Easiness Factor)

    Starts at 2.5
    Changes based on answer quality 
    Determines how quickly the interval extends (e.g. 10 days × EF)

INTERVAL

    Number of days until next repetition
    E.g. 1 → 6 → 15 → 37 → 90 ...
    Calculated based on EF and number of repetitions

NEXT_REVIEW_AT

    Date (timestamp) when the note should be shown again
    Used for selecting notes for review

LAST_QUALITY

    Last note rating (0-5)
    Can be used for analysis or visualization
```






### Improve the main() function

```
   int main() {
   print_logo();
   if (!migrateSchemaIfNeeded()) return ExitStatus::MIGRATION_FAILED;

   auto db = initializeDatabase();
   auto manager = initializeMiniWikiManager(db);

   if (set_editor_if_needed(db, exit_status)) return exit_status;

   runCommandLoop(manager, factory);
   updateSession(manager);

   return 0;
   }
```



### Multi-user support

```
CREATE TABLE USER (
ID INTEGER PRIMARY KEY AUTOINCREMENT,
USERNAME TEXT NOT NULL UNIQUE,
PASSWORD_HASH TEXT NOT NULL,
DISPLAY_NAME TEXT,
ROLE 0 — ADMIN/EDITOR/MODERATOR/REVIEWER/READER — not null
std::string profile_text
status: pending, active, deactivated, deleted
std::string working_note_id
text_editor_path
mark_down_editor_path

);

Role descriptions:
admin - can do anything, during first application launch mini-wiki creates a user: role=admin username=admin password=admin, if user_can_self_register==false then only admin can create new users, the mini-wiki application during first launch asks admin user to create a second user (password set to empty - not required) (enforcement that there won't be just admin user), then application changes current user to the second user
editor - can make edits
reviewer - has permissions of both editor and moderator
moderator - can only approve proposals from readers or other moderators, but cannot edit content directly  
reader - can only read or create change requests that must be approved

root note with ID 1 and title /system
has child /system/users
has children /system/users/{user_name} - this can only be viewed and edited by the user
notes don't have rwx permissions - instead roles are used: admin, editor, reader

Reader can:

    Send change proposals
    Send new notes or other entities 
    View their own profile note, but cannot edit others

Editor/Admin can edit notes of others

--Suggestions from Readers
CREATE TABLE SUGGESTION (
ID INTEGER PRIMARY KEY AUTOINCREMENT,
PARENT_SUGGESTION_ID INTEGER,
FROM_USER_ID INTEGER,
ENTITY_NAME TEXT,
TYPE TEXT CHECK(TYPE IN ('create', 'update', 'delete')),
STATUS TEXT CHECK(STATUS IN (
'pending', 'under_review', 'approved', 'rejected', 'cancelled', 'requests_feedback' )) DEFAULT 'pending',
DATA_JSONTEXT,
CREATED_AT INTEGER,
UPDATED_AT INTEGER,

    FOREIGN KEY(FROM_USER_ID) REFERENCES USER(ID),
    FOREIGN KEY(PARENT_SUGGESTION_ID) REFERENCES SUGGESTION(ID),
    

);


CREATE TABLE SUGGESTION_REVIEW (
ID INTEGER PRIMARY KEY AUTOINCREMENT,
SUGGESTION_ID INTEGER,
REVIEWER_ID INTEGER,
DECISION_STATUS TEXT CHECK(DECISION_STATUS IN (
'approved', 'rejected', 'cancelled', 'requests_feedback' )) DEFAULT NULL,
COMMENT TEXT,
REVIEWED_AT INTEGER,
FOREIGN KEY(SUGGESTION_ID) REFERENCES SUGGESTION(ID),
FOREIGN KEY(REVIEWER_ID) REFERENCES USER(ID)
);
```

