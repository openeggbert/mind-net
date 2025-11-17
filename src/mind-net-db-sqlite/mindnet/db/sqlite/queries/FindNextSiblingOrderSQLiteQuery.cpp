//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/db/sqlite/queries/FindNextSiblingOrderSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>

#include "mindnet/db/sqlite/SqliteFileName.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/essential/Global.hpp"

namespace mindnet::db::sqlite::queries
{
    FindNextSiblingOrderSQLiteQuery::FindNextSiblingOrderSQLiteQuery()
        : Query(QUERY_FindNextSiblingOrder, "FindNextSiblingOrderSQLiteQuery", essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json FindNextSiblingOrderSQLiteQuery::call(nlohmann::json request, api::InvalidateMethod& invalidate_method)
    {
        nlohmann::json response;

        if (!request.contains("note_id"))
        {
            throw std::invalid_argument("Mandatory key note_id is missing");
        }
        if (!request.contains("parent_note_id"))
        {
            throw std::invalid_argument("Mandatory key parent_note_id is missing");
        }
        if (!request.contains("map_id"))
        {
            throw std::invalid_argument("Mandatory key map_id is missing");
        }

        i64 parent_note_id = request["parent_note_id"];
        i64 note_id = request["note_id"];
        i64 map_id = request["map_id"];

        std::string sql = parent_note_id == 0
                              ? "select max(sibling_order) from note where map_id = ? and parent_note_id is null"
                              : "select max(sibling_order) from note where map_id = ? and parent_note_id = ?";

        // --- Execute SQL query ---
        try
        {
            i64 max_sibling_order{};
            i64 next_sibling_order{};
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READONLY);
            db.exec("PRAGMA foreign_keys = ON;");
            db.exec("PRAGMA journal_mode=WAL;");

            essential::debug << sql << essential::commit;
            essential::debug << "Executing FindNextSiblingOrderSQLiteQuery map_id=" << map_id << " parent_note_id=" <<
                parent_note_id << essential::commit;
            SQLite::Statement query(db, sql);


            query.bind(1, map_id);
            if (parent_note_id != 0)
                query.bind(2, parent_note_id);


            if (query.executeStep())
            {
                max_sibling_order = query.isColumnNull(0) ? 0 : query.getColumn(0);
            }

            // Round max_sibling_order up to next hundred and add +100 for next sibling slot
            next_sibling_order = ((max_sibling_order + 99) / 100 + 1) * 100;

            response["next_sibling_order"] = next_sibling_order;
        }
        catch (SQLite::Exception& e)
        {
            response["error"] = e.what();
            response["sql_failed"] = sql;
        }

        return response;
    }
}
