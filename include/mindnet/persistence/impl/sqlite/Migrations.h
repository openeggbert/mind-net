//
// Created by robertvokac on 3/11/25.
//

#ifndef MIGRATIONS_H
#define MIGRATIONS_H
#include <string>
#include <climits>


namespace mindnet::persistence::impl::sqlite {constexpr int MIGRATION_COUNT = 26;
    inline std::string migrations[MIGRATION_COUNT] = {

    	R"(

CREATE TABLE user (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	username TEXT NOT NULL UNIQUE,
	password_hash TEXT NOT NULL,
	display_name TEXT,
	role INTEGER NOT NULL DEFAULT 0 CHECK (role IN (0,1,2,3,4)),
	profile_text TEXT,
    last_login DATETIME,
    email TEXT UNIQUE,
	status INTEGER NOT NULL CHECK (status IN (0,1,2,3,4,5))
);
)",R"(
CREATE TABLE message (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	owner_id INTEGER NOT NULL,
	sender_id INTEGER NOT NULL,
	recipient_id INTEGER NOT NULL,
	subject TEXT,
    priority INTEGER CHECK (priority in (0,1,2,3)),
	body TEXT NOT NULL,
	sent_at DATETIME,
	system_message BOOLEAN DEFAULT 0,
    folder TEXT,
	draft BOOLEAN DEFAULT 0,
	is_read BOOLEAN DEFAULT 0,
	deleted_at DATETIME,
	starred BOOLEAN DEFAULT 0,

	FOREIGN KEY(sender_id) REFERENCES user(id),
	FOREIGN KEY(recipient_id) REFERENCES user(id),
	FOREIGN KEY(owner_id) REFERENCES user(id)
);
)",R"(
CREATE TABLE team (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	name TEXT NOT NULL,
	description TEXT,
	created_by INTEGER NOT NULL,
	leader_id INTEGER NOT NULL,
	FOREIGN KEY(created_by) REFERENCES user(id),
    FOREIGN KEY(leader_id) REFERENCES user(id)
);
)",R"(
CREATE TABLE team_member (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	team_id INTEGER NOT NULL,
	user_id INTEGER NOT NULL,
	role INTEGER NOT NULL DEFAULT 0 CHECK (role IN (0,1,2,3,4)),
	joined_at DATETIME NOT NULL,
	is_active BOOLEAN DEFAULT 1,
    left_at DATETIME,
	FOREIGN KEY(team_id) REFERENCES team(id),
	FOREIGN KEY(user_id) REFERENCES user(id)
);
)",R"(
CREATE TABLE discussion (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  team_id INTEGER NOT NULL,
  title TEXT NOT NULL,
  created_by INTEGER NOT NULL,
  is_pinned BOOLEAN DEFAULT 0,
  edited_at DATETIME,
  FOREIGN KEY (team_id) REFERENCES team(id),
  FOREIGN KEY (created_by) REFERENCES user(id)
);
)",R"(
CREATE TABLE comment (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  discussion_id INTEGER NOT NULL,
  user_id INTEGER NOT NULL,
  content TEXT NOT NULL,
  parent_comment_id INTEGER,
  edited_at DATETIME,
  is_deleted BOOLEAN DEFAULT 0,
  FOREIGN KEY (discussion_id) REFERENCES discussion(id),
  FOREIGN KEY (user_id) REFERENCES user(id),
  FOREIGN KEY(parent_comment_id) REFERENCES comment(id) /*ON DELETE CASCADE*/
);
)",R"(
CREATE TABLE suggestion (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	parent_suggestion_id INTEGER,
	from_user_id INTEGER NOT NULL,
	table_name TEXT NOT NULL,
	operation INTEGER NOT NULL CHECK (operation IN (1, 2, 3, 4, 5)),
	status INTEGER CHECK(status IN (0,1,2,3,4,5)) DEFAULT 0,
	data_json TEXT,
    review_count INTEGER DEFAULT 0,
    priority INTEGER DEFAULT 0,
    FOREIGN KEY(parent_suggestion_id) REFERENCES suggestion(id),
	FOREIGN KEY(from_user_id) REFERENCES user(id)
);
)",R"(
CREATE TABLE suggestion_review (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	suggestion_id INTEGER,
	reviewer_id INTEGER NOT NULL,
	decision_status INTEGER CHECK(decision_status IN (2,3,4,5)),
	comment TEXT,
	reviewed_at DATETIME,
	FOREIGN KEY(suggestion_id) REFERENCES suggestion(id),
	FOREIGN KEY(reviewer_id) REFERENCES user(id)
);
)",R"(
CREATE TABLE history (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
    user_id INTEGER NOT NULL,
    ip_address TEXT,
	table_name TEXT NOT NULL,
	record_id INTEGER NOT NULL,
	operation INTEGER NOT NULL CHECK (operation IN (1, 2, 3, 4, 5)),
	data_json TEXT NOT NULL,
    reason TEXT,
	FOREIGN KEY(user_id) REFERENCES user(id)
);
)",R"(
CREATE TABLE map (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
	name TEXT NOT NULL UNIQUE,
	description TEXT,
    category TEXT,
    owner_id INTEGER,
    is_public BOOLEAN DEFAULT 0,
	FOREIGN KEY(owner_id) REFERENCES user(id)
);
)",R"(
CREATE TABLE node (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
    uuid TEXT NOT NULL UNIQUE,
	map_id INTEGER NOT NULL,
    sibling_position INTEGER NOT NULL,
	title TEXT NOT NULL,
	content_id INTEGER,
    parent_node_id INTEGER,
    type INTEGER DEFAULT 0 CHECK (type IN (0,1)),
    visibility INTEGER DEFAULT 0 CHECK (visibility IN (0, 1, 2, 3)),
    last_shown_at DATETIME,
    expires_at DATETIME DEFAULT NULL,
    is_favorite BOOLEAN DEFAULT 0,
	redirect_node_id INTEGER,
	redirect_reason TEXT,
    importance INTEGER DEFAULT 0 CHECK (importance IN (0, 1, 2, 3)),
    difficulty INTEGER DEFAULT 0 CHECK (difficulty IN (0, 1, 2, 3, 4)),

    FOREIGN KEY (map_id) REFERENCES map(id) /*ON DELETE CASCADE*/,
	FOREIGN KEY (content_id) REFERENCES content(id) ON DELETE SET NULL,
    FOREIGN KEY (parent_node_id) REFERENCES node(id) /*ON DELETE CASCADE*/,
	FOREIGN KEY (redirect_node_id) REFERENCES node(id) ON DELETE SET NULL

);
)",R"(
CREATE TABLE content (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
	content TEXT NOT NULL,
	format INTEGER DEFAULT 0 CHECK (format IN (0, 1, 2)),
    mime_type TEXT NOT NULL,
    version INTEGER DEFAULT 1,
    node_id INTEGER,

	FOREIGN KEY (node_id) REFERENCES node(id) /*ON DELETE CASCADE*/
);
)",R"(
CREATE TABLE property(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
    map_id INTEGER NOT NULL,
	node_id INTEGER NOT NULL,
	key TEXT NOT NULL,
	value TEXT,
    value_type INTEGER DEFAULT 0 CHECK (value_type in (0, 1, 2, 3)),
    description TEXT,
    is_indexed BOOLEAN DEFAULT 0,

	FOREIGN KEY (map_id) REFERENCES map(id) /*ON DELETE CASCADE*/,
    FOREIGN KEY (node_id) REFERENCES node(id) /*ON DELETE CASCADE*/,
	unique (map_id, node_id, key)
);
)",R"(
CREATE TABLE tag_type (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
    map_id INTEGER NOT NULL,
	title TEXT NOT NULL,
    color TEXT,

	FOREIGN KEY (map_id) REFERENCES map(id) /*ON DELETE CASCADE*/,
    UNIQUE(map_id, title)
);
)",R"(
CREATE TABLE tag (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
	node_id INTEGER NOT NULL,
	tag_type_id INTEGER NOT NULL,

	UNIQUE (node_id, tag_type_id),
	FOREIGN KEY (node_id) REFERENCES node(id) /*ON DELETE CASCADE*/,
	FOREIGN KEY (tag_type_id) REFERENCES tag_type(id) /*ON DELETE CASCADE*/
);
)",R"(
CREATE TABLE flag (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
    map_id INTEGER NOT NULL,
	title TEXT NOT NULL,

	FOREIGN KEY (map_id) REFERENCES map(id) /*ON DELETE CASCADE*/,
    UNIQUE(map_id, title)
);
)",R"(
CREATE TABLE collection (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	name TEXT NOT NULL,
	description TEXT,
	order_index INTEGER,
    created_by INTEGER,
    is_public BOOLEAN DEFAULT 0,
    FOREIGN KEY (created_by) REFERENCES user(id)
);
)",R"(
CREATE TABLE collection_node (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	collection_id INTEGER NOT NULL,
	node_id INTEGER NOT NULL,
	order_index INTEGER,

	UNIQUE(collection_id, node_id),
	FOREIGN KEY(collection_id) REFERENCES collection(id),
	FOREIGN KEY(node_id) REFERENCES node(id)
);
)",R"(
		CREATE TABLE question (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
			version INTEGER NOT NULL DEFAULT 1,
			node_id INTEGER,
			question_text TEXT NOT NULL,
			type INTEGER NOT NULL CHECK (type IN (0,1,2,3)),
            difficulty INTEGER DEFAULT 0 CHECK (difficulty IN (0, 1, 2, 3, 4)),
			tags TEXT, -- for example. CSV: "STL,containers"
			answers_json TEXT, -- answers as a json object
			active BOOLEAN DEFAULT 1,
            FOREIGN KEY (node_id) REFERENCES node(id) /*ON DELETE CASCADE*/
		);

    	--answers_json
		--[
		--  { "text": "std::vector", "is_correct": true },
		--  { "text": "std::map", "is_correct": false },
		--  { "text": "std::set", "is_correct": false }
		--]
)",R"(
CREATE TABLE question_review (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    user_id INTEGER NOT NULL,
    question_id INTEGER NOT NULL,
    review_date DATETIME DEFAULT CURRENT_TIMESTAMP,
    grade INTEGER CHECK (grade BETWEEN 0 AND 5),
    response_data TEXT, -- for example. JSON: {"selected": [1, 3]}
    notes TEXT,
    FOREIGN KEY (question_id) REFERENCES question(id) /*ON DELETE CASCADE*/,
    FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE SET NULL
);

)",R"(
-- SM-2 state for each question and user
CREATE TABLE question_sm2_state (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    user_id INTEGER NOT NULL,
    question_id INTEGER NOT NULL,
--
    repetitions INTEGER DEFAULT 0,
    interval INTEGER DEFAULT 1,
    ef_times_100 integer DEFAULT 250 CHECK (ef_times_100 >= 100 and ef_times_100 <= 500),
--
    next_review DATETIME,
    last_review DATETIME,
    last_quality INTEGER DEFAULT 0,
--
    unique (user_id, question_id),
    FOREIGN KEY (question_id) REFERENCES question(id) /*ON DELETE CASCADE*/,
    FOREIGN KEY (user_id) REFERENCES user(id) /*ON DELETE CASCADE*/
);
)",R"(
CREATE TABLE node_link(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
	from_node_id INTEGER NOT NULL,
	to_node_id INTEGER NOT NULL,
    type INTEGER NOT NULL CHECK (type in (0,1)),
    label TEXT,

	CHECK (from_node_id <> to_node_id),
    UNIQUE (from_node_id, to_node_id),
	FOREIGN KEY (from_node_id) REFERENCES node(id) /*ON DELETE CASCADE*/,
	FOREIGN KEY (to_node_id) REFERENCES node(id) /*ON DELETE CASCADE*/
);
)",R"(
CREATE TABLE external_link(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
	from_node_id INTEGER NOT NULL,
	to_url TEXT NOT NULL,

	UNIQUE(from_node_id, to_url),
	FOREIGN KEY (from_node_id) REFERENCES node(id) /*ON DELETE CASCADE*/
);
)",R"(
CREATE INDEX idx_node_map_id ON node(map_id);
CREATE INDEX idx_content_node_id ON content(node_id);
CREATE INDEX idx_tag_node_id ON tag(node_id);
CREATE INDEX idx_node_type ON node(type);
CREATE INDEX idx_property_key ON property(key);
CREATE INDEX idx_history_operation ON history(operation);
CREATE INDEX idx_node_parent_id ON node(parent_node_id);
CREATE INDEX idx_node_shown_expires ON node(last_shown_at, expires_at);
CREATE INDEX idx_node_title ON node(title);
CREATE INDEX idx_tag_title ON tag_type(title);
CREATE INDEX idx_property_value ON property(value);
CREATE INDEX idx_property_key_value ON property(key, value);
CREATE INDEX idx_node_link_from_to ON node_link(from_node_id, to_node_id);
CREATE INDEX idx_external_link_url ON external_link(to_url);


)",R"(
CREATE VIRTUAL TABLE content_fts USING fts5(
    content,
    format UNINDEXED,
    version UNINDEXED,
    node_id UNINDEXED,
    content='content',
    tokenize = 'unicode61'
);
)",R"(
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




    };
}


#endif //MIGRATIONS_H
