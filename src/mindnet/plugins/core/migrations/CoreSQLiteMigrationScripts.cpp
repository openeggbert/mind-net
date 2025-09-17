//
// Created by robertvokac on 9/17/25.
//

#include "mindnet/plugins/core/migrations/CoreSQLiteMigrationScripts.h"

namespace mindnet::plugins::core::migrations
{
    CoreSQLiteMigrationScripts::CoreSQLiteMigrationScripts() : MigrationScripts(DatabaseType::SQLite)
    {
    }

    void CoreSQLiteMigrationScripts::define_migrations()
    {
        migrations = {

            R"(
CREATE TABLE user (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
	username TEXT NOT NULL UNIQUE,
	password_hash TEXT,
	display_name TEXT,
	role INTEGER NOT NULL DEFAULT 0,
	profile_text TEXT,
    last_login DATETIME,
    email TEXT UNIQUE,
	status INTEGER NOT NULL
);
)",
            R"(
CREATE TABLE team (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
	name TEXT NOT NULL,
	description TEXT,
	created_by INTEGER NOT NULL,
	leader_id INTEGER NOT NULL,

	FOREIGN KEY(created_by) REFERENCES user(id),
    FOREIGN KEY(leader_id) REFERENCES user(id)
);
)",
            R"(
CREATE TABLE team_member (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
	team_id INTEGER NOT NULL,
	user_id INTEGER NOT NULL,
	role INTEGER NOT NULL DEFAULT 0,
	status INTEGER NOT NULL,
	joined_at DATETIME NOT NULL,
    left_at DATETIME,

    UNIQUE(team_id, user_id),

	FOREIGN KEY(team_id) REFERENCES team(id),
	FOREIGN KEY(user_id) REFERENCES user(id)
);
)",

            R"(
CREATE TABLE history (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
    user_id INTEGER,
    ip_address TEXT,
	table_name TEXT NOT NULL,
	record_id INTEGER NOT NULL,
	operation INTEGER NOT NULL,
	data_json TEXT NOT NULL,
    reason TEXT,

	FOREIGN KEY(user_id) REFERENCES user(id)
);
)",

        };
    }
}
