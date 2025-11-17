//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/db/sqlite/queries/FindPreviousAndNextNoteSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/essential/Global.hpp"

// TODO (future enhancement):
// Implement fast PREORDER-based navigation for notes (previous/next)
// directly in C++ instead of using SQL recursive traversal.
//
// Target behavior:
//   - Preorder traversal defined by sibling_order within each parent.
//   - NEXT rules:
//       1) If the note has children → return its first child.
//       2) Else if it has a next sibling → return the next sibling.
//       3) Else climb up the parent chain until a next sibling exists.
//       4) If none exists at any ancestor → return null.
//   - PREVIOUS rules:
//       1) If the note has a previous sibling → return the deepest-right
//          descendant of that sibling.
//       2) Else return the parent.
//       3) If the parent is null (root with lowest sibling_order) → return null.
//
// Implementation notes:
//   - Execute lightweight, index-backed SQL queries from C++:
//       * SELECT children by parent_id ORDER BY sibling_order
//       * SELECT siblings by parent_id ORDER BY sibling_order
//       * SELECT parent_id by id
//   - Expect ~0.1–0.5 ms per navigation step even with very large note trees (1M+).
//   - This approach avoids expensive SQL recursive CTE and scales extremely well.
//   - Current database size (~340 notes) is small, so this optimization is not urgent.
//
// When ready, integrate navigation into a dedicated C++ helper class
// (e.g., NoteNavigator) for clean and unit-testable traversal logic.

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

        static std::string does_note_exist_sql = "SELECT 1 FROM note WHERE id = ?";
        static std::string sql = R"(
WITH RECURSIVE preorder AS (
    -- 1) ROOT NODES ordered by sibling_order
    SELECT
        id,
        parent_note_id,
        sibling_order,
        1 AS depth,
        printf('%06d', sibling_order) AS sortkey
    FROM note
    WHERE parent_note_id IS NULL

    UNION ALL

    -- 2) CHILDREN of each node, ordered by sibling_order
    SELECT
        n.id,
        n.parent_note_id,
        n.sibling_order,
        p.depth + 1 AS depth,
        p.sortkey || '-' || printf('%06d', n.sibling_order) AS sortkey
    FROM note n
    JOIN preorder p ON n.parent_note_id = p.id
)

,
ordered AS (
    SELECT
        id,
        ROW_NUMBER() OVER (ORDER BY sortkey) AS preorder_index
    FROM preorder
),

current AS (
    SELECT preorder_index
    FROM ordered
    WHERE id = :note_id
)

SELECT
    (SELECT id FROM ordered o WHERE o.preorder_index = (SELECT preorder_index FROM current) - 1) AS prev_note,
    (SELECT id FROM ordered o WHERE o.preorder_index = (SELECT preorder_index FROM current) + 1) AS next_note;


)";

        bool exists = false;

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode=WAL;");

            essential::debug << does_note_exist_sql << essential::commit;
            essential::debug << "Executing does_note_exist_sql note_id=" << note_id << essential::commit;
            SQLite::Statement query(db, does_note_exist_sql);

            query.bind(1, note_id);

            bool exists = false;
            if (query.executeStep())
            {
                exists = true;
            }

            if (!exists)
            {
                response["error"] = "Note with this ID does not exist";
                response["exists"] = false;
                return response;
            }

        }
        catch (SQLite::Exception& e)
        {
            response["error"] = e.what();
            response["sql_failed"] = does_note_exist_sql;
            return response;
        }

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
