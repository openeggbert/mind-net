//
// Created by robertvokac on 9/7/25.
//
#pragma once

#define REGISTER_MODEL(model, Model, MODEL)\
plugin->register_model(\
        models::MODEL##_DEFINITION,\
        std::make_shared<validators::Model##Validator>(),\
        repository_factory);

#define REGISTER_MIGRATIONS(Plugin, db_type) \
    essential::DatabaseType current_database_type##db_type = \
    essential::g_configuration.database_type;\
    if(current_database_type##db_type == mindnet::essential::DatabaseType:: db_type )\
    plugin->register_migrations(\
    std::make_shared<migrations:: Plugin##db_type##MigrationScripts>()\
);

#include "Plugin.hpp"

namespace mindnet::api
{
    class PluginFactory
    {
    public:
        virtual ~PluginFactory() = default;

        [[nodiscard]] virtual PluginPtr create(std::shared_ptr<RepositoryFactory>& repository_factory) const = 0;
    };
}
