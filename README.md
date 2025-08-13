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
```

## Development

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