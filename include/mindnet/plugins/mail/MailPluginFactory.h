//
// Created by robertvokac on 9/13/25.
//
#ifndef MAILPLUGINFACTORY_H
#define MAILPLUGINFACTORY_H

#include "mindnet/api/PluginFactory.h"

namespace mindnet::plugins::mail
{
    class MailPluginFactory : public api::PluginFactory
    {
    public:
        api::PluginPtr create() const override;
    };
}

#endif // MAILPLUGINFACTORY_H
