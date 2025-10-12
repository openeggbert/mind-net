//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/plugins/repetition/triggers/RSessionBeforeCreateTrigger.h"

#include "mindnet/essential/Global.h"
#include "mindnet/api/AccessTokenContext.h"
#include "mindnet/plugins/repetition/models/RSession.h"
#include "mindnet/util/Utils.h"

namespace mindnet::plugins::repetition::triggers
{
    using_loggers()
    RSessionBeforeCreateTrigger::RSessionBeforeCreateTrigger()
        : Trigger(
            "RSessionBeforeCreateTrigger",
            "Adds IDS to r_sessions during creation.",
            1000,
            {essential::Crudl::Create},
            api::TriggerPhase::Before,
            "r_session"
        )
    {
    }

    void RSessionBeforeCreateTrigger::run(
        mindnet::essential::Crudl operation,
        int stack_depth,
        api::OperationResult& validation_result,
        api::OperationResult& action_result,
        const mindnet::model::ModelDefinition def,
        int user_id,
        int id,
        entity_fields& fields,
        const orm::QueryParams query_params)
    {

        models::RSession r_session;
        r_session.from_values(fields);
        r_session.selected_items = "{\"hello\": \"trigger\"}";
        fields = r_session.to_values();
    }
}
