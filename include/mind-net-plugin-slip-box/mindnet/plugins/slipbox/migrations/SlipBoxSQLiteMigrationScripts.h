//
// Created by robertvokac on 9/17/25.
//
#ifndef SLIPBOXSQLITEMIGRATIONSCRIPTS_H
#define SLIPBOXSQLITEMIGRATIONSCRIPTS_H


#include "../../../../../mind-net-api/mindnet/api/MigrationScripts.h"

namespace mindnet::plugins::slipbox::migrations
{
    class SlipBoxSQLiteMigrationScripts : public api::MigrationScripts
    {
    public:
        SlipBoxSQLiteMigrationScripts();

    protected:
        void define_migrations() override;
    };
}

#endif // SLIPBOXSQLITEMIGRATIONSCRIPTS_H
