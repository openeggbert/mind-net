//
// Created by robertvokac on 9/13/25.
//

#ifndef MIND_NET_ISERVICE_H
#define MIND_NET_ISERVICE_H
#include "crow/json.h"
#include "mindnet/orm/QueryParams.h"
#include "mindnet/model/ModelDefinition.h"
#include "IValidator.h"
#include "IPersistence.h"
#include "PluginRegistry.h"

namespace mindnet::api
{
    using validator = api::IValidator*;
    using mindnet::api::OperationResult;
    using model::ModelDefinition;
    using api::DbPtr;

    class IService
    {
    public:
        IService(const DbPtr& db_)
        {
        };
        virtual ~IService() = default;

        virtual bool has_model(const std::string& name) = 0;
        virtual std::vector<std::string>& list_model_names() = 0;
        virtual nlohmann::json call_query(const std::string& query_name, nlohmann::json& request) = 0;
        //
        virtual std::pair<int, OperationResult> create(const ModelDefinition& def, api::AccessTokenContext& token,
                                                       entity_fields& fields, int stack_depth = 0) = 0;
        virtual std::pair<entity_fields, OperationResult> read(const ModelDefinition& def, api::AccessTokenContext& token,
                                                               int id, int stack_depth = 0) = 0;
        virtual OperationResult update(const ModelDefinition& def, api::AccessTokenContext& token, int id,
                                       entity_fields& fields, int stack_depth = 0) = 0;
        virtual OperationResult remove(ModelDefinition& def, api::AccessTokenContext& token, int id, int stack_depth = 0) = 0;
        virtual std::pair<std::vector<entity_fields>, OperationResult> list(
            ModelDefinition& def, api::AccessTokenContext& token, orm::QueryParams& query_params, int stack_depth = 0) = 0;
        //
        virtual std::optional<ModelDefinition> get_model_definition(const string& model_name) = 0;
        //
        virtual entity_fields request_to_entity_fields(
            crow::json::rvalue& body,
            mindnet::essential::Crudl crudl,
            ModelDefinition& def
        ) = 0;
        virtual const api::PluginRegistryPtr get_plugin_registry() const = 0;
        bool is_restart_scheduled() const {return restart_scheduled;};
        bool is_shutdown_scheduled() const {return shutdown_scheduled;};

        void schedule_restart() {restart_scheduled = true;};
        void schedule_shutdown() {shutdown_scheduled = true;};

    private:
        virtual OperationResult can_create(
            const model::ModelDefinition& model_definition,
            api::AccessTokenContext& token, entity_fields& ef) = 0;
        virtual OperationResult can_read(const ModelDefinition& model_definition, api::AccessTokenContext& token, int id) = 0;
        virtual OperationResult can_update(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                           entity_fields& ef, entity_fields& old_fields) = 0;
        virtual OperationResult can_delete(const ModelDefinition& model_definition, api::AccessTokenContext& token, int id) =
        0;
        virtual OperationResult can_list(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                         string_map& filter) = 0;
        bool restart_scheduled = false;
        bool shutdown_scheduled = false;
    };

    typedef std::shared_ptr<mindnet::api::IService> ServicePtr;
}

#endif //MIND_NET_ISERVICE_H
