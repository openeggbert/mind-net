//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/db/sqlite/queries/GetRSessionSelectedItemsSQLiteQuery.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.h"
#include "mindnet/essential/DatabaseType.h"

namespace mindnet::db::sqlite::queries
{
    const std::string SQL_DUE_ONLY = R"(
SELECT n.id AS note_id
FROM note n
JOIN r{algorithm}_state s ON s.note_id = n.id
{map_join}
WHERE s.user_id = {user_id}
  AND s.next_review <= {now_ms}

  AND n.content_id IS NOT NULL
  AND EXISTS (
      SELECT 1 FROM content c
      WHERE c.id = n.content_id AND TRIM(c.value) <> ''
  )

  AND (
    {filter_under_note} = 0
    OR n.path LIKE (SELECT path || '%' FROM note WHERE id = {filter_under_note})
  )

  {map_where}

{order_by}
LIMIT {limit};
)";

    const std::string SQL_NEW_ONLY = R"(
SELECT n.id AS note_id
FROM note n
LEFT JOIN r{algorithm}_state s
  ON s.note_id = n.id AND s.user_id = {user_id}
{map_join}
WHERE s.note_id IS NULL

  AND n.content_id IS NOT NULL
  AND EXISTS (
      SELECT 1 FROM content c
      WHERE c.id = n.content_id AND TRIM(c.value) <> ''
  )

  AND (
    {filter_under_note} = 0
    OR n.path LIKE (SELECT path || '%' FROM note WHERE id = {filter_under_note})
  )

  {map_where}

{order_by}
LIMIT {limit};
)";


    const std::string SQL_DUE_AND_NEW = R"(
WITH due AS (
    SELECT n.id AS note_id
    FROM note n
    JOIN r{algorithm}_state s ON s.note_id = n.id
    WHERE s.user_id = {user_id}
      AND s.next_review <= {now_ms}
),
new AS (
    SELECT n.id AS note_id
    FROM note n
    LEFT JOIN r{algorithm}_state s
      ON s.note_id = n.id AND s.user_id = {user_id}
    WHERE s.note_id IS NULL
)
SELECT n.id AS note_id
FROM note n
JOIN (
    SELECT note_id FROM due
    UNION
    SELECT note_id FROM new
) x ON x.note_id = n.id
{map_join}

WHERE
      n.content_id IS NOT NULL
  AND EXISTS (
      SELECT 1 FROM content c
      WHERE c.id = n.content_id AND TRIM(c.value) <> ''
  )
  AND (
       {filter_under_note} = 0
       OR n.path LIKE (SELECT path || '%' FROM note WHERE id = {filter_under_note})
  )

  {map_where}

{order_by}
LIMIT {limit};
)";



    const std::string SQL_ALL = R"(
SELECT n.id AS note_id
FROM note n
{map_join}
WHERE
      n.content_id IS NOT NULL
  AND EXISTS (
      SELECT 1 FROM content c
      WHERE c.id = n.content_id AND TRIM(c.value) <> ''
  )
  AND (
       {filter_under_note} = 0
       OR n.path LIKE (SELECT path || '%' FROM note WHERE id = {filter_under_note})
  )

  {map_where}

{order_by}
LIMIT {limit};
)";


    GetRSessionSelectedItemsSQLiteQuery::GetRSessionSelectedItemsSQLiteQuery()
        : Query(QUERY_GetRSessionSelectedItemsQuery, "Returns new note ids for repetition session",
                essential::DatabaseType::SQLite)
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

        int map_id = session["map_id"];
        int map_collection_id = session["map_collection_id"];
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
        case 0: sql_template = &SQL_DUE_ONLY;
            break;
        case 1: sql_template = &SQL_NEW_ONLY;
            break;
        case 2: sql_template = &SQL_DUE_AND_NEW;
            break;
        case 3: sql_template = &SQL_ALL;
            break;
        default:
            throw std::invalid_argument("Invalid scope value");
        }

        // --- ORDER BY based on schedule ---
        std::string order_sql_part;

        switch (schedule)
        {
        case 0: order_sql_part = "ORDER BY n.path";
            break; // DepthFirst
        case 1: order_sql_part = "ORDER BY n.depth ASC, n.id ASC";
            break; // BreadthFirst
        case 2: order_sql_part = "ORDER BY random()";
            break; // Random
        case 3: order_sql_part = "ORDER BY n.path, random()";
            break; // DepthFirstShuffled
        case 4: order_sql_part = "ORDER BY (n.depth % 3), random()";
            break; // Interleaved
        case 5: // DifficultySorted
            if (scope == 0) // DueOnly
                order_sql_part = "ORDER BY s.repetitions ASC, s.next_review ASC";
            else if (scope == 2) // DueAndNew
                order_sql_part = "ORDER BY note_id ASC"; // s.* is no longer visible
            else
                order_sql_part = "ORDER BY n.created_at ASC"; // fallback
            break;
        case 6: order_sql_part = "ORDER BY n.created_at ASC";
            break; // Chronological
        default:
            throw std::invalid_argument("Invalid schedule value");
        }

        std::string map_join_sql;
        std::string map_where_sql;

        if (map_id != 0)
        {
            map_join_sql = "";
            map_where_sql = "AND n.map_id = " + std::to_string(map_id);
        }
        else
        {
            map_join_sql =
                "JOIN map_collection_item mci ON mci.map_id = n.map_id AND mci.map_collection_id = " +
                std::to_string(map_collection_id);

            map_where_sql = "";
        }
        // --- Replace placeholders ---
        std::unordered_map<std::string, std::string> vars = {
            {"map_id", std::to_string(map_id)},
            {"map_collection_id", std::to_string(map_collection_id)},
            {"algorithm", std::to_string(algorithm)},
            {"user_id", std::to_string(user_id)},
            {"filter_under_note", std::to_string(filter_under_note)},
            {"now_ms", std::to_string(now_ms)},
            {"limit", std::to_string(limit)},
            {"order_by", order_sql_part},
            {"map_join", map_join_sql},
            {"map_where", map_where_sql},

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
            debug << "Executing scope=" << scope << " schedule=" << schedule << " algorithm=" << algorithm << commit;
            SQLite::Statement query(db, sql);
            std::vector<int64_t> note_ids;

            while (query.executeStep())
            {
                note_ids.push_back(query.getColumn(0).getInt());
            }

            response["note_ids"] = note_ids;
            //response["sql"] = sql;
        }
        catch (SQLite::Exception& e)
        {
            response["error"] = e.what();
            response["sql_failed"] = sql;
        }

        return response;
    }
}
