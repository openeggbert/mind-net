//
// Created by robertvokac on 9/7/25.
//
#pragma once

#include "mindnet/api/Trigger.hpp"

namespace mindnet::plugins::core::triggers
{
    class HistoryCommonTrigger : public api::Trigger
    {
    public:
        HistoryCommonTrigger();

        ~HistoryCommonTrigger() override = default;
        void run_before_or_after(
            mindnet::essential::Crudl operation,
            int stack_depth,
            api::OperationResult& validation_result,
            api::OperationResult& action_result,
            const model::ModelDefinition def,
            i64 user_id,
            i64 id,
            entity_fields& fields,
            entity_fields& old_fields,
            const orm::QueryParams query_params
        );
    };
}

