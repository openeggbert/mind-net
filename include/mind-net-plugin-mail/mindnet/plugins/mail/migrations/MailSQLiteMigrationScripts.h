//
// Created by robertvokac on 9/17/25.
//
#ifndef MAILMIGRATIONSCRIPTS_H
#define MAILMIGRATIONSCRIPTS_H


#include "../../../../../mind-net-api/mindnet/api/MigrationScripts.h"

namespace mindnet::plugins::mail::migrations
{
    class MailSQLiteMigrationScripts : public api::MigrationScripts
    {
    public:
        MailSQLiteMigrationScripts();

    protected:
        void define_migrations() override;
    };
}

#endif // MAILMIGRATIONSCRIPTS_H
