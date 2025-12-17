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

#include "../../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/slipbox/UpdateNotePathAndDepthSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>

#include "../../../../../../../include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Note.hpp"
#include "mindnet/db/sqlite/SqliteFileName.hpp"
#include "mindnet/essential/DatabaseType.hpp"

namespace mindnet::db::sqlite::queries::slipbox
{
    UpdateNotePathAndDepthSQLiteQuery::UpdateNotePathAndDepthSQLiteQuery()
        : api::Query(QUERY_UpdateNotePathAndDepth, "Updates paths and depth of all descendants",
                     essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json UpdateNotePathAndDepthSQLiteQuery::call(
        nlohmann::json& request, api::InvalidateMethod& invalidate_method
    )
    {
        if (!request.contains("note_id"))
        {
            throw std::runtime_error("Request is missing key \"note_id\"");
        }
        identification note_id = request["note_id"];

        nlohmann::json response;

        std::string sql = R"(
WITH RECURSIVE descendants(id, parent_note_id, new_path, new_depth) AS (
  -- Start from the direct descendants of note 123
  SELECT
    n.id,
    n.parent_note_id,
    p.path || '/' || printf('%06d', n.id) AS new_path,
    p.depth + 1 AS new_depth
  FROM note n
  JOIN note p ON n.parent_note_id = p.id
  WHERE p.id = ?

  UNION ALL

  -- Recursively add more descendants
  SELECT
    n.id,
    n.parent_note_id,
    d.new_path || '/' || printf('%06d', n.id),
    d.new_depth + 1
  FROM note n
  JOIN descendants d ON n.parent_note_id = d.id
)

-- Update all found descendants
UPDATE note
SET path = (
    SELECT new_path FROM descendants WHERE descendants.id = note.id
),
depth = (
    SELECT new_depth FROM descendants WHERE descendants.id = note.id
)
WHERE id IN (SELECT id FROM descendants);

SELECT id FROM descendants;

)";
        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            db.exec("PRAGMA journal_mode=WAL;");
            db.exec("PRAGMA synchronous=NORMAL;");
            db.exec("PRAGMA foreign_keys = ON;");

            SQLite::Statement query(db, sql);
            query.bind(1, note_id);

            // exec UPDATE + then SELECT descendants
            std::vector<identification> changed_ids;

            changed_ids.push_back(note_id);

            while (query.executeStep())
            {
                changed_ids.push_back(query.getColumn(0).getInt64());
            }

            // invalidate descendants
            for (auto id : changed_ids)
            {
                invalidate_method.invalidate(
                    plugins::slipbox::models::NOTE_DEFINITION,
                    id
                );
            }
        }
        catch (SQLite::Exception& e)
        {
            std::cerr << "Exception happened during SQL" << sql << e.what() << " " << std::endl;

            return false;
        }

        return response;
    };
}