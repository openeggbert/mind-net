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

#include <iostream>
#include <string>

#include "mindnet/db/sqlite/SqliteDatabaseMigration.hpp"
#include <SQLiteCpp/SQLiteCpp.h>

#include "mindnet/util/Utils.hpp"
#include "mindnet/orm/SchemaHistoryColumns.hpp"
#include <openssl/sha.h>
#include <iomanip>

#include "mindnet/essential/Global.hpp"
#include "mindnet/api/MigrationScripts.hpp"
#include "mindnet/db/sqlite/SqliteFileName.hpp"
#include "mindnet/orm/SchemaHistory.hpp"
#include <chrono>

#include "mindnet/orm/SchemaHistoryMetaColumns.hpp"
#include "nlohmann/detail/exceptions.hpp"

namespace mindnet::db::sqlite
{
    using std::string;
    using orm::SchemaHistoryColumns;
    using_loggers()

    /* TODO REFACTOR (future improvements):
 *
 * The migration engine works reliably, but the DBMigration::migrate() method
 * has become too large and mixes several responsibilities:
 *   - migration execution
 *   - transaction control
 *   - checksum and chain-hash calculation
 *   - schema_history writing
 *   - foreign key toggling
 *
 * Refactor into smaller helper methods, for example:
 *
 *   bool applyMigration(int version, const std::string& sql);
 *   bool writeHistoryEntry(...);
 *   bool executeWithTransaction(...);
 *   bool toggleForeignKeys(bool enable);
 *
 * Benefits:
 *   - better readability and maintainability
 *   - easier debugging and unit testing
 *   - cleaner control flow
 *
 * The current implementation works and is safe, so this is only a
 * structural/cleanliness improvement for later.
 */

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
        std::map<int, string> sql_chain_hashes{};

        std::map<int, string> meta_migrations{};

    public:
        explicit DBMigration(const string& plugin_name_, api::MigrationScriptsPtr& migration_scripts_ptr_) :
            plugin_name(plugin_name_), migration_scripts_ptr(migration_scripts_ptr_)
        {
            // meta_migrations[2] = R"(ALTER TABLE SCHEMA_HISTORY ADD COLUMN A INTEGER)";
            // meta_migrations[3] = R"(ALTER TABLE SCHEMA_HISTORY ADD COLUMN B INTEGER)";
            // meta_migrations[4] = R"(ALTER TABLE SCHEMA_HISTORY ADD COLUMN C INTEGER)";
            // meta_migrations[5] = R"(ALTER TABLE SCHEMA_HISTORY DROP COLUMN A)";
            // meta_migrations[6] = R"(ALTER TABLE SCHEMA_HISTORY DROP COLUMN B)";
            // meta_migrations[7] = R"(ALTER TABLE SCHEMA_HISTORY DROP COLUMN C)";
        }

        void apply_pragmas(SQLite::Database& db) const
        {
            try
            {
                db.exec("PRAGMA journal_mode=WAL;");
                db.exec("PRAGMA synchronous=NORMAL;");
                db.exec("PRAGMA foreign_keys=ON;");
            }
            catch (...)
            {
            }
        }

        bool execute_sql(
            SQLite::Database& db,
            const string& sql,
            int number
        ) const
        {
            info << "execute_sql()" << commit;
            try
            {
                db.exec(sql);

                return true;
            }
            catch (SQLite::Exception& e)
            {
                err << "Exception happened during SQLite migration # " << number << " for plugin " << plugin_name <<
                    ": " << e.what() << " " << commit;
                err << "SQL: " << sql << commit;
                return false;
            }
        }

        bool create_table_schema_history(SQLite::Database& db)
        {
            trace << "create_table_schema_history()" << commit;
            const string SQL_CREATE_TABLE_MIGRATION =
                R"(
CREATE TABLE "schema_history" (
                "plugin_name" TEXT NOT NULL,
                "version" INTEGER NOT NULL,
                "description" TEXT NOT NULL,

                "script" TEXT NOT NULL,
                "checksum" TEXT NOT NULL,
                "chain_hash" TEXT NOT NULL,
                "installed_on" TEXT NOT NULL,

                "execution_time" INTEGER NOT NULL,
                "success" BOOLEAN NOT NULL,

                PRIMARY KEY(plugin_name, version)
            );
)";
            return execute_sql(db, SQL_CREATE_TABLE_MIGRATION, 0);
        }

        bool create_table_schema_history_meta(SQLite::Database& db)
        {
            trace << "create_table_schema_history_meta()" << commit;
            const string create_sql =
                R"(
CREATE TABLE IF NOT EXISTS "schema_history_meta" (
    "version" INTEGER NOT NULL,
    "checksum" TEXT NOT NULL
);
)";
            const string seed_sql =
                R"(
