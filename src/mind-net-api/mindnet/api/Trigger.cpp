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

#include "mindnet/api/Trigger.hpp"
#include "mindnet/api/Service.hpp"

//

namespace mindnet::api
{
    void Trigger::run_before_or_after(mindnet::essential::Crudl operation, int stack_depth,
                                      OperationResult& validation_result, OperationResult& action_result,
                                      model::ModelDefinition def, identification user_id,
                                      identification id, entity_fields& fields, entity_fields& old_fields,
                                      orm::QueryParams query_params)
    {
        throw std::runtime_error("Empty implementation was called: run_before_or_after()");
    }

    std::optional<std::pair<int, OperationResult>> Trigger::run_instead_of_create(int stack_depth,
        OperationResult& validation_result, const model::ModelDefinition& def, identification user_id,
        identification id,
        entity_fields& fields)
    {
        throw std::runtime_error("Empty implementation was called: run_instead_of_create()");
    }

    std::optional<std::pair<entity_fields, OperationResult>> Trigger::run_instead_of_read(int stack_depth,
        OperationResult& validation_result, const model::ModelDefinition& def, identification user_id,
        identification id)
    {
        throw std::runtime_error("Empty implementation was called: run_instead_of_read()");
    }

    std::optional<OperationResult> Trigger::run_instead_of_update(int stack_depth, OperationResult& validation_result,
                                                                  const model::ModelDefinition& def,
                                                                  identification user_id, identification id,
                                                                  entity_fields& fields, entity_fields& old_fields)
    {
        throw std::runtime_error("Empty implementation was called: run_instead_of_update()");
    }

    std::optional<OperationResult> Trigger::run_instead_of_delete(int stack_depth, OperationResult& validation_result,
                                                                  const model::ModelDefinition& def,
                                                                  identification user_id, identification id)
    {
        throw std::runtime_error("Empty implementation was called: run_instead_of_delete()");
    }

    std::optional<std::pair<std::vector<entity_fields>, OperationResult>> Trigger::run_instead_of_list(
        int stack_depth, OperationResult& validation_result,
        const model::ModelDefinition& def, identification user_id, const orm::QueryParams& query_params)
    {
        throw std::runtime_error("Empty implementation was called: run_instead_of_list()");
    }

    nlohmann::json Trigger::call_query(const std::string& query_name, nlohmann::json& request)
    {
        return service_ptr->call_query(query_name, request);
    }
}