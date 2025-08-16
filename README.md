# note-box

Note Box is a console application used to organize notes in the Zettelkasten like system.

Requirements:
* Linux (Windows or other operating systems are not supported yet) 
  * tested on Debian 13

Used technologies:
* C++17
* SQLite
* CMake

Entities:
* Notes - Tree structure via ID, for example: 34/acegfd
* References
* Collections
* Tags

## Commands
* You can see the list of all commands by typing `help`.
* You can see the help for a command by typing `help [COMMAND]`.

```
add [NOTE_NAME] ... creates a notes as a child of the current one

cd [NOTE_ID] ... navigates to note with NOTE_ID

ls ... lists children

rm [NOTE_ID] ... removes note with given id

pwd ... prints id of the current note

col ls
col add
col edit
col show
col pwd
col cd

exit ... exits the application

quit ... exits the application

help [COMMAND] ... prints info related to the given command

search

edit

show

help

tree

tag

lit ls
lit add
lit edit
lit show

term ls
term ls [CATEGORY]
term add
term add [CATEGORY] [NAME]
term edit
term show
term cat
term rm [CATEGORY] [NAME]
```

## How to build

These are the instructions for Debian 13. 
* For other distributions, you may need to install different packages.


```aiignore
# Install dependencies
apt install cmake g++ libcurl4-openssl-dev
# Install git
apt install git
# Clone the repository
git clone https://github.com/openeggbert/note-box/
# Go to the repository
cd note-box
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
./note-box
```

### Example of Classes

```
Note
NoteManager
NoteRepository
NoteRepositoryImplSqlite
NoteTable
Migrations
```

## TODO

- [ ] TODO-1 Text user interface via ncruses
- [ ] TODO-2 Desktop user interface via QT
- [ ] TODO-3 Support for PostgreSQL storage
- [ ] TODO-4 Crow web server producing REST API and serving pure html files
- [ ] TODO-5 New command ses - starts new subordinate session, user types some commands, typing exit returns user to the previous (parent) session
- [ ] TODO-6 New command ref - manages references
- [ ] TODO-7 New command tag - manages tags
- [ ] TODO-8 New command col - manages collections of notes
- [ ] TODO-9 New command review
- [ ] TODO-10 Custom order of node siblings
- [ ] TODO-11 New table NOTE_PROPERTY
- [ ] TODO-12 New command import
- [ ] TODO-13 New command export
- [ ] TODO-13 New command quiz
- [ ] TODO-14 Improve the main() function
- [ ] TODO-15 New entity System - bool guest_users_are_allowed, bool user_can_self_register
- [ ] TODO-16 Multi-user support
- [ ] TODO-17 Modify table SESSION
- [ ] TODO-18 New entity STARRED_NOTE:  id integer, owner_id integer, note_id integer, added_at integer, visible_to_other_users bool






```
- [ ] TODO-? NAME
```


#### TODO-4 Crow web server producing REST API and serving pure html files

New command server

