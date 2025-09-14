//
// Created by robertvokac on 9/7/25.
//
#ifndef HISTORYCOMMONTRIGGER_H
#define HISTORYCOMMONTRIGGER_H
#include "mindnet/api/Trigger.h"

namespace mindnet::plugins::core::triggers
{
    class HistoryCommonTrigger : public api::Trigger
    {
    public:
        HistoryCommonTrigger();

        ~HistoryCommonTrigger() = default;
        void run(
            plugins::core::enums::Crudl operation,
            const OperationResult& validation_result,
            const OperationResult& action_result,
            const model::ModelDefinition def,
            int user_id,
            int id,
            const entity_fields fields,
            const http::QueryParams query_params
        );

    };

}

#endif // HISTORYCOMMONTRIGGER_H
