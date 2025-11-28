/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mindnet/plugins/core/migrations/CoreSQLiteMigrationScripts.hpp"

namespace mindnet::plugins::core::migrations
{
    CoreSQLiteMigrationScripts::CoreSQLiteMigrationScripts() : MigrationScripts(
        mindnet::essential::DatabaseType::SQLite)
    {
    }

    void CoreSQLiteMigrationScripts::define_migrations()
    {
        add_migration("V1__create_user.sql", R"(
CREATE TABLE user (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

	username TEXT NOT NULL UNIQUE,
	password_hash TEXT,
    email TEXT,

	display_name TEXT,
	profile_text TEXT,

    role INTEGER NOT NULL DEFAULT 0,
	status INTEGER NOT NULL,

    last_login DATETIME
);

CREATE INDEX idx_user_last_login ON user(last_login);
CREATE INDEX idx_user_status ON user(status);
CREATE INDEX idx_user_role ON user(role);
)");

        add_migration("V2__create_team.sql", R"(
CREATE TABLE team (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

	name TEXT NOT NULL UNIQUE,
	description TEXT,
	created_by INTEGER NOT NULL,
	leader_id INTEGER NOT NULL,

	FOREIGN KEY(created_by) REFERENCES user(id),
    FOREIGN KEY(leader_id) REFERENCES user(id)
);

CREATE INDEX idx_team_created_by ON team(created_by);
CREATE INDEX idx_team_leader_id ON team(leader_id);

)");

        add_migration("V3__create_team_member.sql", R"(
CREATE TABLE team_member (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

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

CREATE INDEX idx_team_member_team_id ON team_member(team_id);
CREATE INDEX idx_team_member_user_id ON team_member(user_id);
CREATE INDEX idx_team_member_team_status ON team_member(team_id, status);
CREATE INDEX idx_team_member_team_role ON team_member(team_id, role);
CREATE INDEX idx_team_member_team_joined ON team_member(team_id, joined_at);

)");

        add_migration("V4__create_history.sql", R"(
CREATE TABLE history (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER,
	table_name TEXT NOT NULL,
	record_id INTEGER NOT NULL,
	operation INTEGER NOT NULL,
	data_json TEXT NOT NULL,
    reason TEXT,

	FOREIGN KEY(user_id) REFERENCES user(id)
);

CREATE INDEX idx_history_table_record ON history(table_name, record_id);
CREATE INDEX idx_history_user_id ON history(user_id);
CREATE INDEX idx_history_created_at ON history(created_at);

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

CREATE INDEX idx_api_log_created_at ON api_log(created_at);
CREATE INDEX idx_api_log_user_id ON api_log(user_id);
CREATE INDEX idx_api_log_endpoint ON api_log(endpoint);
CREATE INDEX idx_api_log_entity ON api_log(entity_name, entity_id);

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
CREATE INDEX idx_access_token_valid
  ON access_token(user_id, is_revoked, expires_at);
CREATE INDEX idx_access_token_expires_at
  ON access_token(expires_at);
CREATE INDEX idx_access_token_last_used
  ON access_token(last_used_at);

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

        add_migration("V9__create_auth_log.sql", R"(
CREATE TABLE auth_log (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER,
    ip_address TEXT,
    user_agent TEXT,

    --What was called
    endpoint TEXT NOT NULL,
    method INTEGER NOT NULL,
	action TEXT,

    --Context
    entity_name TEXT,            -- for example: user, access_token, note
    entity_id INTEGER,
    parameters TEXT,
    request_body TEXT,

    --Result
    status_code INTEGER NOT NULL,
    error TEXT,
    success BOOLEAN NOT NULL DEFAULT 0,

	FOREIGN KEY(user_id) REFERENCES user(id)
);
)");

        add_migration("V10__create_super_admin_log.sql", R"(
CREATE TABLE super_admin_log (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER,
    ip_address TEXT,
    user_agent TEXT,

    --What was called
    endpoint TEXT NOT NULL,
    method INTEGER NOT NULL,
	action TEXT NOT NULL,

    --Context
    parameters TEXT,
    request_body TEXT,
    diff TEXT,

    --Result
    status_code INTEGER NOT NULL,
    error TEXT,
    success BOOLEAN NOT NULL DEFAULT 0,

	FOREIGN KEY(user_id) REFERENCES user(id)
);
)");

    	add_migration("V11__create_job_entry.sql", R"(
CREATE TABLE job_entry (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
	
	name TEXT NOT NULL UNIQUE,
	description TEXT,
	plugin_name TEXT NOT_NULL,
	cron_expression TEXT NOT NULL,
	run_once_when_missed BOOLEAN NOT NULL DEFAULT 1,
	enabled BOOLEAN NOT NULL DEFAULT 1,
	last_run DATETIME,
	next_run DATETIME
);
)");
    	add_migration("V12__create_job_run.sql", R"(
CREATE TABLE job_run (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    job_entry_id INTEGER,
    started_at DATETIME,
    finished_at DATETIME,
    success BOOLEAN DEFAULT 0,
    message TEXT,

	FOREIGN KEY(job_entry_id) REFERENCES job_entry(id)

);
)");

    	add_migration("V13__alter_table_job_entry_add_column_configuration.sql", R"(
ALTER TABLE job_entry ADD configuration TEXT;
)");
    }
}
