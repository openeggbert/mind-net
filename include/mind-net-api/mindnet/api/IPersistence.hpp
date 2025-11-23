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


#include "IRepository.hpp"
#include "OperationResult.hpp"
#include "crow/json.h"
#include "AccessTokenContext.hpp"
#include "mindnet/model/ModelDefinition.hpp"


namespace mindnet::api
{
    using mindnet::model::ModelDefinition;

    class IPersistence
    {
    public:
        IPersistence() = default;
        virtual ~IPersistence() = default; // explicitly make it destructible

        virtual bool has_model_with_name(const std::string& name) = 0;

        virtual std::vector<std::string>& list_model_names() = 0;
        //
        virtual std::pair<i64, OperationResult> create(const ModelDefinition& def, AccessTokenContext& token,
                                                       entity_fields& fields) = 0;
        virtual std::pair<entity_fields, OperationResult> read(const ModelDefinition& def, AccessTokenContext& token,
                                                               i64 id) = 0;
        virtual void invalidate(const model::ModelDefinition& def, i64 id) = 0;
        virtual OperationResult update(const ModelDefinition& def, AccessTokenContext& token, i64 id,
                                       entity_fields& fields) = 0;
        virtual OperationResult remove(const ModelDefinition& def, AccessTokenContext& token, i64 id) = 0;
        virtual std::pair<std::vector<entity_fields>, OperationResult> list(
            const ModelDefinition& def, AccessTokenContext& token, orm::QueryParams& query_params) = 0;
        //
        virtual std::optional<ModelDefinition> get_model_definition(const string& model_name) = 0;
        //
        virtual entity_fields request_to_entity_fields(crow::json::rvalue& body, mindnet::essential::Crudl crudl,
                                                       ModelDefinition& def) = 0;
    };

    typedef std::shared_ptr<mindnet::api::IPersistence> DbPtr;
}
