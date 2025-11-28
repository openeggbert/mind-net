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

#pragma once

#include <regex>
#include <string>
#include <vector>

#include "IValidator.hpp"
#include "ModelRegistration.hpp"
#include "Trigger.hpp"
#include "mindnet/essential/DatabaseType.hpp"

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
        explicit MigrationScripts(const essential::DatabaseType database_type_) : database_type(database_type_)
        {
        };
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

            return migrations[migration_number - 1].sql;
        }

        std::string get_migration_name(size_t migration_number)
        {
            ensure_defined();
            ensury_migration_number_is_in_range(migration_number);
            return migrations[migration_number - 1].name;
        }

        std::string get_migration_file_name(size_t migration_number)
        {
            ensure_defined();
            ensury_migration_number_is_in_range(migration_number);
            return migrations[migration_number - 1].file_name;
        }

        [[nodiscard]] const essential::DatabaseType& get_database_type() const { return database_type; }

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
            std::string migration_name = match[2];
            std::ranges::replace(migration_name, '_', ' ');

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
        essential::DatabaseType database_type = essential::DatabaseType::Unknown;
    };

    typedef std::shared_ptr<MigrationScripts> MigrationScriptsPtr;
}