///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : Mind map software.
// Copyright (C) 2023-2023 the original author or authors.
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see
// <https://www.gnu.org/licenses/> or write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include "mindnet/persistence/impl/sqlite/SqliteDatabaseMigration.h"
#include <SQLiteCpp/SQLiteCpp.h>

#include "mindnet/Utils.h"
#include "mindnet/persistence/impl/sqlite/Migrations.h"
#include "../../../../../include/mindnet/persistence/impl/sqlite/MigrationColumns.h"
#include <openssl/sha.h>
#include <iomanip>

#include "mindnet/Global.h"
#include "mindnet/persistence/impl/sqlite/SqliteFileName.h"

namespace mindnet::persistence::impl::sqlite
{
    using models::fields::MigrationColumns;
    SqliteDatabaseMigration::SqliteDatabaseMigration()
    {
        //Not meant to be instantiated
    };

    static SqliteDatabaseMigration* INSTANCE;

    SqliteDatabaseMigration* SqliteDatabaseMigration::getInstance()
    {
        if (INSTANCE == nullptr)
        {
            INSTANCE = new SqliteDatabaseMigration();
        }
        return INSTANCE;
    }

    void SqliteDatabaseMigration::destroyInstance()
    {
        if (INSTANCE != nullptr)
        {
            delete INSTANCE;
            INSTANCE = nullptr;
        }
    }

    class DBMigration
    {

    public:
        DBMigration(
        )
        {

        }

        bool executeSQL(SQLite::Database& db, std::string& sql, int number)
        {
            log << "executeSQL()";
            SQLite::Statement query(db, sql);
            try
            {
                query.exec();
                return true;
            }
            catch (SQLite::Exception& e)
            {
                err << "Exception happened during SQLite migration # " << number << ": " << e.what() << " " << commit;
                err << "SQL: " << sql << commit;
                return false;
            }
        }

        bool createTable(SQLite::Database& db)
        {
            trace << "createTable()" << commit;
            std::string SQL_CREATE_TABLE_MIGRATION =
                R"(
CREATE TABLE "migration" (
                "id" INTEGER NOT NULL,
                "max_migration_number" INTEGER NOT NULL,
                PRIMARY KEY("id")
            );
)";
            return executeSQL(db, SQL_CREATE_TABLE_MIGRATION, 0);
        }

