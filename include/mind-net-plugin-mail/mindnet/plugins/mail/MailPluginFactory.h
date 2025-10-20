//
// Created by robertvokac on 9/13/25.
//
#ifndef MAILSQLITEPLUGINFACTORY_H
#define MAILSQLITEPLUGINFACTORY_H

#include "mindnet/api/PluginFactory.h"

namespace mindnet::plugins::mail
{
    class MailPluginFactory : public api::PluginFactory
    {
    public:
        [[nodiscard]] api::PluginPtr create(std::shared_ptr<api::RepositoryFactory>& repository_factory) const override;
    };
}

#endif // MAILSQLITEPLUGINFACTORY_H
