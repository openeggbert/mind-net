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
        migrations = {
        R"(
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
)",
        R"(
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
)",
        R"(
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
)",
        R"(
CREATE TABLE note (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
	map_id INTEGER NOT NULL,
    title TEXT NOT NULL,
    parent_note_id INTEGER,
    content_id INTEGER UNIQUE,
    sibling_position INTEGER NOT NULL,
    importance INTEGER DEFAULT 0,
    difficulty INTEGER DEFAULT 0,

    FOREIGN KEY (map_id) REFERENCES map(id) ,
	FOREIGN KEY (content_id) REFERENCES content(id) ON DELETE SET NULL,
    FOREIGN KEY (parent_note_id) REFERENCES note(id)
);

CREATE INDEX idx_note_content_id ON note(content_id);
CREATE INDEX idx_note_map_id ON note(map_id);
)",
        R"(
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

)",
        R"(
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
)",
        R"(
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

)",
        R"(
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
)",
        R"(
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
)",
        R"(
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
        )",

        R"(
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

)",
        R"(
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
)",

        };
    }
}
