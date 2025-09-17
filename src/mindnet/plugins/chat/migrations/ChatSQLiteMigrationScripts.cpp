//
// Created by robertvokac on 9/17/25.
//

#include "mindnet/plugins/chat/migrations/ChatSQLiteMigrationScripts.h"

namespace mindnet::plugins::chat::migrations
{
    ChatSQLiteMigrationScripts::ChatSQLiteMigrationScripts() : MigrationScripts(DatabaseType::SQLite)
    {
    }

    void ChatSQLiteMigrationScripts::define_migrations()
    {
        migrations = {

            R"(
CREATE TABLE discussion (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
  team_id INTEGER NOT NULL,
  title TEXT NOT NULL,
  created_by INTEGER NOT NULL,
  is_pinned BOOLEAN DEFAULT 0,
  is_archived BOOLEAN DEFAULT 0,

  FOREIGN KEY (team_id) REFERENCES team(id),
  FOREIGN KEY (created_by) REFERENCES user(id)
);
)",
            R"(
CREATE TABLE comment (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,
    --
  discussion_id INTEGER NOT NULL,
  user_id INTEGER NOT NULL,
  content TEXT NOT NULL,
  parent_comment_id INTEGER,
  is_deleted BOOLEAN DEFAULT 0,

  FOREIGN KEY (discussion_id) REFERENCES discussion(id),
  FOREIGN KEY (user_id) REFERENCES user(id),
  FOREIGN KEY(parent_comment_id) REFERENCES comment(id)
);
)",
        };
    }
}
