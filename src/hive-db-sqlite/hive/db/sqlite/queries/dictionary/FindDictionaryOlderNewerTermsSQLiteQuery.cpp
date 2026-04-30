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

#include "../../../../../../../include/hive-db-sqlite/hive/db/sqlite/queries/dictionary/FindDictionaryOlderNewerTermsSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include "hive/db/sqlite/SqliteFileName.hpp"
#include "hive/essential/DatabaseType.hpp"
#include "hive/essential/Global.hpp"

namespace hive::db::sqlite::queries::dictionary
{
    FindDictionaryOlderNewerTermsSQLiteQuery::FindDictionaryOlderNewerTermsSQLiteQuery()
        : Query(QUERY_FindDictionaryOlderNewerTerms, "FindDictionaryOlderNewerTermsSQLiteQuery",
                essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json FindDictionaryOlderNewerTermsSQLiteQuery::call(nlohmann::json& request,
                                                                  api::InvalidateMethod& invalidate_method,
                                                                  plugins::core::models::OptionalError& optional_error)
    {
        nlohmann::json response;

        if (!request.contains("dictionary_map_id"))
        {
            throw std::invalid_argument("Mandatory key dictionary_map_id is missing");
        }
        identification dictionary_map_id = request["dictionary_map_id"];
        if (!request.contains("dictionary_term_id"))
        {
            throw std::invalid_argument("Mandatory key dictionary_term_id is missing");
        }
        identification dictionary_term_id = request["dictionary_term_id"];

        if (!request.contains("mode"))
        {
            throw std::invalid_argument("mode");
        }
        std::string mode = request["mode"];
        bool older = false;
        bool newer = false;
        if (mode == "older") older = true;
        if (mode == "newer") newer = true;
        if (!older && !newer)
        {
            throw std::invalid_argument("Invalid argument mode: " + mode);
        }

        static const std::string sql_older =
            "SELECT max(id) from dictionary_term where id < ? and dictionary_map_id = ?";
        static const std::string sql_newer =
            "SELECT min(id) from dictionary_term where id > ? and dictionary_map_id = ?";

        const std::string& sql = older ? sql_older : sql_newer;

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode=WAL;");


            essential::debug << sql << essential::commit;
            essential::debug << "Looking up " << mode << " term for dictionary_term_id = " << dictionary_term_id <<
                " and dictionary_map_id =" << dictionary_map_id << essential::commit;

            SQLite::Statement query(db, sql);

            int index{0};
            query.bind(++index, dictionary_term_id);
            query.bind(++index, dictionary_map_id);

            if (query.executeStep())
            {
                response["id"] = query.getColumn(0).isNull() ? 0 : query.getColumn(0).getInt64();
            } else
            {
                response["id"] = 0;
            }
        }
        catch (SQLite::Exception& e)
        {
            response["error"] = e.what();
            response["sql_failed"] = sql;
        }

        return response;
    }
}
