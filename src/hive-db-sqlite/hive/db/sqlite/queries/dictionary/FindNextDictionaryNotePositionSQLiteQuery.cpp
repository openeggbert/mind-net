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

#include "../../../../../../../include/hive-db-sqlite/hive/db/sqlite/queries/dictionary/FindNextDictionaryNotePositionSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>

#include "hive/db/sqlite/SqliteFileName.hpp"
#include "hive/essential/DatabaseType.hpp"
#include "hive/essential/Global.hpp"

namespace hive::db::sqlite::queries::dictionary
{
    FindNextDictionaryNotePositionSQLiteQuery::FindNextDictionaryNotePositionSQLiteQuery()
        : Query(QUERY_FindNextDictionaryNotePosition, "FindNextDictionaryNotePositionSQLiteQuery", essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json FindNextDictionaryNotePositionSQLiteQuery::call(nlohmann::json& request,
                                                         api::InvalidateMethod& invalidate_method,
                                                         plugins::core::models::OptionalError& optional_error)
    {
        nlohmann::json response;

        if (!request.contains("dictionary_term_id"))
        {
            throw std::invalid_argument("Mandatory key dictionary_term_id is missing");
        }

        identification dictionary_term_id = request["dictionary_term_id"];

        std::string sql = "select max(position) from dictionary_note where dictionary_term_id = ? ";

        // --- Execute SQL query ---
        try
        {
            identification max_position{};
            identification next_position{};
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode=WAL;");

            essential::debug << sql << essential::commit;
            essential::debug << "Executing FindNextDictionaryNotePositionSQLiteQuery dictionary_term_id=" << dictionary_term_id << essential::commit;
            SQLite::Statement query(db, sql);

            query.bind(1, dictionary_term_id);

            if (query.executeStep())
            {
                max_position = query.isColumnNull(0) ? 0 : query.getColumn(0);
            }

            // Round max_sibling_order up to next hundred and add +100 for next sibling slot
            next_position = ((max_position + 99) / 100 + 1) * 100;

            response["next_position"] = next_position;
        }
        catch (SQLite::Exception& e)
        {
            response["error"] = e.what();
            response["sql_failed"] = sql;
        }

        return response;
    }
}