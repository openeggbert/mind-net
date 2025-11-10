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
    SELECT id, path
    FROM note
    WHERE id = :note_id
),

ordered AS (
    SELECT id, path
    FROM note
    ORDER BY path ASC
),

prev_note AS (
    SELECT o.id
    FROM ordered o, current c
    WHERE o.path < c.path
    ORDER BY o.path DESC
    LIMIT 1
),

next_note AS (
    SELECT o.id
    FROM ordered o, current c
    WHERE o.path > c.path
    ORDER BY o.path ASC
    LIMIT 1
)

SELECT
    (SELECT id FROM prev_note) AS prev_note,
    (SELECT id FROM next_note) AS next_note;


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
