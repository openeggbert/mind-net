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

#include "../../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/dictionary/FindDictionaryTermMetricsSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/essential/Global.hpp"

namespace mindnet::db::sqlite::queries::dictionary
{
    FindDictionaryTermMetricsSQLiteQuery::FindDictionaryTermMetricsSQLiteQuery()
        : Query(QUERY_FindDictionaryTermMetrics,"FindDictionaryTermMetricsSQLiteQuery", essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json FindDictionaryTermMetricsSQLiteQuery::call(nlohmann::json& request,
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

        if (!request.contains("user_id"))
        {
            throw std::invalid_argument("Mandatory key user_id is missing");
        }
        identification user_id = request["user_id"];

        identification dictionary_term_id{0};
        if (request.contains("dictionary_term_id"))
        {
            dictionary_term_id = request["dictionary_term_id"];
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

static const std::string sql1 = R"(
SELECT
    t.id AS dictionary_term_id,
    COALESCE(tag_cnt.cnt, 0)    AS tag_count,
    COALESCE(flag_cnt.cnt, 0)   AS flag_count,
    COALESCE(link_cnt.cnt, 0)   AS link_count,
    COALESCE(note_cnt.cnt, 0)   AS note_count,
    COALESCE(index_cnt.cnt, 0)  AS index_count,
    COALESCE(source_cnt.cnt, 0) AS source_count,
    COALESCE(alias_cnt.cnt, 0)  AS alias_count,
    COALESCE(visit_cnt.cnt, 0)  AS view_count,
    visit_cnt.last_viewed_at    AS last_viewed_at
FROM dictionary_term t
LEFT JOIN (
    SELECT dictionary_term_id, COUNT(*) cnt
    FROM dictionary_tag GROUP BY dictionary_term_id
) tag_cnt ON tag_cnt.dictionary_term_id = t.id
LEFT JOIN (
    SELECT dictionary_term_id, COUNT(*) cnt
    FROM dictionary_flag GROUP BY dictionary_term_id
) flag_cnt ON flag_cnt.dictionary_term_id = t.id
LEFT JOIN (
    SELECT term_id, COUNT(*) cnt FROM (
        SELECT from_dictionary_term_id AS term_id FROM dictionary_link
        UNION ALL
        SELECT to_dictionary_term_id   AS term_id FROM dictionary_link
    ) GROUP BY term_id
) link_cnt ON link_cnt.term_id = t.id
LEFT JOIN (
    SELECT dictionary_term_id, COUNT(*) cnt
    FROM dictionary_note GROUP BY dictionary_term_id
) note_cnt ON note_cnt.dictionary_term_id = t.id
LEFT JOIN (
    SELECT dictionary_term_id, COUNT(*) cnt
    FROM dictionary_index GROUP BY dictionary_term_id
) index_cnt ON index_cnt.dictionary_term_id = t.id
LEFT JOIN (
    SELECT dictionary_term_id, COUNT(*) cnt
    FROM dictionary_source GROUP BY dictionary_term_id
) source_cnt ON source_cnt.dictionary_term_id = t.id
LEFT JOIN (
    SELECT dictionary_term_id, COUNT(*) cnt
    FROM dictionary_term_alias GROUP BY dictionary_term_id
) alias_cnt ON alias_cnt.dictionary_term_id = t.id
LEFT JOIN (
    SELECT dictionary_term_id, COUNT(*) cnt, MAX(created_at) last_viewed_at
    FROM dictionary_term_visit where user_id = ? GROUP BY dictionary_term_id
) visit_cnt ON visit_cnt.dictionary_term_id = t.id
WHERE (?=1 or t.dictionary_map_id = ?)
)";
        bool term_id_present = dictionary_term_id != 0;
        std::string sql2 = " and t.id = ? ";
        std::string sql3 = " LIMIT ? OFFSET ?";
        std::string sql = sql1 + (term_id_present ? sql2 : "") + sql3;


        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode=WAL;");

            essential::debug << sql << essential::commit;
            essential::debug << "Looking up dictionary metrics for dictionary_map_id =" << dictionary_map_id << essential::commit;

            SQLite::Statement query(db, sql);

            int index{0};
            query.bind(++index, user_id);
            query.bind(++index, dictionary_map_id);
            query.bind(++index, any_map ? 1 : 0);
            if (term_id_present) query.bind(++index, dictionary_term_id);
            query.bind(++index, page_size);
            query.bind(++index, (page_number - 1) * page_size);

            std::vector<nlohmann::json> results;
            while (query.executeStep())
            {
                nlohmann::json result;

                result["dictionary_term_id"] = query.getColumn("dictionary_term_id").getInt64();
                result["dictionary_map_id"] = dictionary_map_id;
                result["tag_count"] = query.getColumn("tag_count").getInt();
                result["flag_count"] = query.getColumn("flag_count").getInt();
                result["link_count"] = query.getColumn("link_count").getInt();
                result["note_count"] = query.getColumn("note_count").getInt();
                result["index_count"] = query.getColumn("index_count").getInt();
                result["source_count"] = query.getColumn("source_count").getInt();
                result["alias_count"] = query.getColumn("alias_count").getInt();
                result["view_count"] = query.getColumn("view_count").getInt();
                result["last_viewed_at"] = query.getColumn("last_viewed_at").getInt64();

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