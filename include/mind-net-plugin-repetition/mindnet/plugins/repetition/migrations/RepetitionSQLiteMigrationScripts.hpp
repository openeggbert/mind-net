//
// Created by robertvokac on 9/17/25.
//
#pragma once



#include "mindnet/api/MigrationScripts.hpp"

namespace mindnet::plugins::repetition::migrations
{
    class RepetitionSQLiteMigrationScripts : public api::MigrationScripts
    {
    public:
        RepetitionSQLiteMigrationScripts();

    protected:
        void define_migrations() override;
    };
}

