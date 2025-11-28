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

#include "crow/json.h"
#include "mindnet/orm/QueryParams.hpp"
#include "mindnet/model/ModelDefinition.hpp"
#include "IValidator.hpp"
#include "IPersistence.hpp"
#include "PluginRegistry.hpp"

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
        virtual std::pair<identification, OperationResult> create(const ModelDefinition& def, api::AccessTokenContext& token,
                                                       entity_fields& fields, int stack_depth) = 0;
        virtual std::pair<entity_fields, OperationResult> read(const ModelDefinition& def,
                                                               api::AccessTokenContext& token,
                                                               identification id, int stack_depth) = 0;
        virtual OperationResult update(const ModelDefinition& def, api::AccessTokenContext& token, identification id,
                                       entity_fields& fields, int stack_depth) = 0;
        virtual OperationResult remove(const ModelDefinition& def, api::AccessTokenContext& token, identification id,
                                       int stack_depth) = 0;
        virtual std::pair<std::vector<entity_fields>, OperationResult> list(
            const ModelDefinition& def, api::AccessTokenContext& token, orm::QueryParams& query_params,
            int stack_depth) = 0;

        std::pair<identification, OperationResult> count(
            const ModelDefinition& def, api::AccessTokenContext& token, orm::QueryParams& query_params,
            int stack_depth)
        {
            auto result = list(def, token, query_params, stack_depth);
            return {result.second.ko() ? 0 : result.first.size(), result.second};
        }

        std::pair<bool, OperationResult> exists(
            const ModelDefinition& def, api::AccessTokenContext& token, orm::QueryParams& query_params,
            int stack_depth)
        {
            auto result = count(def, token, query_params, stack_depth);
            return {result.second.ko() ? false : result.first > 0, result.second};
        }

        std::pair<bool, OperationResult> empty(
            const ModelDefinition& def, api::AccessTokenContext& token, orm::QueryParams& query_params,
            int stack_depth)
        {
            auto result = count(def, token, query_params, stack_depth);
            return {result.second.ko() ? false : result.first == 0, result.second};
        }

        //
        virtual std::optional<ModelDefinition> get_model_definition(const string& model_name) = 0;
        //
        virtual entity_fields request_to_entity_fields(
            crow::json::rvalue& body,
            mindnet::essential::Crudl crudl,
            ModelDefinition& def
        ) = 0;
        virtual const api::PluginRegistryPtr get_plugin_registry() const = 0;
        bool is_restart_scheduled() const { return restart_scheduled; };
        bool is_shutdown_scheduled() const { return shutdown_scheduled; };

        void schedule_restart() { restart_scheduled = true; };
        void schedule_shutdown() { shutdown_scheduled = true; };
        virtual void stop_service() = 0;
    private:
        virtual OperationResult can_create(
            const model::ModelDefinition& model_definition,
            api::AccessTokenContext& token, entity_fields& ef) = 0;
        virtual OperationResult can_read(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                         identification id) = 0;
        virtual OperationResult can_update(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                           entity_fields& ef, entity_fields& old_fields) = 0;
        virtual OperationResult can_delete(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                           identification id) =
        0;
        virtual OperationResult can_list(const ModelDefinition& model_definition, api::AccessTokenContext& token,
                                         string_map& filter) = 0;
        bool restart_scheduled = false;
        bool shutdown_scheduled = false;
    };

    typedef std::shared_ptr<mindnet::api::IService> ServicePtr;
}

