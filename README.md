# note-box

## Organization of notes

### Tree structure

Via ID

For example: 34/acegfd

### References

### Collections

### Tags

## Commands

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

## Development

```aiignore
apt install libcurl4-openssl-dev
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

- Text user interface via ncruses
- Desktop user interface via QT
- Support for PostgreSQL storage
- Crow web server producing REST API and serving pure html files