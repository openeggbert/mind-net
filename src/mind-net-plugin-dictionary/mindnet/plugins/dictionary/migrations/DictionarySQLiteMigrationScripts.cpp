/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 */

#include "mindnet/plugins/dictionary/migrations/DictionarySQLiteMigrationScripts.hpp"

namespace mindnet::plugins::dictionary::migrations
{
    DictionarySQLiteMigrationScripts::DictionarySQLiteMigrationScripts()
        : MigrationScripts(mindnet::essential::DatabaseType::SQLite)
    {
    }

    void DictionarySQLiteMigrationScripts::define_migrations()
    {
        //
        // V1 — dictionary_map
        //
        add_migration("V1__create_dictionary_map.sql", R"(
CREATE TABLE dictionary_map (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    name TEXT NOT NULL UNIQUE,
    description TEXT,
    position INTEGER,

    owner_id INTEGER NOT NULL,
    team_id INTEGER,
    owner_rights INTEGER NOT NULL CHECK(owner_rights BETWEEN 0 AND 7),
    team_rights INTEGER NOT NULL CHECK(team_rights BETWEEN 0 AND 7),
    other_rights INTEGER NOT NULL CHECK(other_rights BETWEEN 0 AND 7),

    FOREIGN KEY(owner_id) REFERENCES user(id),
    FOREIGN KEY(team_id) REFERENCES team(id)
);

CREATE INDEX idx_dictionary_map_owner_id ON dictionary_map(owner_id);
CREATE INDEX idx_dictionary_map_team_id ON dictionary_map(team_id);
)");


        //
        // V2 — dictionary_term
        //
        add_migration("V2__create_dictionary_term.sql", R"(
CREATE TABLE dictionary_term (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    dictionary_map_id INTEGER NOT NULL,
    title TEXT NOT NULL,
    definition TEXT,
    difficulty INTEGER NOT NULL,

    UNIQUE(dictionary_map_id, title),

    FOREIGN KEY(dictionary_map_id) REFERENCES dictionary_map(id)
);

CREATE INDEX idx_dictionary_term_map ON dictionary_term(dictionary_map_id);
CREATE INDEX idx_dictionary_term_title ON dictionary_term(title);
)");

        //
        // V3 — dictionary_term_visit
        //
        add_migration("V3__create_dictionary_term_visit.sql", R"(
CREATE TABLE dictionary_term_visit (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    dictionary_term_id INTEGER NOT NULL,
    user_id INTEGER NOT NULL,

    FOREIGN KEY(dictionary_term_id) REFERENCES dictionary_term(id),
    FOREIGN KEY(user_id) REFERENCES user(id)
);

CREATE INDEX idx_dictionary_term_visit_term ON dictionary_term_visit(dictionary_term_id);
CREATE INDEX idx_dictionary_term_visit_user ON dictionary_term_visit(user_id);
)");


        //
        // V4 — dictionary_link
        //
        add_migration("V4__create_dictionary_link.sql", R"(
CREATE TABLE dictionary_link (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    from_dictionary_term_id INTEGER NOT NULL,
    to_dictionary_term_id INTEGER NOT NULL,

    CHECK(from_dictionary_term_id <> to_dictionary_term_id),
    UNIQUE(from_dictionary_term_id, to_dictionary_term_id),

    FOREIGN KEY(from_dictionary_term_id) REFERENCES dictionary_term(id),
    FOREIGN KEY(to_dictionary_term_id) REFERENCES dictionary_term(id)
);

CREATE INDEX idx_dictionary_link_from ON dictionary_link(from_dictionary_term_id);
CREATE INDEX idx_dictionary_link_to ON dictionary_link(to_dictionary_term_id);
)");


        //
        // V5 — dictionary_note
        //
        add_migration("V5__create_dictionary_note.sql", R"(
CREATE TABLE dictionary_note (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    dictionary_term_id INTEGER NOT NULL,
    title TEXT NOT NULL,
    content TEXT,
    position INTEGER,

    UNIQUE(dictionary_term_id, title),

    FOREIGN KEY(dictionary_term_id) REFERENCES dictionary_term(id)
);

CREATE INDEX idx_dictionary_note_term ON dictionary_note(dictionary_term_id);
CREATE INDEX idx_dictionary_note_position ON dictionary_note(position);
)");

        //
        // V6 — dictionary_tag_type
        //
        add_migration("V6__create_dictionary_tag_type.sql", R"(
CREATE TABLE dictionary_tag_type (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    dictionary_map_id INTEGER NOT NULL,
    title TEXT NOT NULL,

    UNIQUE(dictionary_map_id, title),

    FOREIGN KEY(dictionary_map_id) REFERENCES dictionary_map(id)
);

CREATE INDEX idx_dictionary_tag_type_map ON dictionary_tag_type(dictionary_map_id);
)");

        //
        // V7 — dictionary_tag
        //
        add_migration("V7__create_dictionary_tag.sql", R"(
CREATE TABLE dictionary_tag (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    dictionary_term_id INTEGER NOT NULL,
    dictionary_tag_type_id INTEGER NOT NULL,

    UNIQUE(dictionary_term_id, dictionary_tag_type_id),

    FOREIGN KEY(dictionary_term_id) REFERENCES dictionary_term(id),
    FOREIGN KEY(dictionary_tag_type_id) REFERENCES dictionary_tag_type(id)
);

CREATE INDEX idx_dictionary_tag_term ON dictionary_tag(dictionary_term_id);
CREATE INDEX idx_dictionary_tag_type ON dictionary_tag(dictionary_tag_type_id);
)");
    }
}
