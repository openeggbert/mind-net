//
// Created by robertvokac on 9/17/25.
//

#include "mindnet/plugins/suggestion/migrations/SuggestionSQLiteMigrationScripts.h"

namespace mindnet::plugins::suggestion::migrations
{
    SuggestionSQLiteMigrationScripts::SuggestionSQLiteMigrationScripts() : MigrationScripts(DatabaseType::SQLite)
    {
    }

    void SuggestionSQLiteMigrationScripts::define_migrations()
    {
        migrations = {

            R"(
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
)",
            R"(
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
)",
        };
    }
}
