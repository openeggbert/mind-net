//
// Created by robertvokac on 3/11/25.
//

#ifndef MIGRATIONS_H
#define MIGRATIONS_H
#include <string>
#include <climits>


namespace mindnet::persistence::impl::sqlite {constexpr int MIGRATION_COUNT = 22;
    inline std::string migrations[MIGRATION_COUNT] = {

    	R"(

CREATE TABLE user (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
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
    --
	owner_id INTEGER NOT NULL,
	sender_id INTEGER NOT NULL,
	recipient_id INTEGER NOT NULL,
	subject TEXT,
    important BOOLEAN DEFAULT 0 CHECK (important in (0,1)),
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
    --
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
    --
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
    --
  team_id INTEGER NOT NULL,
  title TEXT NOT NULL,
  created_by INTEGER NOT NULL,
  is_pinned BOOLEAN DEFAULT 0,
  FOREIGN KEY (team_id) REFERENCES team(id),
  FOREIGN KEY (created_by) REFERENCES user(id)
);
)",R"(
CREATE TABLE comment (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
  discussion_id INTEGER NOT NULL,
  user_id INTEGER NOT NULL,
  content TEXT NOT NULL,
  parent_comment_id INTEGER,
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
    --
	parent_suggestion_id INTEGER,
	from_user_id INTEGER NOT NULL,
	table_name TEXT NOT NULL,
	operation INTEGER NOT NULL CHECK (operation IN (1, 2, 3, 4, 5)),
	status INTEGER CHECK(status IN (0,1,2,3,4,5)) DEFAULT 0,
	data_json TEXT,
    review_count INTEGER DEFAULT 0,
    FOREIGN KEY(parent_suggestion_id) REFERENCES suggestion(id),
	FOREIGN KEY(from_user_id) REFERENCES user(id)
);
)",R"(
CREATE TABLE suggestion_review (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
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
)", R"(
CREATE TABLE map (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
	name TEXT NOT NULL UNIQUE,
	description TEXT,
    category TEXT,
    --
    owner_id INTEGER,
    team_id INTEGER,
    owner_rights INTEGER CHECK (owner_rights >= 1 and owner_rights <= 7),
    team_rights INTEGER CHECK (team_rights >= 1 and team_rights <= 7),
    other_rights INTEGER CHECK (other_rights >= 1 and other_rights <= 7),
    --

	FOREIGN KEY(owner_id) REFERENCES user(id)
);
)",R"(
CREATE TABLE note (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
	map_id INTEGER NOT NULL,
    title TEXT NOT NULL,
    parent_note_id INTEGER,
    content_id INTEGER,
    sibling_position INTEGER NOT NULL,
    importance INTEGER DEFAULT 0 CHECK (importance IN (0, 1, 2, 3)),
    difficulty INTEGER DEFAULT 0 CHECK (difficulty IN (0, 1, 2, 3, 4)),

    FOREIGN KEY (map_id) REFERENCES map(id) /*ON DELETE CASCADE*/,
	FOREIGN KEY (content_id) REFERENCES content(id) ON DELETE SET NULL,
    FOREIGN KEY (parent_note_id) REFERENCES note(id) /*ON DELETE CASCADE*/
);

CREATE INDEX idx_note_content_id ON note(content_id);
CREATE INDEX idx_note_map_id ON note(map_id);
)",R"(
CREATE TABLE content (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
	value TEXT NOT NULL,
	format INTEGER DEFAULT 0 CHECK (format IN (0, 1, 2)),
    version INTEGER DEFAULT 1
);
CREATE INDEX idx_content_value ON content(value);
)",R"(
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
)",R"(
CREATE TABLE property(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
    map_id INTEGER NOT NULL,
	note_id INTEGER NOT NULL,
	key TEXT NOT NULL,
	value TEXT,

	FOREIGN KEY (map_id) REFERENCES map(id) /*ON DELETE CASCADE*/,
    FOREIGN KEY (note_id) REFERENCES note(id) /*ON DELETE CASCADE*/,
	unique (map_id, note_id, key)
);

CREATE INDEX idx_property_map_note_key ON property(map_id, note_id, key);

)",R"(
CREATE TABLE tag_type (
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
CREATE TABLE tag (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
	note_id INTEGER NOT NULL,
	tag_type_id INTEGER NOT NULL,

	UNIQUE (note_id, tag_type_id),
	FOREIGN KEY (note_id) REFERENCES note(id) /*ON DELETE CASCADE*/,
	FOREIGN KEY (tag_type_id) REFERENCES tag_type(id) /*ON DELETE CASCADE*/
);

CREATE INDEX idx_tag_note_id ON tag(note_id);

)",R"(
CREATE TABLE collection (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
	name TEXT NOT NULL,
	description TEXT,
	order_index INTEGER,
    created_by INTEGER,
    is_public BOOLEAN DEFAULT 0,
    FOREIGN KEY (created_by) REFERENCES user(id)
);
)",R"(
CREATE TABLE collection_item (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
	collection_id INTEGER NOT NULL,
	note_id INTEGER NOT NULL,
	order_index INTEGER,

	UNIQUE(collection_id, note_id),
	FOREIGN KEY(collection_id) REFERENCES collection(id),
	FOREIGN KEY(note_id) REFERENCES note(id)
);
)",
//     	R"(
// 		CREATE TABLE question (
// 	id INTEGER PRIMARY KEY AUTOINCREMENT,
// 	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
// 	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
// 			version INTEGER NOT NULL DEFAULT 1,
// 			node_id INTEGER,
// 			question_text TEXT NOT NULL,
// 			type INTEGER NOT NULL CHECK (type IN (0,1,2,3)),
//             difficulty INTEGER DEFAULT 0 CHECK (difficulty IN (0, 1, 2, 3, 4)),
// 			tags TEXT, -- for example. CSV: "STL,containers"
// 			answers_json TEXT, -- answers as a json object
// 			active BOOLEAN DEFAULT 1,
//             FOREIGN KEY (node_id) REFERENCES node(id) /*ON DELETE CASCADE*/
// 		);
//
//     	--answers_json
// 		--[
// 		--  { "text": "std::vector", "is_correct": true },
// 		--  { "text": "std::map", "is_correct": false },
// 		--  { "text": "std::set", "is_correct": false }
// 		--]
// )",
    	R"(
CREATE TABLE review (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
    user_id INTEGER NOT NULL,
    note_id INTEGER NOT NULL,
    review_date DATETIME DEFAULT CURRENT_TIMESTAMP,
    grade INTEGER CHECK (grade BETWEEN 0 AND 5),
    response_data TEXT, -- for example. JSON: {"selected": [1, 3]}
    notes TEXT,
    FOREIGN KEY (note_id) REFERENCES note(id) /*ON DELETE CASCADE*/,
    FOREIGN KEY (user_id) REFERENCES user(id)
);

)",R"(
-- SM-2 state for each note and user
CREATE TABLE sm2_state (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
    user_id INTEGER NOT NULL,
    note_id INTEGER NOT NULL,
--
    repetitions INTEGER DEFAULT 0,
    interval INTEGER DEFAULT 1,
    ef_times_100 INTEGER DEFAULT 250 CHECK (ef_times_100 >= 100 and ef_times_100 <= 500),
--
    next_review DATETIME,
    last_review DATETIME,
    last_quality INTEGER DEFAULT 0,
--
    unique (user_id, note_id),
    FOREIGN KEY (note_id) REFERENCES note(id) /*ON DELETE CASCADE*/,
    FOREIGN KEY (user_id) REFERENCES user(id) /*ON DELETE CASCADE*/
);
)",R"(
CREATE TABLE reference(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
	from_note_id INTEGER NOT NULL,
	to_note_id INTEGER NOT NULL,
    label TEXT,

	CHECK (from_note_id <> to_note_id),
    UNIQUE (from_note_id, to_note_id),
	FOREIGN KEY (from_note_id) REFERENCES note(id) /*ON DELETE CASCADE*/,
	FOREIGN KEY (to_note_id) REFERENCES note(id) /*ON DELETE CASCADE*/
);

CREATE INDEX idx_reference_from_to ON reference(from_note_id, to_note_id);

)",R"(
CREATE TABLE link(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
	from_note_id INTEGER NOT NULL,
	to_url TEXT NOT NULL,

	UNIQUE(from_note_id, to_url),
	FOREIGN KEY (from_note_id) REFERENCES note(id) /*ON DELETE CASCADE*/
);

CREATE INDEX idx_link_from_note ON link(from_note_id);
)",




    };
}


#endif //MIGRATIONS_H
