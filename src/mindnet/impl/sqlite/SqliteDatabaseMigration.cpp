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

#include "mindnet/impl/sqlite/SqliteDatabaseMigration.h"
#include <SQLiteCpp/SQLiteCpp.h>

#include "mindnet/Utils.h"
#include "mindnet/impl/sqlite/MigrationColumns.h"
#include <openssl/sha.h>
#include <iomanip>

#include "mindnet/Global.h"
#include "mindnet/api/MigrationScripts.h"
#include "mindnet/impl/sqlite/SqliteFileName.h"

namespace mindnet::impl::sqlite
{
    using sqlite::MigrationColumns;

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
    private:
        const string plugin_name;
        api::MigrationScriptsPtr& migration_scripts_ptr;
    public:
        explicit DBMigration(const string& plugin_name_, api::MigrationScriptsPtr& migration_scripts_ptr_) : plugin_name(plugin_name_), migration_scripts_ptr(migration_scripts_ptr_) {};

        bool executeSQL(SQLite::Database& db, std::string& sql, int number) const
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
                err << "Exception happened during SQLite migration # " << number << " for plugin " << plugin_name << ": " << e.what() << " " << commit;
                err << "SQL: " << sql << commit;
                return false;
            }
        }

        bool create_table(SQLite::Database& db)
        {
            trace << "create_table()" << commit;
            std::string SQL_CREATE_TABLE_MIGRATION =
                R"(
CREATE TABLE "migration" (
                "plugin" TEXT NOT NULL,
                "last_migration_number" INTEGER NOT NULL,
                PRIMARY KEY("plugin")
            );
)";
            return executeSQL(db, SQL_CREATE_TABLE_MIGRATION, 0);
        }


        int get_last_migration_number(SQLite::Database& db)
        {
            trace << "get_last_migration_number()" << commit;
            SQLite::Statement query(
                db, std::string(
                    std::string("SELECT ") +
                    MigrationColumns::LAST_MIGRATION_NUMBER +
                    std::string(" AS L FROM ") +
                    MigrationColumns::MODEL_NAME +
                    " WHERE " + MigrationColumns::PLUGIN + "= '" + plugin_name + "'"
                )
            );
            try
            {
                bool some_result = query.executeStep();
                if (!some_result) {return -1;}
                int last = query.getColumn(0);

                return last;
            }
            catch (SQLite::Exception& e)
            {
                err << "Exception happened during SQLite migration get_last_migration_number(): " << e.what() <<
                    commit;
                return -1;
            }
        }

        bool initialize_plugin_in_table_if_needed(SQLite::Database& db)
        {
            int last_migration_number = get_last_migration_number(db);
            if (last_migration_number != -1)
            {
                //nothing to do
                return true;

            }
            trace << "initialize_plugin_in_table_if_needed()" << commit;
            std::string SQL_INSERT_INTO_TABLE_MIGRATION =
                "INSERT INTO \"migration\" VALUES ('" + plugin_name + "', 0)";
            try
            {
                return executeSQL(db, SQL_INSERT_INTO_TABLE_MIGRATION, 0);
            }
            catch (std::exception& e)
            {
                err << "Exception happened during SQLite migration init_table(): " << e.what() << std::endl;
                throw;
            }
        }

        bool validate_table_existence(SQLite::Database& db)
        {
            trace << "validateTableExists()" << commit;
            bool doesTableExist = false;
            try
            {
                SQLite::Statement query(
                    db, std::string("SELECT * FROM ") + MigrationColumns::MODEL_NAME);
                query.executeStep();
                doesTableExist = true;
            }
            catch (SQLite::Exception& e)
            {
                doesTableExist = false;
            }
            if (!doesTableExist)
            {
                err << "Table " << MigrationColumns::MODEL_NAME << " does not exist." << commit;
                return false;
            }
            return true;
        }

        bool validate(SQLite::Database& db)
        {
            trace << "validate()" << commit;
            if (!validate_table_existence(db))
            {
                bool created = create_table(db);
                if (created)
                {
                    info << "Table " << MigrationColumns::MODEL_NAME << " created." << std::endl;
                }
                else
                {
                    err << "Table " << MigrationColumns::MODEL_NAME << " could not be created." << commit;
                    return false;
                }
            }

            {
                if (!initialize_plugin_in_table_if_needed(db))
                {
                    err << "Table " << MigrationColumns::MODEL_NAME << " could not be initialized for plugin " << plugin_name << commit;
                    return false;
                }

                info << "Table " << MigrationColumns::MODEL_NAME << " initialized for plugin " << plugin_name << std::endl;
            }
            if (!validate_table_existence(db)) return false;

            return true;
        }

        bool update_migration_number(SQLite::Database& db, int max_migration_number)
        {
            trace << "updateMigration()" << commit;
            SQLite::Statement query(
                db,
                "UPDATE " +
                std::string(MigrationColumns::MODEL_NAME) +
                " SET " + MigrationColumns::LAST_MIGRATION_NUMBER +
                " = ?" +
                " WHERE " + MigrationColumns::PLUGIN +
                " = '" + plugin_name + "'"
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
                db.exec("PRAGMA foreign_keys = ON;");

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
                int maxMigrationNumber = get_last_migration_number(db);
                trace << std::string(std::string("maxMigrationNumber=") + std::to_string(maxMigrationNumber)).c_str() <<
                    commit;
                if (maxMigrationNumber == -1) return false;
                for (int migrationNumber = (maxMigrationNumber == 0 ? 1 : maxMigrationNumber + 1); migrationNumber <=
                     migration_scripts_ptr->get_count(); migrationNumber++)
                {
                    debug << "Going to migrate migration " << migrationNumber << commit;
                    if (migrationNumber <= maxMigrationNumber)
                    {
                        debug << "Skipping already finished migration " << migrationNumber << std::endl;
                        continue;
                    }
                    std::string sql = migration_scripts_ptr->get_migration(migrationNumber - 1);

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

    bool SqliteDatabaseMigration::migrate(const string& plugin_name, api::MigrationScriptsPtr& migration_scripts_ptr)
    {
        using std::string;

        DBMigration dbMigration = DBMigration(plugin_name, migration_scripts_ptr);
        return dbMigration.migrate();
    }
}
