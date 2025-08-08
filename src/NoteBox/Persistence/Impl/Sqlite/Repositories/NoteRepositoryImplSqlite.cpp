///////////////////////////////////////////////////////////////////////////////////////////////
// note-box: Note management tool.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see
// <https://www.gnu.org/licenses/> or write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////

/**
 *
 * @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */

#include "NoteBox/Persistence/Impl/Sqlite/Repositories/NoteRepositoryImplSqlite.h"
#include "NoteBox/Entity/Note.h"

#include <iostream>
#include <string>
#include <vector>

#include "SQLiteCpp/Database.h"
#include "NoteBox/Persistence/Impl/Sqlite/SqliteFileName.h"

namespace NoteBox::Impl::Sqlite::Repositories
{
    using Persistence::Impl::Sqlite::SQLITE_FILE_NAME;

    NoteRepositoryImplSqlite::NoteRepositoryImplSqlite() = default;

    NoteRepositoryImplSqlite::~NoteRepositoryImplSqlite() = default;

    void NoteRepositoryImplSqlite::create(const Entity::Note& note)
    {
        std::string sql = "INSERT INTO NOTE (ID, PARENT_NOTE_ID, TITLE, CONTENT_ID, QUESTION, CREATED_AT, UPDATED_AT, "
            "LAST_SHOWN_AT, LAST_REVIEWED_AT, REVIEW_IN_X_DAYS, EXPIRES_AT, IMPORTANCE, DIFFICULTY, SOURCE_ID) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            query.bind(1, note.id);
            query.bind(2, note.parent_note_id);
            query.bind(3, note.title);
            query.bind(4, note.content_id);
            query.bind(5, note.question);
            query.bind(6, static_cast<int64_t>(note.created_at));
            query.bind(7, static_cast<int64_t>(note.updated_at));
            query.bind(8, static_cast<int64_t>(note.last_shown_at));
            query.bind(9, static_cast<int64_t>(note.last_reviewed_at));
            query.bind(10, note.review_in_x_days);
            query.bind(11, static_cast<int64_t>(note.expires_at));
            query.bind(12, note.importance);
            query.bind(13, note.difficulty);
            query.bind(14, note.source_id);

            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            std::cerr << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    Entity::Note NoteRepositoryImplSqlite::read(const std::string& id)
    {
        std::string sql = "SELECT ID, PARENT_NOTE_ID, TITLE, CONTENT_ID, QUESTION, CREATED_AT, UPDATED_AT, "
            "LAST_SHOWN_AT, LAST_REVIEWED_AT, REVIEW_IN_X_DAYS, EXPIRES_AT, IMPORTANCE, DIFFICULTY, SOURCE_ID "
            "FROM NOTE WHERE ID = ?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            query.bind(1, id);

            if (query.executeStep())
            {
                using std::string;
                string note_id = query.getColumn(0).getString();
                string title = query.getColumn(2).getString();
                string content_id = query.getColumn(3).getString();

                Entity::Note note(note_id, title, content_id);
                note.parent_note_id = query.getColumn(1).getString();
                note.question = query.getColumn(4).getString();
                note.created_at = query.getColumn(5).getInt64();
                note.updated_at = query.getColumn(6).getInt64();
                note.last_shown_at = query.getColumn(7).getInt64();
                note.last_reviewed_at = query.getColumn(8).getInt64();
                note.review_in_x_days = query.getColumn(9).getInt();
                note.expires_at = query.getColumn(10).getInt64();
                note.importance = query.getColumn(11).getInt();
                note.difficulty = query.getColumn(12).getInt();
                note.source_id = query.getColumn(13).getInt();

                return note;
            }
            throw std::runtime_error("Note not found");
        }
        catch (SQLite::Exception& e)
        {
            std::cerr << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    std::vector<Entity::Note> NoteRepositoryImplSqlite::list(std::string& parent_note_id)
    {
        std::string sql = "SELECT ID, PARENT_NOTE_ID, TITLE, CONTENT_ID, QUESTION, CREATED_AT, UPDATED_AT, "
            "LAST_SHOWN_AT, LAST_REVIEWED_AT, REVIEW_IN_X_DAYS, EXPIRES_AT, IMPORTANCE, DIFFICULTY, SOURCE_ID "
            "FROM NOTE WHERE PARENT_NOTE_ID = ?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            query.bind(1, parent_note_id);
            std::vector<Entity::Note> notes;

            while (query.executeStep())
            {
                using std::string;

                string note_id = query.getColumn(0).getString();
                string title = query.getColumn(2).getString();
                string content_id = query.getColumn(3).getString();

                Entity::Note note(note_id, title, content_id);
                note.parent_note_id = query.getColumn(1).getString();
                note.question = query.getColumn(4).getString();
                note.created_at = query.getColumn(5).getInt64();
                note.updated_at = query.getColumn(6).getInt64();
                note.last_shown_at = query.getColumn(7).getInt64();
                note.last_reviewed_at = query.getColumn(8).getInt64();
                note.review_in_x_days = query.getColumn(9).getInt();
                note.expires_at = query.getColumn(10).getInt64();
                note.importance = query.getColumn(11).getInt();
                note.difficulty = query.getColumn(12).getInt();
                note.source_id = query.getColumn(13).getInt();

                notes.push_back(note);
            }

            return notes;
        }
        catch (SQLite::Exception& e)
        {
            std::cerr << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    void NoteRepositoryImplSqlite::remove(const std::string& id)
    {
        std::string sql = "DELETE FROM NOTE WHERE ID = ?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            query.bind(1, id);
            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            std::cerr << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    void NoteRepositoryImplSqlite::update(const Entity::Note& note)
    {
        std::string sql = "UPDATE NOTE SET PARENT_NOTE_ID = ?, TITLE = ?, CONTENT_ID = ?, QUESTION = ?, "
            "CREATED_AT = ?, UPDATED_AT = ?, LAST_SHOWN_AT = ?, LAST_REVIEWED_AT = ?, "
            "REVIEW_IN_X_DAYS = ?, EXPIRES_AT = ?, IMPORTANCE = ?, DIFFICULTY = ?, SOURCE_ID = ? "
            "WHERE ID = ?";

        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement query(db, sql);

        try
        {
            query.bind(1, note.parent_note_id);
            query.bind(2, note.title);
            query.bind(3, note.content_id);
            query.bind(4, note.question);
            query.bind(5, static_cast<int64_t>(note.created_at));
            query.bind(6, static_cast<int64_t>(note.updated_at));
            query.bind(7, static_cast<int64_t>(note.last_shown_at));
            query.bind(8, static_cast<int64_t>(note.last_reviewed_at));
            query.bind(9, note.review_in_x_days);
            query.bind(10, static_cast<int64_t>(note.expires_at));
            query.bind(11, note.importance);
            query.bind(12, note.difficulty);
            query.bind(13, note.source_id);
            query.bind(14, note.id);

            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            std::cerr << "Exception during SQLite statement execution: " << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }
}
