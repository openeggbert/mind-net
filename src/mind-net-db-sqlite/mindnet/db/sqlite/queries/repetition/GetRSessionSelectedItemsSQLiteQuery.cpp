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

#include "../../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/repetition/GetRSessionSelectedItemsSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.hpp"
#include "mindnet/essential/DatabaseType.hpp"

namespace mindnet::db::sqlite::queries::repetition
{
    const std::string SQL_DUE_ONLY = R"(
SELECT n.id AS note_id
FROM note n {parent_join}
JOIN r{algorithm}_state s ON s.note_id = n.id
{map_join}
WHERE s.user_id = {user_id}
  AND s.next_review <= {now_ms}

  AND {eligible_filter} n.content_id IS NOT NULL
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
    const std::string SQL_DUE_ONLY_DFS = R"(
WITH RECURSIVE ord(note_id, ord_key) AS (
    SELECT n0.id,
           printf('/%06d', n0.sibling_order)
    FROM note n0
    WHERE n0.parent_note_id IS NULL AND n0.map_id = {map_id}

    UNION ALL

    SELECT c.id,
           ord.ord_key || '/' || printf('%06d', c.sibling_order)
    FROM note c
    JOIN ord ON ord.note_id = c.parent_note_id WHERE c.map_id = {map_id}
),
due AS (
    SELECT n.id AS note_id
    FROM note n
    JOIN r{algorithm}_state s ON s.note_id = n.id
    WHERE s.user_id = {user_id}
      AND s.next_review <= {now_ms}
)
SELECT n.id AS note_id
FROM note n
JOIN due d ON d.note_id = n.id
LEFT JOIN ord o ON o.note_id = n.id
{map_join}
WHERE
      {eligible_filter} n.content_id IS NOT NULL
  AND EXISTS (SELECT 1 FROM content c WHERE c.id = n.content_id AND TRIM(c.value) <> '')
  AND (
        {filter_under_note} = 0
        OR n.path LIKE (SELECT path || '%' FROM note WHERE id = {filter_under_note})
  )
  {map_where}
ORDER BY o.ord_key ASC, n.id ASC
LIMIT {limit};
)";

    const std::string SQL_NEW_ONLY = R"(
SELECT n.id AS note_id
FROM note n {parent_join}
LEFT JOIN r{algorithm}_state s
  ON s.note_id = n.id AND s.user_id = {user_id}
{map_join}
WHERE s.note_id IS NULL

  AND {eligible_filter} n.content_id IS NOT NULL
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

    const std::string SQL_NEW_ONLY_DFS = R"(
WITH RECURSIVE ord(note_id, ord_key) AS (
    SELECT n0.id,
           printf('/%06d', n0.sibling_order)
    FROM note n0
    WHERE n0.parent_note_id IS NULL AND n0.map_id = {map_id}

    UNION ALL

    SELECT c.id,
           ord.ord_key || '/' || printf('%06d', c.sibling_order)
    FROM note c
    JOIN ord ON ord.note_id = c.parent_note_id WHERE c.map_id = {map_id}
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
JOIN new ne ON ne.note_id = n.id
LEFT JOIN ord o ON o.note_id = n.id
{map_join}
WHERE
      {eligible_filter} n.content_id IS NOT NULL
  AND EXISTS (SELECT 1 FROM content c WHERE c.id = n.content_id AND TRIM(c.value) <> '')
  AND (
        {filter_under_note} = 0
        OR n.path LIKE (SELECT path || '%' FROM note WHERE id = {filter_under_note})
  )
  {map_where}
ORDER BY o.ord_key ASC, n.id ASC
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
FROM note n {parent_join}
JOIN (
    SELECT note_id FROM due
    UNION
    SELECT note_id FROM new
) x ON x.note_id = n.id
{map_join}

WHERE
      {eligible_filter} n.content_id IS NOT NULL
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

    const std::string SQL_DUE_AND_NEW_DFS = R"(
WITH RECURSIVE ord(note_id, ord_key) AS (
    SELECT n0.id,
           printf('/%06d', n0.sibling_order)
    FROM note n0
    WHERE n0.parent_note_id IS NULL AND n0.map_id = {map_id}

    UNION ALL

    SELECT c.id,
           ord.ord_key || '/' || printf('%06d', c.sibling_order)
    FROM note c
    JOIN ord ON ord.note_id = c.parent_note_id WHERE c.map_id = {map_id}
),
due AS (
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
),
all_set AS (
    SELECT note_id FROM due
    UNION
    SELECT note_id FROM new
)
SELECT n.id AS note_id
FROM note n
JOIN all_set x ON x.note_id = n.id
LEFT JOIN ord o ON o.note_id = n.id
{map_join}
WHERE
      {eligible_filter} n.content_id IS NOT NULL
  AND EXISTS (SELECT 1 FROM content c WHERE c.id = n.content_id AND TRIM(c.value) <> '')
  AND (
        {filter_under_note} = 0
        OR n.path LIKE (SELECT path || '%' FROM note WHERE id = {filter_under_note})
  )
  {map_where}
ORDER BY o.ord_key ASC, n.id ASC
LIMIT {limit};
)";

    const std::string SQL_ALL = R"(
