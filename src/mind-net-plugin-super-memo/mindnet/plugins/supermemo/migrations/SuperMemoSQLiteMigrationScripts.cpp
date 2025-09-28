//
// Created by robertvokac on 9/17/25.
//

#include "mindnet/plugins/supermemo/migrations/SuperMemoSQLiteMigrationScripts.h"

namespace mindnet::plugins::supermemo::migrations
{
    SuperMemoSQLiteMigrationScripts::SuperMemoSQLiteMigrationScripts() : MigrationScripts(mindnet::essential::DatabaseType::SQLite)
    {
    }

    void SuperMemoSQLiteMigrationScripts::define_migrations()
    {

        add_migration("V1__create_review.sql",R"(
CREATE TABLE review (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
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
        add_migration("V2__create_sm18_state.sql",R"(
-- SM-2 state for each note and user
CREATE TABLE sm2_state (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
    user_id INTEGER NOT NULL,
    note_id INTEGER,
    question_id INTEGER check (note_id NOT NULL OR question_id NOT NULL),
--
    repetitions INTEGER DEFAULT 0,
    interval INTEGER DEFAULT 1,
    ef_times_100 INTEGER DEFAULT 250 CHECK (ef_times_100 >= 100 and ef_times_100 <= 500),
--
    next_review DATETIME,
    last_review DATETIME,
    last_quality INTEGER DEFAULT 0,
--
    UNIQUE (user_id, note_id),
UNIQUE (user_id, question_id),

    FOREIGN KEY (note_id) REFERENCES note(id) ,
    FOREIGN KEY (question_id) REFERENCES question(id) ,
    FOREIGN KEY (user_id) REFERENCES user(id)
);
)");

    }
}
