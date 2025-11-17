//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/api/Persistence.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/api/SqliteGlobal.hpp"

namespace {
    bool database_type_is_sqlite()
    {
        return mindnet::essential::g_configuration.database_type == mindnet::essential::DatabaseType::SQLite;
    }
}
namespace mindnet::api
{
    using_loggers()
    static constexpr bool ENABLE_READ_CACHE = false;

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
        model_cache_.set_capacity_size(0);
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

    std::pair<int, OperationResult> Persistence::create(
        const ModelDefinition& def,
        api::AccessTokenContext& token,
        entity_fields& fields)
    {
        SQLITE_LOCK_GUARD()
        string error;
        int newId = get_repository(def.get_model_name())->create(fields, error);

        if (newId >= 0)
        {
            if (ENABLE_READ_CACHE && def.is_read_cache_enabled()) model_cache_.put(def.get_model_name(), newId, fields);
            return {newId, ok_result};
        }
        return {newId, {500, error}};
    }

    std::pair<entity_fields, OperationResult> Persistence::read(const model::ModelDefinition& def,
                                                                api::AccessTokenContext& token, const i64 id)
    {
        const std::string& table = def.get_model_name();

        // 1) Try cache
        entity_fields cached;
        if (ENABLE_READ_CACHE && def.is_read_cache_enabled() && model_cache_.get(table, id, cached))
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
        if (ENABLE_READ_CACHE && def.is_read_cache_enabled()) model_cache_.put(table, id, ef);

        return {ef, ok_result};
    }
    void Persistence::invalidate(const model::ModelDefinition& def, const i64 id)
    {
        if (ENABLE_READ_CACHE && def.is_read_cache_enabled())
            model_cache_.invalidate(def.get_model_name(), id);
    }

    OperationResult Persistence::update(
        const model::ModelDefinition& def, api::AccessTokenContext& token,
        i64 id, entity_fields& fields
    )
    {
        SQLITE_LOCK_GUARD()
        string error;
        get_repository(def.get_model_name())->update(id, fields, error);
        if (error.empty())
        {
            if (ENABLE_READ_CACHE && def.is_read_cache_enabled()) model_cache_.invalidate(def.get_model_name(), id);
            return ok_result;
        }
        return {500, error};
    }

    OperationResult Persistence::remove(const model::ModelDefinition& def, api::AccessTokenContext& token, i64 id)
    {
        SQLITE_LOCK_GUARD()
        string_map empty_map;

        string error;
        get_repository(def.get_model_name())->remove(id, error);

        if (error.empty())
        {
            if (ENABLE_READ_CACHE && def.is_read_cache_enabled()) model_cache_.invalidate(def.get_model_name(), id);
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
        auto l = get_repository(def.get_model_name())->list(query_params, error);
        if (error.empty())
        {
            return {l, ok_result};
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