```aiignore
note-box-web/
├── index.html
├── style.css
└── app.js

📄 index.html

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>NoteBox Web</title>
  <link rel="stylesheet" href="style.css">
</head>
<body>
  <h1>NoteBox</h1>

  <div id="login">
    <input type="text" id="username" placeholder="Username">
    <input type="password" id="password" placeholder="Password">
    <button onclick="login()">Login</button>
  </div>

  <div id="note-form">
    <textarea id="note-content" placeholder="Write a note..."></textarea>
    <button onclick="createNote()">Create Note</button>
  </div>

  <div id="notes"></div>

  <script src="app.js"></script>
</body>
</html>

🎨 style.css

body {
  font-family: sans-serif;
  margin: 20px;
}

textarea {
  width: 100%;
  height: 100px;
  margin-bottom: 10px;
}

⚙️ app.js

let token = null;

function login() {
  const username = document.getElementById("username").value;
  const password = document.getElementById("password").value;

  fetch("/api/login", {
    method: "POST",
    headers: {"Content-Type": "application/json"},
    body: JSON.stringify({username, password})
  })
  .then(res => res.json())
  .then(data => {
    token = data.token;
    loadNotes();
  });
}

function createNote() {
  const content = document.getElementById("note-content").value;

  fetch("/api/notes", {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
      "Authorization": `Bearer ${token}`
    },
    body: JSON.stringify({content})
  })
  .then(() => loadNotes());
}

function loadNotes() {
  fetch("/api/notes", {
    headers: token ? {"Authorization": `Bearer ${token}`} : {}
  })
  .then(res => res.json())
  .then(notes => {
    const container = document.getElementById("notes");
    container.innerHTML = "";
    notes.forEach(note => {
      const div = document.createElement("div");
      div.innerHTML = `
        <p>${note.content}</p>
        ${token ? `
          <button onclick="updateNote(${note.id})">Update</button>
          <button onclick="deleteNote(${note.id})">Delete</button>
        ` : ""}
      `;
      container.appendChild(div);
    });
  });
}

function updateNote(id) {
  const newContent = prompt("New content:");
  if (!newContent) return;

  fetch(`/api/notes/${id}`, {
    method: "PUT",
    headers: {
      "Content-Type": "application/json",
      "Authorization": `Bearer ${token}`
    },
    body: JSON.stringify({content: newContent})
  })
  .then(() => loadNotes());
}

function deleteNote(id) {
  fetch(`/api/notes/${id}`, {
    method: "DELETE",
    headers: {
      "Authorization": `Bearer ${token}`
    }
  })
  .then(() => loadNotes());
}

```


#### TODO-9 New command review

* Shows the title of the note.
* Shows: Press ENTER to show content, or type 's' to skip:
* When user presses ENTER, the content of the note is shown.
* Asks: How well do you understand this? (0–5):
* User enters a number (0-5)
* Application updates REVIEW_IN_X_DAYS, EASINESS_FACTOR, REPETITION, EXPIRES_AT
* Continues to the next note

##### SM-2 (SuperMemo 2)

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

# TODO-10 Custom order of node siblings

* New commands pos
* New column POSITION_SIBLING in the table NOTE, interval is 100

# TODO-11 New table NOTE_PROPERTY

New command prop

```
CREATE TABLE NOTE_PROPERTY (
NOTE_ID TEXT NOT NULL,
KEY TEXT NOT NULL,
VALUE TEXT,
PRIMARY KEY(NOTE_ID, KEY),
FOREIGN KEY(NOTE_ID) REFERENCES NOTE(ID)
);
```




#### TODO-14 Improve the main() function

```
   int main() {
   print_logo();
   if (!migrateSchemaIfNeeded()) return ExitStatus::MIGRATION_FAILED;

   auto db = initializeDatabase();
   auto manager = initializeNoteBoxManager(db);

   if (set_editor_if_needed(db, exit_status)) return exit_status;

   runCommandLoop(manager, factory);
   updateSession(manager);

   return 0;
   }
```



