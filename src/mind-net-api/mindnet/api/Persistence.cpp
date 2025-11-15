//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/api/Persistence.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#define lock_if_needed()                                            \
std::unique_lock<std::mutex> sqlite_lock;                       \
if (database_type_is_sqlite())                                  \
{                                                               \
sqlite_lock = std::unique_lock<std::mutex>(                 \
mindnet::api::Persistence::sqlite_mutex                 \
);                                                          \
}


namespace {
    bool database_type_is_sqlite()
    {
        return mindnet::essential::g_configuration.database_type == mindnet::essential::DatabaseType::SQLite;
    }
}
namespace mindnet::api
{
    using_loggers()

    std::mutex Persistence::sqlite_mutex;

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
        lock_if_needed()
        string error;
        int last_id = get_repository(def.get_model_name())->create(fields, error);
        return {last_id, {last_id < 0 ? 500 : 0, error}};
    }

    std::pair<entity_fields, OperationResult> Persistence::read(const model::ModelDefinition& def,
                                                                api::AccessTokenContext& token, const int id)
    {
        lock_if_needed()
        string error;
        entity_fields ef = get_repository(def.get_model_name())->read(id, error);
        if (error.empty())
        {
            return {ef, {}};
        }
        return {{}, {500, error}};
    }

    OperationResult Persistence::update(
        const model::ModelDefinition& def, api::AccessTokenContext& token,
        int id, entity_fields& fields
    )
    {
        lock_if_needed()
        string error;
        get_repository(def.get_model_name())->update(id, fields, error);
        if (error.empty()) { return ok_result; }
        return {500, error};
    }

    OperationResult Persistence::remove(const model::ModelDefinition& def, api::AccessTokenContext& token, int id)
    {
        lock_if_needed()
        string_map empty_map;

        string error;
        get_repository(def.get_model_name())->remove(id, error);
        if (error.empty()) { return ok_result; }
        else { return {500, error}; }
    }

    std::pair<std::vector<entity_fields>, OperationResult> Persistence::list(
        const ModelDefinition& def,
        api::AccessTokenContext& token,
        orm::QueryParams& query_params)
    {
        lock_if_needed()
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
