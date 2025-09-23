//
// Created by robertvokac on 9/13/25.
//
#ifndef SLIPBOXPLUGINFACTORY_H
#define SLIPBOXPLUGINFACTORY_H


#include "mindnet/api/PluginFactory.h"

namespace mindnet::plugins::slipbox
{
    class SlipBoxPluginFactory : public api::PluginFactory
    {
    public:
        [[nodiscard]] api::PluginPtr create() const override;
    };
}

#endif // SLIPBOXPLUGINFACTORY_H