SELECT n.id AS note_id
FROM note n {parent_join}
{map_join}
WHERE
      {eligible_filter} n.content_id IS NOT NULL
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

    const std::string SQL_ALL_DFS = R"(
WITH RECURSIVE ord(note_id, ord_key) AS (
    SELECT n0.id,
           printf('/%06d', n0.sibling_order)
    FROM note n0
    WHERE n0.parent_note_id IS NULL AND n0.map_id = {map_id}

    UNION ALL

    SELECT c.id,
           ord.ord_key || '/' || printf('%06d', c.sibling_order)
    FROM note c
    JOIN ord ON ord.note_id = c.parent_note_id WHERE c.map_id = {map_id}
)
SELECT n.id AS note_id
FROM note n
LEFT JOIN note p ON p.id = n.parent_note_id
LEFT JOIN ord o ON o.note_id = n.id
{map_join}
WHERE
      {eligible_filter} n.content_id IS NOT NULL
  AND EXISTS (SELECT 1 FROM content c WHERE c.id = n.content_id AND TRIM(c.value) <> '')
  AND (
        {filter_under_note} = 0
        OR n.path LIKE (SELECT path || '%' FROM note WHERE id = {filter_under_note})
  )
  {map_where}
ORDER BY o.ord_key ASC, n.id ASC
LIMIT {limit};
)";

    GetRSessionSelectedItemsSQLiteQuery::GetRSessionSelectedItemsSQLiteQuery()
        : Query(QUERY_GetRSessionSelectedItemsQuery, "Returns new note ids for repetition session",
                essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json GetRSessionSelectedItemsSQLiteQuery::call(nlohmann::json& request,
                                                             api::InvalidateMethod& invalidate_method)
    {
        nlohmann::json response;

        if (!request.contains("r_session"))
        {
            throw std::invalid_argument("Mandatory key r_session is missing");
        }
        nlohmann::json session = request["r_session"];

        identification map_id = session["map_id"];
        identification map_collection_id = session["map_collection_id"];
        int algorithm = session["algorithm"];
        identification filter_under_note = session["filter_under_note"];
        int schedule = session["schedule"];
        int scope = session["scope"];
        identification user_id = session["user_id"];
        int limit = session.value("limit", 250);
        long long now_ms = std::time(nullptr) * 1000LL;




        std::string eligible_filter;

        static std::string exclude_norepetition =
"NOT EXISTS (\n"
"    SELECT 1 FROM flag f2\n"
"    WHERE f2.note_id = n.id\n"
"      AND f2.title = 'norepetition'\n"
")\nAND ";

if (session["filter_eligible"] == 0) {
    eligible_filter = "1=1 AND";
}
else {
    switch (scope) {

    case 0: // DUE only
        eligible_filter =
            "EXISTS (\n"
            "    SELECT 1 FROM r" + std::to_string(algorithm) + "_state s2\n"
            "    WHERE s2.note_id = n.id\n"
            "      AND s2.user_id = " + std::to_string(user_id) + "\n"
            "      AND s2.eligible = 1\n"
            ")\nAND";
        break;

    case 1: // NEW only
        eligible_filter =
            "EXISTS (\n"
            "    SELECT 1 FROM flag f\n"
            "    WHERE f.note_id = n.id\n"
            "      AND f.title = 'repetition'\n"
            ")\nAND";
        break;

    case 2: // DUE + NEW
        eligible_filter =
            "(\n"
            "    EXISTS (\n"
            "        SELECT 1 FROM r" + std::to_string(algorithm) + "_state s2\n"
            "        WHERE s2.note_id = n.id\n"
            "          AND s2.user_id = " + std::to_string(user_id) + "\n"
            "          AND s2.eligible = 1\n"
            "    )\n"
            "    OR\n"
            "    EXISTS (\n"
            "        SELECT 1 FROM flag f\n"
            "        WHERE f.note_id = n.id\n"
            "          AND f.title = 'repetition'\n"
            "    )\n"
            ")\nAND";
        break;

    case 3: // ALL
        eligible_filter =
            "(\n"
            "    -- pokud je note ve state tabulce → musí být eligible=1\n"
            "    EXISTS (\n"
            "        SELECT 1 FROM r" + std::to_string(algorithm) + "_state s2\n"
            "        WHERE s2.note_id = n.id\n"
            "          AND s2.user_id = " + std::to_string(user_id) + "\n"
            "          AND s2.eligible = 1\n"
            "    )\n"
            "    OR\n"
            "    -- pokud není ve state tabulce → musí mít repetition flag\n"
            "    (\n"
            "        NOT EXISTS (\n"
            "            SELECT 1 FROM r" + std::to_string(algorithm) + "_state s3\n"
            "            WHERE s3.note_id = n.id\n"
            "              AND s3.user_id = " + std::to_string(user_id) + "\n"
            "        )\n"
            "        AND EXISTS (\n"
            "            SELECT 1 FROM flag f\n"
            "            WHERE f.note_id = n.id\n"
            "              AND f.title = 'repetition'\n"
            "        )\n"
            "    )\n"
            ")\nAND";
        break;

    default:
        throw std::invalid_argument("Invalid scope value for eligibility.");
    }
}
        eligible_filter = exclude_norepetition + eligible_filter;

        // --- Select SQL template based on scope ---
        const std::string* sql_template = nullptr;
        switch (scope)
        {
        case 0:
            sql_template = (schedule == 0 ? &SQL_DUE_ONLY_DFS : &SQL_DUE_ONLY);
            break;
        case 1:
            sql_template = (schedule == 0 ? &SQL_NEW_ONLY_DFS : &SQL_NEW_ONLY);
            break;
        case 2:
            sql_template = (schedule == 0 ? &SQL_DUE_AND_NEW_DFS : &SQL_DUE_AND_NEW);
            break;
        case 3:
            sql_template = (schedule == 0 ? &SQL_ALL_DFS : &SQL_ALL);
            break;
        default:
            throw std::invalid_argument("Invalid scope value");
        }

        // --- ORDER BY based on schedule ---
        std::string order_sql_part;

        switch (schedule)
        {
        case 0: // DepthFirst → DFS uses its own ORDER BY, do NOT set order_sql_part
            order_sql_part = "";
            break;

        case 1: // BreadthFirst
            order_sql_part = "ORDER BY n.depth ASC, n.sibling_order ASC, n.id ASC";
            break;

        case 2: // Random
            order_sql_part = "ORDER BY random()";
            break;

        case 3: // DepthFirstShuffled
            order_sql_part =
                "ORDER BY "
                "CASE WHEN n.parent_note_id IS NULL THEN 0 ELSE 1 END, "
                "COALESCE(p.sibling_order, 0), "
                "n.sibling_order ASC, "
                "random()";
            break;

        case 4: // Interleaved
            order_sql_part = "ORDER BY (n.depth % 3), n.sibling_order ASC, random()";
            break;

        case 5: // DifficultySorted
            if (scope == 0)
                order_sql_part = "ORDER BY s.repetitions ASC, s.next_review ASC, n.sibling_order ASC";
            else if (scope == 2)
                order_sql_part = "ORDER BY note_id ASC";
            else
                order_sql_part = "ORDER BY n.created_at ASC";
            break;

        case 6: // Chronological
            order_sql_part = "ORDER BY n.created_at ASC";
            break;

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

        std::string parent_join_sql = "LEFT JOIN note p ON p.id = n.parent_note_id";

        // --- Replace placeholders ---
        std::unordered_map<std::string, std::string> vars = {
            {"map_id", std::to_string(map_id)},
            {"map_collection_id", std::to_string(map_collection_id)},
            {"algorithm", std::to_string(algorithm)},
            {"user_id", std::to_string(user_id)},
            {"filter_under_note", std::to_string(filter_under_note)},
            {"now_ms", std::to_string(now_ms)},
            {"limit", std::to_string(limit)},
            {"order_by", (schedule == 0 ? "" : order_sql_part)},
            {"map_join", map_join_sql},
            {"map_where", map_where_sql},
            {"parent_join", parent_join_sql},
            {"eligible_filter", eligible_filter},

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

            essential::debug << sql << essential::commit;
            essential::debug << "Executing scope=" << scope << " schedule=" << schedule << " algorithm=" << algorithm <<
                essential::commit;
            SQLite::Statement query(db, sql);
            std::vector<identification> note_ids;

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