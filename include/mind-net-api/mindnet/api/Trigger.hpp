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

#include <memory>
#include <vector>
#include "TriggerPhase.hpp"
#include "mindnet/essential/Crudl.hpp"
#include "mindnet/orm/QueryParams.hpp"
#include "mindnet/model/ModelDefinition.hpp"
#include <nlohmann/json.hpp>
#include "mindnet/api/AbstractTriggerJob.hpp"


namespace mindnet::api
{
    class Trigger : public AbstractTriggerJob
    {
    public:

        Trigger(
            const std::string& name_,
            const std::string& description_,
            int priority_,
            const std::vector<mindnet::essential::Crudl>& operations_,
            TriggerPhase phase_,
            const std::string& table_)
            : AbstractTriggerJob(name_, description_),
              priority(priority_),
              operations(std::move(operations_)),
              phase(phase_),
              table(table_)
        {
        }

        virtual ~Trigger() = default;

        virtual void run_before_or_after(
            mindnet::essential::Crudl operation,
            int stack_depth,
            OperationResult& validation_result,
            OperationResult& action_result,
            model::ModelDefinition def,
            i64 user_id,
            i64 id,
            entity_fields& fields,
            entity_fields& old_fields,
            orm::QueryParams query_params
        );

        std::optional<std::pair<int, OperationResult>> run_instead_of_create(
    int stack_depth,
    OperationResult& validation_result,
    const model::ModelDefinition& def,
    i64 user_id,
    i64 id,
    entity_fields& fields);

        virtual std::optional<std::pair<entity_fields, OperationResult>> run_instead_of_read(
            int stack_depth,
            OperationResult& validation_result,
            const model::ModelDefinition& def,
            i64 user_id,
            i64 id);

        virtual std::optional<OperationResult> run_instead_of_update(
            int stack_depth,
            OperationResult& validation_result,
            const model::ModelDefinition& def,
            i64 user_id,
            i64 id,
            entity_fields& fields,
            entity_fields& old_fields);

        virtual std::optional<OperationResult> run_instead_of_delete(
            int stack_depth,
            OperationResult& validation_result,
            const model::ModelDefinition& def,
            i64 user_id,
            i64 id);
        virtual std::optional<std::pair<std::vector<entity_fields>, OperationResult>> run_instead_of_list(
            int stack_depth,
            OperationResult& validation_result,
            const model::ModelDefinition& def,
            i64 user_id,
            const orm::QueryParams& query_params);

        //
        inline const int get_priority() const { return priority; }
        //
        inline const std::vector<mindnet::essential::Crudl>& get_operations() const { return operations; }
        inline const TriggerPhase& get_phase() const { return phase; }
        inline const std::string& get_table() const { return table; }
        nlohmann::json call_query(const std::string& query_name, nlohmann::json& request);

    private:
        //std::string condition;
        int priority{};
        //
        std::vector<mindnet::essential::Crudl> operations;
        TriggerPhase phase = TriggerPhase::Before;
        std::string table;
    };
    typedef std::shared_ptr<Trigger> TriggerPtr;
}

