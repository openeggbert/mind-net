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
        add_migration("V1__create_user.sql", R"(
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
)");

        add_migration("V2__create_team.sql", R"(
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
)");

        add_migration("V3__create_team_member.sql", R"(
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
)");

        add_migration("V4__create_history.sql", R"(
CREATE TABLE history (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
    user_id INTEGER,
	table_name TEXT NOT NULL,
	record_id INTEGER NOT NULL,
	operation INTEGER NOT NULL,
	data_json TEXT NOT NULL,
    reason TEXT,

	FOREIGN KEY(user_id) REFERENCES user(id)
);
)");

    	add_migration("V5__create_log.sql", R"(
CREATE TABLE api_log (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
    --who
    user_id INTEGER,
    ip_address TEXT,
    --what
    endpoint TEXT NOT NULL,
    method INTEGER NOT NULL,
	entity_name TEXT NOT NULL,
	entity_id INTEGER,
    parameters TEXT,
    request_body TEXT,
    status_code INTEGER NOT NULL,
    error TEXT,

	FOREIGN KEY(user_id) REFERENCES user(id)
);
)");

    }
}
