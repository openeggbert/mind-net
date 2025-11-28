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

#include "mindnet/db/sqlite/queries/FindNotesInMapSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/essential/Global.hpp"

namespace mindnet::db::sqlite::queries
{
    FindNotesInMapSQLiteQuery::FindNotesInMapSQLiteQuery()
        : Query(QUERY_FindNotesInMap, "FindNotesInMapSQLiteQuery", essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json FindNotesInMapSQLiteQuery::call(nlohmann::json& request, api::InvalidateMethod& invalidate_method)
    {
        nlohmann::json response;

        if (!request.contains("note_titles"))
        {
            throw std::invalid_argument("Mandatory key note_titles is missing");
        }

        if (!request.contains("map_id"))
        {
            throw std::invalid_argument("Mandatory key map_id is missing");
        }
        std::vector<std::string> note_titles = request["note_titles"];

        identification map_id = request["map_id"];

        std::string sql = "select id, title from note where map_id = ? and title in (";
        int note_titles_index = 0;
        int note_titles_last_index = note_titles.size() - 1;
        for (auto& m : note_titles)
        {
            sql += "?";
            note_titles_last_index = note_titles.size() - 1;
            if (note_titles_index < note_titles_last_index)
            {
                sql += ",";
            }
            note_titles_index++;
        }
        sql += ")";

        // --- Execute SQL query ---
        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode=WAL;");

            essential::debug << sql << essential::commit;
            essential::debug << "Executing FindNotesInMapSQLiteQuery map_id " << map_id << essential::commit;
            SQLite::Statement query(db, sql);
            std::vector<string> found_note_titles;
            std::map<string, identification> found_note_ids;
            int index = 0;
            query.bind(++index, map_id);
            for (auto& m : note_titles)
            {
                query.bind(++index, m);
            }

            while (query.executeStep())
            {
                found_note_ids[query.getColumn(1)] = query.getColumn(0).getInt64();
                found_note_titles.push_back(query.getColumn(1));
            }

            response["found_note_titles"] = found_note_titles;
            response["found_note_ids"] = found_note_ids;
        }
        catch (SQLite::Exception& e)
        {
            essential::err << e.what() << essential::commit;
            response["error"] = e.what();
            response["sql_failed"] = sql;
        }

        return response;
    }
}