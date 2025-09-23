//
// Created by robertvokac on 9/7/25.
//
#ifndef HISTORYCOMMONTRIGGER_H
#define HISTORYCOMMONTRIGGER_H
#include "../../../../../mind-net-api/mindnet/api/Trigger.h"

namespace mindnet::plugins::core::triggers
{
    class HistoryCommonTrigger : public api::Trigger
    {
    public:
        HistoryCommonTrigger();

        ~HistoryCommonTrigger() override = default;
        void run(
            mindnet::essential::Crudl operation,
            int stack_depth,
            api::OperationResult& validation_result,
            api::OperationResult& action_result,
            const model::ModelDefinition def,
            int user_id,
            int id,
            const entity_fields fields,
            const orm::QueryParams query_params
        );

    };

}

#endif // HISTORYCOMMONTRIGGER_H
