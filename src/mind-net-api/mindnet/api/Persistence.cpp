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

#include "mindnet/api/Persistence.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/api/SqliteGlobal.hpp"
#include "mindnet/api/CompilationFlags.hpp"

namespace
{
    bool database_type_is_sqlite()
    {
        return mindnet::essential::g_configuration.database_type == mindnet::essential::DatabaseType::SQLite;
    }
}

namespace mindnet::api
{
    using_loggers()

    Persistence::Persistence(PluginRegistryPtr& plugin_registry_ptr)
    {
        for (auto& plugin_name : plugin_registry_ptr->get_plugin_names_sorted_by_dependencies())
        {
            auto plugin = plugin_registry_ptr->get_plugin(plugin_name);
            for (auto& model_registration : plugin->get_model_registrations())
            {
                auto model_definition = model_registration->model_definition;
                auto model_name = model_definition.get_model_name();
                repositories[model_name] = model_registration->repository;
                repository_names.emplace_back(model_name);
            }
        }
        model_cache_.set_capacity_size(essential::g_configuration.read_cache_capacity_size);
        model_cache_.set_capacity_bytes(essential::g_configuration.read_cache_capacity_bytes);
    }

    Persistence::~Persistence()
    {
    }

    std::shared_ptr<IRepository> Persistence::get_repository(const std::string& name)
    {
        return repositories.count(name) ? repositories[name] : nullptr;
    }

    bool Persistence::has_model_with_name(const std::string& name)
    {
        return repositories.count(name) > 0;
    }

    std::vector<std::string>& Persistence::list_model_names()
    {
        return repository_names;
    }

    std::pair<identification, OperationResult> Persistence::create(
        const ModelDefinition& def,
        api::AccessTokenContext& token,
        entity_fields& fields)
    {
        SQLITE_LOCK_GUARD()
        string error;
        int newId = get_repository(def.get_model_name())->create(fields, error);

        if (newId >= 0)
        {
            if (READ_CACHE_ENABLED&& def
            .
            is_cache_enabled() && def.is_cached_after_create()
            )
            {
                fields[0] = newId;
                model_cache_.put(def.get_model_name(), newId, fields);
            }
            return {newId, ok_result};
        }
        return {newId, {500, error}};
    }

    std::pair<entity_fields, OperationResult> Persistence::read(const model::ModelDefinition& def,
                                                                api::AccessTokenContext& token, const identification id)
    {
        const std::string& table = def.get_model_name();

        // 1) Try cache
        entity_fields cached;
        if (READ_CACHE_ENABLED&& def
        .
        is_cache_enabled() && model_cache_.get(table, id, cached)
        )
        {
            return {cached, ok_result};
        }
        // 2) DB read
        SQLITE_LOCK_GUARD()
        string error;
        entity_fields ef = get_repository(def.get_model_name())->read(id, error);
        if (!error.empty())
        {
            return {{}, {500, error}};
        }

        // 3) Save to cache
        if (READ_CACHE_ENABLED&& def
        .
        is_cache_enabled()
        )
        model_cache_.put(table, id, ef);

        return {ef, ok_result};
    }

    void Persistence::invalidate(const model::ModelDefinition& def, const identification id)
    {
        if (READ_CACHE_ENABLED&& def
        .
        is_cache_enabled()
        )
        model_cache_.invalidate(def.get_model_name(), id);
    }

    OperationResult Persistence::update(
        const model::ModelDefinition& def, api::AccessTokenContext& token,
        identification id, entity_fields& fields
    )
    {
        SQLITE_LOCK_GUARD()
        string error;
        get_repository(def.get_model_name())->update(id, fields, error);
        if (error.empty())
        {
            if (READ_CACHE_ENABLED&& def
            .
            is_cache_enabled()
            )
            model_cache_.invalidate(def.get_model_name(), id);
            return ok_result;
        }
        return {500, error};
    }

    OperationResult Persistence::remove(const model::ModelDefinition& def, api::AccessTokenContext& token,
                                        identification id)
    {
        SQLITE_LOCK_GUARD()
        string_map empty_map;

        string error;
        get_repository(def.get_model_name())->remove(id, error);

        if (error.empty())
        {
            if (READ_CACHE_ENABLED&& def
            .
            is_cache_enabled()
            )
            model_cache_.invalidate(def.get_model_name(), id);
            return ok_result;
        }

        return {500, error};
    }

    std::pair<std::vector<entity_fields>, OperationResult> Persistence::list(
        const ModelDefinition& def,
        api::AccessTokenContext& token,
        orm::QueryParams& query_params)
    {
        SQLITE_LOCK_GUARD()
        string error;
        std::vector<entity_fields> items;
        const auto& model_name = def.get_model_name();
        const auto& repo = get_repository(model_name);
        if (READ_CACHE_ENABLED&& LIST_CACHE_ENABLED && def
        .
        is_cache_enabled()
        )
        {
            std::vector<identification> ids = repo->list_ids(query_params, error);
            std::vector<identification> ids_not_in_cache;
            for (auto& id : ids)
            {
                entity_fields cached;
                if (model_cache_.get(model_name, id, cached))
                {
                    items.push_back(cached);
                }
                else
                {
                    ids_not_in_cache.push_back(id);
                }
            }
            if (!ids_not_in_cache.empty())
            {
                auto items_from_db = repo->list_in_ids(ids_not_in_cache, error);;
                for (auto& item : items_from_db)
                {
                    items.push_back(item);
                    identification number = std::get<int64_t>(item[0]);
                    model_cache_.put(model_name, number, item);
                }
            }
        }
        else
        {
            items = repo->list(query_params, error);
        }
        if (error.empty())
        {
            return {items, ok_result};
        }
        return {{}, {500, error}};
    }

    std::optional<ModelDefinition> Persistence::get_model_definition(const string& model_name)
    {
        if (!has_model_with_name(model_name))
        {
            warn << "Model " << model_name << " not found" << commit;
            return std::nullopt;
        }
        return get_repository(model_name)->get_model_definition();
    }

    entity_fields Persistence::request_to_entity_fields(
        crow::json::rvalue& body, const mindnet::essential::Crudl crudl, model::ModelDefinition& def)
    {
        return get_repository(def.get_model_name())->request_to_entity_fields(body, crudl);
    }
}