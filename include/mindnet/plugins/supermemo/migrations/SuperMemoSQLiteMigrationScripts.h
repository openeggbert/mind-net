//
// Created by robertvokac on 9/17/25.
//
#ifndef SUPERMEMOSQLITEMIGRATIONSCRIPTS_H
#define SUPERMEMOSQLITEMIGRATIONSCRIPTS_H


#include "mindnet/api/MigrationScripts.h"

namespace mindnet::plugins::supermemo::migrations
{
    class SuperMemoSQLiteMigrationScripts : public api::MigrationScripts
    {
    public:
        SuperMemoSQLiteMigrationScripts();

    protected:
        void define_migrations() override;
    };
}

#endif // SUPERMEMOSQLITEMIGRATIONSCRIPTS_H
