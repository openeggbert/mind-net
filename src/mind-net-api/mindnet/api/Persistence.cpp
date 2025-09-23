//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/api/Persistence.h"

#include "mindnet/essential/Global.h"
#include "mindnet/api/LoginToken.h"


namespace mindnet::api
{
    using_loggers()

    Persistence::Persistence(PluginRegistryPtr& plugin_registry_ptr)
    {

        for (auto& plugin_name : plugin_registry_ptr->get_plugin_names_sorted_by_dependencies())
        {
            auto plugin = plugin_registry_ptr->get_plugin(plugin_name);
            for (auto& model_registration: plugin->get_model_registrations())
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
        api::LoginToken& token,
        entity_fields& fields)
    {
        string error;
        int last_id = get_repository(def.get_model_name())->create(fields, error);
        return {last_id, {last_id < 0 ? 500 : 0, error}};
    }

    std::pair<entity_fields, OperationResult> Persistence::read(const model::ModelDefinition& def,
                                                                api::LoginToken& token, const int id)
    {
        string error;
        entity_fields ef = get_repository(def.get_model_name())->read(id, error);
        if (error.empty())
        {
            return {ef, {}};
        }
        return {{}, {500, error}};
    }

    OperationResult Persistence::update(
        const model::ModelDefinition& def, api::LoginToken& token,
        int id, entity_fields& fields
    )
    {
        string error;
        get_repository(def.get_model_name())->update(id, fields, error);
        if (error.empty()) { return ok_result; }
        return {500, error};
    }

    OperationResult Persistence::remove(model::ModelDefinition& def, api::LoginToken& token, int id)
    {
        string_map empty_map;

        string error;
        get_repository(def.get_model_name())->remove(id, error);
        if (error.empty()) { return ok_result; }
        else { return {500, error}; }
    }

    std::pair<std::vector<entity_fields>, OperationResult> Persistence::list(
        ModelDefinition& def,
        api::LoginToken& token,
        orm::QueryParams& query_params)
    {
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
