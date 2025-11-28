/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mindnet/plugins/slipbox/triggers/InsteadOfReadNoteNavigationTrigger.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include <string>
#include <vector>
#include "../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindPreviousAndNextNoteSQLiteQuery.hpp"

#include "mindnet/util/Utils.hpp"

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
                        identification user_id, identification id)
    {
        nlohmann::json req;
        req["note_id"] = cast64(id);
        nlohmann::json res;
        identification prev_note_id{};
        identification next_note_id{};
        try
        {
            res = call_query(db::sqlite::queries::QUERY_FindPreviousAndNextNote, req);

            if (res.contains("error") && res.contains("exists") && res["exists"] == false)
            {
                std::optional<std::pair<entity_fields, api::OperationResult>> result;
                models::NoteNavigation note_navigation;
                auto v = note_navigation.to_values();
                result = std::make_pair<entity_fields, api::OperationResult>(std::move(v), {
                                                                                 404,
                                                                                 std::string(
                                                                                     "There is no note with ID ") +
                                                                                 std::to_string(id)
                                                                             });
                return result;
            }

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
            result = std::make_pair<entity_fields, api::OperationResult>(std::move(v), {
                                                                             500,
                                                                             std::string(
                                                                                 "Query FindPreviousAndNextNote failed ")
                                                                             + e.what()
                                                                         });

            return result;
        }

        models::NoteNavigation note_navigation;
        note_navigation.set_id(id);
        note_navigation.note_id = id;
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
