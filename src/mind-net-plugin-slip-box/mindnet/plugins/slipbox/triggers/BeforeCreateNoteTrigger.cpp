//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/plugins/slipbox/triggers/BeforeCreateNoteTrigger.hpp"

#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/slipbox/models/Link.hpp"
#include "mindnet/plugins/slipbox/models/Note.hpp"
#include "../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindNextSiblingOrderSQLiteQuery.hpp"


namespace mindnet::plugins::slipbox::triggers
{
    using_loggers()

    BeforeCreateNoteTrigger::BeforeCreateNoteTrigger()
        : Trigger(
            "BeforeCreateNoteTrigger",
            "Set the value of the sibling order.",
            1000,
            {essential::Crudl::Create},
            api::TriggerPhase::Before,
            "note"
        )
    {
    }


    void BeforeCreateNoteTrigger::run_before_or_after(
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
        if (validation_result.ko())
        {
            return;
        }
        models::Note new_note;
        new_note.from_values(fields);

        i64 map_id = new_note.map_id;
        i64 parent_note_id = new_note.parent_note_id;


        nlohmann::json req;
        req["map_id"] = map_id;
        req["parent_note_id"] = parent_note_id;
        req["note_id"] = id;
        nlohmann::json res;
        try
        {
            res = call_query(db::sqlite::queries::QUERY_FindNextSiblingOrder, req);
            i64 next_sibling_order = res["next_sibling_order"];
            new_note.sibling_order = next_sibling_order;
            auto v = new_note.to_values();
            fields = v;
            info << "Query FindNextSiblingOrder successful" << commit;
        }
        catch (std::exception& e)
        {
            err << "Query UpdateNotePathAndDepth failed" << commit;
        }
    }
}