INSERT INTO "schema_history_meta"(version, checksum)
SELECT 1, 'e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855'
WHERE NOT EXISTS (SELECT 1 FROM "schema_history_meta");
)";

            return execute_sql(db, create_sql, 0) && execute_sql(db, seed_sql, 0);
        }

        bool create_table(SQLite::Database& db, char const* table_name)
        {
            if (strcmp(table_name, SchemaHistoryColumns::MODEL_NAME) == 0)
            {
                return create_table_schema_history(db);
            }
            if (strcmp(table_name, orm::SchemaHistoryMetaColumns::MODEL_NAME) == 0)
            {
                return create_table_schema_history_meta(db);
            }
            err << "create_table(): Unsupported table: " << table_name << commit;
            return false;
        }

        int get_last_successful_version(SQLite::Database& db)
        {
            trace << "get_last_successful_version()" << commit;
            SQLite::Statement query(
                db, string(
                    string("SELECT MAX(") +
                    SchemaHistoryColumns::VERSION +
                    string(") AS V FROM ") +
                    SchemaHistoryColumns::MODEL_NAME +
                    " WHERE " + SchemaHistoryColumns::PLUGIN_NAME + "=? " +
                    " AND " + SchemaHistoryColumns::SUCCESS + " =? "
                )
            );
            query.bind(1, plugin_name);
            query.bind(2, 1);
            try
            {
                if (query.executeStep())
                {
                    if (query.getColumn(0).isNull())
                    {
                        return 0;
                    }
                    return query.getColumn(0).getInt();
                }
                return 0;
            }
            catch (SQLite::Exception& e)
            {
                err << "Exception happened during SQLite migration get_last_migration_number(): " << e.what() <<
                    commit;
                return -1;
            }
        }

        int get_last_meta_version(SQLite::Database& db)
        {
            trace << "get_last_meta_version()" << commit;
            SQLite::Statement query(
                db, string(
                    string("SELECT MAX(") +
                    orm::SchemaHistoryMetaColumns::VERSION +
                    string(") FROM ") +
                    orm::SchemaHistoryMetaColumns::MODEL_NAME
                )
            );
            try
            {
                if (query.executeStep())
                {
                    if (query.getColumn(0).isNull())
                    {
                        return 1;
                    }
                    return query.getColumn(0).getInt();
                }
                return 1;
            }
            catch (SQLite::Exception& e)
            {
                err << "Exception happened during SQLite migration get_last_meta_version(): " << e.what() <<
                    commit;
                return -1;
            }
        }

        bool insert_meta_version(SQLite::Database& db, int new_version)
        {
            trace << "insert_meta_version()" << commit;
            SQLite::Statement query(
                db, string(
                    string("INSERT INTO ") +
                    orm::SchemaHistoryMetaColumns::MODEL_NAME +
                    string(" (version, checksum) ") +
                    " VALUES (?,?)"
                )
            );
            try
            {
                query.bind(1, new_version);
                query.bind(2, util::Utils::compute_sha256(meta_migrations[new_version]));
                query.exec();
                return true;
            }
            catch (SQLite::Exception& e)
            {
                err << "Exception happened during SQLite updating insert_meta_version(): " << e.what() <<
                    commit;
                return false;
            }
        }

        bool has_schema_history(SQLite::Database& db, int version)
        {
            trace << "has_migration() " << "version=" << version << commit;

            auto sql =
                string("SELECT COUNT(*) AS C FROM ") +
                SchemaHistoryColumns::MODEL_NAME +
                " WHERE " + SchemaHistoryColumns::PLUGIN_NAME + "=? AND " +
                SchemaHistoryColumns::VERSION + "=?";
            try
            {
                SQLite::Statement query(
                    db, sql);
                query.bind(1, plugin_name);
                query.bind(2, version);
                query.executeStep();
                int count = query.getColumn(0);
                return count != 0;
            }
            catch (SQLite::Exception& e)
            {
                throw std::runtime_error(
                    "SQL Select for has_migration() and migration #" + std::string(std::to_string(version)) +
                    " for plugin " + plugin_name + " failed: " + e.what());
            }
        }

        orm::SchemaHistory find_schema_history(SQLite::Database& db, int version)
        {
            trace << "find_migration() " << "version=" << version << commit;

            auto sql =
                string("SELECT ") +
                SchemaHistoryColumns::PLUGIN_NAME + ", " +
                SchemaHistoryColumns::VERSION + ", " +
                SchemaHistoryColumns::DESCRIPTION + ", " +
                SchemaHistoryColumns::SCRIPT + ", " +
                SchemaHistoryColumns::HASH_SHA256 + ", " +
                SchemaHistoryColumns::CHAIN_HASH + ", " +
                SchemaHistoryColumns::INSTALLED_ON + ", " +
                SchemaHistoryColumns::EXECUTION_TIME + ", " +
                SchemaHistoryColumns::SUCCESS + " FROM " +
                SchemaHistoryColumns::MODEL_NAME +
                " WHERE " + SchemaHistoryColumns::PLUGIN_NAME + "=? AND " +
                SchemaHistoryColumns::VERSION + "=?";
            try
            {
                orm::SchemaHistory migration;
                SQLite::Statement query(
                    db, sql);
                query.bind(1, plugin_name);
                query.bind(2, version);

                if (!query.executeStep())
                    throw std::runtime_error("Migration not found: plugin=" + plugin_name +
                        ", version=" + std::to_string(version));
                //
                int i = 0;
                migration.plugin_name = query.getColumn(i++).getString();
                migration.version = query.getColumn(i++);
                migration.description = query.getColumn(i++).getString();
                //
                migration.script = query.getColumn(i++).getString();
                migration.checksum = query.getColumn(i++).getString();
                migration.chain_hash = query.getColumn(i++).getString();
                migration.installed_on = query.getColumn(i++).getString();

                migration.execution_time = query.getColumn(i++);
                migration.success = query.getColumn(i++).getInt() == 1;
                return migration;
            }
            catch (SQLite::Exception& e)
            {
                throw std::runtime_error(
                    "SQL Select for migration #" + std::string(std::to_string(version)) + " for plugin " + plugin_name +
                    " failed: " + e.what());
            }
        }

        bool validate_table_exists(SQLite::Database& db, char const* table_name)
        {
            trace << "validateTableExists()" << commit;
            bool doesTableExist = false;

            SQLite::Statement query(
                db, string("SELECT count(*) FROM sqlite_master WHERE type='table' AND name=?"));
            // Query to check if table exists in SQLite
            query.bind(1, table_name);

            query.executeStep();
            bool exists = query.getColumn(0).getInt() > 0;
            if (exists)
            {
                info << "Table " << table_name << " exists." << commit;
            }
            else
            {
                err << "Table " << table_name << " does not exist." << commit;
            }
            return exists;
        }

        bool validate(SQLite::Database& db)
        {
            trace << "validate()" << commit;

            //TODO split into validate_structure() validate_meta_migrations() validate_schema_history()

            for (const char* table_name : {SchemaHistoryColumns::MODEL_NAME, orm::SchemaHistoryMetaColumns::MODEL_NAME})
            {
                if (!validate_table_exists(db, table_name))
                {
                    if (create_table(db, table_name))
                    {
                        info << "Table " << table_name << " created." << commit;
                    }
                    else
                    {
                        err << "Table " << table_name << " could not be created." << commit;
                        return false;
                    }
                }
            }
            for (const char* table_name : {SchemaHistoryColumns::MODEL_NAME, orm::SchemaHistoryMetaColumns::MODEL_NAME})
            {
                if (!validate_table_exists(db, table_name)) return false;
            }

            int last_meta_version = get_last_meta_version(db);
            int expected_max_meta_version = meta_migrations.empty() ? 1 : meta_migrations.rbegin()->first;

            for (int i = 1; i <= last_meta_version; ++i)
            {
                SQLite::Statement q(db,
                                    "SELECT checksum FROM " + string(orm::SchemaHistoryMetaColumns::MODEL_NAME) +
                                    " WHERE version=?"
                );
                q.bind(1, i);
                if (!q.executeStep())
                {
                    throw std::runtime_error("Meta migration version " + std::to_string(i) + " not found");
                }
                std::string found_checksum = q.getColumn(0).getString();

                std::string expected_sql;
                if (i == 1)
                {
                    expected_sql = ""; // bootstrap
                }
                else
                {
                    expected_sql = meta_migrations[i];
                }
                std::string expected_checksum = util::Utils::compute_sha256(expected_sql);

                if (found_checksum != expected_checksum)
                {
                    err << "Meta migration checksum mismatch for version " << i
                        << ": expected " << expected_checksum
                        << " but found " << found_checksum << commit;
                    return false;
                }
            }

            for (int i = last_meta_version + 1; i <= expected_max_meta_version; ++i)
            {
                if (!meta_migrations.contains(i))
                {
                    err << "meta_migrations does not contain key " << i << "." << commit;
                    return false;
                }
                SQLite::Transaction tx(db);

                try
                {
                    db.exec(meta_migrations[i]);
                    insert_meta_version(db, i);
                    tx.commit();
                }
                catch (std::exception& e)
                {
                    err << "Migrating " << orm::SchemaHistoryMetaColumns::MODEL_NAME <<
                        " failed for migration number " << i << ": " << e.what() << commit;
                    return false;
                }
            }

            int last_successful_version = get_last_successful_version(db);
            if (last_successful_version == -1) return false;
            for (int version = 1; version <= last_successful_version; version++)
            {
                bool has = has_schema_history(db, version);
                if (!has)
                {
                    throw std::runtime_error(std::string(
                        "Migration sql script expected in database, but not found: plugin=" + plugin_name + ", version="
                        + std::to_string(version)));
                }
                try
                {
                    orm::SchemaHistory migration = find_schema_history(db, version);
                    sql_chain_hashes[version] = migration.chain_hash;

                    string expected_description = migration_scripts_ptr->get_migration_name(version);
                    string found_description = migration.description;
                    if (expected_description != found_description)
                    {
                        err << "SchemaHistory description mismatch for plugin " << plugin_name
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
                        err << "SchemaHistory script mismatch for plugin " << plugin_name
                            << " version " << version << ": expected '"
                            << expected_script << "' but found '"
                            << found_script << "'" << commit;
                        return false;
                    }

                    //
                    auto& sql = migration_scripts_ptr->get_sql(version);
                    //
                    string expected_checksum = util::Utils::compute_sha256(sql);
                    string found_checksum = migration.checksum;
                    if (expected_checksum != found_checksum)
                    {
                        err << "SchemaHistory checksum mismatch for plugin " << plugin_name
                            << " version " << version << ": expected '"
                            << expected_checksum << "' but found '"
                            << found_checksum << "'" << commit;
                        return false;
                    }
                    //

                    string chain_hash_for_previous_migration = version == 1 ? "" : sql_chain_hashes[version - 1];
                    string expected_chain_hash = util::Utils::compute_sha256(
                        chain_hash_for_previous_migration + expected_checksum);
                    string found_chain_hash = migration.chain_hash;
                    if (expected_chain_hash != found_chain_hash)
                    {
                        err << "SchemaHistory chain_hash mismatch for plugin " << plugin_name
                            << " version " << version << ": expected '"
                            << expected_chain_hash << "' but found '"
                            << found_chain_hash << "'" << commit;
                        return false;
                    }
                }
                catch (const SQLite::Exception& e)
                {
                    err << e.what() << commit;
                    return false;
                }
            }

            return true;
        }

        bool insert_migration(
            SQLite::Database& db,
            const int migration_number,
            const string& checksum,
            const string& chain_hash,
            const string& installed_on,
            const int execution_time,
            bool success)
        {
            const string& migration_name = migration_scripts_ptr->get_migration_name(migration_number);
            const string& migration_file_name = migration_scripts_ptr->get_migration_file_name(migration_number);

            trace << "insert_migration()" << commit;
            SQLite::Statement query(
                db,
                "INSERT INTO " +
                string(SchemaHistoryColumns::MODEL_NAME) +
                " VALUES(?,?,?, ?,?,?,?, ?,?) "
            );
            int i = 0;
            query.bind(++i, plugin_name);
            query.bind(++i, migration_number);
            query.bind(++i, migration_name);

            query.bind(++i, migration_file_name);
            query.bind(++i, checksum);
            query.bind(++i, chain_hash);
            query.bind(++i, installed_on);

            query.bind(++i, execution_time);
            query.bind(++i, success ? 1 : 0);
            try
            {
                query.exec();
                return true;
            }
            catch (SQLite::Exception& e)
            {
                err
                    << "Exception happened during inserting migration "
                    << migration_file_name << ": " << e.what() << commit;
                return false;
            }
        }

        bool remove_migration(
            SQLite::Database& db,
            const int migration_number)
        {
            trace << "remove_migration()" << commit;
            SQLite::Statement query(
                db,
                "DELETE FROM " +
                string(SchemaHistoryColumns::MODEL_NAME) +
                " WHERE " + SchemaHistoryColumns::PLUGIN_NAME + "=? AND " +
                SchemaHistoryColumns::VERSION + "=?"
            );
            int i = 0;
            query.bind(++i, plugin_name);
            query.bind(++i, migration_number);
            try
            {
                query.exec();
                return true;
            }
            catch (SQLite::Exception& e)
            {
                err
                    << "Exception happened during removing migration "
                    << migration_number << ": " << e.what() << commit;
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
                if (bool validated = validate(db))
                {
                    trace << "Database validated." << commit;
                }
                else
                {
                    err << "Database validation failed." << commit;
                    return false;
                }
                trace << "Going to find out the last successful version" << commit;
                int last_successful_version = get_last_successful_version(db);
                trace << string(string("last_successful_version=") + std::to_string(last_successful_version)).c_str() <<
                    commit;
                if (last_successful_version == -1) return false;

                int last_applied = last_successful_version; // number of last applied migration from DB
                int available = migration_scripts_ptr->get_count(); // how many migrations are available

                trace << "last_applied=" << last_applied
                    << ", available=" << available << commit;

                for (int version = last_applied + 1; version <= available; version++)
                {
                    debug << "Applying migration " << migration_scripts_ptr->get_migration_file_name(version) << commit;

                    if (has_schema_history(db, version))
                    {
                        orm::SchemaHistory schema_history = find_schema_history(db, version);
                        if (schema_history.success)
                        {
                            debug << "Migration " << version << " already applied, skipping." << commit;
                            continue;
                        }

                        if (bool removed = remove_migration(db, version); !removed)
                        {
                            err << "Removing SchemaHistory failed: " << plugin_name << " " << version << commit;
                            return false;
                        }
                    }

                    const string& sql = migration_scripts_ptr->get_sql(version);

                    bool disableFK = (
                        sql.find("/* FK_OFF */") != string::npos ||
                        sql.find("-- FK_OFF") != string::npos
                    );

                    auto start = std::chrono::high_resolution_clock::now();
                    bool migrated = false;

                    string checksum;
                    string chain_hash;
                    long duration{0};
                    string installed_on;

                    try
                    {
                        if (disableFK)
                        {
                            info << "Temporarily disabling foreign key checks for migration " << version << commit;
                            db.exec("PRAGMA foreign_keys=OFF;");
                        }

                        db.exec("BEGIN;");
                        // 1️⃣ execute the migration
                        db.exec(sql);
                        migrated = true;

                        // 2️⃣ prepare data for schema_history
                        auto end = std::chrono::high_resolution_clock::now();
                        duration = duration_cast<std::chrono::microseconds>(end - start).count();

                        string prev_chain = version == 1 ? "" : sql_chain_hashes[version - 1];
                        checksum = util::Utils::compute_sha256(sql);
                        chain_hash = util::Utils::compute_sha256(prev_chain + checksum);
                        sql_chain_hashes[version] = chain_hash;

                        installed_on = get_current_datetime();

                        // 3️⃣ writing the migration record (part of the same transaction!)
                        bool inserted = insert_migration(db, version, checksum, chain_hash, installed_on, duration,
                                                         migrated);
                        if (!inserted)
                        {
                            throw std::runtime_error(
                                "Failed to insert schema_history for migration " + std::to_string(version));
                        }

                        db.exec("COMMIT;");
                        SQLite::Statement chk(db, "PRAGMA foreign_key_check;");
                        while (chk.executeStep())
                        {
                            warn << "Foreign key violation: table=" << chk.getColumn(0).getString()
                                << " rowid=" << chk.getColumn(1).getInt()
                                << " ref-table=" << chk.getColumn(2).getString()
                                << commit;
                        }

                        if (disableFK)
                        {
                            info << "Re-enabling foreign key checks after migration " << version << commit;
                            db.exec("PRAGMA foreign_keys=ON;");
                        }

                        info << "Migration " << version << " applied successfully." << commit;
                    }
                    catch (const std::exception& e)
                    {
                        try
                        {
                            db.exec("ROLLBACK;");
                            insert_migration(db, version, checksum, chain_hash, installed_on, duration,
                                             migrated);
                        }
                        catch (...)
                        {
                        }
                        if (disableFK)
                        {
                            try { db.exec("PRAGMA foreign_keys=ON;"); }
                            catch (...)
                            {
                            }
                        }

                        err << "Migration " << version << " failed and was rolled back: " << e.what() << commit;
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