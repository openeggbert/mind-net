//
// Created by robertvokac on 9/17/25.
//

#include "mindnet/plugins/slipbox/migrations/SlipBoxSQLiteMigrationScripts.h"

namespace mindnet::plugins::slipbox::migrations
{
    SlipBoxSQLiteMigrationScripts::SlipBoxSQLiteMigrationScripts() : MigrationScripts(DatabaseType::SQLite)
    {
    }

    void SlipBoxSQLiteMigrationScripts::define_migrations()
    {
        add_migration("V1__create_map.sql", R"(
CREATE TABLE map (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
	name TEXT NOT NULL UNIQUE,
	description TEXT,
    category TEXT,
    --
    owner_id INTEGER NOT NULL,
    team_id INTEGER,
    owner_rights INTEGER CHECK (owner_rights >= 0 and owner_rights <= 7),
    team_rights INTEGER CHECK (team_rights >= 0 and team_rights <= 7),
    other_rights INTEGER CHECK (other_rights >= 0 and other_rights <= 7),
    --

	FOREIGN KEY(owner_id) REFERENCES user(id),
    FOREIGN KEY(team_id) REFERENCES team(id)
);
)");
        add_migration("V2__create_content.sql", R"(
CREATE TABLE content (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
	value TEXT NOT NULL,
	format INTEGER DEFAULT 0,
    version INTEGER DEFAULT 1
);
CREATE INDEX idx_content_value ON content(value);
)");
        add_migration("V3__create_content_fts.sql", R"(
CREATE VIRTUAL TABLE content_fts USING fts5(
    value,
    format UNINDEXED,
    version UNINDEXED,
    tokenize = 'unicode61'
);

CREATE TRIGGER content_ai AFTER INSERT ON content BEGIN
  INSERT INTO content_fts(rowid, value) VALUES (new.id, new.value);
END;

CREATE TRIGGER content_ad AFTER DELETE ON content BEGIN
  DELETE FROM content_fts WHERE rowid = old.id;
END;

CREATE TRIGGER content_au AFTER UPDATE ON content BEGIN
  UPDATE content_fts SET value = new.value WHERE rowid = old.id;
END;
)");
        add_migration("V4__create_note.sql", R"(
CREATE TABLE note (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
	map_id INTEGER NOT NULL,
    parent_note_id INTEGER,
    content_id INTEGER UNIQUE,
    source_id INTEGER,
    title TEXT NOT NULL,
    sibling_order INTEGER NOT NULL,
    importance INTEGER DEFAULT 0,
    difficulty INTEGER DEFAULT 0,

    FOREIGN KEY (map_id) REFERENCES map(id),
    FOREIGN KEY (parent_note_id) REFERENCES note(id),
    FOREIGN KEY (content_id) REFERENCES content(id),
    FOREIGN KEY (source_id) REFERENCES source(id)
);

--Indexes
CREATE INDEX idx_note_map_id ON note(map_id);
CREATE INDEX idx_note_parent_note_id ON note(parent_note_id);
CREATE INDEX idx_note_content_id ON note(content_id);
CREATE INDEX idx_note_source_id ON note(source_id);

CREATE INDEX idx_note_parent_sibling ON note(parent_note_id, sibling_order);

)");
        add_migration("V5__create_property.sql", R"(
CREATE TABLE property(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
    map_id INTEGER NOT NULL,
	note_id INTEGER NOT NULL,
	key TEXT NOT NULL,
	value TEXT,

	FOREIGN KEY (map_id) REFERENCES map(id) ,
    FOREIGN KEY (note_id) REFERENCES note(id) ,
	UNIQUE (map_id, note_id, key)
);

CREATE INDEX idx_property_map_note_key ON property(map_id, note_id, key);

)");
        add_migration("V6__create_tag_type.sql", R"(
CREATE TABLE tag_type (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
    map_id INTEGER NOT NULL,
	title TEXT NOT NULL,

	FOREIGN KEY (map_id) REFERENCES map(id) ,
    UNIQUE(map_id, title)
);
)");
        add_migration("V7__create_tag.sql", R"(
CREATE TABLE tag (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
	note_id INTEGER NOT NULL,
	tag_type_id INTEGER NOT NULL,

	UNIQUE (note_id, tag_type_id),

	FOREIGN KEY (note_id) REFERENCES note(id) ,
	FOREIGN KEY (tag_type_id) REFERENCES tag_type(id)
);

CREATE INDEX idx_tag_note_id ON tag(note_id);

)");
        add_migration("V8__create_collection.sql", R"(
CREATE TABLE collection (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
	name TEXT NOT NULL,
	description TEXT,
	order_index INTEGER,
    created_by INTEGER,
    is_public BOOLEAN DEFAULT 0,

    FOREIGN KEY (created_by) REFERENCES user(id)
);
)");
        add_migration("V9__create_collection_item.sql", R"(
CREATE TABLE collection_item (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
	collection_id INTEGER NOT NULL,
	note_id INTEGER NOT NULL,
	order_index INTEGER,

	UNIQUE(collection_id, note_id),

	FOREIGN KEY(collection_id) REFERENCES collection(id),
	FOREIGN KEY(note_id) REFERENCES note(id)
);
)");
        add_migration("V10__create_question.sql", R"(
        		CREATE TABLE question (
        	id INTEGER PRIMARY KEY AUTOINCREMENT,
        	created_at DATETIME ,
        	updated_at DATETIME ,
--
        			note_id INTEGER,
        			question_text TEXT NOT NULL,
        			answers_json TEXT, -- answers as a json object
                    FOREIGN KEY (note_id) REFERENCES note(id)
        		);

            	--answers_json
        		--[
        		--  { "text": "std::vector", "is_correct": true },
        		--  { "text": "std::map", "is_correct": false },
        		--  { "text": "std::set", "is_correct": false }
        		--]
        )");

        add_migration("V11__create_reference.sql", R"(
CREATE TABLE reference(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME ,
    updated_at DATETIME ,
    --
	from_note_id INTEGER NOT NULL,
	to_note_id INTEGER NOT NULL,
    label TEXT,

	CHECK (from_note_id <> to_note_id),
    UNIQUE (from_note_id, to_note_id),

	FOREIGN KEY (from_note_id) REFERENCES note(id) ,
	FOREIGN KEY (to_note_id) REFERENCES note(id)
);

CREATE INDEX idx_reference_from_to ON reference(from_note_id, to_note_id);

)");
        add_migration("V12__create_link.sql", R"(
CREATE TABLE link(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME ,
    updated_at DATETIME ,
    --
	from_note_id INTEGER NOT NULL,
	to_url TEXT NOT NULL,

	UNIQUE(from_note_id, to_url),

	FOREIGN KEY (from_note_id) REFERENCES note(id)
);

CREATE INDEX idx_link_from_note ON link(from_note_id);
)");


        add_migration("V13__create_concept.sql", R"(
CREATE TABLE concept(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME ,
    updated_at DATETIME ,
    --
    map_id INTEGER NOT NULL,
    note_id INTEGER,
	title TEXT NOT NULL,
	disambiguation TEXT,

	UNIQUE(map_id, title, disambiguation),

	FOREIGN KEY (note_id) REFERENCES note(id),
    FOREIGN KEY (map_id) REFERENCES map(id)
);

CREATE INDEX idx_concept_note ON concept(note_id);
CREATE INDEX idx_concept_title ON concept(title);
)");

        add_migration("V14__create_source.sql", R"(
CREATE TABLE source(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME ,
    updated_at DATETIME ,
    --
    map_id INTEGER NOT NULL,
	title TEXT NOT NULL,
	author TEXT,
	year INTEGER,
	publisher TEXT,
    edition TEXT,
    pages TEXT,
	url TEXT,
    type INTEGER NOT NULL,

	UNIQUE(map_id, title),

    FOREIGN KEY (map_id) REFERENCES map(id)
);

CREATE INDEX idx_source_title ON source(title);
)");


        add_migration("V15__create_idea.sql", R"(
CREATE TABLE idea(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME ,
    updated_at DATETIME ,
    --
    user_id INTEGER NOT NULL,
    title TEXT NOT NULL,
	content TEXT,
    category TEXT,
    due_at INTEGER,
	is_important BOOL,
	is_public BOOL,
    is_pinned BOOL,

    FOREIGN KEY (user_id) REFERENCES user(id)
);

CREATE INDEX idx_idea_title ON idea(title);
)");


    	add_migration("V16__create_wanted_note.sql", R"(
CREATE TABLE wanted_note(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME ,
    updated_at DATETIME ,
    --
	from_note_id INTEGER NOT NULL,
	to_note_title TEXT NOT NULL,

    UNIQUE (from_note_id, to_note_title),

	FOREIGN KEY (from_note_id) REFERENCES note(id)
);

CREATE INDEX idx_wanted_note_from_note_id ON wanted_note(from_note_id);

)");
    }
}
