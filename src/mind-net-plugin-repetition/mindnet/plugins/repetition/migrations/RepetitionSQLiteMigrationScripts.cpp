//
// Created by robertvokac on 9/17/25.
//

#include "mindnet/plugins/repetition/migrations/RepetitionSQLiteMigrationScripts.h"

namespace mindnet::plugins::repetition::migrations
{
    RepetitionSQLiteMigrationScripts::RepetitionSQLiteMigrationScripts() : MigrationScripts(
        mindnet::essential::DatabaseType::SQLite)
    {
    }

    void RepetitionSQLiteMigrationScripts::define_migrations()
    {

    	add_migration("V1__create_r_global_setting.sql", R"(
    	CREATE TABLE r_global_setting (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

	key TEXT NOT NULL,
	value TEXT NOT NULL,

	UNIQUE (key)
);
)");

    	add_migration("V2__create_r_user_setting.sql",R"(
CREATE TABLE r_user_setting (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER NOT NULL,
    key TEXT NOT NULL,
    value TEXT NOT NULL,

    UNIQUE (user_id, key),

    FOREIGN KEY (user_id) REFERENCES user(id)
);
)");

        add_migration("V3__create_r_session.sql", R"(
CREATE TABLE r_session (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER NOT NULL,
    map_id INTEGER NOT NULL,
    cloned_from_session_id INTEGER,

    algorithm INTEGER NOT NULL,

    notes BOOLEAN NOT NULL DEFAULT 1,
    questions BOOLEAN NOT NULL DEFAULT 1,
    scope INTEGER NOT NULL,

    filter_under_note INTEGER,
    filter_date_from DATETIME,
    filter_date_to DATETIME,
    filter_tag INTEGER,
    filter_collection INTEGER,

    selected_items TEXT NOT NULL DEFAULT '{}', --example: {"note_ids":[3,4,5,6,7], "question_ids":[3,4,6,7,8]}
    pinned BOOL DEFAULT 0,

    FOREIGN KEY (user_id) REFERENCES user(id),
    FOREIGN KEY (map_id) REFERENCES map(id),
    FOREIGN KEY (cloned_from_session_id) REFERENCES r_session(id),
    FOREIGN KEY (filter_under_note) REFERENCES note(id),
    FOREIGN KEY (filter_tag) REFERENCES tag_type(id),
    FOREIGN KEY (filter_collection) REFERENCES collection(id)
);

)");

    	add_migration("V4__create_r_review.sql", R"(
CREATE TABLE r_review (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER NOT NULL,
    r_session_id INTEGER,

    algorithm INTEGER NOT NULL,

    note_id INTEGER,
    question_id INTEGER,
    CHECK (note_id IS NOT NULL OR question_id IS NOT NULL),

    review_date DATETIME,
    grade INTEGER CHECK (grade BETWEEN 0 AND 5),
    response_data TEXT, -- for example. JSON: {"selected": [1, 3]}
    notes TEXT,

	started_at DATETIME,   -- when started answering
	ended_at DATETIME,	 -- when finished
	latency_ms INTEGER,	-- response time in ms

	-- user behavior
	changed_answer BOOLEAN DEFAULT 0, -- changed answer during review?

    details_json TEXT NOT NULL DEFAULT '{}',
	-- optional JSON with algorithm-specific details
	-- e.g. SM-18: {"R_before":0.72,"S_before":2.1,"S_after":2.4,"next_interval":4.5}

    FOREIGN KEY (user_id) REFERENCES user(id),
    FOREIGN KEY (r_session_id) REFERENCES r_session(id),
    FOREIGN KEY (note_id) REFERENCES note(id),
    FOREIGN KEY (question_id) REFERENCES question(id)

);

)");
    	add_migration("V5__create_r0_state.sql", R"(

CREATE TABLE r0_state (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

	user_id INTEGER NOT NULL,
	note_id INTEGER,
	question_id INTEGER,
	CHECK (note_id IS NOT NULL OR question_id IS NOT NULL),

	repetitions INTEGER DEFAULT 0,	  -- number of completed repetitions 
	interval INTEGER DEFAULT 1,		 -- current interval in days (from fixed sequence)
	next_review DATETIME,			   -- when next repetition should occur
	last_review DATETIME,			   -- when last repetition occurred
	last_quality INTEGER DEFAULT 0,	 -- last grade

	UNIQUE (user_id, note_id),
	UNIQUE (user_id, question_id),

	FOREIGN KEY (note_id) REFERENCES note(id),
	FOREIGN KEY (question_id) REFERENCES question(id),
	FOREIGN KEY (user_id) REFERENCES user(id)
);

)");

	    add_migration("V6__create_r2_state.sql", R"(
CREATE TABLE r2_state (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER NOT NULL,
    note_id INTEGER,
    question_id INTEGER,
    CHECK (note_id IS NOT NULL OR question_id IS NOT NULL),

    repetitions INTEGER DEFAULT 0,
    interval INTEGER DEFAULT 1,
    ef_times_100 INTEGER DEFAULT 250 CHECK (ef_times_100 >= 100 and ef_times_100 <= 500),

    next_review DATETIME,
    last_review DATETIME,
    last_quality INTEGER DEFAULT 0,

    UNIQUE (user_id, note_id),
    UNIQUE (user_id, question_id),

    FOREIGN KEY (note_id) REFERENCES note(id) ,
    FOREIGN KEY (question_id) REFERENCES question(id) ,
    FOREIGN KEY (user_id) REFERENCES user(id)
);
)");

    	add_migration("V7__create_r4_state.sql", R"(
-- R-4 state for each note/question and user
CREATE TABLE r4_state (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

	user_id INTEGER NOT NULL,
	note_id INTEGER,
	question_id INTEGER,
	CHECK (note_id IS NOT NULL OR question_id IS NOT NULL),

	repetitions INTEGER DEFAULT 0,		  -- which repetition 
	interval INTEGER DEFAULT 1,			 -- current interval (days)
	ef_times_100 INTEGER DEFAULT 250 
		CHECK (ef_times_100 >= 100 and ef_times_100 <= 500),
											 -- E-Factor * 100
	correction_factor_times_100 INTEGER DEFAULT 100, 	 -- new: coefficient for interval correction
											-- (SM-4 adds adaptive adjustments)

	next_review DATETIME,				   -- when next repetition should occur
	last_review DATETIME,				   -- when last repetition occurred
	last_quality INTEGER DEFAULT 0,		 -- last grade (0-5)

	UNIQUE (user_id, note_id),
	UNIQUE (user_id, question_id),

	FOREIGN KEY (note_id) REFERENCES note(id),
	FOREIGN KEY (question_id) REFERENCES question(id),
	FOREIGN KEY (user_id) REFERENCES user(id)
);
)");


    	add_migration("V8__create_r18_state.sql", R"(
CREATE TABLE r18_state (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    user_id INTEGER NOT NULL,
    note_id INTEGER,
    question_id INTEGER,
    CHECK (note_id IS NOT NULL OR question_id IS NOT NULL),

    stability_times_100 INTEGER DEFAULT 100,    -- S
	last_interval_times_100 INTEGER DEFAULT 0,  -- last interval (days)
    repetitions INTEGER DEFAULT 0,
    lapses INTEGER DEFAULT 0,
    next_review DATETIME,
    last_review DATETIME,
    last_quality INTEGER DEFAULT 0,

    UNIQUE (user_id, note_id),
    UNIQUE (user_id, question_id),

    FOREIGN KEY (note_id) REFERENCES note(id),
    FOREIGN KEY (question_id) REFERENCES question(id),
    FOREIGN KEY (user_id) REFERENCES user(id)
);
)");

    	add_migration("V9__create_r18_perf_agg.sql", R"(
CREATE TABLE r18_perf_agg (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    user_id INTEGER NOT NULL,
    bin_log_t_times_100 INTEGER NOT NULL,  -- log(t) × 100
    total INTEGER NOT NULL,
    correct INTEGER NOT NULL,

    UNIQUE (user_id, bin_log_t_times_100),

    FOREIGN KEY (user_id) REFERENCES user(id)
);

    	)");

    	add_migration("V10__create_repetition_indexes.sql", R"(

-- Index for fast selection of "due items" (SM-18 and other algorithms)
CREATE INDEX IF NOT EXISTS idx_r18_state_user_due
    ON r18_state (user_id, next_review);

CREATE INDEX IF NOT EXISTS idx_r4_state_user_due
    ON r4_state (user_id, next_review);

CREATE INDEX IF NOT EXISTS idx_r2_state_user_due
    ON r2_state (user_id, next_review);

CREATE INDEX IF NOT EXISTS idx_r0_state_user_due
    ON r0_state (user_id, next_review);

-- Indexes for fast lookups by note_id and question_id
-- in review log and state tables

-- r_review
CREATE INDEX IF NOT EXISTS idx_r_review_note
    ON r_review (note_id);
CREATE INDEX IF NOT EXISTS idx_r_review_question
    ON r_review (question_id);

-- r0_state
CREATE INDEX IF NOT EXISTS idx_r0_state_note
    ON r0_state (note_id);
CREATE INDEX IF NOT EXISTS idx_r0_state_question
    ON r0_state (question_id);

-- r2_state
CREATE INDEX IF NOT EXISTS idx_r2_state_note
    ON r2_state (note_id);
CREATE INDEX IF NOT EXISTS idx_r2_state_question
    ON r2_state (question_id);

-- r4_state
CREATE INDEX IF NOT EXISTS idx_r4_state_note
    ON r4_state (note_id);
CREATE INDEX IF NOT EXISTS idx_r4_state_question
    ON r4_state (question_id);

-- r18_state
CREATE INDEX IF NOT EXISTS idx_r18_state_note
    ON r18_state (note_id);
CREATE INDEX IF NOT EXISTS idx_r18_state_question
    ON r18_state (question_id);

)");
    }
}
