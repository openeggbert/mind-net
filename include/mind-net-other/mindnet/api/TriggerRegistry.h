//
// Created by robertvokac on 9/7/25.
//
#ifndef TRIGGERREGISTRY_H
#define TRIGGERREGISTRY_H


#include <memory>
#include <string>
#include <unordered_map>

#include "mindnet/api/OperationResult.h"
#include "TriggerPhase.h"
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
            mindnet::core::Crudl op,
            TriggerPtr& trigger);

        void execute(
            TriggerPhase phase,
            const mindnet::core::Crudl& operation,
            int stack_depth,
            OperationResult& validation_result,
            OperationResult& action_result,
            const mindnet::model::ModelDefinition& def,
            int user_id,
            int id,
            const entity_fields& fields = empty_entity_fields,
            const http::QueryParams& query_params = empty_query_params);

    private:
        using CrudLMap = std::unordered_map<mindnet::core::Crudl, std::vector<TriggerPtr>>;
        using PhaseMap = std::unordered_map<TriggerPhase, CrudLMap>;
        using TableMap = std::unordered_map<std::string, PhaseMap>;

        TableMap registry_;
    };
    typedef std::shared_ptr<TriggerRegistry> TriggerRegistryPtr;
}

#endif // TRIGGERREGISTRY_H
