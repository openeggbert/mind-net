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
//

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "OperationResult.hpp"
#include "TriggerPhase.hpp"
#include "hive/essential/Crudl.hpp"
#include <vector>
#include "hive/orm/QueryParams.hpp"
#include "hive/model/ModelDefinition.hpp"
#include "Trigger.hpp"

namespace hive::api
{
    using CrudLMap = std::unordered_map<hive::essential::Crudl, std::vector<TriggerPtr>>;
    using PhaseMap = std::unordered_map<TriggerPhase, CrudLMap>;
    using TableMap = std::unordered_map<std::string, PhaseMap>;

    static entity_fields empty_entity_fields;
    const orm::QueryParams empty_query_params;

    class TriggerRegistry
    {
    public:
        void register_trigger(
            const std::string& table,
            TriggerPhase phase,
            hive::essential::Crudl op,
            TriggerPtr& trigger);

        void execute_before_or_after(
            TriggerPhase phase,
            const hive::essential::Crudl& operation,
            int stack_depth,
            OperationResult& validation_result,
            OperationResult& action_result,
            const hive::model::ModelDefinition& def,
            identification user_id,
            identification id,
            entity_fields& fields = empty_entity_fields,
            entity_fields& old_fields = empty_entity_fields,
            const orm::QueryParams& query_params = empty_query_params);

        std::optional<std::pair<i64, OperationResult>> execute_instead_of_create(
            int stack_depth,
            OperationResult& validation_result,
            const model::ModelDefinition& def,
            identification user_id,
            identification id,
            entity_fields& fields);

        std::optional<std::pair<entity_fields, OperationResult>> execute_instead_of_read(
            int stack_depth,
            OperationResult& validation_result,
            const model::ModelDefinition& def,
            identification user_id,
            identification id);

        std::optional<OperationResult> execute_instead_of_update(
            int stack_depth,
            OperationResult& validation_result,
            const model::ModelDefinition& def,
            identification user_id,
            identification id,
            entity_fields& fields,
            entity_fields& old_fields);

        std::optional<OperationResult> execute_instead_of_delete(
            int stack_depth,
            OperationResult& validation_result,
            const model::ModelDefinition& def,
            identification user_id,
            identification id,
            entity_fields& fields);
        std::optional<std::pair<std::vector<entity_fields>, OperationResult>> execute_instead_of_list(
            int stack_depth,
            OperationResult& validation_result,
            const model::ModelDefinition& def,
            identification user_id,
            const orm::QueryParams& query_params);

    private:
        TableMap registry_;
    };

    typedef std::shared_ptr<TriggerRegistry> TriggerRegistryPtr;
}