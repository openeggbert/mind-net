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

-- First child of current (DFS: go down first)
first_child AS (
    SELECT n.id
    FROM note n, current c
    WHERE n.parent_note_id = c.id
    ORDER BY n.sibling_order ASC
    LIMIT 1
),

-- Next sibling of current (same parent, higher sibling_order)
next_sibling AS (
    SELECT n.id
    FROM note n, current c
    WHERE n.parent_note_id IS c.parent_note_id
      AND n.sibling_order > c.sibling_order
    ORDER BY n.sibling_order ASC
    LIMIT 1
),

-- Ancestor that has a next sibling (walk up until a parent has another child after it)
ancestor_with_next AS (
    WITH RECURSIVE a(id, parent_id, sibling_order) AS (
        SELECT id, parent_note_id, sibling_order
        FROM note
        WHERE id = (SELECT parent_note_id FROM current)

        UNION ALL

        SELECT n.id, n.parent_note_id, n.sibling_order
        FROM note n
        JOIN a ON n.id = a.parent_id
    )
    SELECT p2.id
    FROM a
    JOIN note p2 ON p2.parent_note_id IS a.parent_id
                AND p2.sibling_order > a.sibling_order
    ORDER BY p2.sibling_order ASC
    LIMIT 1
),

-- Previous sibling of current
prev_sibling AS (
    SELECT n.id
    FROM note n, current c
    WHERE n.parent_note_id IS c.parent_note_id
      AND n.sibling_order < c.sibling_order
    ORDER BY n.sibling_order DESC
    LIMIT 1
),

-- Deepest descendant of previous sibling:
-- we build a lexicographic ordering key based on sibling_order
prev_sibling_deepest AS (
    WITH RECURSIVE d(id, ord_key, depth) AS (
        SELECT ps.id,
               '/' || printf('%06d', n0.sibling_order),
               0
        FROM prev_sibling ps
        JOIN note n0 ON n0.id = ps.id

        UNION ALL

        SELECT c.id,
               d.ord_key || '/' || printf('%06d', c.sibling_order),
               d.depth + 1
        FROM note c
        JOIN d ON c.parent_note_id = d.id
    )
    SELECT id
    FROM d
    ORDER BY ord_key DESC, depth DESC
    LIMIT 1
),

parent AS (
    SELECT parent_note_id AS id FROM current
)

SELECT
    -- PREVIOUS rules (reverse DFS):
    -- 1) deepest descendant of previous sibling
    -- 2) previous sibling itself
    -- 3) go up to parent
    CASE
        WHEN (SELECT id FROM prev_sibling_deepest) IS NOT NULL
            THEN (SELECT id FROM prev_sibling_deepest)
        WHEN (SELECT id FROM prev_sibling) IS NOT NULL
            THEN (SELECT id FROM prev_sibling)
        WHEN (SELECT id FROM parent) IS NOT NULL
            THEN (SELECT id FROM parent)
        ELSE NULL
    END AS prev_note,

    -- NEXT rules ( DFS pre-order ):
    -- 1) first child
    -- 2) next sibling
    -- 3) next sibling of any ancestor
    CASE
        WHEN (SELECT id FROM first_child) IS NOT NULL
            THEN (SELECT id FROM first_child)
        WHEN (SELECT id FROM next_sibling) IS NOT NULL
            THEN (SELECT id FROM next_sibling)
        WHEN (SELECT id FROM ancestor_with_next) IS NOT NULL
            THEN (SELECT id FROM ancestor_with_next)
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
