//
// Created by robertvokac on 9/17/25.
//
#ifndef CHATSQLITEMIGRATIONSCRIPTS_H
#define CHATSQLITEMIGRATIONSCRIPTS_H

#include "../../../../../mind-net-api/mindnet/api/MigrationScripts.h"

namespace mindnet::plugins::chat::migrations
{
    class ChatSQLiteMigrationScripts : public api::MigrationScripts
    {
    public:
        ChatSQLiteMigrationScripts();

    protected:
        void define_migrations() override;
    };
}


#endif // CHATSQLITEMIGRATIONSCRIPTS_H
