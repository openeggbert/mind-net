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
#include "../../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/slipbox/GetQuestionIdsSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.hpp"
#include "mindnet/essential/DatabaseType.hpp"

namespace mindnet::db::sqlite::queries::slipbox
{
    const std::string SQL = R"(
SELECT q.id
FROM question q
JOIN note n ON n.id = q.note_id
{map_join}
WHERE
      q.question_text IS NOT NULL
  AND TRIM(q.question_text) <> ''

  AND (
         {filter_under_note} = 0
         OR n.path LIKE (
              SELECT path || '%'
              FROM note
              WHERE id = {filter_under_note}
         )
  )

  {map_where}

ORDER BY random()
LIMIT {limit};
)";

    GetQuestionIdsSQLiteQuery::GetQuestionIdsSQLiteQuery()
        : Query(QUERY_GetQuestionIds,
                "Returns random question ids under given note (or whole map)",
                essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json GetQuestionIdsSQLiteQuery::call(
        nlohmann::json& request,
        api::InvalidateMethod& invalidate_method,
 plugins::core::models::OptionalError& optional_error)
    {
        nlohmann::json response;

        if (!request.contains("test"))
            throw std::invalid_argument("Mandatory key 'test' missing");

        nlohmann::json test = request["test"];

        // --- mandatory inputs ---
        identification under_note_id = test.value("under_note_id", 0);
        identification map_id = test.value("map_id", 0);
        int limit = test.value("answer_count_limit", 100);

        // --- prepare SQL variables ---
        std::string filter_under_note_sql = std::to_string(under_note_id);

        std::string map_join_sql;
        std::string map_where_sql;

        if (map_id != 0)
        {
            map_join_sql = "";
            map_where_sql = "AND n.map_id = " + std::to_string(map_id);
        }
        else
        {
            // no map filtering
            map_join_sql = "";
            map_where_sql = "";
        }

        std::unordered_map<std::string, std::string> vars = {
            {"filter_under_note", filter_under_note_sql},
            {"map_join", map_join_sql},
            {"map_where", map_where_sql},
            {"limit", std::to_string(limit)}
        };

        std::string sql = SQL;
        for (const auto& [key, val] : vars)
        {
            std::string placeholder = "{" + key + "}";
            size_t pos = 0;
            while ((pos = sql.find(placeholder, pos)) != std::string::npos)
            {
                sql.replace(pos, placeholder.size(), val);
                pos += val.size();
            }
        }

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode=WAL;");

            essential::debug << sql << essential::commit;

            SQLite::Statement query(db, sql);
            std::string ids;

            while (query.executeStep())
            {
                ids += query.getColumn(0).getText();
                ids += ",";
            }
            if (!ids.empty() && ids.at(ids.size() - 1) == ',')
            {
                ids.pop_back();
            }

            response["question_ids"] = ids;
        }
        catch (SQLite::Exception& e)
        {
            response["error"] = e.what();
            response["sql_failed"] = sql;
        }

        return response;
    }
}