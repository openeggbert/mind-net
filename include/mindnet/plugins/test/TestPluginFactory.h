//
// Created by robertvokac on 9/13/25.
//
#ifndef TESTPLUGINFACTORY_H
#define TESTPLUGINFACTORY_H

#include "mindnet/api/PluginFactory.h"

namespace mindnet::plugins::test
{
    class TestPluginFactory : public api::PluginFactory
    {
    public:
        api::PluginPtr create() const override;
    };
}

#endif // TESTPLUGINFACTORY_H
