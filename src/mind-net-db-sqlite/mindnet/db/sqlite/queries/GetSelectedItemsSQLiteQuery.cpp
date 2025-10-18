//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/db/sqlite/queries/GetSelectedItemsSQLiteQuery.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.h"
#include "mindnet/essential/DatabaseType.h"

namespace mindnet::db::sqlite::queries
{
    GetSelectedItemsSQLiteQuery::GetSelectedItemsSQLiteQuery()
    : Query("GetSelectedItemsQuery", "Returns new note ids for repetition session", essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json GetSelectedItemsSQLiteQuery::call(nlohmann::json request)
    {
        nlohmann::json response;

        std::string sql = "select max(id) from note;";
        int max_note_id{};
        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            db.exec("PRAGMA journal_mode=WAL;");
            db.exec("PRAGMA synchronous=NORMAL;");
            db.exec("PRAGMA foreign_keys = ON;");

            SQLite::Statement query(db, sql);

            if (query.executeStep())
            {
                if (query.getColumn(0).isNull())
                {
                    max_note_id = 0;
                }
                max_note_id = query.getColumn(0).getInt();
            }
            db.exec(sql);
        }
        catch (SQLite::Exception& e)
        {
            std::cerr << "Exception happened during SQL" << sql << e.what() << " " << std::endl;

            return false;
        }

        response["hello"] = "world";
        response["max_note_id"] = std::to_string(max_note_id);
        return response;
    };
}
