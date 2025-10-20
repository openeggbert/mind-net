//
// Created by robertvokac on 9/13/25.
//
#ifndef CHATPLUGINFACTORY_H
#define CHATPLUGINFACTORY_H

#include "mindnet/api/PluginFactory.h"

namespace mindnet::plugins::chat
{
    class ChatPluginFactory : public api::PluginFactory
    {
    public:
        [[nodiscard]] api::PluginPtr create(std::shared_ptr<api::RepositoryFactory>& repository_factory) const override;
    };
}

#endif // CHATPLUGINFACTORY_H
