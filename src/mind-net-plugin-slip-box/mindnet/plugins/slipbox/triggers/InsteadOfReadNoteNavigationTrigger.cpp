//
// Created by robertvokac on 9/7/25.
//

#include "mindnet/plugins/slipbox/triggers/InsteadOfReadNoteNavigationTrigger.h"
#include "mindnet/essential/Global.h"
#include "mindnet/api/AccessTokenContext.h"
#include <string>
#include <vector>
#include "../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindPreviousAndNextNoteSQLiteQuery.h"

#include "mindnet/util/Utils.h"

namespace mindnet::plugins::slipbox::triggers
{
    using_loggers()

    InsteadOfReadNoteNavigationTrigger::InsteadOfReadNoteNavigationTrigger()
        : Trigger(
            "InsteadOfReadNoteNavigationTrigger",
            "Calls custom sql for read note_navigation request",
            1000,
            {essential::Crudl::Read},
            api::TriggerPhase::InsteadOf,
            "note_navigation"
        )
    {
    }

    std::optional<std::pair<entity_fields, api::OperationResult>> InsteadOfReadNoteNavigationTrigger::
    run_instead_of_read(int stack_depth, api::OperationResult& validation_result, const model::ModelDefinition& def,
                        int user_id, int id)
    {
        nlohmann::json req;
        req["note_id"] = cast64(id);
        nlohmann::json res;
        i64 prev_note_id{};
        i64 next_note_id{};
        try
        {
            res = call_query(db::sqlite::queries::QUERY_FindPreviousAndNextNote, req);
            if (res["prev_note_id"].is_number()) prev_note_id = res["prev_note_id"];
            if (res["next_note_id"].is_number()) next_note_id = res["next_note_id"];
            info << res.dump() << commit;
            info << "Query FindPreviousAndNextNote successful" << commit;
        }
        catch (std::exception& e)
        {
            err << "Query FindPreviousAndNextNote failed " << e.what() << commit;
            std::optional<std::pair<entity_fields, api::OperationResult>> result;
            models::NoteNavigation note_navigation;
            auto v = note_navigation.to_values();
            result = std::make_pair<entity_fields, api::OperationResult>(std::move(v), {500, std::string("Query FindPreviousAndNextNote failed ") + e.what()});

            return result;
        }

        models::NoteNavigation note_navigation;
        note_navigation.set_id(id);
        note_navigation.prev_note_id = prev_note_id;
        note_navigation.next_note_id = next_note_id;
        auto values = note_navigation.to_values();
        int64_t now = static_cast<int64_t>(util::Utils::current_unix_timestamp_ms());
        values[1] = now;
        values[2] = now;
        std::optional<std::pair<entity_fields, api::OperationResult>> result;
        result = std::make_pair<entity_fields, api::OperationResult>(std::move(values), ok_result);
        return result;

    }
}
