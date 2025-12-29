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

#include "../../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/dictionary/FindDictionaryTermsForReviewSQLiteQuery.hpp"

#include <SQLiteCpp/SQLiteCpp.h>

#include "mindnet/db/sqlite/SqliteFileName.hpp"
#include "mindnet/essential/DatabaseType.hpp"

namespace mindnet::db::sqlite::queries::dictionary
{
    // -------------------------------------------------------------------------
    // SQL QUERIES
    // -------------------------------------------------------------------------

    static const std::string SQL_DUE = R"(
SELECT t.id
FROM dictionary_term t
JOIN dictionary_state_18 s
  ON s.dictionary_term_id = t.id
WHERE
    t.dictionary_map_id = ?
AND s.user_id = ?
AND s.next_review <= ?
AND t.status != 6
AND t.is_for_repetition = 1
AND (? = 1 OR TRIM(t.definition) <> '')
ORDER BY RANDOM()
LIMIT 100;
)";

    static const std::string SQL_NOT_DUE = R"(
SELECT t.id
FROM dictionary_term t
JOIN dictionary_state_18 s
  ON s.dictionary_term_id = t.id
WHERE
    t.dictionary_map_id = ?
AND s.user_id = ?
AND s.next_review > ?
AND t.status != 6
AND t.is_for_repetition = 1
AND (? = 1 OR TRIM(t.definition) <> '')
ORDER BY RANDOM()
LIMIT 100;
)";

    static const std::string SQL_NEVER = R"(
SELECT t.id
FROM dictionary_term t
WHERE
    t.dictionary_map_id = ?
AND t.status != 6
AND t.is_for_repetition = 1
AND (? = 1 OR TRIM(t.definition) <> '')
AND NOT EXISTS (
    SELECT 1
    FROM dictionary_state_18 s
    WHERE s.dictionary_term_id = t.id
      AND s.user_id = ?
)
ORDER BY RANDOM()
LIMIT 100;
)";

    static const std::string SQL_ALL = R"(
SELECT t.id
FROM dictionary_term t
WHERE
    t.dictionary_map_id = ?
AND t.status != 6
AND (? = 1 OR TRIM(t.definition) <> '')
ORDER BY RANDOM()
LIMIT 100;
)";

    static constexpr int MAX_COUNT_OF_TERMS = 100;

    using BindValue = std::variant<i64, std::string>;

    // -------------------------------------------------------------------------
    // CTOR
    // -------------------------------------------------------------------------

    FindDictionaryTermsForReviewSQLiteQuery::FindDictionaryTermsForReviewSQLiteQuery()
        : Query(
            QUERY_FindDictionaryTermsForReview,
            "Returns term ids for SuperMemo 18 review session",
            essential::DatabaseType::SQLite)
    {
    }

    // -------------------------------------------------------------------------
    // HELPER
    // -------------------------------------------------------------------------

    static void execute_sql_query(
        const std::string& sql,
        const std::string& request_dump,
        std::vector<identification>& term_ids,
        const std::vector<BindValue>& bind_values)
    {
        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);

        essential::debug << sql << essential::commit;
        essential::debug << "Request = " << request_dump << essential::commit;

        SQLite::Statement stmt(db, sql);

        int index = 0;
        for (const auto& v : bind_values)
        {
            if (std::holds_alternative<i64>(v))
            {
                stmt.bind(++index, std::get<i64>(v));
            }
            else
            {
                stmt.bind(++index, std::get<std::string>(v));
            }
        }

        while (stmt.executeStep())
        {
            term_ids.push_back(stmt.getColumn(0).getInt());
            if (term_ids.size() >= MAX_COUNT_OF_TERMS)
                break;
        }
    }

    // -------------------------------------------------------------------------
    // CALL
    // -------------------------------------------------------------------------

    nlohmann::json FindDictionaryTermsForReviewSQLiteQuery::call(
        nlohmann::json& request,
        api::InvalidateMethod&,
        plugins::core::models::OptionalError&)
    {
        nlohmann::json response;

        if (!request.contains("dictionary_map_id"))
            throw std::invalid_argument("Mandatory key dictionary_map_id is missing");
        if (!request.contains("user_id"))
            throw std::invalid_argument("Mandatory key user_id is missing");
        if (!request.contains("algorithm"))
            throw std::invalid_argument("Mandatory key algorithm is missing");

        identification dictionary_map_id = request["dictionary_map_id"];
        identification user_id = request["user_id"];
        identification algorithm = request["algorithm"];

        if (algorithm != 18)
            throw std::invalid_argument("Unsupported algorithm: " + std::to_string(algorithm));

        bool is_due = request.value("is_due", false);
        bool is_not_due = request.value("is_not_due", false);
        bool is_never = request.value("is_never", false);
        bool has_definition = request.value("has_definition", true);

        // default: due + new
        if (!(is_due || is_not_due || is_never))
        {
            is_due = true;
            is_never = true;
        }

        bool select_all = is_due && is_not_due && is_never;

        const i64 now_ms = static_cast<i64>(std::time(nullptr)) * 1000LL;
        const std::string request_dump = request.dump();

        std::vector<identification> term_ids;

        try
        {
            if (select_all)
            {
                execute_sql_query(
                    SQL_ALL,
                    request_dump,
                    term_ids,
                    {
                        dictionary_map_id,
                        has_definition ? 0LL : 1LL
                    });
            }
            else
            {
                if (is_due)
                {
                    execute_sql_query(
                        SQL_DUE,
                        request_dump,
                        term_ids,
                        {
                            dictionary_map_id,
                            user_id,
                            now_ms,
                            has_definition ? 0LL : 1LL
                        });
                }

                if (is_never)
                {
                    execute_sql_query(
                        SQL_NEVER,
                        request_dump,
                        term_ids,
                        {
                            dictionary_map_id,
                            has_definition ? 0LL : 1LL,
                            user_id
                        });
                }

                if (is_not_due)
                {
                    execute_sql_query(
                        SQL_NOT_DUE,
                        request_dump,
                        term_ids,
                        {
                            dictionary_map_id,
                            user_id,
                            now_ms,
                            has_definition ? 0LL : 1LL
                        });
                }
            }
        }
        catch (const SQLite::Exception& e)
        {
            response["error"] = e.what();
            return response;
        }

        response["term_ids"] = term_ids;
        return response;
    }

} // namespace
