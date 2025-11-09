//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/api/TriggerRegistry.h"
#include <iostream>

namespace mindnet::api
{
    void TriggerRegistry::register_trigger(
        const std::string& table,
        TriggerPhase phase,
        mindnet::essential::Crudl op,
        TriggerPtr& trigger)
    {
        registry_[table][phase][op].push_back(trigger);
    }

    void fill_run_list(
    TriggerPhase phase,
const mindnet::essential::Crudl& operation,
const mindnet::model::ModelDefinition& def,
        std::vector<TriggerPtr>& run_list,
        TableMap& registry_

        )
    {
        std::vector<std::string> tables{def.get_model_name(), "*"};
        for (auto& table : tables)
        {
            auto tableIt = registry_.find(table);
            if (tableIt == registry_.end()) continue;

            auto phaseIt = tableIt->second.find(phase);
            if (phaseIt == tableIt->second.end()) continue;

            auto crudIt = phaseIt->second.find(operation);
            if (crudIt == phaseIt->second.end()) continue;

            const auto& vec = crudIt->second;
            run_list.insert(run_list.end(), vec.begin(), vec.end());
        }

        std::stable_sort(run_list.begin(), run_list.end(),
    [](const TriggerPtr& a, const TriggerPtr& b){
        return a->get_priority() > b->get_priority();
    });

    }
    void TriggerRegistry::execute_before_or_after(
        TriggerPhase phase,
        const mindnet::essential::Crudl& operation,
        int stack_depth,
        OperationResult& validation_result,
        OperationResult& action_result,
        const mindnet::model::ModelDefinition& def,
        int user_id,
        int id,
        entity_fields& fields,
        entity_fields& old_fields,
        const orm::QueryParams& query_params)
    {
        std::vector<TriggerPtr> run_list;
        fill_run_list(phase, operation, def, run_list, registry_);

        for (auto& trigger : run_list)
        {
            trigger->run_before_or_after(operation, stack_depth + 1, validation_result, action_result,
                         def, user_id, id, fields, old_fields, query_params);
        }
    }

    std::optional<std::pair<int, OperationResult>> TriggerRegistry::execute_instead_of_create(int stack_depth,
        OperationResult& validation_result, const model::ModelDefinition& def, int user_id, int id,
        entity_fields& fields)
    {
        auto phase= TriggerPhase::InsteadOf;
        auto operation = essential::Crudl::Create;

        std::vector<TriggerPtr> run_list;
        fill_run_list(phase, operation, def, run_list, registry_);
        if (run_list.empty()) return std::nullopt;

        return run_list[0]->run_instead_of_create(stack_depth + 1, validation_result,
                         def, user_id, id, fields);
    }

    std::optional<std::pair<entity_fields, OperationResult>> TriggerRegistry::execute_instead_of_read(int stack_depth,
        OperationResult& validation_result, const model::ModelDefinition& def, int user_id, int id)
    {
        auto phase= TriggerPhase::InsteadOf;
        auto operation = essential::Crudl::Read;

        std::vector<TriggerPtr> run_list;
        fill_run_list(phase, operation, def, run_list, registry_);
        if (run_list.empty()) return std::nullopt;

        return run_list[0]->run_instead_of_read(stack_depth + 1, validation_result,
                         def, user_id, id);
    }

    std::optional<OperationResult> TriggerRegistry::execute_instead_of_update(int stack_depth,
        OperationResult& validation_result, const model::ModelDefinition& def, int user_id, int id,
        entity_fields& fields, entity_fields& old_fields)
    {
        auto phase= TriggerPhase::InsteadOf;
        auto operation = essential::Crudl::Update;

        std::vector<TriggerPtr> run_list;
        fill_run_list(phase, operation, def, run_list, registry_);
        if (run_list.empty()) return std::nullopt;

        return run_list[0]->run_instead_of_update(stack_depth + 1, validation_result,
                         def, user_id, id, fields, old_fields);
    }

    std::optional<OperationResult> TriggerRegistry::execute_instead_of_delete(int stack_depth,
        OperationResult& validation_result, const model::ModelDefinition& def, int user_id, int id)
    {
        auto phase= TriggerPhase::InsteadOf;
        auto operation = essential::Crudl::Delete;

        std::vector<TriggerPtr> run_list;
        fill_run_list(phase, operation, def, run_list, registry_);
        if (run_list.empty()) return std::nullopt;

        return run_list[0]->run_instead_of_delete(stack_depth + 1, validation_result,
                         def, user_id, id);
    }

    std::optional<std::pair<std::vector<entity_fields>, OperationResult>> TriggerRegistry::execute_instead_of_list(int stack_depth,
        OperationResult& validation_result, const model::ModelDefinition& def, int user_id,
        const orm::QueryParams& query_params)
    {
        auto phase= TriggerPhase::InsteadOf;
        auto operation = essential::Crudl::List;

        std::vector<TriggerPtr> run_list;
        fill_run_list(phase, operation, def, run_list, registry_);
        if (run_list.empty()) return std::nullopt;

        return run_list[0]->run_instead_of_list(stack_depth + 1, validation_result,
                         def, user_id, query_params);
    }
}
