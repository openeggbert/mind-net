//
// Created by robertvokac on 9/17/25.
//

#pragma once


#include "mindnet/api/MigrationScripts.hpp"

namespace mindnet::plugins::core::migrations
{
    class CoreSQLiteMigrationScripts : public api::MigrationScripts
    {
    public:
        CoreSQLiteMigrationScripts();

    protected:
        void define_migrations() override;
    };
}


