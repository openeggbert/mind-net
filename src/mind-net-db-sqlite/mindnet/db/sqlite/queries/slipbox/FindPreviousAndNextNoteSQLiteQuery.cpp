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

#include "../../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/slipbox/FindPreviousAndNextNoteSQLiteQuery.hpp"
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

namespace mindnet::db::sqlite::queries::slipbox
{
    FindPreviousAndNextNoteSQLiteQuery::FindPreviousAndNextNoteSQLiteQuery()
        : Query(QUERY_FindPreviousAndNextNote, "FindPreviousAndNextNoteSQLiteQuery", essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json FindPreviousAndNextNoteSQLiteQuery::call(nlohmann::json& request,
    api::InvalidateMethod& invalidate_method,
 plugins::core::models::OptionalError& optional_error)
    {
        nlohmann::json response;

        if (!request.contains("note_id"))
        {
            throw std::invalid_argument("Mandatory key note_id is missing");
        }

        identification note_id = request["note_id"];

        identification map_id{0};

        static std::string get_note_map_sql = "SELECT map_id FROM note WHERE id = ?";
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
      AND map_id = :map_id

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
    WHERE n.map_id = :map_id
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

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode=WAL;");

            essential::debug << get_note_map_sql << essential::commit;
            essential::debug << "Looking up map_id for note_id=" << note_id << essential::commit;

            SQLite::Statement query(db, get_note_map_sql);
            query.bind(1, note_id);

            if (!query.executeStep())
            {
                response["error"] = "Note with this ID does not exist";
                response["exists"] = false;
                return response;
            }

            map_id = query.getColumn(0).getInt64();
        }
        catch (SQLite::Exception& e)
        {
            response["error"] = e.what();
            response["sql_failed"] = get_note_map_sql;
            return response;
        }

        // --- Execute SQL query ---
        try
        {
            identification prev_note_id{};
            identification next_note_id{};
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode=WAL;");

            essential::debug << sql << essential::commit;
            essential::debug << "Executing FindPreviousAndNextNoteSQLiteQuery note_id=" << note_id << essential::commit;
            SQLite::Statement query(db, sql);

            query.bind(1, map_id);
            query.bind(2, note_id);

            if (query.executeStep())
            {
                essential::debug << "is column 0 null? " << query.isColumnNull(0) << essential::commit;
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