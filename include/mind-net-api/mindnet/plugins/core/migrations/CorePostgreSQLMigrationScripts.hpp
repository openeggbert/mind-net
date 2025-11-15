//
// Created by robertvokac on 9/17/25.
//
#ifndef COREPOSTGRESQLMIGRATIONSCRIPTS_H
#define COREPOSTGRESQLMIGRATIONSCRIPTS_H


#include "mindnet/api/MigrationScripts.hpp"

namespace mindnet::plugins::core::migrations
{
    class CorePostgreSQLMigrationScripts : public api::MigrationScripts
    {
    public:
        CorePostgreSQLMigrationScripts();

    protected:
        void define_migrations() override;
    };
}


#endif // COREPOSTGRESQLMIGRATIONSCRIPTS_H
