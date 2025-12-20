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
    static const std::string SQL_DUE = R"(
SELECT t.id AS term_id
FROM dictionary_term t
JOIN dictionary_state_4 s ON s.dictionary_term_id = t.id
AND t.dictionary_map_id = ?
WHERE s.user_id = ?
  AND s.next_review <= ? --now_ms
  AND (? = 1 OR TRIM(t.definition) <> '')

AND t.dictionary_map_id = ?

ORDER BY RANDOM() LIMIT 100;
)";

    static const std::string SQL_NEW = R"(
SELECT t.id AS term_id
FROM dictionary_term t
LEFT JOIN dictionary_state_4 s
  ON s.dictionary_term_id = t.id AND s.user_id = ?
AND t.dictionary_map_id = ?
WHERE s.dictionary_term_id IS NULL

  AND (? = 1 OR TRIM(t.definition) <> '')

AND t.dictionary_map_id = ?

ORDER BY RANDOM() LIMIT 100;
)";

    static const std::string SQL_NOT_DUE = R"(
SELECT t.id AS term_id
FROM dictionary_term t
JOIN dictionary_state_4 s ON s.dictionary_term_id = t.id
AND t.dictionary_map_id = ?
WHERE s.user_id = ?
  AND s.next_review > ? --now_ms
  AND (? = 1 OR TRIM(t.definition) <> '')

AND t.dictionary_map_id = ?

ORDER BY RANDOM() LIMIT 100;
)";

    static const std::string SQL_ALL = R"(
SELECT t.id AS term_id
FROM dictionary_term t

WHERE
    (? = 1 OR TRIM(t.definition) <> '')

AND t.dictionary_map_id = ?

ORDER BY RANDOM() LIMIT 100;
)";
    static constexpr int MAX_COUNT_OF_TERMS = 100;

    using BindValue = std::variant<std::string, i64>;

    FindDictionaryTermsForReviewSQLiteQuery::FindDictionaryTermsForReviewSQLiteQuery()
        : Query(QUERY_FindDictionaryTermsForReview, "Returns term ids for repetition session",
                essential::DatabaseType::SQLite)
    {
    }

    void execute_sql_query(
        const std::string& sql,
        const string& request_string,
        std::vector<identification>& term_ids,
        const std::vector<BindValue>& bind_values)
    {
        SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
        db.exec("PRAGMA foreign_keys = ON;");
        db.exec("PRAGMA journal_mode=WAL;");

        essential::debug << sql << essential::commit;
        essential::debug << "Executing =" << request_string << essential::commit;
        SQLite::Statement stmt(db, sql);
        int i = 0;
        for (auto& v : bind_values)
        {
            if (holds_alternative<std::string>(v))
            {
                std::string s = std::get<std::string>(v);
                stmt.bind(++i, s);
            }
            else if (holds_alternative<i64>(v))
            {
                i64 n = std::get<i64>(v);
                stmt.bind(++i, n);
            }
            else
            {
                throw std::runtime_error("Unsupported variant.");
            }
        }

        while (stmt.executeStep())
        {
            term_ids.push_back(stmt.getColumn(0).getInt());
            if (term_ids.size() >= MAX_COUNT_OF_TERMS) break;
        }
    }

    nlohmann::json FindDictionaryTermsForReviewSQLiteQuery::call(nlohmann::json& request,
                                                                 api::InvalidateMethod& invalidate_method,
                                                                 plugins::core::models::OptionalError& optional_error)
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
        identification algorithm = request.value("algorithm", 4);
        if (algorithm != 4)
            throw std::invalid_argument("Unsupported algorithm: " + std::to_string(algorithm));

        bool is_due = request.value("is_due", 0);
        bool is_new = request.value("is_new", 0);
        bool is_not_due = request.value("is_not_due", 0);
        bool include_empty_definition = request.value("include_empty_definition", 0);

        if (!(is_due || is_new || is_not_due))
        {
            is_due = true;
            is_new = true;
        }
        bool is_all = is_due && is_new && is_not_due;
        if (is_all)
        {
            is_due = false;
            is_new = false;
            is_not_due = false;
        }

        long long now_ms = std::time(nullptr) * 1000LL;
        string request_string = request.dump();
        // --- Execute SQL query ---
        std::vector<identification> term_ids;
        if (is_due)
            try
            {
                std::vector<BindValue> bind_values;
                bind_values.push_back(dictionary_map_id);
                bind_values.push_back(user_id);
                bind_values.push_back(now_ms);
                bind_values.push_back(include_empty_definition? 1 : 0);
                bind_values.push_back(dictionary_map_id);
                execute_sql_query(SQL_DUE,request_string,term_ids,bind_values);
            }
            catch (SQLite::Exception& e)
            {
                response["error"] = e.what();
                response["sql_failed"] = SQL_DUE;
                return response;
            }
        if (is_new)
            try
            {
                std::vector<BindValue> bind_values;
                bind_values.push_back(user_id);
                bind_values.push_back(dictionary_map_id);
                bind_values.push_back(include_empty_definition? 1 : 0);
                bind_values.push_back(dictionary_map_id);
                execute_sql_query(SQL_NEW, request_string,term_ids,bind_values);
            }
            catch (SQLite::Exception& e)
            {
                response["error"] = e.what();
                response["sql_failed"] = SQL_NEW;
                return response;
            }
        if (is_not_due)
            try
            {
                std::vector<BindValue> bind_values;
                bind_values.push_back(dictionary_map_id);
                bind_values.push_back(user_id);
                bind_values.push_back(now_ms);
                bind_values.push_back(include_empty_definition? 1 : 0);
                bind_values.push_back(dictionary_map_id);
                execute_sql_query(SQL_NOT_DUE,request_string,term_ids,bind_values);
            }
            catch (SQLite::Exception& e)
            {
                response["error"] = e.what();
                response["sql_failed"] = SQL_NOT_DUE;
                return response;
            }
        if (is_all)
            try
            {
                std::vector<BindValue> bind_values;
                bind_values.push_back(include_empty_definition? 1 : 0);
                bind_values.push_back(dictionary_map_id);
                execute_sql_query(SQL_ALL,request_string,term_ids,bind_values);
            }
            catch (SQLite::Exception& e)
            {
                response["error"] = e.what();
                response["sql_failed"] = SQL_ALL;
                return response;
            }

        response["term_ids"] = term_ids;
        return response;
    }
}
