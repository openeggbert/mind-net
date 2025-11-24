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
            "UpdateNotePathAndDepthAfterTrigger",
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