        bool initTable(SQLite::Database& db)
        {
            trace << "initTable()" << commit;
            std::string SQL_INSERT_INTO_TABLE_MIGRATION =
                R"(
INSERT INTO "migration" VALUES (1,0)
)";
            try
            {
                return executeSQL(db, SQL_INSERT_INTO_TABLE_MIGRATION, 0);
            }
            catch (std::exception& e)
            {
                err << "Exception happened during SQLite migration initTable(): " << e.what() << std::endl;
                throw;
            }
        }


        bool validateTableExists(SQLite::Database& db)
        {
            trace << "validateTableExists()" << commit;
            bool doesTableExist = false;
            try
            {
                SQLite::Statement query(
                    db, std::string("SELECT * FROM ") + models::fields::MigrationColumns::MODEL_NAME);
                query.executeStep();
                doesTableExist = true;
            }
            catch (SQLite::Exception& e)
            {
                doesTableExist = false;
            }
            if (!doesTableExist)
            {
                err << "Table " << models::fields::MigrationColumns::MODEL_NAME << " does not exist." << commit;
                return false;
            }
            return true;
        }

        bool validate(SQLite::Database& db)
        {
            trace << "validate()" << commit;
            if (!validateTableExists(db))
            {
                bool created = createTable(db);
                if (created)
                {
                    std::cout << "Table " << MigrationColumns::MODEL_NAME << " created." << std::endl;
                    bool inited = initTable(db);
                    if (!inited)
                    {
                        err << "Table " << MigrationColumns::MODEL_NAME << " could not be initialized."  << commit;
                        return false;
                    }
                    else
                    {
                        std::cout << "Table " << MigrationColumns::MODEL_NAME << " initialized." << std::endl;
                    }
                }
                else
                {
                    err << "Table " << MigrationColumns::MODEL_NAME << " could not be created."  << commit;
                    return false;
                }
            }
            if (!validateTableExists(db)) return false;

            return true;
        }

        int get_newest_migration_number(SQLite::Database& db)
        {
            trace << "getNewestMigrationNumber()" << commit;
            SQLite::Statement query(
                db, std::string(
                    std::string("SELECT ") +
                    MigrationColumns::MAX_MIGRATION_NUMBER +
                    std::string(" AS M FROM ") +
                    MigrationColumns::MODEL_NAME +
                    " WHERE " + MigrationColumns::ID + "=1"
                )
                );
            try
            {
                query.executeStep();
                int max = query.getColumn(0);

                return max;
            }
            catch (SQLite::Exception& e)
            {
                err << "Exception happened during SQLite migration getNewestMigrationNumber(): " << e.what() <<
                    commit;
                return -1;
            }
        }

        bool update_migration_number(SQLite::Database& db, int max_migration_number)
        {
            trace << "updateMigration()" << commit;
            SQLite::Statement query(
                db,
                "UPDATE " +
                std::string(MigrationColumns::MODEL_NAME) +
                " SET " + MigrationColumns::MAX_MIGRATION_NUMBER +
                " = ?"
            );
            int i = 0;
            query.bind(++i, max_migration_number);
            try
            {
                query.exec();
                return true;
            }
            catch (SQLite::Exception& e)
            {
                err
                    << "Exception happened during updating max_migrating_number to # "
                    << max_migration_number << ": " << e.what() << commit;
                return false;
            }
        }

        std::string get_current_datetime()
        {
            trace << "getCurrentDateTime()" << commit;
            using namespace std::chrono;

            // Get current time point
            auto now = system_clock::now();
            auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
            std::time_t now_c = system_clock::to_time_t(now);
            std::tm now_tm = *std::localtime(&now_c);

            std::ostringstream oss;
            oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << ':'
                << std::setw(3) << std::setfill('0') << ms.count();

            return oss.str();
        }

        bool migrate()
        {
            trace << "migrate()" << commit;
            try
            {
                trace << "Opening SQLite database" << commit;

                SQLite::Database db(SQLITE_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

                trace << "SQLite database opened." << commit;
                trace << "Going to validate database" << commit;
                bool validated = validate(db);
                if (validated)
                {
                    trace << "Database validated." << commit;
                }
                else
                {
                    err << "Database validation failed." << commit;
                    return false;
                }
                trace << "Going to find out the maxMigrationNumber" << commit;
                int maxMigrationNumber = get_newest_migration_number(db);
                trace << std::string(std::string("maxMigrationNumber=") + std::to_string(maxMigrationNumber)).c_str() << commit;
                if (maxMigrationNumber == -1) return false;
                for (int migrationNumber = (maxMigrationNumber == 0 ? 1 : maxMigrationNumber + 1); migrationNumber <= MIGRATION_COUNT; migrationNumber++)
                {
                    std::cout << "Going to migrate migration " << migrationNumber << std::endl;
                    if (migrationNumber <= maxMigrationNumber)
                    {
                        std::cout << "Skipping already finished migration " << migrationNumber << std::endl;
                        continue;
                    }
                    std::string sql = migrations[migrationNumber - 1];


                    bool migrated = executeSQL(db, sql, migrationNumber);


                    if (migrated)
                    {
                        bool updated = update_migration_number(db, migrationNumber);
                        if (!updated)
                        {
                            err << "Migration " << migrationNumber <<
                                " failed, it could not be updated in the database. " << commit;
                            return false;
                        }
                    }
                    else
                    {
                        err << "Migration " << migrationNumber << " failed." << commit;
                        return false;
                    }
                }
            }
            catch (std::exception& e)
            {
                err << "Exception happened during SQLite migration: " << e.what() << " " << commit;
                return false;
            }

            return true;
        }
    };

    bool SqliteDatabaseMigration::migrate()
    {
        using std::string;

        DBMigration dbMigration = DBMigration();
        return dbMigration.migrate();
    }
}
