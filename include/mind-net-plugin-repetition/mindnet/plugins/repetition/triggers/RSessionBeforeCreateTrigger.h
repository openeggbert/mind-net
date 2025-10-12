//
// Created by robertvokac on 9/7/25.
//
#ifndef RSESSIONBEFORECREATETRIGGER_H
#define RSESSIONBEFORECREATETRIGGER_H

#include "mindnet/api/Trigger.h"
#include "mindnet/plugins/repetition/enums/RepetitionEntity.h"

namespace mindnet::plugins::repetition::triggers
{
    struct RepetitionId
    {
        enums::RepetitionEntity entity;
        int64_t id;
    };
    class RSessionBeforeCreateTrigger : public api::Trigger
    {
    public:
        RSessionBeforeCreateTrigger();

        ~RSessionBeforeCreateTrigger() override = default;
        void run(
            mindnet::essential::Crudl operation,
            int stack_depth,
            api::OperationResult& validation_result,
            api::OperationResult& action_result,
            const model::ModelDefinition def,
            int user_id,
            int id,
            entity_fields& fields,
            const orm::QueryParams query_params
        );

    };

}

#endif // RSESSIONBEFORECREATETRIGGER_H
