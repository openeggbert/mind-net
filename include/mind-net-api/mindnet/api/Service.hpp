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
                                               entity_fields& fields, int stack_depth) override;
        std::pair<entity_fields, OperationResult>
        read(const ModelDefinition& def, api::AccessTokenContext& token, i64 id, int stack_depth) override;
        OperationResult update(const ModelDefinition& def, api::AccessTokenContext& token, i64 id,
                               entity_fields& fields, int stack_depth) override;
        OperationResult remove(const ModelDefinition& def, api::AccessTokenContext& token, i64 id, int stack_depth) override;
        std::pair<std::vector<entity_fields>, OperationResult> list(const ModelDefinition& def,
                                                                    api::AccessTokenContext& token,
                                                                    orm::QueryParams& query_params,
                                                                    int stack_depth) override;

        std::optional<ModelDefinition> get_model_definition(const string& model_name) override;

        entity_fields request_to_entity_fields(
            crow::json::rvalue& body, mindnet::essential::Crudl crudl, ModelDefinition& def
        ) override;
        const api::PluginRegistryPtr get_plugin_registry() const override;
        void stop_service() override;
    private:
        std::shared_ptr<IValidator> get_validator(const std::string& name);
        OperationResult can_create(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                   entity_fields& ef) override;
        OperationResult can_read(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                 i64 id) override;
        OperationResult can_update(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                   entity_fields& ef, entity_fields& old_fields) override;
        OperationResult can_delete(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                   i64 id) override;
        OperationResult can_list(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                 string_map& filter) override;

        //
    };
}
