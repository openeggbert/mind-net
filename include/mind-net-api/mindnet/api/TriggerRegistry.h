//
// Created by robertvokac on 9/7/25.
//
#ifndef TRIGGERREGISTRY_H
#define TRIGGERREGISTRY_H


#include <memory>
#include <string>
#include <unordered_map>

#include "OperationResult.h"
#include "TriggerPhase.h"
#include "mindnet/essential/Crudl.h"
#include <vector>
#include "mindnet/orm/QueryParams.h"
#include "mindnet/model/ModelDefinition.h"
#include "Trigger.h"

namespace mindnet::api
{
    static entity_fields empty_entity_fields;
    const orm::QueryParams empty_query_params;

    class TriggerRegistry
    {
    public:
        void register_trigger(
            const std::string& table,
            TriggerPhase phase,
            mindnet::essential::Crudl op,
            TriggerPtr& trigger);

        void execute(
            TriggerPhase phase,
            const mindnet::essential::Crudl& operation,
            int stack_depth,
            OperationResult& validation_result,
            OperationResult& action_result,
            const mindnet::model::ModelDefinition& def,
            int user_id,
            int id,
            entity_fields& fields = empty_entity_fields,
            entity_fields& old_fields = empty_entity_fields,
            const orm::QueryParams& query_params = empty_query_params);

    private:
        using CrudLMap = std::unordered_map<mindnet::essential::Crudl, std::vector<TriggerPtr>>;
        using PhaseMap = std::unordered_map<TriggerPhase, CrudLMap>;
        using TableMap = std::unordered_map<std::string, PhaseMap>;

        TableMap registry_;
    };

    typedef std::shared_ptr<TriggerRegistry> TriggerRegistryPtr;
}

#endif // TRIGGERREGISTRY_H
