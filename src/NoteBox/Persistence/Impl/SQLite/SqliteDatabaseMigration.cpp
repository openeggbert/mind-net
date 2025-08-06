///////////////////////////////////////////////////////////////////////////////////////////////
// bit-backup: Tool detecting bit rots in files.
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

#include "NoteBox/Persistence/Impl/SQLite/SqliteDatabaseMigration.h"
#include <SQLiteCpp/SQLiteCpp.h>

#include "NoteBox/ProgressTracker.h"
#include "NoteBox/Utils.h"
#include "NoteBox/Persistence/Impl/SQLite/Migrations.h"
#include "NoteBox/Persistence/Impl/SQLite/MigrationTable.h"
#include <openssl/sha.h>
#include <iomanip>

namespace NoteBox::Persistence::Impl::SQLite {
    SqliteDatabaseMigration::SqliteDatabaseMigration() {
        //Not meant to be instantiated
    };

    static SqliteDatabaseMigration *INSTANCE;

    SqliteDatabaseMigration *SqliteDatabaseMigration::getInstance() {
        if (INSTANCE == nullptr) {
            INSTANCE = new SqliteDatabaseMigration();
        }
        return INSTANCE;
    }

    void SqliteDatabaseMigration::destroyInstance() {
        if (INSTANCE != nullptr) {
            delete INSTANCE;
            INSTANCE = nullptr;
        }
    }

    class DBMigration {
    private:
        std::string jdbcUrl;

    public:
        DBMigration(
            const std::string &jdbcUrlIn
        ) {
            this->jdbcUrl = jdbcUrlIn;
        }

        bool executeSQL(SQLite::Database& db, std::string& sql, int number) {
            SQLite::Statement query(db, sql);
            try {
                query.exec();
                return true;
            } catch (SQLite::Exception &e) {
                std::cerr << "Exception happened during SQLite migration # " << number << ": " << e.what() << " " << std::endl;
                return false;
            }
        }

        bool createTable(SQLite::Database& db) {
            std::string SQL_CREATE_TABLE_MIGRATION =
                    R"(
CREATE TABLE "MIGRATION" (
                "ID" INTEGER NOT NULL,
                "MAX_MIGRATION_NUMBER" INTEGER NOT NULL,
                PRIMARY KEY("ID")
            );
)";
            return executeSQL(db, SQL_CREATE_TABLE_MIGRATION, 0);
        }

        bool validateTableExists(SQLite::Database& db) {
            bool doesTableExist = false;
            SQLite::Statement query(
                db, std::string("SELECT * FROM ") + MigrationTable::TABLE_NAME);
            try {
                query.executeStep();
                doesTableExist = true;
            } catch (SQLite::Exception &e) {
                doesTableExist = false;
            }
            if (!doesTableExist) {
                std::cerr << "Table " << DBMigrationSchemaHistoryTable::TABLE_NAME << " does not exist." << std::endl;
                return false;
            }
            return true;
        }
        bool validateThatThereAreNoMigrationFailures(SQLite::Database& db) {
            bool noFailures = false;
            SQLite::Statement query(
                db, std::string(
                    "SELECT COUNT(*) AS C FROM ") +
                    DBMigrationSchemaHistoryTable::TABLE_NAME +
                    " WHERE " +
                    DBMigrationSchemaHistoryTable::SUCCESS +
                    " = 0"
                    );
            try {
                query.executeStep();
                int count = query.getColumn(0);

                noFailures = count == 0;
            } catch (SQLite::Exception &e) {
                std::cerr << "Exception happened during SQLite migration validateThatThereAreNoMigrationFailures(): " << e.what() << std::endl;
                return false;
            }
            if (!noFailures) {
                std::cerr << "Table " << DBMigrationSchemaHistoryTable::TABLE_NAME << " has some migration failures." << std::endl;
                return false;
            }
            return true;
        }

        bool validate(SQLite::Database& db) {
            if (!validateTableExists(db)) {
                bool create = createTable(db);
                if (!create) return false;
            }
            if (!validateTableExists(db)) return false;
            if (!validateThatThereAreNoMigrationFailures(db)) return false;
            return true;
        }

