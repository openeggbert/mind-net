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
        const entity_fields& fields,
        const orm::QueryParams& query_params)
    {

        std::vector<std::string> v{def.get_model_name(), "*"};
        for (auto& s : v)
        {
            auto tableIt = registry_.find(s);
            if (tableIt == registry_.end()) continue;

            auto phaseIt = tableIt->second.find(phase);
            if (phaseIt == tableIt->second.end()) return;

            auto crudIt = phaseIt->second.find(operation);
            if (crudIt == phaseIt->second.end()) return;

            for (auto& trigger : crudIt->second)
            {
                trigger->run(operation, ++stack_depth, validation_result, action_result,
                             def, user_id, id, fields, query_params);
            }
        }
    }
}
