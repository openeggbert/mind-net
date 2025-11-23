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


#include "mindnet/db/sqlite/queries/CleanupHistoryOrphansSQLiteQuery.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include "mindnet/db/sqlite/SqliteFileName.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/util/Utils.hpp"


namespace mindnet::db::sqlite::queries
{
    using_loggers()

    CleanupHistoryOrphansSQLiteQuery::CleanupHistoryOrphansSQLiteQuery()
        : api::Query(QUERY_CleanupHistoryOrphans, "Cleanup the history table - orphans",
                     essential::DatabaseType::SQLite)
    {
    }

    nlohmann::json CleanupHistoryOrphansSQLiteQuery::call(nlohmann::json request, api::InvalidateMethod& invalidate_method)
    {
        if (!request.contains("history_orphan_threshold_in_days"))
            throw std::runtime_error("history_orphan_threshold_in_days not found");

        int history_orphan_threshold_in_days = request["history_orphan_threshold_in_days"];

        nlohmann::json response;

        auto now = util::Utils::current_unix_timestamp_ms();


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


        info << history_rows_without_delete_operation_sql << commit;


        try
        {
            SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            db.exec("PRAGMA journal_mode=WAL;");
            db.exec("PRAGMA synchronous=NORMAL;");
            db.exec("PRAGMA foreign_keys = ON;");

            SQLite::Statement query(db, history_rows_without_delete_operation_sql);
            query.bind(1, cast64(now - history_orphan_threshold_in_days * MILLISECONDS_PER_DAY));

            std::vector<std::pair<std::string, i64>> table_id_pairs_to_be_deleted;
            while (query.executeStep())
            {
                string table_name = query.getColumn(0).getString();
                i64 record_id = query.getColumn(1).getInt64();

                if (record_id == 0) continue;
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
                i64 record_id = p.second;

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
        }

        return response;
    };
}
