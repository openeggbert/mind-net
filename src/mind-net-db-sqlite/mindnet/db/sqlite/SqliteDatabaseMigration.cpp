///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
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

#include "mindnet/db/sqlite/SqliteDatabaseMigration.h"
#include <SQLiteCpp/SQLiteCpp.h>

#include "mindnet/util/Utils.h"
#include "mindnet/orm/MigrationColumns.h"
#include <openssl/sha.h>
#include <iomanip>

#include "mindnet/essential/Global.h"
#include "mindnet/api/MigrationScripts.h"
#include "mindnet/db/sqlite/SqliteFileName.h"
#include "mindnet/orm/Migration.h"
#include <algorithm>
#include <chrono>

namespace mindnet::db::sqlite
{
    using std::string;
    using orm::MigrationColumns;
    using_loggers()

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
        std::map<int, string> sql_hashes{};
    public:
        explicit DBMigration(const string& plugin_name_, api::MigrationScriptsPtr& migration_scripts_ptr_) : plugin_name(plugin_name_), migration_scripts_ptr(migration_scripts_ptr_) {};

        bool executeSQL(
            SQLite::Database& db,
            const string& sql,
            int number
            ) const
        {
            info << "executeSQL()" << commit;
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

        bool create_table_migration(SQLite::Database& db)
        {
            trace << "create_table_migration()" << commit;
            string SQL_CREATE_TABLE_MIGRATION =
                R"(
CREATE TABLE "migration" (
                "plugin_name" TEXT NOT NULL,
                "version" INTEGER NOT NULL,
                "description" TEXT NOT NULL,
                "script" TEXT NOT NULL,
                "hash" TEXT NOT NULL,
                "installed_on" TEXT NOT NULL,
                "execution_time" INTEGER NOT NULL,
                PRIMARY KEY(plugin_name, version)
            );
)";
            return executeSQL(db, SQL_CREATE_TABLE_MIGRATION, 0);
        }


