//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/db/sqlite/queries/VacuumSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/util/Utils.hpp"

namespace mindnet::db::sqlite::queries
{
    using_loggers()

    VacuumSQLiteQuery::VacuumSQLiteQuery()
        : api::Query(QUERY_Vacuum, "Vacuum the database",
                essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json VacuumSQLiteQuery::call(nlohmann::json request, api::InvalidateMethod& invalidate_method)
    {
        nlohmann::json response;
        std::string vacuum_sql = "VACUUM";

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            db.exec("PRAGMA journal_mode=WAL;");
            db.exec("PRAGMA synchronous=NORMAL;");
            db.exec("PRAGMA foreign_keys = ON;");

            SQLite::Statement query(db, vacuum_sql);
            query.exec();
        }
        catch (SQLite::Exception& e)
        {
            std::cerr << "Exception happened during SQL " << vacuum_sql << e.what() << " " << std::endl;
            response["error"] = e.what();
            response["vacuum_sql"] = vacuum_sql;
        }

        return response;
    };
}
