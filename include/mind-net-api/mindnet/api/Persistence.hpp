//
// Created by robertvokac on 8/6/25.
//
#pragma once

#include "IPersistence.hpp"
#include "IRepository.hpp"
#include "PluginRegistry.hpp"
#include "crow/json.h"
#include "AccessTokenContext.hpp"
#include "ModelCache.hpp"
#include "mindnet/model/ModelDefinition.hpp"
#include "OperationResult.hpp"
#include "ShardedModelCache.hpp"

namespace mindnet::api
{
    using mindnet::model::ModelDefinition;
    using mindnet::api::OperationResult;

    class Persistence : public api::IPersistence
    {
    private:
        std::map<std::string, std::shared_ptr<IRepository>> repositories;
        std::vector<std::string> repository_names;
        std::shared_ptr<IRepository> get_repository(const std::string& name);
        ModelCache model_cache_;

    public:
        Persistence(PluginRegistryPtr& get_plugin_registry);
        ~Persistence() override;

        bool has_model_with_name(const std::string& name) override;

        std::vector<std::string>& list_model_names() override;

        std::pair<int, OperationResult> create(const ModelDefinition& def, api::AccessTokenContext& token,
                                               entity_fields& fields) override;
        std::pair<entity_fields, OperationResult>
        read(const ModelDefinition& def, api::AccessTokenContext& token, i64 id) override;
        void invalidate(const model::ModelDefinition& def, i64 id);
        OperationResult update(const ModelDefinition& def, api::AccessTokenContext& token, i64 id,
                               entity_fields& fields) override;
        OperationResult remove(const ModelDefinition& def, api::AccessTokenContext& token, i64 id) override;
        std::pair<std::vector<entity_fields>, OperationResult> list(const ModelDefinition& def,
                                                                    api::AccessTokenContext& token,
                                                                    orm::QueryParams& query_params) override;

        std::optional<ModelDefinition> get_model_definition(const string& model_name) override;

        entity_fields request_to_entity_fields(crow::json::rvalue& body, mindnet::essential::Crudl crudl,
                                               ModelDefinition& def) override;
    };
}

