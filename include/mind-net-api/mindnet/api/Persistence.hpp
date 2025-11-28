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

        std::pair<identification, OperationResult> create(const ModelDefinition& def, api::AccessTokenContext& token,
                                                          entity_fields& fields) override;
        std::pair<entity_fields, OperationResult>
        read(const ModelDefinition& def, api::AccessTokenContext& token, identification id) override;
        void invalidate(const model::ModelDefinition& def, identification id) override;
        OperationResult update(const ModelDefinition& def, api::AccessTokenContext& token, identification id,
                               entity_fields& fields) override;
        OperationResult remove(const ModelDefinition& def, api::AccessTokenContext& token, identification id) override;
        std::pair<std::vector<entity_fields>, OperationResult> list(const ModelDefinition& def,
                                                                    api::AccessTokenContext& token,
                                                                    orm::QueryParams& query_params) override;

        std::optional<ModelDefinition> get_model_definition(const string& model_name) override;

        entity_fields request_to_entity_fields(crow::json::rvalue& body, mindnet::essential::Crudl crudl,
                                               ModelDefinition& def) override;
    };
}