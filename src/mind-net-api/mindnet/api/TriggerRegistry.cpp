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

    void TriggerRegistry::execute(
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
        std::vector<TriggerPtr> runlist;
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
            runlist.insert(runlist.end(), vec.begin(), vec.end());
        }

        std::stable_sort(runlist.begin(), runlist.end(),
    [](const TriggerPtr& a, const TriggerPtr& b){
        return a->get_priority() > b->get_priority();
    });

        for (auto& trigger : runlist)
        {
            trigger->run(operation, stack_depth + 1, validation_result, action_result,
                         def, user_id, id, fields, old_fields, query_params);
        }
    }
}
