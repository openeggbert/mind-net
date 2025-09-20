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


    	add_migration("V1__create_suggestion.sql",R"(
CREATE TABLE suggestion (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
	parent_suggestion_id INTEGER,
	from_user_id INTEGER NOT NULL,
	table_name TEXT NOT NULL,
	operation INTEGER NOT NULL,
	status INTEGER DEFAULT 0,
	data_json TEXT,
    review_count INTEGER DEFAULT 0,

    FOREIGN KEY(parent_suggestion_id) REFERENCES suggestion(id),
	FOREIGN KEY(from_user_id) REFERENCES user(id)
);
)");
    	add_migration("V2__create_suggestion_review.sql",R"(
CREATE TABLE suggestion_review (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
	suggestion_id INTEGER,
	reviewer_id INTEGER NOT NULL,
	decision_status INTEGER,
	comment TEXT,
	reviewed_at DATETIME,

	FOREIGN KEY(suggestion_id) REFERENCES suggestion(id),
	FOREIGN KEY(reviewer_id) REFERENCES user(id)
);
)");

    }
}
