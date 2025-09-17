//
// Created by robertvokac on 9/17/25.
//
#ifndef TESTSQLITEMIGRATIONSCRIPTS_H
#define TESTSQLITEMIGRATIONSCRIPTS_H

#include "mindnet/api/MigrationScripts.h"

namespace mindnet::plugins::test::migrations
{
    class TestSQLiteMigrationScripts : public api::MigrationScripts
    {
    public:
        TestSQLiteMigrationScripts();

    protected:
        void define_migrations() override;
    };
}

#endif // TESTSQLITEMIGRATIONSCRIPTS_H
