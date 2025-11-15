//
// Created by robertvokac on 9/7/25.
//
#ifndef AFTERUPDATECONTENTSEMANTICVERSIONTRIGGER_H
#define AFTERUPDATECONTENTSEMANTICVERSIONTRIGGER_H


#include "mindnet/api/Trigger.hpp"

namespace mindnet::plugins::repetition::triggers
{
    class AfterUpdateContentSemanticVersionTrigger : public api::Trigger
    {
    public:
        AfterUpdateContentSemanticVersionTrigger();

        ~AfterUpdateContentSemanticVersionTrigger() override = default;
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
#endif // AFTERUPDATECONTENTSEMANTICVERSIONTRIGGER_H
