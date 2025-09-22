//
// Created by robertvokac on 9/7/25.
//
#ifndef MIGRATIONSCRIPTS_H
#define MIGRATIONSCRIPTS_H
#include <regex>
#include <string>
#include <vector>

#include "IValidator.h"
#include "ModelRegistration.h"
#include "Trigger.h"
#include "jwt-cpp/jwt.h"
#include "mindnet/essential/DatabaseType.h"
#include "mindnet/db/sqlite/RepositoryHelper.h"

namespace mindnet::api
{

    struct MigrationScript
    {
        const int number;
        const std::string name;
        const std::string file_name;
        const std::string sql;
    };

    const std::regex sql_file_name_format("^V(\\d+)__([a-zA-Z0-9_]+)\\.sql$");
    class MigrationScripts
    {
    public:
        explicit MigrationScripts(const core::DatabaseType database_type_ ) : database_type(database_type_){};
        virtual ~MigrationScripts() = default;

        size_t get_count()
        {
            ensure_defined();
            return migrations.size();
        }

        const std::string& get_sql(size_t migration_number)
        {
            ensure_defined();
            ensury_migration_number_is_in_range(migration_number);

            return migrations[migration_number-1].sql;
        }

        std::string get_migration_name(size_t migration_number)
        {
            ensury_migration_number_is_in_range(migration_number);
            return migrations[migration_number-1].name;
        }

        std::string get_migration_file_name(size_t migration_number)
        {
            ensury_migration_number_is_in_range(migration_number);
            return migrations[migration_number-1].file_name;
        }

        [[nodiscard]] const core::DatabaseType& get_database_type() const { return database_type; }

    protected:
        virtual void define_migrations() = 0;

        void add_migration(const std::string sql_file_name, const std::string sql)
        {
            std::smatch match;
            if (!std::regex_match(sql_file_name, match, sql_file_name_format))
            {
                throw std::runtime_error("Invalid format for sql file name: " + sql_file_name);
            }

            const int migration_number = std::stoi(match[1]);
            const std::string migration_name = match[2];
            int last_migration_number = migrations.size();
            const int expected_next_migration_number = last_migration_number + 1;
            if (migration_number != expected_next_migration_number)
            {
                throw std::runtime_error("Invalid number for sql file name: " + sql_file_name);
            }
            MigrationScript migration_script{
                migration_number, migration_name, sql_file_name, sql
            };
            migrations.emplace_back(migration_script);
        }

    private:
        std::vector<MigrationScript> migrations;

        void ensure_defined()
        {
            if (!defined)
            {
                define_migrations();
                defined = true;
            }
        }

        void ensury_migration_number_is_in_range(size_t migration_number)
        {
            if (migration_number < 1 || migration_number > migrations.size())
            {
                throw std::out_of_range(
                    "Migration number " + std::to_string(migration_number) +
                    " out of range (1.." + std::to_string(migrations.size()) + ")");
            }
        }

        bool defined = false;
        core::DatabaseType database_type = core::DatabaseType::Unknown;

    };

    typedef std::shared_ptr<MigrationScripts> MigrationScriptsPtr;
}
#endif // MIGRATIONSCRIPTS_H
