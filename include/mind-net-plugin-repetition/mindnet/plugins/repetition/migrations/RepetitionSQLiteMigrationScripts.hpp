//
// Created by robertvokac on 9/17/25.
//
#ifndef REPETITIONSQLITEMIGRATIONSCRIPTS_H
#define REPETITIONSQLITEMIGRATIONSCRIPTS_H


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

#endif // REPETITIONSQLITEMIGRATIONSCRIPTS_H
