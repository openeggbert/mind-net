//
// Created by robertvokac on 9/13/25.
//
#ifndef COREPLUGINFACTORY_H
#define COREPLUGINFACTORY_H

#include "mindnet/api/PluginFactory.hpp"

namespace mindnet::plugins::core
{
    class CorePluginFactory : public api::PluginFactory
    {
    public:
        CorePluginFactory() = default;
        ~CorePluginFactory() override = default;
        [[nodiscard]] api::PluginPtr create(std::shared_ptr<api::RepositoryFactory>& repository_factory) const override;
    };
}

#endif // COREPLUGINFACTORY_H
