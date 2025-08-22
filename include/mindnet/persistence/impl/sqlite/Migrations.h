//
// Created by robertvokac on 3/11/25.
//

#ifndef MIGRATIONS_H
#define MIGRATIONS_H
#include <string>
#include <climits>


namespace mindnet::persistence::impl::sqlite {constexpr int MIGRATION_COUNT = 11;
    inline std::string migrations[MIGRATION_COUNT] = {

    	R"(
CREATE TABLE history (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	table_name TEXT NOT NULL,
	record_id INTEGER NOT NULL,
	operation INTEGER NOT NULL CHECK (operation IN (1, 2, 3, 4, 5)),
	payload TEXT NOT NULL,
    reason TEXT DEFAULT NULL
);
        )",



        R"(
CREATE TABLE map (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	name TEXT NOT NULL UNIQUE,
	description TEXT,
    category TEXT DEFAULT NULL
);
        )",

    	R"(
CREATE TABLE node (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    uuid TEXT NOT NULL UNIQUE,
	map_id INTEGER NOT NULL,
    sibling_position INTEGER NOT NULL,
	title TEXT NOT NULL,
	content_id INTEGER,
    parent_node_id INTEGER,
    type INTEGER DEFAULT 0 CHECK (type IN (0,1)),
    visibility INTEGER DEFAULT 0 CHECK (visibility IN (0, 1, 2, 3)),
    last_shown_at DATETIME DEFAULT NULL,
    expires_at DATETIME DEFAULT NULL,
    is_favorite BOOLEAN DEFAULT 0,
	is_redirect BOOLEAN DEFAULT 0,
	redirect_node_id INTEGER,
	redirect_reason TEXT,
    importance INTEGER DEFAULT 0 CHECK (importance IN (0, 1, 2, 3)),
    difficulty INTEGER DEFAULT 0 CHECK (difficulty IN (0, 1, 2, 3, 4)),
	CHECK ( (is_redirect = 0 AND redirect_node_id IS NULL) OR (is_redirect = 1 AND redirect_node_id IS NOT NULL) ),

    FOREIGN KEY (map_id) REFERENCES map(id) ON DELETE CASCADE,
	FOREIGN KEY (content_id) REFERENCES content(id) ON DELETE SET NULL,
    FOREIGN KEY (parent_node_id) REFERENCES node(id) ON DELETE CASCADE,
	FOREIGN KEY (redirect_node_id) REFERENCES node(id) ON DELETE SET NULL

);

        )",

    	R"(
CREATE TABLE content (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	content TEXT NOT NULL,
	format INTEGER CHECK (format IN (0, 1, 2)),
    version INTEGER DEFAULT 1,
    node_id INTEGER,

	FOREIGN KEY (node_id) REFERENCES node(id) ON DELETE CASCADE
);
)",



        R"(
CREATE TABLE node_property(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    map_id INTEGER NOT NULL,
	node_id INTEGER NOT NULL,
	key TEXT NOT NULL,
	value TEXT,
    value_type INTEGER DEFAULT 0 CHECK (value_type in (0, 1, 2, 3)),
    is_indexed BOOLEAN DEFAULT 0,
	FOREIGN KEY (map_id) REFERENCES map(id) ON DELETE CASCADE,
    FOREIGN KEY (node_id) REFERENCES node(id) ON DELETE CASCADE,
	unique (map_id, node_id, key)
);


        )",
        R"(
CREATE TABLE tag (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    map_id INTEGER NOT NULL,
	title TEXT NOT NULL,
	FOREIGN KEY (map_id) REFERENCES map(id) ON DELETE CASCADE,
    UNIQUE(map_id, title)
);


        )",
        R"(
CREATE TABLE node_tag (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	node_id INTEGER NOT NULL,
	tag_id INTEGER NOT NULL,
	UNIQUE (node_id, tag_id),
	FOREIGN KEY (node_id) REFERENCES node(id) ON DELETE CASCADE,
	FOREIGN KEY (tag_id) REFERENCES tag(id) ON DELETE CASCADE
);


        )",
        R"(
CREATE TABLE node_link(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	from_node_id INTEGER NOT NULL,
	to_node_id INTEGER NOT NULL,
    label TEXT,
	CHECK (from_node_id <> to_node_id),
    UNIQUE (from_node_id, to_node_id),
	FOREIGN KEY (from_node_id) REFERENCES node(id) ON DELETE CASCADE,
	FOREIGN KEY (to_node_id) REFERENCES node(id) ON DELETE CASCADE

);


        )",

        R"(
CREATE TABLE external_link(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	from_node_id INTEGER NOT NULL,
	to_url TEXT NOT NULL,
	UNIQUE(from_node_id, to_url),
	FOREIGN KEY (from_node_id) REFERENCES node(id) ON DELETE CASCADE
);


        )",

    	        R"(
    	CREATE INDEX idx_node_map_id ON node(map_id);
    	CREATE INDEX idx_content_node_id ON content(node_id);
    	CREATE INDEX idx_node_tag_node_id ON node_tag(node_id);
    	CREATE INDEX idx_node_type ON node(type);
    	CREATE INDEX idx_node_property_key ON node_property(key);
        CREATE INDEX idx_history_operation ON history(operation);

CREATE INDEX idx_node_parent_id ON node(parent_node_id);
CREATE INDEX idx_node_shown_expires ON node(last_shown_at, expires_at);
CREATE INDEX idx_node_title ON node(title);
CREATE INDEX idx_tag_title ON tag(title);
CREATE INDEX idx_node_property_value ON node_property(value);
CREATE INDEX idx_node_property_key_value ON node_property(key, value);
CREATE INDEX idx_node_link_from_to ON node_link(from_node_id, to_node_id);
CREATE INDEX idx_external_link_url ON external_link(to_url);


		        )",
R"(
CREATE VIRTUAL TABLE content_fts USING fts5(
    content,
    format UNINDEXED,
    version UNINDEXED,
    node_id UNINDEXED,
    content='content',
    tokenize = 'unicode61'
);

CREATE TRIGGER content_ai AFTER INSERT ON content BEGIN
  INSERT INTO content_fts(rowid, content) VALUES (new.id, new.content);
END;

CREATE TRIGGER content_ad AFTER DELETE ON content BEGIN
  DELETE FROM content_fts WHERE rowid = old.id;
END;

CREATE TRIGGER content_au AFTER UPDATE ON content BEGIN
  UPDATE content_fts SET content = new.content WHERE rowid = old.id;
END;


)",





//         R"(
// aaaaaaaa
//         )",



    };
}


#endif //MIGRATIONS_H
