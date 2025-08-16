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

### TODO

- [ ] TODO-1 Text user interface via ncruses
- [ ] TODO-2 Desktop user interface via QT
- [ ] TODO-3 Support for PostgreSQL storage
- [ ] TODO-4 Crow web server producing REST API and serving pure html files
- [ ] TODO-5 new command ses - starts new subordinate session, user types some commands, typing exit returns user to the previous (parent) session
- [ ] TODO-6 new command ref - manages references
- [ ] TODO-7 new command tag - manages tags
- [ ] TODO-? NAME
- [ ] TODO-? NAME
- [ ] TODO-? NAME
- [ ] TODO-? NAME
- [ ] TODO-? NAME
- [ ] TODO-? NAME
- [ ] TODO-? NAME

#### TODO-? Text user interface via ncruses
(Some details about task #1)

