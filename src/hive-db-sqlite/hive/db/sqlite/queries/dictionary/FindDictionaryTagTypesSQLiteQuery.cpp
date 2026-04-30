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

#include "../../../../../../../include/hive-db-sqlite/hive/db/sqlite/queries/dictionary/FindDictionaryTagTypesSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include "hive/db/sqlite/SqliteFileName.hpp"
#include "hive/essential/DatabaseType.hpp"
#include "hive/essential/Global.hpp"

namespace hive::db::sqlite::queries::dictionary
{
    FindDictionaryTagTypesSQLiteQuery::FindDictionaryTagTypesSQLiteQuery()
        : Query(QUERY_FindDictionaryTagTypes,"FindDictionaryTagTypesSQLiteQuery", essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json FindDictionaryTagTypesSQLiteQuery::call(nlohmann::json& request,
    api::InvalidateMethod& invalidate_method,
plugins::core::models::OptionalError& optional_error)
    {
        nlohmann::json response;

        if (!request.contains("dictionary_map_id"))
        {
            throw std::invalid_argument("Mandatory key dictionary_map_id is missing");
        }

        identification dictionary_map_id = request["dictionary_map_id"];
        bool any_map = dictionary_map_id == 0;

        if (!request.contains("title_part"))
        {
            throw std::invalid_argument("Mandatory key title_part is missing");
        }

        string title_part = request["title_part"];

        // int page_size = request["page_size"];
        // int page_number = request["page_number"];
        // if (page_number * page_size > 50)
        // {
        //     page_size = 50;
        //     page_number = 1;
        // }
        int page_size = 20;
        int page_number = 1;

        static std::string sql = "select id, title from dictionary_tag_type where (?=1 or dictionary_map_id = ?) and title like ? limit ? offset ?";

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode=WAL;");

            essential::debug << sql << essential::commit;
            essential::debug << "Looking up tag_types (id, title) for dictionary_map_id=" << dictionary_map_id << " and title_part=" << title_part << essential::commit;

            SQLite::Statement query(db, sql);
            query.bind(1, any_map ? 1 : 0);
            query.bind(2, dictionary_map_id);
            std::string pattern = "%" + title_part + "%";
            query.bind(3, pattern);
            query.bind(4, page_size);
            query.bind(5, (page_number - 1) * page_size);

            std::vector<std::pair<identification, std::string>> results;
            while (query.executeStep())
            {
                identification id = query.getColumn(0);
                std::string title = query.getColumn(1);
                results.push_back(std::make_pair(id, title));
            }
            response["results"] = results;
        }
        catch (SQLite::Exception& e)
        {
            response["error"] = e.what();
            response["sql_failed"] = sql;
        }

        return response;
    }
}