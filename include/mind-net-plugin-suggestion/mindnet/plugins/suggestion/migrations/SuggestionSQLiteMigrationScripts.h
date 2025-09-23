//
// Created by robertvokac on 9/17/25.
//
#ifndef SUGGESTIONSQLITEMIGRATIONSCRIPTS_H
#define SUGGESTIONSQLITEMIGRATIONSCRIPTS_H


#include "mindnet/api/MigrationScripts.h"

namespace mindnet::plugins::suggestion::migrations
{
    class SuggestionSQLiteMigrationScripts : public api::MigrationScripts
    {
    public:
        SuggestionSQLiteMigrationScripts();

    protected:
        void define_migrations() override;
    };
}

#endif // SUGGESTIONSQLITEMIGRATIONSCRIPTS_H
