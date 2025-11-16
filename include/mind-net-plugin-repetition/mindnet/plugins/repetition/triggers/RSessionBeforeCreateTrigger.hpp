//
// Created by robertvokac on 9/7/25.
//
#pragma once


#include "mindnet/api/Trigger.hpp"

namespace mindnet::plugins::repetition::triggers
{
    class RSessionBeforeCreateTrigger : public api::Trigger
    {
    public:
        RSessionBeforeCreateTrigger();

        ~RSessionBeforeCreateTrigger() override = default;
        void run_before_or_after(
            mindnet::essential::Crudl operation,
            int stack_depth,
            api::OperationResult& validation_result,
            api::OperationResult& action_result,
            const model::ModelDefinition def,
            int user_id,
            int id,
            entity_fields& fields,
            entity_fields& old_fields,
            const orm::QueryParams query_params
        );
    };
}

