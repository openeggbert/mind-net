//
// Created by robertvokac on 3/11/25.
//

#ifndef MIGRATIONS_H
#define MIGRATIONS_H
#include <string>


namespace miniwiki::persistence::impl::sqlite {
    constexpr int MIGRATION_COUNT = 17;
    inline std::string migrations[MIGRATION_COUNT] = {

        R"(
CREATE TABLE content (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	content TEXT NOT NULL,
	format INTEGER,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

        )",

        R"(
CREATE TABLE namespace (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	name TEXT NOT NULL UNIQUE,
	description TEXT
);


        )",
        R"(
CREATE TABLE user (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	username TEXT NOT NULL UNIQUE,
	password_hash TEXT NOT NULL,
    email TEXT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    active BOOL DEFAULT 0,
    token TEXT
);


        )",
        R"(
CREATE TABLE role (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	name TEXT NOT NULL UNIQUE,
	description TEXT
);


        )",
        R"(
CREATE TABLE user_role (
	user_id INTEGER NOT NULL,
	role_id INTEGER NOT NULL,
	PRIMARY KEY(user_id, role_id),
	FOREIGN KEY(user_id) REFERENCES user(id) ON DELETE CASCADE,
	FOREIGN KEY(role_id) REFERENCES role(id) ON DELETE CASCADE
);
        )",

    	R"(
CREATE TABLE infobox (
	id INTEGER PRIMARY KEY,
	data JSON NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

        )",

        R"(
CREATE TABLE revision (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	article_id INTEGER NOT NULL,
	content_id INTEGER NOT NULL,
    infobox_id INTEGER,
	user_id INTEGER,
	comment TEXT,
	deleted BOOLEAN DEFAULT 0,
	minor_edit BOOLEAN DEFAULT 0,
	edited_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	reverted_from_revision_id INTEGER,

	FOREIGN KEY (article_id) REFERENCES article(id) ON DELETE CASCADE,
	FOREIGN KEY (content_id) REFERENCES content(id) ON DELETE CASCADE,
	FOREIGN KEY (infobox_id) REFERENCES infobox(id) ON DELETE CASCADE,
	FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE SET NULL,
	FOREIGN KEY (reverted_from_revision_id) REFERENCES revision(id) ON DELETE SET NULL
);


        )",
        R"(
CREATE TABLE article (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	namespace_id INTEGER NOT NULL,
	title TEXT NOT NULL,
	revision_id INTEGER,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	is_redirect BOOLEAN DEFAULT 0,
	redirect_article_id INTEGER,
	redirect_reason TEXT,
    language TEXT,
	CHECK ( (is_redirect = 0 AND redirect_article_id IS NULL) OR (is_redirect = 1 AND redirect_article_id IS NOT NULL) ),
	UNIQUE(namespace_id, title),

	FOREIGN KEY (revision_id) REFERENCES revision(id) ON DELETE SET NULL,
	FOREIGN KEY (redirect_article_id) REFERENCES article(id) ON DELETE SET NULL,
	FOREIGN KEY (namespace_id) REFERENCES namespace(id) ON DELETE CASCADE
);


        )",
        R"(
CREATE TABLE wanted_article (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	title TEXT NOT NULL,
	namespace_id INTEGER NOT NULL,
	requested_by_user_id INTEGER,
	requested_at DATETIME DEFAULT CURRENT_TIMESTAMP,
	FOREIGN KEY(namespace_id) REFERENCES namespace(id) ON DELETE CASCADE,
	FOREIGN KEY(requested_by_user_id) REFERENCES user(id) ON DELETE SET NULL,
	UNIQUE(namespace_id, title)
);



        )",

        R"(
CREATE TABLE article_property(
	article_id INTEGER,
	key TEXT,
	value TEXT,
	FOREIGN KEY (article_id) REFERENCES article(id) ON DELETE CASCADE,
	PRIMARY KEY(article_id, key)
);


        )",
        R"(
CREATE TABLE category (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	title TEXT NOT NULL UNIQUE,
	parent_id INTEGER,
	FOREIGN KEY (parent_id) REFERENCES category(id) ON DELETE SET NULL,
	CHECK (parent_id IS NULL OR parent_id <> id)
);


        )",
        R"(
CREATE TABLE article_category (
	article_id INTEGER NOT NULL,
	category_id INTEGER NOT NULL,
	PRIMARY KEY (article_id, category_id),
	FOREIGN KEY (article_id) REFERENCES article(id) ON DELETE CASCADE,
	FOREIGN KEY (category_id) REFERENCES category(id) ON DELETE CASCADE
);


        )",
        R"(
CREATE TABLE article_link(
	from_article_id INTEGER,
	to_article_id INTEGER,
	CHECK (from_article_id <> to_article_id),
	FOREIGN KEY (from_article_id) REFERENCES article(id) ON DELETE CASCADE,
	FOREIGN KEY (to_article_id) REFERENCES article(id) ON DELETE CASCADE,
	PRIMARY KEY(from_article_id, to_article_id)
);


        )",
        R"(
CREATE TABLE category_link(
	from_article_id INTEGER,
	to_category_id INTEGER,
	FOREIGN KEY (from_article_id) REFERENCES article(id) ON DELETE CASCADE,
	FOREIGN KEY (to_category_id) REFERENCES category(id) ON DELETE CASCADE,
	PRIMARY KEY(from_article_id, to_category_id)
);


        )",
        R"(
CREATE TABLE external_link(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	from_article_id INTEGER,
	to_url TEXT,
	UNIQUE(from_article_id, to_url),
	FOREIGN KEY (from_article_id) REFERENCES article(id) ON DELETE CASCADE
);


        )",
//         R"(
// -- Fulltext for article content with join column
// CREATE VIRTUAL TABLE content_fts USING fts5(
// 	content,
// 	content_id UNINDEXED
// );
//
//
//         )",
        R"(
CREATE INDEX idx_revision_article ON revision(article_id);
CREATE INDEX idx_article_namespace_title ON article(namespace_id, title);
CREATE INDEX idx_article_category_category ON article_category(category_id);
CREATE INDEX idx_category_parent ON category(parent_id);
CREATE INDEX idx_revision_edited_at ON revision(edited_at);
CREATE INDEX idx_revision_user ON revision(user_id);
CREATE INDEX idx_article_title ON article(title);
CREATE INDEX idx_article_category_article ON article_category(article_id);


        )",
        R"(
-- Trigger for automatic update of updated_at on new revision
CREATE TRIGGER trg_article_updated_at
AFTER INSERT ON revision
FOR EACH ROW
BEGIN
	UPDATE article
	SET updated_at = CURRENT_TIMESTAMP, revision_id = NEW.id
	WHERE id = NEW.article_id;
END;






        )",
//         R"(
// CREATE TRIGGER trg_content_fts_insert
// AFTER INSERT ON content
// BEGIN
//   INSERT INTO content_fts(rowid, content, content_id) VALUES (new.id, new.content, new.id);
// END;
//
// CREATE TRIGGER trg_content_fts_update
// AFTER UPDATE ON content
// BEGIN
//   UPDATE content_fts SET content = new.content WHERE rowid = new.id;
// END;
//
// CREATE TRIGGER trg_content_fts_delete
// AFTER DELETE ON content
// BEGIN
//   DELETE FROM content_fts WHERE rowid = old.id;
// END;
//
//         )",

//         R"(
// aaaaaaaa
//         )",



    };
}


#endif //MIGRATIONS_H
