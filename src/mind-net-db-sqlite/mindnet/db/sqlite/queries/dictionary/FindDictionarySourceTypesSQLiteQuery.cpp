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

#include "../../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/dictionary/FindDictionarySourceTypesSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/essential/Global.hpp"

namespace mindnet::db::sqlite::queries::dictionary
{
    FindDictionarySourceTypesSQLiteQuery::FindDictionarySourceTypesSQLiteQuery()
        : Query(QUERY_FindDictionarySourceTypes,"FindDictionarySourceTypesSQLiteQuery", essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json FindDictionarySourceTypesSQLiteQuery::call(nlohmann::json& request,
                                                            api::InvalidateMethod& invalidate_method)
    {
        nlohmann::json response;

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

        static std::string sql = "select id, title, edition from dictionary_source_type where title like ? limit ? offset ?";

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode=WAL;");

            essential::debug << sql << essential::commit;
            essential::debug << "Looking up tag_types (id, title) for title_part=" << title_part << essential::commit;

            SQLite::Statement query(db, sql);
            std::string pattern = "%" + title_part + "%";
            query.bind(1, pattern);
            query.bind(2, page_size);
            query.bind(3, (page_number - 1) * page_size);

            std::vector<std::pair<identification, std::string>> results;
            while (query.executeStep())
            {
                identification id = query.getColumn(0);
                std::string title = query.getColumn(1);
                std::string edition = query.getColumn(2);
                if (!edition.empty())
                {
                    title = title + " (" + edition + ")";
                }
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