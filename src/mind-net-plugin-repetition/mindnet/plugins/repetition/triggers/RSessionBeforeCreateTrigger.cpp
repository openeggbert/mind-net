//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/plugins/repetition/triggers/RSessionBeforeCreateTrigger.h"

#include "mindnet/essential/Global.h"
#include "mindnet/api/AccessTokenContext.h"
#include "mindnet/plugins/repetition/models/RSession.h"
#include "mindnet/util/Utils.h"
#include "../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/GetRSessionSelectedItemsSQLiteQuery.h"

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

    std::vector<int> find_repetition_ids(
        std::vector<int>& ids,
        int stack_depth,
        int user_id,
        models::RSession& r_session,
        int note_id)
    {
        return ids;
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

        if (r_session.schedule == enums::RepetitionSchedule::DepthFirstShuffled)
        {
            validation_result = {400, "Unsupported schedule: DepthFirstShuffled"};
            return;
        }

        if (r_session.schedule == enums::RepetitionSchedule::Interleaved)
        {
            validation_result = {400, "Unsupported schedule: Interleaved"};
            return;
        }

        auto token = api::AccessTokenContext(user_id, "", 200);;
        std::vector<int> ids;

        switch (r_session.algorithm)
        {
        case enums::RepetitionAlgorithm::Repetition0:
            {
                find_repetition_ids(ids, stack_depth, user_id, r_session, r_session.filter_under_note == 0);
            }
            break;
        case enums::RepetitionAlgorithm::Repetition2: break;
        case enums::RepetitionAlgorithm::Repetition4: break;
        case enums::RepetitionAlgorithm::Repetition18: break;
        default:
            {
                validation_result = {400, "Unsupported algorithm."};
                return;
            }
        }
        nlohmann::json req;

        r_session.selected_items = call_query(db::sqlite::queries::QUERY_GetRSessionSelectedItemsQuery, req).dump();
        fields = r_session.to_values();
    }
}
