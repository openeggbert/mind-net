//
// Created by robertvokac on 9/7/25.
//
#ifndef TRIGGERREGISTRY_H
#define TRIGGERREGISTRY_H


#include <memory>
#include <string>
#include <unordered_map>

#include "mindnet/OperationResult.h"
#include "mindnet/TriggerPhase.h"
#include "mindnet/plugins/core/enums/Crudl.h"
#include <vector>
#include "mindnet/http/QueryParams.h"
#include "mindnet/model/ModelDefinition.h"
#include "mindnet/api/Trigger.h"

namespace mindnet::api
{
    constexpr entity_fields empty_entity_fields;
    const http::QueryParams empty_query_params;
    class TriggerRegistry
    {
    public:
        void register_trigger(
            const std::string& table,
            TriggerPhase phase,
            plugins::core::enums::Crudl op,
            TriggerPtr& trigger);

        void execute(
            TriggerPhase phase,
            const plugins::core::enums::Crudl& operation,
            int stack_depth,
            const OperationResult& validation_result,
            const OperationResult& action_result,
            const mindnet::model::ModelDefinition& def,
            int user_id,
            int id,
            const entity_fields& fields = empty_entity_fields,
            const http::QueryParams& query_params = empty_query_params);

    private:
        using CrudLMap = std::unordered_map<plugins::core::enums::Crudl, std::vector<TriggerPtr>>;
        using PhaseMap = std::unordered_map<TriggerPhase, CrudLMap>;
        using TableMap = std::unordered_map<std::string, PhaseMap>;

        TableMap registry_;
    };
    typedef std::shared_ptr<TriggerRegistry> TriggerRegistryPtr;
}

#endif // TRIGGERREGISTRY_H
