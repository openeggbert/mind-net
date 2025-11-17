//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/db/sqlite/queries/CleanupSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/util/Utils.hpp"

namespace mindnet::db::sqlite::queries
{
    using_loggers()

    CleanupSQLiteQuery::CleanupSQLiteQuery()
        : api::Query(QUERY_Cleanup, "Cleanup the database",
                essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json CleanupSQLiteQuery::call(nlohmann::json request, api::InvalidateMethod& invalidate_method)
    {
        if (!request.contains("api_log_threshold_in_days"))
            throw std::runtime_error("api_log_threshold_in_days not found");
        if (!request.contains("history_read_threshold_in_days"))
            throw std::runtime_error("history_read_threshold_in_days not found");
        if (!request.contains("history_list_threshold_in_days"))
            throw std::runtime_error("api_log_threshold_in_days not found");
        if (!request.contains("access_token_threshold_in_days"))
            throw std::runtime_error("access_token_threshold_in_days not found");

        int api_log_threshold_in_days = request["api_log_threshold_in_days"];
        int history_read_threshold_in_days = request["history_read_threshold_in_days"];
        int history_list_threshold_in_days = request["history_list_threshold_in_days"];
        int access_token_threshold_in_days = request["access_token_threshold_in_days"];

        nlohmann::json response;

        auto now = util::Utils::current_unix_timestamp_ms();

        std::string cleanup_table_api_log_sql = "delete from api_log where created_at <= ?";
        std::string cleanup_table_history_read_sql = "delete from history where operation = 2 and created_at <= ?";
        std::string cleanup_table_history_list_sql = "delete from history where operation = 5 and created_at <= ?";
        std::string cleanup_table_access_token_sql = R"SQL(
DELETE FROM access_token
WHERE created_at <= ?
  AND expires_at < CAST(strftime('%f','now') * 1000 AS INTEGER)
  AND id NOT IN (SELECT access_token_id FROM login_session)
)SQL";

        info << cleanup_table_api_log_sql << commit;
        info << cleanup_table_history_read_sql << commit;
        info << cleanup_table_history_list_sql << commit;
        info << cleanup_table_access_token_sql << commit;

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            db.exec("PRAGMA journal_mode=WAL;");
            db.exec("PRAGMA synchronous=NORMAL;");
            db.exec("PRAGMA foreign_keys = ON;");

            SQLite::Statement query(db, cleanup_table_api_log_sql);
            query.bind(1, cast64(now - api_log_threshold_in_days * MILLISECONDS_PER_DAY));

            int affected = query.exec();
            response["api_log_deleted_count"] = cast64(affected);
            essential::info << "api_log_deleted_count: " << affected << essential::commit;
        }
        catch (SQLite::Exception& e)
        {
            std::cerr << "Exception happened during SQL " << cleanup_table_api_log_sql << e.what() << " " << std::endl;
            response["error"] = e.what();
            response["cleanup_table_api_log_sql"] = cleanup_table_api_log_sql;
        }

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            db.exec("PRAGMA journal_mode=WAL;");
            db.exec("PRAGMA synchronous=NORMAL;");
            db.exec("PRAGMA foreign_keys = ON;");

            SQLite::Statement query(db, cleanup_table_history_read_sql);
            query.bind(1, cast64(now - history_read_threshold_in_days * MILLISECONDS_PER_DAY));

            int affected = query.exec();
            response["history_read_deleted_count"] = cast64(affected);
            essential::info << "history_read_deleted_count: " << affected << essential::commit;
        }
        catch (SQLite::Exception& e)
        {
            std::cerr << "Exception happened during SQL " << cleanup_table_history_read_sql << e.what() << " " << std::endl;
            response["error"] = e.what();
            response["cleanup_table_history_read_sql"] = cleanup_table_history_read_sql;
        }

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            db.exec("PRAGMA journal_mode=WAL;");
            db.exec("PRAGMA synchronous=NORMAL;");
            db.exec("PRAGMA foreign_keys = ON;");

            SQLite::Statement query(db, cleanup_table_history_list_sql);
            query.bind(1, cast64(now - history_list_threshold_in_days * MILLISECONDS_PER_DAY));

            int affected = query.exec();
            response["history_list_deleted_count"] = cast64(affected);
            essential::info << "history_list_deleted_count: " << affected << essential::commit;
        }
        catch (SQLite::Exception& e)
        {
            std::cerr << "Exception happened during SQL " << cleanup_table_history_list_sql << e.what() << " " << std::endl;
            response["error"] = e.what();
            response["cleanup_table_history_list_sql"] = cleanup_table_history_list_sql;
        }

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            db.exec("PRAGMA journal_mode=WAL;");
            db.exec("PRAGMA synchronous=NORMAL;");
            db.exec("PRAGMA foreign_keys = ON;");

            SQLite::Statement query(db, cleanup_table_access_token_sql);
            query.bind(1, cast64(now - access_token_threshold_in_days * MILLISECONDS_PER_DAY));

            int affected = query.exec();
            response["access_token_deleted_count"] = cast64(affected);
            essential::info << "access_token_deleted_count: " << affected << essential::commit;
        }
        catch (SQLite::Exception& e)
        {
            std::cerr << "Exception happened during SQL " << cleanup_table_access_token_sql << e.what() << " " << std::endl;
            response["error"] = e.what();
            response["cleanup_table_access_token_sql"] = cleanup_table_access_token_sql;
        }

        return response;
    };
}
