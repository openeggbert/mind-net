//
// Created by robertvokac on 9/7/25.
//
#ifndef RREVIEWAFTERCREATETRIGGER_H
#define RREVIEWAFTERCREATETRIGGER_H


#include "mindnet/api/Trigger.h"
#include "mindnet/plugins/repetition/models/RReview.h"

namespace mindnet::plugins::repetition::triggers
{

    class RReviewAfterCreateTrigger : public api::Trigger
    {
    public:
        RReviewAfterCreateTrigger();

        ~RReviewAfterCreateTrigger() override = default;
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

#endif // RREVIEWAFTERCREATETRIGGER_H
