//
// Created by robertvokac on 3/11/25.
//

#ifndef MIGRATIONS_H
#define MIGRATIONS_H
#include <string>
#include <climits>


namespace mindnet::persistence::impl::sqlite {constexpr int MIGRATION_COUNT = 10;
    inline std::string migrations[MIGRATION_COUNT] = {

    	R"(
    	CREATE TABLE history (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	table_name TEXT NOT NULL,
	record_id INTEGER NOT NULL,
	operation TEXT NOT NULL CHECK (operation IN ('create', 'update', 'delete')),
	payload TEXT NOT NULL,
	performed_by TEXT,
	performed_at DATETIME DEFAULT CURRENT_TIMESTAMP
);
        )",



        R"(
CREATE TABLE map (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	name TEXT NOT NULL UNIQUE,
	description TEXT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);
        )",

    	R"(
CREATE TABLE node (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    uuid TEXT NOT NULL UNIQUE,
	map_id INTEGER NOT NULL,
	title TEXT NOT NULL,
	content_id INTEGER,
    parent_node_id INTEGER,
    type TEXT DEFAULT 'generic', -- for example: 'term', 'concept', 'category', 'redirect'
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    shown_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	is_redirect BOOLEAN DEFAULT 0,
	redirect_node_id INTEGER,
	redirect_reason TEXT,
	CHECK ( (is_redirect = 0 AND redirect_node_id IS NULL) OR (is_redirect = 1 AND redirect_node_id IS NOT NULL) ),

    FOREIGN KEY (map_id) REFERENCES map(id) ON DELETE CASCADE,
	FOREIGN KEY (content_id) REFERENCES content(id) ON DELETE SET NULL,
    FOREIGN KEY (parent_node_id) REFERENCES node(id) ON DELETE SET NULL,
	FOREIGN KEY (redirect_node_id) REFERENCES node(id) ON DELETE SET NULL

);

        )",

    	R"(
CREATE TABLE content (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	content TEXT NOT NULL,
	format INTEGER,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    node_id INTEGER,
    reverted_from_content_id INTEGER,

	FOREIGN KEY (node_id) REFERENCES node(id) ON DELETE CASCADE,
	FOREIGN KEY (reverted_from_content_id) REFERENCES content(id) ON DELETE CASCADE
);
)",



        R"(
CREATE TABLE node_property(
    id INTEGER PRIMARY KEY,
    map_id INTEGER,
	node_id INTEGER,
	key TEXT,
	value TEXT,
    value_type TEXT DEFAULT 'string', -- for example: 'string', 'number', 'boolean', 'date'
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	FOREIGN KEY (map_id) REFERENCES map(id) ON DELETE CASCADE,
    FOREIGN KEY (node_id) REFERENCES node(id) ON DELETE CASCADE,
	unique (map_id, node_id, key)
);


        )",
        R"(
CREATE TABLE tag (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    map_id INTEGER,
	title TEXT NOT NULL UNIQUE,
	FOREIGN KEY (map_id) REFERENCES MAP(id) ON DELETE SET NULL,
    UNIQUE(map_id, title)
);


        )",
        R"(
CREATE TABLE node_tag (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	node_id INTEGER NOT NULL,
	tag_id INTEGER NOT NULL,
	UNIQUE (node_id, tag_id),
	FOREIGN KEY (node_id) REFERENCES node(id) ON DELETE CASCADE,
	FOREIGN KEY (tag_id) REFERENCES tag(id) ON DELETE CASCADE
);


        )",
        R"(
CREATE TABLE node_link(
	from_node_id INTEGER,
	to_node_id INTEGER,
    label TEXT,
	CHECK (from_node_id <> to_node_id),
	FOREIGN KEY (from_node_id) REFERENCES node(id) ON DELETE CASCADE,
	FOREIGN KEY (to_node_id) REFERENCES node(id) ON DELETE CASCADE,
	PRIMARY KEY(from_node_id, to_node_id)
);


        )",

        R"(
CREATE TABLE external_link(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	from_node_id INTEGER,
	to_url TEXT,
	UNIQUE(from_node_id, to_url),
	FOREIGN KEY (from_node_id) REFERENCES node(id) ON DELETE CASCADE
);


        )",

    	        R"(
    	CREATE INDEX idx_node_map_id ON node(map_id);
    	CREATE INDEX idx_content_node_id ON content(node_id);
    	CREATE INDEX idx_node_tag_node_id ON node_tag(node_id);
    	CREATE INDEX idx_node_type ON node(type);
    	CREATE INDEX idx_node_status ON node(status);
    	CREATE INDEX idx_node_property_key ON node_property(key);

		        )",






//         R"(
// aaaaaaaa
//         )",



    };
}


#endif //MIGRATIONS_H
