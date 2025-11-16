//
// Created by robertvokac on 9/13/25.
//
#pragma once


/*
 * Global parameters:
 * Session is valid for x days=default 1 day
 * Limit for session.selected_item=default 100 items
 */
#include "mindnet/api/PluginFactory.hpp"

namespace mindnet::plugins::repetition
{
    class RepetitionPluginFactory : public api::PluginFactory
    {
    public:
        [[nodiscard]] api::PluginPtr create(std::shared_ptr<api::RepositoryFactory>& repository_factory) const override;
    };
}

