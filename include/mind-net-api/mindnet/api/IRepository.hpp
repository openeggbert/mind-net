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

#include <string>

#include "crow/json.h"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/model/ModelDefinition.hpp"
#include "mindnet/orm/QueryParams.hpp"

namespace mindnet::api
{
    using std::string;

    class IRepository
    {
    public:
        virtual ~IRepository() = default;
        IRepository(
            const model::ModelDefinition& model_definition
        );
        virtual identification create(const entity_fields& fields, string& error) = 0;
        virtual entity_fields read(identification id, string& error) = 0;
        virtual bool update(identification id, entity_fields& fields, string& error) = 0;
        virtual bool remove(identification id, string& error) = 0;
        virtual std::vector<entity_fields> list(orm::QueryParams& query_params, string& error) = 0;
        virtual std::vector<entity_fields> list_in_ids(std::vector<identification>& ids, string& error) = 0;
        virtual std::vector<identification> list_ids(orm::QueryParams& query_params, string& error) = 0;

        [[nodiscard]] virtual mindnet::model::ModelDefinition& get_model_definition() = 0;
        virtual entity_fields request_to_entity_fields(crow::json::rvalue& body,
                                                       mindnet::essential::Crudl crudl) =
        0;

    protected:
        model::ModelDefinition model_definition;
    };
}

