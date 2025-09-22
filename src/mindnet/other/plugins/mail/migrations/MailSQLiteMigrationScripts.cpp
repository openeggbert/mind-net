//
// Created by robertvokac on 9/17/25.
//

#include "mindnet/other/plugins/mail/migrations/MailSQLiteMigrationScripts.h"

namespace mindnet::plugins::mail::migrations
{
    MailSQLiteMigrationScripts::MailSQLiteMigrationScripts() : MigrationScripts(mindnet::core::DatabaseType::SQLite)
    {
    }

    void MailSQLiteMigrationScripts::define_migrations()
    {


    	add_migration("V1__create_message.sql",R"(
CREATE TABLE message (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
	owner_id INTEGER NOT NULL,
	sender_id INTEGER NOT NULL,
	recipient_id INTEGER NOT NULL,
	subject TEXT,
    important BOOLEAN DEFAULT 0 CHECK (important in (0,1)),
	body TEXT NOT NULL,
	sent_at DATETIME,
	system_message BOOLEAN DEFAULT 0,
    folder TEXT,
	draft BOOLEAN DEFAULT 0,
	is_read BOOLEAN DEFAULT 0,
	deleted_at DATETIME,
	starred BOOLEAN DEFAULT 0,

	FOREIGN KEY(sender_id) REFERENCES user(id),
	FOREIGN KEY(recipient_id) REFERENCES user(id),
	FOREIGN KEY(owner_id) REFERENCES user(id)
);
)");


    }
}
