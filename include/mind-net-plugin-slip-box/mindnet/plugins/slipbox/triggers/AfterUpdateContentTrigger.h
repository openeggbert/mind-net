//
// Created by robertvokac on 9/7/25.
//
#ifndef AFTERUPDATECONTENTTRIGGER_H
#define AFTERUPDATECONTENTTRIGGER_H


#include "mindnet/api/Trigger.h"

namespace mindnet::plugins::slipbox::triggers
{
    class AfterUpdateContentTrigger : public api::Trigger
    {
    public:
        AfterUpdateContentTrigger();

        ~AfterUpdateContentTrigger() override = default;
        void run(
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
#endif // AFTERUPDATECONTENTTRIGGER_H
