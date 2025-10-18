//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/db/sqlite/queries/GetRSessionSelectedItemsSQLiteQuery.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.h"
#include "mindnet/essential/DatabaseType.h"

namespace mindnet::db::sqlite::queries
{
    const std::string SQL_DUE_ONLY= R"(
SELECT n.id AS note_id
FROM note n
JOIN r{algorithm}_state s ON s.note_id = n.id
WHERE s.user_id = {user_id}
  AND s.next_review <= {now_ms}
  AND n.path LIKE (SELECT path || '%' FROM note WHERE id = {filter_under_note})
{order_by}
LIMIT {limit};

)";
    const std::string SQL_NEW_ONLY= R"(SELECT n.id AS note_id
FROM note n
LEFT JOIN r{algorithm}_state s
  ON s.note_id = n.id AND s.user_id = {user_id}
WHERE s.note_id IS NULL
  AND n.path LIKE (SELECT path || '%' FROM note WHERE id = {filter_under_note})
{order_by}
LIMIT {limit};
)";
    const std::string SQL_DUE_AND_NEW= R"(
WITH root AS (
    SELECT path AS prefix FROM note WHERE id = {filter_under_note}
),
due AS (
    SELECT n.id AS note_id
    FROM note n
    JOIN r{algorithm}_state s ON s.note_id = n.id
    WHERE s.user_id = {user_id}
      AND s.next_review <= {now_ms}
      AND n.path LIKE (SELECT prefix || '%' FROM root)
),
new AS (
    SELECT n.id AS note_id
    FROM note n
    LEFT JOIN r{algorithm}_state s
      ON s.note_id = n.id AND s.user_id = {user_id}
    WHERE s.note_id IS NULL
      AND n.path LIKE (SELECT prefix || '%' FROM root)
)
SELECT note_id
FROM (
    SELECT note_id FROM due
    UNION
    SELECT note_id FROM new
)
{order_by}
LIMIT {limit};

)";
    const std::string SQL_ALL= R"(
WITH root AS (
    SELECT path AS prefix FROM note WHERE id = {filter_under_note}
),
due AS (
    SELECT n.id AS note_id
    FROM note n
    JOIN r18_state s ON s.note_id = n.id
    WHERE s.user_id = {user_id}
      AND s.next_review <= {now_ms}
      AND n.path LIKE (SELECT prefix || '%' FROM root)
),
new AS (
    SELECT n.id AS note_id
    FROM note n
    LEFT JOIN r18_state s
      ON s.note_id = n.id AND s.user_id = {user_id}
    WHERE s.note_id IS NULL
      AND n.path LIKE (SELECT prefix || '%' FROM root)
)
SELECT note_id
FROM (
    SELECT note_id FROM due
    UNION
    SELECT note_id FROM new
)
{order_by}
LIMIT {limit};

)";

    GetRSessionSelectedItemsSQLiteQuery::GetRSessionSelectedItemsSQLiteQuery()
    : Query(QUERY_GetRSessionSelectedItemsQuery, "Returns new note ids for repetition session", essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json GetRSessionSelectedItemsSQLiteQuery::call(nlohmann::json request)
    {
        nlohmann::json response;

        if (!request.contains("r_session"))
        {
            throw std::invalid_argument("Mandatory key r_session is missing");
        }
        nlohmann::json session = request["r_session"];

        int algorithm = session["algorithm"];
        int filter_under_note = session["filter_under_note"];
        int schedule = session["schedule"];
        int scope = session["scope"];
        int user_id = session["user_id"];
        int limit = session.value("limit", 100);
        long long now_ms = std::time(nullptr) * 1000LL;

        // --- Select SQL template based on scope ---
        const std::string* sql_template = nullptr;
        switch (scope)
        {
        case 0: sql_template = &SQL_DUE_ONLY; break;
        case 1: sql_template = &SQL_NEW_ONLY; break;
        case 2: sql_template = &SQL_DUE_AND_NEW; break;
        case 3: sql_template = &SQL_ALL; break;
        default:
            throw std::invalid_argument("Invalid scope value");
        }

        // --- ORDER BY based on schedule ---
        std::string order_sql_part;
        switch (schedule)
        {
        case 0: order_sql_part = "ORDER BY n.path"; break;
        case 1: order_sql_part = "ORDER BY n.depth ASC, n.id ASC"; break;
        case 2: order_sql_part = "ORDER BY random()"; break;
        case 3: order_sql_part = "ORDER BY n.path, random()"; break;
        case 4: order_sql_part = "ORDER BY (n.depth % 3), random()"; break;
        case 5: order_sql_part = "ORDER BY s.repetitions ASC, s.next_review ASC"; break;
        case 6: order_sql_part = "ORDER BY n.created_at ASC"; break;
        default:
            throw std::invalid_argument("Invalid schedule value");
        }

        // --- Replace placeholders ---
        std::unordered_map<std::string, std::string> vars = {
            {"algorithm", std::to_string(algorithm)},
            {"user_id", std::to_string(user_id)},
            {"filter_under_note", std::to_string(filter_under_note)},
            {"now_ms", std::to_string(now_ms)},
            {"limit", std::to_string(limit)},
            {"order_by", order_sql_part}
        };
      
        std::string sql = *sql_template;
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

        // --- Execute SQL query ---
        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode=WAL;");

            debug << sql << commit;
            SQLite::Statement query(db, sql);
            std::vector<int64_t> note_ids = nlohmann::json::array();

            while (query.executeStep())
            {
                note_ids.push_back(query.getColumn(0).getInt());
            }

            response["note_ids"] = note_ids;
            response["sql"] = sql;
        }
        catch (SQLite::Exception& e)
        {
            response["error"] = e.what();
            response["sql_failed"] = sql;
        }

        return response;
    }
}
