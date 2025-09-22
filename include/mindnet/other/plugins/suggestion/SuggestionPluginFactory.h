//
// Created by robertvokac on 9/13/25.
//
#ifndef SUGGESTIONPLUGINFACTORY_H
#define SUGGESTIONPLUGINFACTORY_H

#include "mindnet/other/api/PluginFactory.h"

namespace mindnet::plugins::suggestion
{
    class SuggestionPluginFactory : public api::PluginFactory
    {
    public:
        [[nodiscard]] api::PluginPtr create() const override;
    };
}

#endif // SUGGESTIONPLUGINFACTORY_H
