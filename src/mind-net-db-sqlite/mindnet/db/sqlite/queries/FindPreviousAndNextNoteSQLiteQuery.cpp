//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/db/sqlite/queries/FindPreviousAndNextNoteSQLiteQuery.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.h"
#include "mindnet/essential/DatabaseType.h"
#include "mindnet/essential/Global.h"

namespace mindnet::db::sqlite::queries
{
    FindPreviousAndNextNoteSQLiteQuery::FindPreviousAndNextNoteSQLiteQuery()
        : Query(QUERY_FindPreviousAndNextNote, "FindPreviousAndNextNoteSQLiteQuery", essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json FindPreviousAndNextNoteSQLiteQuery::call(nlohmann::json request)
    {
        nlohmann::json response;

        if (!request.contains("note_id"))
        {
            throw std::invalid_argument("Mandatory key note_id is missing");
        }

        i64 note_id = request["note_id"];

        std::string sql = R"(
WITH current AS (
    SELECT id, parent_note_id, sibling_order
    FROM note
    WHERE id = :note_id
),

-- 1) next sibling
next_sibling AS (
    SELECT n.id
    FROM note n, current c
    WHERE n.parent_note_id IS c.parent_note_id
      AND n.sibling_order > c.sibling_order
    ORDER BY n.sibling_order ASC
    LIMIT 1
),

-- 2) first child
first_child AS (
    SELECT n.id
    FROM note n, current c
    WHERE n.parent_note_id = c.id
    ORDER BY n.sibling_order ASC
    LIMIT 1
),

-- 3) parent's sibling (if parent exists)
next_parent_sibling AS (
    SELECT p2.id
    FROM note p
    JOIN current c ON p.id = c.parent_note_id
    JOIN note p2 ON p2.parent_note_id = p.parent_note_id
                 AND p2.sibling_order > p.sibling_order
    ORDER BY p2.sibling_order ASC
    LIMIT 1
),

-- PREV
prev_sibling AS (
    SELECT n.id
    FROM note n, current c
    WHERE n.parent_note_id IS c.parent_note_id
      AND n.sibling_order < c.sibling_order
    ORDER BY n.sibling_order DESC
    LIMIT 1
),

prev_sibling_last_child AS (
    SELECT c2.id
    FROM prev_sibling ps
    JOIN note c2 ON c2.parent_note_id = ps.id
    ORDER BY c2.sibling_order DESC
    LIMIT 1
),

prev_parent AS (
    SELECT parent_note_id AS id
    FROM current
)

SELECT

    CASE
        WHEN (SELECT id FROM prev_sibling_last_child) IS NOT NULL
            THEN (SELECT id FROM prev_sibling_last_child)

        WHEN (SELECT id FROM prev_sibling) IS NOT NULL
            THEN (SELECT id FROM prev_sibling)

        WHEN (SELECT id FROM prev_parent) IS NOT NULL
            THEN (SELECT id FROM prev_parent)

        ELSE NULL
    END AS prev_note,

    CASE
        WHEN (SELECT id FROM next_sibling) IS NOT NULL
            THEN (SELECT id FROM next_sibling)

        WHEN (SELECT id FROM first_child) IS NOT NULL
            THEN (SELECT id FROM first_child)

        WHEN (SELECT id FROM next_parent_sibling) IS NOT NULL
            THEN (SELECT id FROM next_parent_sibling)

        ELSE NULL
    END AS next_note;

)";

        // --- Execute SQL query ---
        try
        {
            i64 prev_note_id{};
            i64 next_note_id{};
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode=WAL;");

            essential::debug << sql << essential::commit;
            essential::debug << "Executing FindPreviousAndNextNoteSQLiteQuery note_id=" << note_id << essential::commit;
            SQLite::Statement query(db, sql);

            query.bind(1, note_id);

            if (query.executeStep())
            {
                prev_note_id = query.isColumnNull(0) ? 0 : query.getColumn(0);
                next_note_id = query.isColumnNull(1) ? 0 : query.getColumn(1);
            }

            response["prev_note_id"] = prev_note_id;
            response["next_note_id"] = next_note_id;
        }
        catch (SQLite::Exception& e)
        {
            response["error"] = e.what();
            response["sql_failed"] = sql;
        }

        return response;
    }
}
