//
// Created by robertvokac on 9/17/25.
//

#include "mindnet/plugins/slipbox/migrations/SlipBoxSQLiteMigrationScripts.h"

namespace mindnet::plugins::slipbox::migrations
{
    SlipBoxSQLiteMigrationScripts::SlipBoxSQLiteMigrationScripts() : MigrationScripts(mindnet::essential::DatabaseType::SQLite)
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
    --
	map_id INTEGER NOT NULL,
    parent_note_id INTEGER,
    content_id INTEGER UNIQUE,
    source_id INTEGER,
    --
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

    	add_migration("V17__create_alert.sql", R"(
CREATE TABLE alert (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,

	-- when notification should trigger
	trigger_at DATETIME NOT NULL,
    last_triggered_at DATETIME,
    trigger_count INTEGER,
    snooze_until DATETIME,
    expires_at DATETIME,

	-- optionally how often to repeat (e.g. "DAILY", "WEEKLY", "NONE")
	repeat_interval INTEGER DEFAULT 0,
    repeat_count INTEGER DEFAULT 0,
    repeat_until DATETIME DEFAULT 0,

	user_id INTEGER NOT NULL,
    -- reference to note (optional)
	note_id INTEGER,
    url TEXT,

	-- optional notification title/content
	title TEXT NOT NULL,
	message TEXT,

	-- alert status: ACTIVE, TRIGGERED, DISMISSED, SNOOZED, FAILED
	status INTEGER NOT NULL DEFAULT 0,

	-- metadata (e.g. category, priority, sound signal etc.)
	important BOOLEAN DEFAULT 0,
    channel INTEGER,

	UNIQUE(user_id, title, trigger_at),
	FOREIGN KEY (user_id) REFERENCES user(id),
	FOREIGN KEY (note_id) REFERENCES note(id)
);

CREATE INDEX idx_alert_user_id ON alert(user_id);
CREATE INDEX idx_alert_note_id ON alert(note_id);

)");


    	add_migration("V18__create_flag.sql", R"(
CREATE TABLE flag(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME ,
    updated_at DATETIME ,
    --
    map_id INTEGER NOT NULL,
	title TEXT NOT NULL,

    UNIQUE (map_id, title),

	FOREIGN KEY (map_id) REFERENCES map(id)
);

CREATE INDEX idx_flag_map_id ON flag(map_id);

)");
    	add_migration("V19__create_project.sql", R"(
CREATE TABLE project (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    -- reference
	note_id INTEGER,		  -- which note the project is linked to

	-- content
    title TEXT NOT NULL,
    description TEXT,
    progress INTEGER CHECK(progress BETWEEN 0 AND 100),

	-- project state
    in_progress BOOLEAN DEFAULT 0,
    important BOOLEAN DEFAULT 0,
    due_date DATETIME,

    -- metadata
    created_by INTEGER,           -- user_id
    owner_id INTEGER,           -- user_id
    assigned_to INTEGER,          -- user_id
    category TEXT,
    is_public BOOLEAN DEFAULT 0,

    UNIQUE(note_id, title),
    FOREIGN KEY (note_id) REFERENCES note(id),
    FOREIGN KEY (created_by) REFERENCES user(id),
    FOREIGN KEY (owner_id) REFERENCES user(id),
    FOREIGN KEY (assigned_to) REFERENCES user(id)
);

CREATE INDEX idx_project_note_id ON project(note_id);
CREATE INDEX idx_project_due_date ON project(due_date);
CREATE INDEX idx_project_in_progress ON project(in_progress);
CREATE INDEX idx_project_created_by ON project(created_by);
CREATE INDEX idx_project_owner_id ON project(owner_id);
CREATE INDEX idx_project_assigned_to ON project(assigned_to);

)");


    	add_migration("V20__create_task.sql", R"(
CREATE TABLE task (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    -- reference
	note_id INTEGER,		  -- which note the task is linked to
    project_id INTEGER,

	-- content
    title TEXT NOT NULL,
    description TEXT,
    progress INTEGER CHECK(progress BETWEEN 0 AND 100),

	-- task state
    status INTEGER DEFAULT 0,   -- OPEN, IN_PROGRESS, DONE, CANCELLED, BLOCKED
    important BOOLEAN DEFAULT 0,
    as_soon_as_possible BOOLEAN DEFAULT 0,
    start_date DATETIME,
    due_date DATETIME,            -- deadline
    completed_at DATETIME,

    -- metadata
    created_by INTEGER,           -- user_id
    owner_id INTEGER,           -- user_id
    assigned_to INTEGER,          -- user_id
    category TEXT,
    context TEXT,
	tags TEXT,					-- optional tags
    is_public BOOLEAN DEFAULT 0,

    parent_task_id INTEGER,
    blocked_by_task_id INTEGER,
    related_tasks TEXT,

    UNIQUE(note_id, title),
    FOREIGN KEY (note_id) REFERENCES note(id),
    FOREIGN KEY (project_id) REFERENCES project(id),
    FOREIGN KEY (created_by) REFERENCES user(id),
    FOREIGN KEY (owner_id) REFERENCES user(id),
    FOREIGN KEY (assigned_to) REFERENCES user(id),
    FOREIGN KEY (parent_task_id) REFERENCES task(id),
    FOREIGN KEY (blocked_by_task_id) REFERENCES task(id)
);

CREATE INDEX idx_task_note_id ON task(note_id);
CREATE INDEX idx_task_project_id ON task(project_id);
CREATE INDEX idx_task_due_date ON task(due_date);
CREATE INDEX idx_task_status ON task(status);
CREATE INDEX idx_task_status_due ON task(status, due_date);
CREATE INDEX idx_task_created_by ON task(created_by);
CREATE INDEX idx_task_owner_id ON task(owner_id);
CREATE INDEX idx_task_assigned_to ON task(assigned_to);
CREATE INDEX idx_task_parent ON task(parent_task_id);
CREATE INDEX idx_task_blocked ON task(blocked_by_task_id);



)");

    	add_migration("V21__create_pinned_note.sql", R"(
CREATE TABLE pinned_note(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME ,
    updated_at DATETIME ,
    --
    user_id INTEGER NOT NULL,
    note_id INTEGER NOT NULL,
	position INTEGER,			 -- optional: pinning order (1,2,3...)
    is_public BOOLEAN DEFAULT 0,

    UNIQUE(user_id, note_id),

    FOREIGN KEY (user_id) REFERENCES user(id),
    FOREIGN KEY (note_id) REFERENCES note(id)
);

CREATE INDEX idx_pinned_note_user_id ON pinned_note(user_id);
CREATE INDEX idx_pinned_note_note_id ON pinned_note(note_id);

)");
    }
}
