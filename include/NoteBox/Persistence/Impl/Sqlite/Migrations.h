//
// Created by robertvokac on 3/11/25.
//

#ifndef MIGRATIONS_H
#define MIGRATIONS_H
#include <string>


namespace NoteBox::Persistence::Impl::Sqlite {
    constexpr int MIGRATION_COUNT = 9;
    inline std::string migrations[MIGRATION_COUNT] = {

        R"(
CREATE TABLE LITERATURE_SOURCE (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    TITLE TEXT NOT NULL,
    AUTHOR TEXT,
    YEAR TEXT,
    PUBLICATION TEXT,
    URL TEXT,
    CONTENT TEXT,

    UNIQUE(TITLE, AUTHOR)
);



        )",

        R"(
CREATE TABLE NOTE (
    ID TEXT PRIMARY KEY,           -- for example '/1a', '/1a2b', etc.
    PARENT_NOTE_ID TEXT,                --
    TITLE TEXT NOT NULL,
    CONTENT TEXT,
    QUESTION TEXT,
    NOTE_TYPE TEXT DEFAULT 'standard', -- 'standard', 'index', 'literature'
    CREATED_AT TEXT,               -- ISO datetime (for example '2025-08-04T19:10:00Z')
    UPDATED_AT TEXT,
    LAST_SHOWN_AT TEXT,               -- ISO datetime (for example '2025-08-04T19:10:00Z')
    LAST_REVIEWED_AT TEXT,               -- ISO datetime (for example '2025-08-04T19:10:00Z')
    REVIEW_IN_X_DAYS INTEGER,
    IMPORTANCE INTEGER,
    DIFFICULTY INTEGER,
    SOURCE_ID INTEGER,                -- FK for LITERATURE_SOURCE
    FOREIGN KEY(PARENT_NOTE_ID) REFERENCES NOTE(ID),
    FOREIGN KEY(SOURCE_ID) REFERENCES LITERATURE_SOURCE(ID)
);

        )",

        R"(
CREATE TABLE TAG (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    NAME TEXT NOT NULL UNIQUE
);



        )",
        R"(
CREATE TABLE NOTE_TAG (
    NOTE_ID TEXT NOT NULL,
    TAG_ID INTEGER NOT NULL,
    PRIMARY KEY(NOTE_ID, TAG_ID),
    FOREIGN KEY(NOTE_ID) REFERENCES NOTE(ID),
    FOREIGN KEY(TAG_ID) REFERENCES TAG(ID)
);


        )",
        R"(
CREATE TABLE REFERENCE (
    FROM_NOTE_ID TEXT NOT NULL,
    TO_NOTE_ID TEXT NOT NULL,
    LINK_TYPE TEXT DEFAULT 'related', -- e.g. 'supports', 'contradicts', etc.
    PRIMARY KEY(FROM_NOTE_ID, TO_NOTE_ID),
    FOREIGN KEY(FROM_NOTE_ID) REFERENCES NOTE(ID),
    FOREIGN KEY(TO_NOTE_ID) REFERENCES NOTE(ID)
);




        )",
            R"(
CREATE TABLE SESSION (
    ID INTEGER PRIMARY KEY CHECK (id = 1), -- always only one row
    CURRENT_PATH TEXT NOT NULL,
    LAST_OPENED INTEGER                      -- last exiting time
);




        )",
            R"(
CREATE TABLE HISTORY (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    NOTE_ID TEXT,
    ACTION TEXT,          -- e.g. 'open', 'edit', 'cd'
    TIMESTAMP TEXT,
    FOREIGN KEY(NOTE_ID) REFERENCES NOTE(ID)
);




        )",


        R"(
CREATE TABLE COLLECTION (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    PARENT_COLLECTION_ID INTEGER,
    NAME TEXT NOT NULL,
    DESCRIPTION TEXT,
    ORDER_INDEX INTEGER,

    FOREIGN KEY(PARENT_COLLECTION_ID) REFERENCES COLLECTION(ID)
);

        )",

        R"(

CREATE TABLE COLLECTION_NOTE (
    COLLECTION_ID INTEGER NOT NULL,
    NOTE_ID TEXT NOT NULL,
    ORDER_INDEX INTEGER,

    PRIMARY KEY(COLLECTION_ID, NOTE_ID),
    FOREIGN KEY(COLLECTION_ID) REFERENCES COLLECTION(ID),
    FOREIGN KEY(NOTE_ID) REFERENCES NOTE(ID)
);

        )",

//         R"(
// aaaaaaaa
//         )",



    };
}


#endif //MIGRATIONS_H
