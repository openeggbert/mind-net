//
// Created by robertvokac on 9/17/25.
//

#include "mindnet/plugins/core/migrations/CorePostgreSQLMigrationScripts.h"

namespace mindnet::plugins::core::migrations
{
    CorePostgreSQLMigrationScripts::CorePostgreSQLMigrationScripts() : MigrationScripts(
        mindnet::essential::DatabaseType::PostgreSQL)
    {
    }

    void CorePostgreSQLMigrationScripts::define_migrations()
    {
    }
}
