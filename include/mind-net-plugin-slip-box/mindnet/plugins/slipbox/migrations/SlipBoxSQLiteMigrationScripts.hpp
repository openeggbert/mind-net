//
// Created by robertvokac on 9/17/25.
//
#pragma once



#include "mindnet/api/MigrationScripts.hpp"

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

