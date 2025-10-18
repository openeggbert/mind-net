//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/plugins/slipbox/triggers/UpdateNotePathAndDepthAfterTrigger.h"

#include "../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/UpdateNotePathAndDepthSQLiteQuery.h"
#include "mindnet/essential/Global.h"
#include "mindnet/api/AccessTokenContext.h"
#include "mindnet/plugins/slipbox/models/Note.h"
#include "mindnet/util/Utils.h"

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

    void UpdateNotePathAndDepthAfterTrigger::run(
        mindnet::essential::Crudl operation,
        int stack_depth,
        api::OperationResult& validation_result,
        api::OperationResult& action_result,
        const mindnet::model::ModelDefinition def,
        int user_id,
        int id,
        entity_fields& fields,
        entity_fields& old_fields,
        const orm::QueryParams query_params)
    {
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
        } catch (...)
        {
            err << "Query UpdateNotePathAndDepth failed" << commit;
        }
    }
}