#### TODO-16 Multi-user support

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
admin - can do anything, during first application launch note-box creates a user: role=admin username=admin password=admin, if user_can_self_register==false then only admin can create new users, the note-box application during first launch asks admin user to create a second user (password set to empty - not required) (enforcement that there won't be just admin user), then application changes current user to the second user
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

#### TODO-17 Modify table SESSION

```
CREATE TABLE SESSION (
ID INTEGER PRIMARY KEY
USER_ID INTEGER
STARTED_AT INTEGER
ENDED_AT INTEGER
);
```



```
#### TODO-? Text user interface via ncruses
(Some details about task #1)
```


## Miscellaneous

```aiignore
 CREATE TABLE TASK (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    TITLE TEXT NOT NULL,                    -- stručný název úkolu
    COMMENT TEXT,                           -- popis úkolu
    OWNER_ID INTEGER NOT NULL,              -- kdo úkol vytvořil
    TEAM_ID INTEGER,                        -- tým, ke kterému úkol patří
  CATEGORY TEXT,
    NOTE_ID TEXT,                           -- odkaz na poznámku nebo dokument
    CREATED_AT INTEGER NOT NULL,            -- timestamp vytvoření
    DUE_DATE INTEGER,                       -- deadline
    STATUS TEXT CHECK(STATUS IN ('todo', 'in_progress', 'done', 'archived')) DEFAULT 'todo',
    PRIORITY INTEGER CHECK(PRIORITY BETWEEN 1 AND 5) DEFAULT 3,
    ASSIGNEE_ID INTEGER,                    -- komu je úkol přiřazen
    COMPLETED_AT INTEGER,                   -- kdy byl úkol dokončen
    IS_PRIVATE BOOLEAN DEFAULT 0,           -- zda je úkol viditelný jen pro autora
    TAGS TEXT,                              -- volitelné štítky (např. "urgent,backend")

    FOREIGN KEY(OWNER_ID) REFERENCES USER(ID),
    FOREIGN KEY(ASSIGNEE_ID) REFERENCES USER(ID),
    FOREIGN KEY(TEAM_ID) REFERENCES TEAM(ID)
);

CREATE TABLE MESSAGE (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    OWNER_ID INTEGER NOT NULL,
    SENDER_ID INTEGER NOT NULL,
    RECIPIENT_ID INTEGER NOT NULL,
    SUBJECT TEXT,
    BODY TEXT NOT NULL,
    SENT_AT INTEGER,
    SYSTEM_MESSAGE BOOLEAN DEFAULT 0,
    DRAFT BOOLEAN DEFAULT 0,
    IS_READ BOOLEAN DEFAULT 0,
    IN_TRASH BOOLEAN DEFAULT 0,
    STARRED BOOLEAN DEFAULT 0,

    FOREIGN KEY(SENDER_ID) REFERENCES USER(ID),
    FOREIGN KEY(RECIPIENT_ID) REFERENCES USER(ID),
    FOREIGN KEY(OWNER_ID) REFERENCES USER(ID)
);

CREATE TABLE TEAM (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    NAME TEXT NOT NULL,
    DESCRIPTION TEXT,
    CREATED_BY INTEGER NOT NULL,
          LEADER_ID INTEGER NOT NULL,
   CREATED_AT INTEGER NOT NULL,    
   FOREIGN KEY(CREATED_BY) REFERENCES USER(ID) ); 

CREATE TABLE TEAM_MEMBER (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    TEAM_ID INTEGER NOT NULL,
    USER_ID INTEGER NOT NULL,
    ROLE TEXT DEFAULT 'member', -- např. 'admin', 'editor', 'viewer'
    JOINED_AT INTEGER NOT NULL,
    IS_ACTIVE BOOLEAN DEFAULT 1,
    FOREIGN KEY(TEAM_ID) REFERENCES TEAM(ID),
    FOREIGN KEY(USER_ID) REFERENCES USER(ID)
);

CREATE TABLE DISCUSSION (
  ID INTEGER PRIMARY KEY AUTOINCREMENT,
  TEAM_ID INTEGER NOT NULL,
  TITLE TEXT NOT NULL,
  CREATED_BY INTEGER NOT NULL,
  CREATED_AT DATETIME DEFAULT CURRENT_TIMESTAMP,
  IS_PINNED BOOLEAN DEFAULT 0,
  FOREIGN KEY (TEAM_ID) REFERENCES TEAM(ID),
  FOREIGN KEY (CREATED_BY) REFERENCES USER(ID)
);

CREATE TABLE COMMENT (
  ID INTEGER PRIMARY KEY AUTOINCREMENT,
  DISCUSSION_ID INTEGER NOT NULL,
  USER_ID INTEGER NOT NULL,
  CONTENT TEXT NOT NULL,
  PARENT_ID INTEGER, 
  CREATED_AT DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (DISCUSSION_ID) REFERENCES DISCUSSION(ID),
  FOREIGN KEY (USER_ID) REFERENCES USER(ID)
);

```