        int getNewestMigrationNumber(SQLite::Database& db) {
            SQLite::Statement query(
                db, std::string(
                    std::string("SELECT MAX(") +
                    DBMigrationSchemaHistoryTable::INSTALLED_RANK +
                    std::string(") AS M FROM ") +
                    DBMigrationSchemaHistoryTable::TABLE_NAME
                    ));
            try {
                query.executeStep();
                int max = query.getColumn(0);

                return max;
            } catch (SQLite::Exception &e) {
                std::cerr << "Exception happened during SQLite migration getNewestMigrationNumber(): " << e.what() << std::endl;
                return -1;
            }
        }

        bool addMigration(SQLite::Database& db, Entity::DBMigrationSchemaHistory& migration) {
            SQLite::Statement query(
                db,
            "INSERT INTO " +
            std::string(DBMigrationSchemaHistoryTable::TABLE_NAME) +
            " VALUES (?,?,?,?,?,   ?,?,?,?,?,   ?,?)"
            );
            int i = 0;
            query.bind(++i, migration.id);
            query.bind(++i, migration.migrationGroup);
            query.bind(++i, migration.installedRank);
            query.bind(++i, migration.version);
            query.bind(++i, migration.description);
            ////
            query.bind(++i, migration.type);
            query.bind(++i, migration.script);
            query.bind(++i, migration.hash);
            query.bind(++i, migration.installedBy);
            query.bind(++i, migration.installedOn);
            ////
            query.bind(++i, migration.executionTime);
            query.bind(++i, migration.success);
            try {
                query.exec();
                return true;
            } catch (SQLite::Exception &e) {
                std::cerr << "Exception happened during adding # " << migration.installedRank << "migration to database: " << e.what() << std::endl;
                return false;
            }
        }

        std::string sha1(const std::string& input) {
            unsigned char hash[SHA_DIGEST_LENGTH]; // SHA1 digest size
            SHA1(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);

            std::ostringstream oss;
            for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
                oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
            }

            return oss.str();
        }


        std::string getCurrentDateTime() {
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


        bool migrate() {
            try {
                SQLite::Database db(directoryWhereSqliteFileIs, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

                if (!validate(db)) return FAILURE;
                int maxMigrationNumber = getNewestMigrationNumber(db);
                if (maxMigrationNumber == -1) return FAILURE;
                for (int migrationNumber = 1; migrationNumber <= MIGRATION_COUNT; migrationNumber++) {
                    if (migrationNumber <= maxMigrationNumber) {
                        std::cout << "Skipping already finished migration " << migrationNumber << std::endl;
                        continue;
                    }
                    std::string sql = migrations[migrationNumber - 1];
                    Core::ProgressTracker pt = 1;
                    pt.start();
                    bool migrated = executeSQL(db, sql, migrationNumber);
                    pt.nextDone();
                    int elapsedMilliseconds = pt.getElapsedMillisecondsSinceStart();
                    if (migrated) {

                        Entity::DBMigrationSchemaHistory db_migration_schema_history = {
                            Core::Utils::generateUUIDv4(),
                            "bitbackup",
                            migrationNumber,
                            std::to_string(migrationNumber),
                            sql,
                            "SQL",
                            std::to_string(migrationNumber),
                            sha1(sql),
                            installedBy,
                            getCurrentDateTime(),
                            elapsedMilliseconds,
                            true
                        };

                        bool added = addMigration(db, db_migration_schema_history);
                        if (!added) {
                            std::cerr << "Migration " << migrationNumber << " failed, it could not be added to the database. " << std::endl;
                            return FAILURE;
                        }
                    } else {
                        std::cerr << "Migration " << migrationNumber << " failed." << std::endl;
                        return MigrationResult::FAILURE;
                    }
                }
            } catch (std::exception &e) {
                std::cout << "Exception happened during SQLite migration: " << e.what() << " " << std::endl;
                return MigrationResult::FAILURE;
            }

            return MigrationResult::SUCCESS;
        }
    };

    bool SqliteDatabaseMigration::migrate() {
        using std::string;
        string jdbcUrl = Utils::createJdbcUrl(".");
        std::cerr << "jdbcUrl=" << jdbcUrl;

        DBMigration dbMigration = DBMigration(jdbcUrl);
        return dbMigration.migrate();
    }
}
