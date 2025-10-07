//
// Created by robertvokac on 9/17/25.
//

#include "mindnet/plugins/repetition/migrations/RepetitionSQLiteMigrationScripts.h"

namespace mindnet::plugins::repetition::migrations
{
    RepetitionSQLiteMigrationScripts::RepetitionSQLiteMigrationScripts() : MigrationScripts(mindnet::essential::DatabaseType::SQLite)
    {
    }

    void RepetitionSQLiteMigrationScripts::define_migrations()
    {


        add_migration("V1__create_r2_session.sql",R"(
CREATE TABLE r2_session (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER NOT NULL,
    map_id INTEGER NOT NULL,
    cloned_from_session_id INTEGER

    algorithm INTEGER NOT NULL,
    notes BOOLEAN NOT NULL DEFAULT 1,
    questions INTEGER NOT NULL DEFAULT 1,
    scope INTEGER NOT NULL,

    filter_notes_under INTEGER,
    filter_date_from DATETIME,
    filter_date_to DATETIME,
    filter_tag TEXT,
    filter_collection INTEGER,

    FOREIGN KEY (user_id) REFERENCES user(id),
    FOREIGN KEY (map_id) REFERENCES map(id),
    FOREIGN KEY (cloned_from_session_id) REFERENCES r2_session(id),
    FOREIGN KEY (filter_notes_under) REFERENCES note(id),
    FOREIGN KEY (filter_tag) REFERENCES tag_type(id),
    FOREIGN KEY (filter_collection) REFERENCES collection(id)
);

)");

        add_migration("V2__create_r2_review.sql",R"(
CREATE TABLE r2_review (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER NOT NULL,
    note_id INTEGER,
    question_id INTEGER check (note_id NOT NULL OR question_id NOT NULL),
    review_date DATETIME,
    grade INTEGER CHECK (grade BETWEEN 0 AND 5),
    response_data TEXT, -- for example. JSON: {"selected": [1, 3]}
    notes TEXT,

    FOREIGN KEY (note_id) REFERENCES note(id) ,
    FOREIGN KEY (question_id) REFERENCES question(id) ,
    FOREIGN KEY (user_id) REFERENCES user(id)
);

)");
        add_migration("V3__create_r2_state.sql",R"(
-- R-2 state for each note and user
CREATE TABLE r2_state (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER NOT NULL,
    note_id INTEGER,
    question_id INTEGER check (note_id NOT NULL OR question_id NOT NULL),

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

    }
}
