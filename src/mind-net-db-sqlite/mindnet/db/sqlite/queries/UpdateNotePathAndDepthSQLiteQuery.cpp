//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/db/sqlite/queries/UpdateNotePathAndDepthSQLiteQuery.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.h"
#include "mindnet/essential/DatabaseType.h"

namespace mindnet::db::sqlite::queries
{
    UpdateNotePathAndDepthSQLiteQuery::UpdateNotePathAndDepthSQLiteQuery()
    : Query(QUERY_UpdateNotePathAndDepth, "Updates paths and depth of all descendants", essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json UpdateNotePathAndDepthSQLiteQuery::call(nlohmann::json request)
    {
        if (!request.contains("note_id"))
        {
            throw std::runtime_error("Request is missing key \"note_id\"");
        }
        int note_id = request["note_id"];

        nlohmann::json response;

        std::string sql = R"(
WITH RECURSIVE descendants(id, parent_note_id, new_path, new_depth) AS (
  -- Start from the direct descendants of note 123
  SELECT
    n.id,
    n.parent_note_id,
    p.path || '.' || printf('%06d', n.id) AS new_path,
    p.depth + 1 AS new_depth
  FROM note n
  JOIN note p ON n.parent_note_id = p.id
  WHERE p.id = ?

  UNION ALL

  -- Recursively add more descendants
  SELECT
    n.id,
    n.parent_note_id,
    d.new_path || '.' || printf('%06d', n.id),
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

)";
        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            db.exec("PRAGMA journal_mode=WAL;");
            db.exec("PRAGMA synchronous=NORMAL;");
            db.exec("PRAGMA foreign_keys = ON;");

            SQLite::Statement query(db, sql);
            query.bind(1, note_id);

            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            std::cerr << "Exception happened during SQL" << sql << e.what() << " " << std::endl;

            return false;
        }

        return response;
    };
}
