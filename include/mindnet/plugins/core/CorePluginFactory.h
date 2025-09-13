//
// Created by robertvokac on 9/13/25.
//
#ifndef COREPLUGINFACTORY_H
#define COREPLUGINFACTORY_H

#include "mindnet/api/PluginFactory.h"

namespace mindnet::plugins::core
{
    class CorePluginFactory : public mindnet::api::PluginFactory
    {
    public:
        mindnet::api::PluginPtr create() const override;
    };
}

#endif // COREPLUGINFACTORY_H
