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

#include "../../../../../../../include/hive-db-sqlite/hive/db/sqlite/queries/core/CleanupHistoryOrphansSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include "hive/db/sqlite/SqliteFileName.hpp"
#include "hive/essential/DatabaseType.hpp"
#include "hive/util/Utils.hpp"

namespace hive::db::sqlite::queries::core
{
    using_loggers()

    CleanupHistoryOrphansSQLiteQuery::CleanupHistoryOrphansSQLiteQuery()
        : api::Query(QUERY_CleanupHistoryOrphans, "Cleanup the history table - orphans",
                     essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json CleanupHistoryOrphansSQLiteQuery::call(nlohmann::json& request,
                                                          api::InvalidateMethod& invalidate_method,
                                                          plugins::core::models::OptionalError& optional_error)
    {
        if (!request.contains("history_orphan_threshold_in_days"))
            throw std::runtime_error("history_orphan_threshold_in_days not found");

        int history_orphan_threshold_in_days = request["history_orphan_threshold_in_days"];

        bool delete_deleted_rows = false;
        {
            if (request.contains("delete_deleted_rows"))
                delete_deleted_rows = request["delete_deleted_rows"];
        }
        nlohmann::json response;

        auto now = util::Utils::current_unix_timestamp_ms();

        std::string does_table_exist_sql = "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name=?;";

        std::string history_rows_without_delete_operation_sql = R"SQL(
SELECT distinct table_name, record_id
FROM history
WHERE
created_at <= ? and
(table_name, record_id) IN (
    SELECT table_name, record_id
    FROM history
    GROUP BY table_name, record_id
    HAVING SUM(CASE WHEN operation = 4 THEN 1 ELSE 0 END) = 0
)

)SQL";
        std::string history_rows_with_delete_operation_sql = R"SQL(
SELECT distinct table_name, record_id
FROM history
WHERE
created_at <= ? and
(table_name, record_id) IN (
    SELECT table_name, record_id
    FROM history
    GROUP BY table_name, record_id
    HAVING SUM(CASE WHEN operation = 4 THEN 1 ELSE 0 END) = 1
)

)SQL";

        std::map<string, bool> table_exist_cache;

        info << history_rows_without_delete_operation_sql << commit;

        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            db.exec("PRAGMA journal_mode=WAL;");
            db.exec("PRAGMA synchronous=NORMAL;");
            db.exec("PRAGMA foreign_keys = ON;");

            SQLite::Statement query(db, history_rows_without_delete_operation_sql);
            query.bind(1, cast64(now - history_orphan_threshold_in_days * MILLISECONDS_PER_DAY));


            std::vector<std::pair<std::string, identification>> table_id_pairs_to_be_deleted;
            while (query.executeStep())
            {
                string table_name = query.getColumn(0).getString();
                identification record_id = query.getColumn(1).getInt64();

                if (record_id == 0) continue;

                if (!table_exist_cache.contains(table_name))
                {
                    SQLite::Statement query_does_table_exist(db, does_table_exist_sql);
                    query_does_table_exist.bind(1, table_name);
                    if (query_does_table_exist.executeStep())
                    {
                        int count = query_does_table_exist.getColumn(0).getInt64();
                        table_exist_cache[table_name] = count != 0;
                    }
                }
                if (!table_exist_cache.at(table_name))
                {
                    table_id_pairs_to_be_deleted.push_back({table_name, record_id});
                    continue;
                }

                SQLite::Statement query_concrete_table(
                    db, "select count(*) from " + table_name + " where id = " + std::to_string(record_id));
                if (query_concrete_table.executeStep())
                {
                    int count = query_concrete_table.getColumn(0).getInt64();
                    if (count == 0)
                    {
                        table_id_pairs_to_be_deleted.push_back({table_name, record_id});
                        std::cout << "will be deleted: table_name: " << table_name << std::endl;
                        std::cout << "will be deleted: record_id: " << record_id << std::endl;
                    }
                }
            }

            if (table_id_pairs_to_be_deleted.empty())
                info << "There is no history orphan row to be deleted." << commit;
            for (auto& p : table_id_pairs_to_be_deleted)
            {
                SQLite::Statement query_delete_table_history(
                    db, "delete from history where table_name=? and record_id=?");
                string table_name = p.first;
                identification record_id = p.second;

                if (record_id == 0) continue;

                query_delete_table_history.bind(1, table_name);
                query_delete_table_history.bind(2, record_id);

                int deleted_count = query_delete_table_history.exec();
            }
        }
        catch (SQLite::Exception& e)
        {
            std::cerr << "Exception happened during SQL" << history_rows_without_delete_operation_sql << e.what() << " "
                << std::endl;
            response["error"] = e.what();
            response["history_rows_without_delete_operation_sql"] = history_rows_without_delete_operation_sql;
            return response;
        }

        info << history_rows_with_delete_operation_sql << commit;

        if (delete_deleted_rows)
        {
            try
            {
                SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
                db.exec("PRAGMA journal_mode=WAL;");
                db.exec("PRAGMA synchronous=NORMAL;");
                db.exec("PRAGMA foreign_keys = ON;");

                SQLite::Statement query(db, history_rows_with_delete_operation_sql);
                query.bind(1, cast64(now - history_orphan_threshold_in_days * MILLISECONDS_PER_DAY));

                std::vector<std::pair<std::string, identification>> table_id_pairs_to_be_deleted;
                while (query.executeStep())
                {
                    string table_name = query.getColumn(0).getString();
                    identification record_id = query.getColumn(1).getInt64();

                    if (record_id == 0) continue;

                    table_id_pairs_to_be_deleted.push_back({table_name, record_id});
                    std::cout << "will be deleted: table_name: " << table_name << std::endl;
                    std::cout << "will be deleted: record_id: " << record_id << std::endl;
                }

                if (table_id_pairs_to_be_deleted.empty())
                    info << "There is no history orphan row to be deleted." << commit;
                for (auto& p : table_id_pairs_to_be_deleted)
                {
                    SQLite::Statement query_delete_table_history(
                        db, "delete from history where table_name=? and record_id=?");
                    string table_name = p.first;
                    identification record_id = p.second;

                    if (record_id == 0) continue;

                    query_delete_table_history.bind(1, table_name);
                    query_delete_table_history.bind(2, record_id);

                    int deleted_count = query_delete_table_history.exec();
                }
            }
            catch (SQLite::Exception& e)
            {
                std::cerr << "Exception happened during SQL" << history_rows_without_delete_operation_sql << e.what() <<
                    " "
                    << std::endl;
                response["error"] = e.what();
                response["history_rows_without_delete_operation_sql"] = history_rows_without_delete_operation_sql;
                return response;
            }
        }


        return response;
    };
}
