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

#include "../../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/dictionary/FindDictionaryFlagsSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/essential/Global.hpp"

namespace mindnet::db::sqlite::queries::dictionary
{
    FindDictionaryFlagsSQLiteQuery::FindDictionaryFlagsSQLiteQuery()
        : Query(QUERY_FindDictionaryFlags,"FindDictionaryFlagsSQLiteQuery", essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json FindDictionaryFlagsSQLiteQuery::call(nlohmann::json& request,
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

        if (!request.contains("user_id"))
        {
            throw std::invalid_argument("Mandatory key user_id is missing");
        }

        identification user_id = request["user_id"];
        bool all_titles = title_part == "***";

        int page_size = request["page_size"];
        int page_number = request["page_number"];
        if (page_number * page_size > 50)
        {
            page_size = 50;
            page_number = 1;
        }
        // int page_size = 20;
        // int page_number = 1;

        static std::string sql_filtered_titles = R"(
select distinct title
from dictionary_flag
where (?=1 or dictionary_map_id = ?)
  and title like ?
  and (
        is_public = 1
        or (is_public = 0 and user_id = ?)
      )
order by random()
limit ? offset ?

)";

        static std::string sql_all_title = R"(
select distinct title
from dictionary_flag
where (?=1 or dictionary_map_id = ?)
  and (
        is_public = 1
        or (is_public = 0 and user_id = ?)
      )
order by random()
limit ? offset ?

)";
        std::string sql = all_titles ? sql_all_title : sql_filtered_titles;

// index
// create index idx_dictionary_flag_autocomplete
// on dictionary_flag(dictionary_map_id, is_public, user_id, title);

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode=WAL;");

            essential::debug << sql << essential::commit;
            essential::debug << "Looking up flag (title) for dictionary_map_id=" << dictionary_map_id << " and title_part=" << title_part << essential::commit;

            std::string pattern = "%" + title_part + "%";

            SQLite::Statement query(db, sql);
            int index = 0;
            query.bind(++index, any_map ? 1 : 0);
            query.bind(++index, dictionary_map_id);
            if (!all_titles) query.bind(++index, pattern);
            query.bind(++index, user_id);

            query.bind(++index, page_size);
            query.bind(++index, (page_number - 1) * page_size);

            std::vector<std::string> results;
            while (query.executeStep())
            {
                std::string title = query.getColumn(0);
                results.push_back(title);
            }
            std::sort(results.begin(), results.end());
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