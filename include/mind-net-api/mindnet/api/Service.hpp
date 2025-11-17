//
// Created by robertvokac on 9/13/25.
//

#pragma once
#include "IService.hpp"
#include "PluginRegistry.hpp"
#include "TriggerRegistry.hpp"
#include "cronq/CronScheduler.hpp"

namespace mindnet::api
{
    using validator = api::IValidator*;
    using mindnet::api::OperationResult;
    using model::ModelDefinition;

    class InvalidateMethodImpl: public InvalidateMethod
    {
    public:
        InvalidateMethodImpl(const DbPtr& db) : db_(db)
        {

        }
        DbPtr db_;
        void invalidate(const model::ModelDefinition& def, i64 id) {
            db_->invalidate(def, id);
        };
    };

    class Service : public api::IService
    {
    private:
        std::map<std::string, std::shared_ptr<IValidator>> validators;
        api::DbPtr db_ptr;
        api::PluginRegistryPtr plugin_registry_ptr;
        api::TriggerRegistryPtr trigger_registry_ptr;
        std::map<string, QueryPtr> query_map;
        std::map<string, JobPtr> job_map;
        cronq::CronSchedulerPtr cron_scheduler;
        InvalidateMethodImpl invalidate_method;

    public:
        Service(const api::DbPtr& db_ptr, const api::PluginRegistryPtr& plugin_registry);
        ~Service() override;

        bool has_model(const std::string& model_name) override;

        std::vector<std::string>& list_model_names() override;

        nlohmann::json call_query(const std::string& query_name, nlohmann::json& request) override;

        std::pair<i64, OperationResult> create(const ModelDefinition& def, api::AccessTokenContext& token,
                                               entity_fields& fields, int stack_depth = 0);
        std::pair<entity_fields, OperationResult>
        read(const ModelDefinition& def, api::AccessTokenContext& token, i64 id, int stack_depth = 0);
        OperationResult update(const ModelDefinition& def, api::AccessTokenContext& token, i64 id,
                               entity_fields& fields, int stack_depth = 0);
        OperationResult remove(const ModelDefinition& def, api::AccessTokenContext& token, i64 id, int stack_depth = 0);
        std::pair<std::vector<entity_fields>, OperationResult> list(const ModelDefinition& def,
                                                                    api::AccessTokenContext& token,
                                                                    orm::QueryParams& query_params,
                                                                    int stack_depth = 0);

        std::optional<ModelDefinition> get_model_definition(const string& model_name) override;

        entity_fields request_to_entity_fields(
            crow::json::rvalue& body, mindnet::essential::Crudl crudl, ModelDefinition& def
        ) override;
        const api::PluginRegistryPtr get_plugin_registry() const override;
        void stop_service();
    private:
        std::shared_ptr<IValidator> get_validator(const std::string& name);
        OperationResult can_create(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                   entity_fields& ef) override;
        OperationResult can_read(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                 int id) override;
        OperationResult can_update(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                   entity_fields& ef, entity_fields& old_fields) override;
        OperationResult can_delete(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                   int id) override;
        OperationResult can_list(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                 string_map& filter) override;

        //
    };
}
