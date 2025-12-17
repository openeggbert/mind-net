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
    disambiguation TEXT,
    definition TEXT,
    importance INTEGER NOT NULL,
    difficulty INTEGER NOT NULL,

    UNIQUE(dictionary_map_id, title, disambiguation),

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
    dictionary_map_id INTEGER NOT NULL,

    FOREIGN KEY(dictionary_term_id) REFERENCES dictionary_term(id),
    FOREIGN KEY(user_id) REFERENCES user(id),
    FOREIGN KEY(dictionary_map_id) REFERENCES dictionary_map(id)
);

CREATE INDEX idx_dictionary_term_visit_term ON dictionary_term_visit(dictionary_term_id);
CREATE INDEX idx_dictionary_term_visit_user ON dictionary_term_visit(user_id);
CREATE INDEX idx_dictionary_term_visit_map ON dictionary_term_visit(dictionary_map_id);
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

        //
        // V8 — dictionary_flag
        //
        add_migration("V8__create_dictionary_flag.sql", R"(
CREATE TABLE dictionary_flag (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    dictionary_term_id INTEGER NOT NULL,
    user_id INTEGER NOT NULL,
    title TEXT NOT NULL,
    is_public INTEGER NOT NULL CHECK(is_public IN (0,1)),

    UNIQUE(dictionary_term_id, user_id, title),

    FOREIGN KEY(dictionary_term_id) REFERENCES dictionary_term(id),
    FOREIGN KEY(user_id) REFERENCES user(id)
);

CREATE INDEX idx_dictionary_flag_term ON dictionary_flag(dictionary_term_id);
CREATE INDEX idx_dictionary_flag_user ON dictionary_flag(user_id);
)");

        //
        // V9 — dictionary_review
        //
        add_migration("V9__create_dictionary_review.sql", R"(
CREATE TABLE dictionary_review (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    user_id INTEGER NOT NULL,
    dictionary_map_id INTEGER NOT NULL,
    dictionary_term_id INTEGER NOT NULL,
    algorithm TEXT NOT NULL,

    review_date DATETIME NOT NULL,
    grade INTEGER NOT NULL,

    started_at DATETIME,
    ended_at DATETIME,
    latency_ms INTEGER,

    answer_change_count INTEGER DEFAULT 0,
    details_json TEXT,

    FOREIGN KEY(user_id) REFERENCES user(id),
    FOREIGN KEY(dictionary_map_id) REFERENCES dictionary_map(id),
    FOREIGN KEY(dictionary_term_id) REFERENCES dictionary_term(id)
);

CREATE INDEX idx_dictionary_review_user ON dictionary_review(user_id);
CREATE INDEX idx_dictionary_review_term ON dictionary_review(dictionary_term_id);
CREATE INDEX idx_dictionary_review_date ON dictionary_review(review_date);
)");

        //
        // V10 — dictionary_source_type
        //
        add_migration("V10__create_dictionary_source_type.sql", R"(
CREATE TABLE dictionary_source_type (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    title TEXT NOT NULL,
    author TEXT,
    year INTEGER,
    publisher TEXT,
    edition TEXT,
    pages INTEGER,
    url TEXT,
    type TEXT,
    note TEXT,

    UNIQUE(title, edition)
);

CREATE INDEX idx_dictionary_source_type_title
    ON dictionary_source_type(title);
)");

        //
        // V11 — dictionary_source
        //
        add_migration("V11__create_dictionary_source.sql", R"(
CREATE TABLE dictionary_source (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    dictionary_term_id INTEGER NOT NULL,
    dictionary_source_type_id INTEGER NOT NULL,

    page TEXT,
    note TEXT,

    UNIQUE(dictionary_term_id, dictionary_source_type_id, page)

    FOREIGN KEY(dictionary_term_id) REFERENCES dictionary_term(id),
    FOREIGN KEY(dictionary_source_type_id) REFERENCES dictionary_source_type(id)
);

CREATE INDEX idx_dictionary_source_term
    ON dictionary_source(dictionary_term_id);
CREATE INDEX idx_dictionary_source_type
    ON dictionary_source(dictionary_source_type_id);
)");



        //
        // V12 — dictionary_state_4
        //
        add_migration("V12__create_dictionary_state_4.sql", R"(
CREATE TABLE dictionary_state_4 (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    user_id INTEGER NOT NULL,
    dictionary_term_id INTEGER NOT NULL,

    repetitions INTEGER NOT NULL DEFAULT 0,
    interval INTEGER NOT NULL DEFAULT 0,
    ef_times_100 INTEGER NOT NULL DEFAULT 250,
    correction_factor_times_100 INTEGER NOT NULL DEFAULT 100,

    next_review DATETIME,
    last_review DATETIME,
    last_quality INTEGER,

    last_seen_semantic_version INTEGER,
    content_modified_since_last_review INTEGER NOT NULL CHECK(content_modified_since_last_review IN (0,1)),

    UNIQUE(user_id, dictionary_term_id),

    FOREIGN KEY(user_id) REFERENCES user(id),
    FOREIGN KEY(dictionary_term_id) REFERENCES dictionary_term(id)
);

CREATE INDEX idx_dictionary_state_user
    ON dictionary_state_4(user_id);
CREATE INDEX idx_dictionary_state_term
    ON dictionary_state_4(dictionary_term_id);
)");

        //
        // V13 — dictionary_term_alias
        //
        add_migration("V13__create_dictionary_term_alias.sql", R"(
CREATE TABLE dictionary_term_alias (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    dictionary_term_id INTEGER NOT NULL,
    alias TEXT NOT NULL,

    UNIQUE(dictionary_term_id, alias),

    FOREIGN KEY(dictionary_term_id) REFERENCES dictionary_term(id)
);

CREATE INDEX idx_dictionary_term_alias_term
    ON dictionary_term_alias(dictionary_term_id);
)");


        //
        // V14 — dictionary_index_type
        //
        add_migration("V14__create_dictionary_index_type.sql", R"(
CREATE TABLE dictionary_index_type (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    dictionary_map_id INTEGER NOT NULL,
    title TEXT NOT NULL,
    description TEXT,
    position INTEGER,

    UNIQUE(dictionary_map_id, title),

    FOREIGN KEY(dictionary_map_id) REFERENCES dictionary_map(id)
);

CREATE INDEX idx_dictionary_index_type_map
    ON dictionary_index_type(dictionary_map_id);

CREATE INDEX idx_dictionary_index_type_position
    ON dictionary_index_type(position);
)");


        //
        // V15 — dictionary_index
        //
        add_migration("V15__create_dictionary_index.sql", R"(
CREATE TABLE dictionary_index (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    dictionary_index_type_id INTEGER NOT NULL,
    dictionary_term_id INTEGER NOT NULL,

    position INTEGER,

    is_entry_point INTEGER NOT NULL DEFAULT 0 CHECK(is_entry_point IN (0, 1)),

    UNIQUE(dictionary_index_type_id, dictionary_term_id),

    FOREIGN KEY(dictionary_index_type_id) REFERENCES dictionary_index_type(id),
    FOREIGN KEY(dictionary_term_id) REFERENCES dictionary_term(id)
);

CREATE INDEX idx_dictionary_index_type
    ON dictionary_index(dictionary_index_type_id);

CREATE INDEX idx_dictionary_index_term
    ON dictionary_index(dictionary_term_id);

CREATE INDEX idx_dictionary_index_position
    ON dictionary_index(dictionary_index_type_id, position);
)");


    }
}
