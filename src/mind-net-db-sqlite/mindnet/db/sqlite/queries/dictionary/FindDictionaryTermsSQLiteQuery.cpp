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

#include "../../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/dictionary/FindDictionaryTermsSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/util/triple.hpp"

namespace mindnet::db::sqlite::queries::dictionary
{
    FindDictionaryTermsSQLiteQuery::FindDictionaryTermsSQLiteQuery()
        : Query(QUERY_FindDictionaryTerms, "FindDictionaryTermsSQLiteQuery", essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json FindDictionaryTermsSQLiteQuery::call(nlohmann::json& request,
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
        bool include_aliases = false;
        if (request.contains("include_aliases"))
        {
            include_aliases = request["include_aliases"] == true;
        }

        // int page_size = request["page_size"];
        // int page_number = request["page_number"];
        // if (page_number * page_size > 50)
        // {
        //     page_size = 50;
        //     page_number = 1;
        // }
        int page_size = 20;
        int page_number = 1;

        bool random = include_aliases ? false : title_part == "*";
        static std::string sql_standard_with_alias = R"(
SELECT DISTINCT
    t.id,
    t.title,
    t.disambiguation,
    '' AS alias_used
FROM dictionary_term t
WHERE (? = 1 OR t.dictionary_map_id = ?)
  AND t.title LIKE ?
  AND NOT EXISTS (
      SELECT 1
      FROM dictionary_term_alias a
      WHERE a.dictionary_term_id = t.id
        AND a.alias LIKE ?
  )

UNION

SELECT DISTINCT
    t.id,
    t.title,
    t.disambiguation,
    a.alias AS alias_used
FROM dictionary_term_alias a
JOIN dictionary_term t ON t.id = a.dictionary_term_id
WHERE (? = 1 OR t.dictionary_map_id = ?)
  AND a.alias LIKE ?

LIMIT ? OFFSET ?;

)";

        static std::string sql_standard =
            "select id, title, disambiguation from dictionary_term where (?=1 or dictionary_map_id = ?) and title like ? limit ? offset ?";
        static std::string sql_random =
            "select id, title, disambiguation from dictionary_term where (?=1 or dictionary_map_id = ?) order by random() limit ?";
        std::string& sql = random ? sql_random : (include_aliases ? sql_standard_with_alias : sql_standard);

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode=WAL;");

            essential::debug << sql << essential::commit;
            essential::debug << "Looking up terms (id, title) for dictionary_map_id=" << dictionary_map_id <<
                " and title_part=" << title_part << essential::commit;

            SQLite::Statement query(db, sql);
            int index = 0;
            query.bind(++index, any_map ? 1 : 0);
            query.bind(++index, dictionary_map_id);
            if (!random)
            {
                std::string pattern = "%" + title_part + "%";
                query.bind(++index, pattern);
            }
            if (include_aliases)
            {
                std::string pattern = "%" + title_part + "%";
                query.bind(++index, pattern);
                query.bind(++index, any_map ? 1 : 0);
                query.bind(++index, dictionary_map_id);
                query.bind(++index, pattern);
            }
            query.bind(++index, page_size);
            if (!random) query.bind(++index, (page_number - 1) * page_size);

            std::vector<nlohmann::json> results;

            while (query.executeStep())
            {
                nlohmann::json result;
                identification id = query.getColumn(0);
                std::string title = query.getColumn(1);
                std::string disambiguation = query.getColumn(2);
                std::string alias = include_aliases ? query.getColumn(3) : "";
                result["id"] = id;
                result["dictionary_term_id"] = id;
                result["title"] = title;
                result["disambiguation"] = disambiguation;
                result["alias"] = alias;
                results.push_back(result);
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
