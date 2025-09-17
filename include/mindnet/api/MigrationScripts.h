//
// Created by robertvokac on 9/7/25.
//
#ifndef MIGRATIONSCRIPTS_H
#define MIGRATIONSCRIPTS_H
#include <string>
#include <vector>

#include "IValidator.h"
#include "ModelRegistration.h"
#include "Trigger.h"
#include "jwt-cpp/jwt.h"
#include "mindnet/DatabaseType.h"
#include "mindnet/impl/sqlite/RepositoryHelper.h"

namespace mindnet::api
{

    class MigrationScripts
    {
    public:
        explicit MigrationScripts(const DatabaseType database_type_ ) : database_type(database_type_){};
        virtual ~MigrationScripts() = default;

        size_t get_count()
        {
            ensure_defined();
            return migrations.size();
        }

        const std::string& get_migration(size_t migration_number)
        {
            ensure_defined();
            if (migration_number >= migrations.size())
            {
                throw std::out_of_range(
                    "Migration number " + std::to_string(migration_number) +
                    " out of range " + std::to_string(migrations.size() - 1));
            }

            return migrations[migration_number];
        }

        DatabaseType get_database_type() const { return database_type; }

    protected:
        virtual void define_migrations() = 0;

        std::vector<std::string> migrations;

    private:
        void ensure_defined()
        {
            if (!defined)
            {
                define_migrations();
                defined = true;
            }
        }

        bool defined = false;
        DatabaseType database_type = DatabaseType::Unknown;

    };

    typedef std::shared_ptr<MigrationScripts> MigrationScriptsPtr;
}
#endif // MIGRATIONSCRIPTS_H
