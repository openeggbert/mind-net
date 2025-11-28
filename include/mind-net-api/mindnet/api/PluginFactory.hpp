/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

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
