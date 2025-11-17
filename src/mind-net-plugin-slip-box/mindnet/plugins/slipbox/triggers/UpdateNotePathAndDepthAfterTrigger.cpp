//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/plugins/slipbox/triggers/UpdateNotePathAndDepthAfterTrigger.hpp"

#include "../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/UpdateNotePathAndDepthSQLiteQuery.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/plugins/slipbox/models/Note.hpp"
#include "mindnet/util/Utils.hpp"

namespace mindnet::plugins::slipbox::triggers
{
    using_loggers()

    UpdateNotePathAndDepthAfterTrigger::UpdateNotePathAndDepthAfterTrigger()
        : Trigger(
            "NoteUpdateParentBeforeTrigger",
            "Updates note path and depth of all descendants.",
            1000,
            {essential::Crudl::Update},
            api::TriggerPhase::After,
            "note"
        )
    {
    }

    void UpdateNotePathAndDepthAfterTrigger::run_before_or_after(
        mindnet::essential::Crudl operation,
        int stack_depth,
        api::OperationResult& validation_result,
        api::OperationResult& action_result,
        const mindnet::model::ModelDefinition def,
        i64 user_id,
        i64 id,
        entity_fields& fields,
        entity_fields& old_fields,
        const orm::QueryParams query_params)
    {
        if (action_result.ko())
        {
            return;
        }
        models::Note new_note;
        models::Note old_note;
        new_note.from_values(fields);
        old_note.from_values(old_fields);
        if (old_note.parent_note_id == new_note.parent_note_id)
        {
            //Nothing to do.
            return;
        }

        nlohmann::json req;
        req["note_id"] = cast64(new_note.get_id());
        nlohmann::json res;
        try
        {
            res = call_query(db::sqlite::queries::QUERY_UpdateNotePathAndDepth, req);
            info << "Query UpdateNotePathAndDepth successful" << commit;
        }
        catch (std::string& e)
        {
            err << "Query UpdateNotePathAndDepth failed" << commit;
        }
    }
}
