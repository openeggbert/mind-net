//
// Created by robertvokac on 9/17/25.
//

#include "mindnet/plugins/core/migrations/CoreSQLiteMigrationScripts.h"

namespace mindnet::plugins::core::migrations
{
    CoreSQLiteMigrationScripts::CoreSQLiteMigrationScripts() : MigrationScripts(mindnet::essential::DatabaseType::SQLite)
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

    	add_migration("V5__create_api_log.sql", R"(
CREATE TABLE api_log (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER,
    ip_address TEXT,

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

    	add_migration("V6__create_access_token.sql", R"(
CREATE TABLE access_token (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER NOT NULL,

    token_hash TEXT NOT NULL,
    token_purpose INTEGER NOT NULL, --session, api, service

    is_created_by_user BOOLEAN NOT NULL DEFAULT 0,
    name TEXT,
    description TEXT,

    issued_at DATETIME NOT NULL,
	expires_at DATETIME,
    is_revoked BOOLEAN NOT NULL DEFAULT 0,
    revoked_at DATETIME,

    last_used_at DATETIME,
	ip_address TEXT, -- audit: last use from which IP
    user_agent TEXT,

    allowed_operations TEXT,

	FOREIGN KEY(user_id) REFERENCES user(id)
);

CREATE UNIQUE INDEX idx_access_token_hash ON access_token(token_hash);
CREATE INDEX idx_access_token_user_id ON access_token(user_id);
)");

    	add_migration("V7__create_refresh_token.sql", R"(
CREATE TABLE refresh_token (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER NOT NULL,

    token_hash TEXT NOT NULL,

    issued_at DATETIME NOT NULL,
	expires_at DATETIME,
    is_revoked BOOLEAN NOT NULL DEFAULT 0,
    revoked_at DATETIME,

    replaced_by_id INTEGER, -- rotation chain
    rotated_from_id INTEGER,

    last_used_at DATETIME,
	ip_address TEXT, -- audit: last use from which IP
    user_agent TEXT,

	FOREIGN KEY(user_id) REFERENCES user(id),
    FOREIGN KEY(replaced_by_id) REFERENCES refresh_token(id) ON DELETE SET NULL,
    FOREIGN KEY(rotated_from_id) REFERENCES refresh_token(id) ON DELETE SET NULL
);

CREATE UNIQUE INDEX idx_refresh_token_hash ON refresh_token(token_hash);
CREATE INDEX idx_refresh_token_user_id ON refresh_token(user_id);
CREATE INDEX idx_refresh_token_expires_at ON refresh_token(expires_at);
)");

    	add_migration("V8__create_login_session.sql", R"(
CREATE TABLE login_session (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER NOT NULL,

    access_token_id INTEGER NOT NULL,
    refresh_token_id INTEGER,

    issued_at DATETIME NOT NULL,
	expires_at DATETIME,
    is_revoked BOOLEAN NOT NULL DEFAULT 0,
    revoked_at DATETIME,

    last_used_at DATETIME,
	ip_address TEXT, -- audit: last use from which IP
    user_agent TEXT,

	FOREIGN KEY(user_id) REFERENCES user(id),
    FOREIGN KEY(access_token_id) REFERENCES access_token(id) ON DELETE CASCADE,
    FOREIGN KEY(refresh_token_id) REFERENCES refresh_token(id) ON DELETE CASCADE
);

CREATE INDEX idx_login_session_user_id ON login_session(user_id);
CREATE INDEX idx_login_session_access_token_id ON login_session(access_token_id);
CREATE INDEX idx_login_session_expires_at ON login_session(expires_at);

)");











    }
}
