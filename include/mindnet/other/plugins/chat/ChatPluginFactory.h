//
// Created by robertvokac on 9/13/25.
//
#ifndef CHATPLUGINFACTORY_H
#define CHATPLUGINFACTORY_H

#include "mindnet/other/api/PluginFactory.h"

namespace mindnet::plugins::chat
{
    class ChatPluginFactory : public api::PluginFactory
    {
    public:
        [[nodiscard]] api::PluginPtr create() const override;
    };
}

#endif // CHATPLUGINFACTORY_H
