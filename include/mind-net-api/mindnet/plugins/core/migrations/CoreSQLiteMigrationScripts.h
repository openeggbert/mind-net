//
// Created by robertvokac on 9/17/25.
//

#ifndef MIND_NET_CORESQLITEMIGRATIONSCRIPTS_H
#define MIND_NET_CORESQLITEMIGRATIONSCRIPTS_H
#include "mindnet/api/MigrationScripts.h"

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


#endif //MIND_NET_CORESQLITEMIGRATIONSCRIPTS_H