        int get_last_migration_number(SQLite::Database& db)
        {
            trace << "get_last_migration_number()" << commit;
            SQLite::Statement query(
                db, string(
                    string("SELECT MAX(") +
                    MigrationColumns::VERSION +
                    string(") AS V FROM ") +
                    MigrationColumns::MODEL_NAME +
                    " WHERE " + MigrationColumns::PLUGIN_NAME + "= ?"
                )
            );
            query.bind(1, plugin_name);
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

        bool has_migration(SQLite::Database& db, int version)
        {
            trace << "has_migration() " << "version=" << version << commit;

            auto sql  =
                string("SELECT COUNT(*) AS C FROM ") +
                MigrationColumns::MODEL_NAME +
                " WHERE " + MigrationColumns::PLUGIN_NAME + "=? AND " +
                MigrationColumns::VERSION + "=?";
            try
            {

                SQLite::Statement query(
                    db, sql);
                query.bind(1, plugin_name);
                query.bind(2, version);
                query.executeStep();
                //

                int count = query.getColumn(0);

                return count != 0;
            }
            catch (SQLite::Exception& e)
            {
                throw std::runtime_error("SQL Select for has_migration() and migration #" + std::string(std::to_string(version)) + " for plugin " + plugin_name  + " failed: " + e.what());
            }

        }

        orm::Migration find_migration(SQLite::Database& db, int version)
        {
            trace << "find_migration() " << "version=" << version << commit;

            auto sql  =
                string("SELECT * FROM ") +
                MigrationColumns::MODEL_NAME +
                " WHERE " + MigrationColumns::PLUGIN_NAME + "=? AND " +
                MigrationColumns::VERSION + "=?";
            try
            {
                orm::Migration migration;
                SQLite::Statement query(
                    db, sql);
                query.bind(1, plugin_name);
                query.bind(2, version);
                query.executeStep();
                //
                int i = 0;
                migration.plugin_name = query.getColumn(i++).getString();
                migration.version = query.getColumn(i++);
                migration.description = query.getColumn(i++).getString();
                //
                migration.script = query.getColumn(i++).getString();
                migration.hash_sha256 = query.getColumn(i++).getString();
                migration.installed_on = query.getColumn(i++).getString();
                migration.execution_time = query.getColumn(i++);
                return migration;
            }
            catch (SQLite::Exception& e)
            {
            throw std::runtime_error("SQL Select for migration #" + std::string(std::to_string(version)) + " for plugin " + plugin_name  + " failed: " + e.what());
            }

        }

        bool validate_table_existence(SQLite::Database& db)
        {
            trace << "validateTableExists()" << commit;
            bool doesTableExist = false;
            try
            {
                SQLite::Statement query(
                    db, string("SELECT * FROM ") + MigrationColumns::MODEL_NAME);
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
                bool created = create_table_migration(db);
                if (created)
                {
                    info << "Table " << MigrationColumns::MODEL_NAME << " created." << commit;
                }
                else
                {
                    err << "Table " << MigrationColumns::MODEL_NAME << " could not be created." << commit;
                    return false;
                }
            }

            if (!validate_table_existence(db)) return false;

            {
                int max_version = get_last_migration_number(db);
                for (int version = 1; version <= max_version; version++)
                {
                    bool has = has_migration(db, version);
                    if (!has)
                    {
                        throw std::runtime_error(std::string("Migration sql script expected in database, but not found: plugin=" + plugin_name + ", version=" + std::to_string(version)));
                    }
                    try
                    {
                        orm::Migration migration = find_migration(db, version);
                        sql_hashes[version] = migration.hash_sha256;

                        string expected_description = migration_scripts_ptr->get_migration_name(version);
                        string found_description = migration.description;
                        if (expected_description != found_description)
                        {
                            err << "Migration description mismatch for plugin " << plugin_name
                                << " version " << version << ": expected '"
                                << expected_description << "' but found '"
                                << found_description << "'" << commit;
                            return false;
                        }
                        //
                        string expected_script = migration_scripts_ptr->get_migration_file_name(version);
                        string found_script = migration.script;
                        if (expected_script != found_script)
                        {
                            err << "Migration script mismatch for plugin " << plugin_name
                                << " version " << version << ": expected '"
                                << expected_script << "' but found '"
                                << found_script << "'" << commit;
                            return false;
                        }

                        //
                        auto& sql = migration_scripts_ptr->get_sql(version);
                        string hash_for_previous_migration = version == 1 ? "" : sql_hashes[version - 1];
                        string expected_hash = util::Utils::hash_sha_256(hash_for_previous_migration + util::Utils::hash_sha_256(sql));
                        string found_hash = migration.hash_sha256;
                        if (expected_hash != found_hash)
                        {
                            err << "Migration hash mismatch for plugin " << plugin_name
                                << " version " << version << ": expected '"
                                << expected_hash << "' but found '"
                                << found_hash << "'" << commit;
                            return false;
                        }
                    } catch (const SQLite::Exception& e)
                    {
                        err << e.what() << commit;
                        return false;
                    }

                }
            }


            return true;
        }

        bool insert_migration(
            SQLite::Database& db,
            int migration_number,
            string hash_sha256,
            string installed_on,
            int execution_time)
        {
            const string& migration_name = migration_scripts_ptr->get_migration_name(migration_number);
            const string& migration_file_name = migration_scripts_ptr->get_migration_file_name(migration_number);

            trace << "insert_migration()" << commit;
            SQLite::Statement query(
                db,
                "INSERT INTO " +
                string(MigrationColumns::MODEL_NAME) +
                " VALUES(?,?,?,?,?,?,?) "
            );
            int i = 0;
            query.bind(++i, plugin_name);
            query.bind(++i, migration_number);
            query.bind(++i, migration_name);
            query.bind(++i, migration_file_name);
            query.bind(++i, hash_sha256);
            query.bind(++i, installed_on);
            query.bind(++i, execution_time);
            try
            {
                query.exec();
                return true;
            }
            catch (SQLite::Exception& e)
            {
                err
                    << "Exception happened during inserting migration "
                    << migration_scripts_ptr << ": " << e.what() << commit;
                return false;
            }
        }

        string get_current_datetime()
        {
            trace << "getCurrentDateTime()" << commit;
            using namespace std::chrono;

            // Get current time point
            auto now = system_clock::now();
            auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
            std::time_t now_c = system_clock::to_time_t(now);
            std::tm now_tm = *std::localtime(&now_c);

            std::ostringstream oss;
            oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S")
                << '.' << std::setw(3) << std::setfill('0') << ms.count();

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
                trace << "Going to find out the max_migration_number" << commit;
                int max_migration_number = get_last_migration_number(db);
                trace << string(string("max_migration_number=") + std::to_string(max_migration_number)).c_str() <<
                    commit;
                if (max_migration_number == -1) return false;


                int last_applied = max_migration_number; // number of last applied migration from DB
                int available = migration_scripts_ptr->get_count(); // how many migrations are available

                trace << "last_applied=" << last_applied
                      << ", available=" << available << commit;

                for (int migration_number = last_applied + 1;
                     migration_number <= available;
                     migration_number++)
                {
                    debug << "Going to migrate migration "
                          << migration_scripts_ptr->get_migration_file_name(migration_number) << commit;

                    const string& sql = migration_scripts_ptr->get_sql(migration_number);

                    auto start_of_sql_execution = std::chrono::high_resolution_clock::now();
                    bool migrated = executeSQL(db, sql, migration_number);
                    auto end_of_sql_execution = std::chrono::high_resolution_clock::now();
                    auto duration_of_sql_execution_in_microseconds = duration_cast<std::chrono::microseconds>(end_of_sql_execution - start_of_sql_execution).count();

                    string hash_of_previous_migration = migration_number == 1 ? "" : sql_hashes[migration_number - 1];
                    string hash = util::Utils::hash_sha_256(hash_of_previous_migration + util::Utils::hash_sha_256(sql));
                    sql_hashes[migration_number] = hash;

                    string installed_on = get_current_datetime();
                    if (migrated)
                    {
                        bool inserted = insert_migration(db, migration_number, hash, installed_on, duration_of_sql_execution_in_microseconds);
                        if (!inserted)
                        {
                            err << "Migration " << migration_number
                                << " failed, it could not be inserted into the database." << commit;
                            return false;
                        }
                    }
                    else
                    {
                        err << "Migration " << migration_number << " failed." << commit;
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
        DBMigration dbMigration = DBMigration(plugin_name, migration_scripts_ptr);
        return dbMigration.migrate();
    }
}
