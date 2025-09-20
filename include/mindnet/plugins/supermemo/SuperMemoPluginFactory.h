//
// Created by robertvokac on 9/13/25.
//
#ifndef SUPERMEMOPLUGINFACTORY_H
#define SUPERMEMOPLUGINFACTORY_H


#include "mindnet/api/PluginFactory.h"

namespace mindnet::plugins::supermemo
{
    class SuperMemoPluginFactory : public api::PluginFactory
    {
    public:
        [[nodiscard]] api::PluginPtr create() const override;
    };
}

#endif // SUPERMEMOPLUGINFACTORY_H
