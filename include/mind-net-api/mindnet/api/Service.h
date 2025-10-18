//
// Created by robertvokac on 9/13/25.
//

#ifndef MIND_NET_SERVICE_H
#define MIND_NET_SERVICE_H
#include "IService.h"
#include "PluginRegistry.h"
#include "TriggerRegistry.h"

namespace mindnet::api
{
    using validator = api::IValidator*;
    using mindnet::api::OperationResult;
    using model::ModelDefinition;

    class Service : public api::IService
    {
    private:
        std::map<std::string, std::shared_ptr<IValidator>> validators;
        api::DbPtr db_ptr;
        api::PluginRegistryPtr plugin_registry_ptr;
        api::TriggerRegistryPtr trigger_registry_ptr;
        std::map<string,QueryPtr> query_map;

    public:
        Service(const api::DbPtr& db_ptr, const api::PluginRegistryPtr& plugin_registry);
        ~Service() override;

        bool has_model(const std::string& model_name) override;

        std::vector<std::string>& list_model_names() override;

        nlohmann::json call_query(const std::string& query_name, nlohmann::json& request) override;

        std::pair<int, OperationResult> create(const ModelDefinition& def, api::AccessTokenContext& token,
                                               entity_fields& fields, int stack_depth = 0) ;
        std::pair<entity_fields, OperationResult>
        read(const ModelDefinition& def, api::AccessTokenContext& token, int id, int stack_depth = 0) ;
        OperationResult update(const ModelDefinition& def, api::AccessTokenContext& token, int id,
                               entity_fields& fields, int stack_depth = 0) ;
        OperationResult remove(ModelDefinition& def, api::AccessTokenContext& token, int id, int stack_depth = 0) ;
        std::pair<std::vector<entity_fields>, OperationResult> list(ModelDefinition& def, api::AccessTokenContext& token,
                                                                    orm::QueryParams& query_params, int stack_depth = 0) ;

        std::optional<ModelDefinition> get_model_definition(const string& model_name) override;

        entity_fields request_to_entity_fields(
            crow::json::rvalue& body, mindnet::essential::Crudl crudl, ModelDefinition& def
        ) override;
        const api::PluginRegistryPtr get_plugin_registry() const override;

    private:
        std::shared_ptr<IValidator> get_validator(const std::string& name);
        OperationResult can_create(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                   entity_fields& ef) override;
        OperationResult can_read(const ModelDefinition& model_definition, api::AccessTokenContext& token, int id) override;
        OperationResult can_update(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                   entity_fields& ef) override;
        OperationResult can_delete(const ModelDefinition& model_definition, api::AccessTokenContext& token, int id) override;
        OperationResult can_list(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                 string_map& filter) override;

        //
    };
}

#endif //MIND_NET_SERVICE_H
