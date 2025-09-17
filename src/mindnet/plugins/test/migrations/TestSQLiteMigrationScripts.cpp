//
// Created by robertvokac on 9/17/25.
//

#include "mindnet/plugins/test/migrations/TestSQLiteMigrationScripts.h"

namespace mindnet::plugins::test::migrations
{
    TestSQLiteMigrationScripts::TestSQLiteMigrationScripts() : MigrationScripts(DatabaseType::SQLite)
    {
    }

    void TestSQLiteMigrationScripts::define_migrations()
    {
        migrations = {
            R"(
CREATE TABLE review (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    --
    user_id INTEGER NOT NULL,
    note_id INTEGER,
    question_id INTEGER check (note_id NOT NULL OR question_id NOT NULL),
    review_date DATETIME DEFAULT CURRENT_TIMESTAMP,
    grade INTEGER CHECK (grade BETWEEN 0 AND 5),
    response_data TEXT, -- for example. JSON: {"selected": [1, 3]}
    notes TEXT,

    FOREIGN KEY (note_id) REFERENCES note(id) /*ON DELETE CASCADE*/,
    FOREIGN KEY (question_id) REFERENCES question(id) /*ON DELETE CASCADE*/,
    FOREIGN KEY (user_id) REFERENCES user(id)
);

)",
            R"(
-- SM-2 state for each note and user
CREATE TABLE sm2_state (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
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

    FOREIGN KEY (note_id) REFERENCES note(id) /*ON DELETE CASCADE*/,
    FOREIGN KEY (question_id) REFERENCES question(id) /*ON DELETE CASCADE*/,
    FOREIGN KEY (user_id) REFERENCES user(id) /*ON DELETE CASCADE*/
);
)",
        };
    }
}
