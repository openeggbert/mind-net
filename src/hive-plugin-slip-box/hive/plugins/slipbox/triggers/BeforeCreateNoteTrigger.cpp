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

#include "hive/plugins/slipbox/triggers/BeforeCreateNoteTrigger.hpp"

#include "hive/api/AccessTokenContext.hpp"
#include "hive/essential/Global.hpp"
#include "hive/plugins/slipbox/models/Link.hpp"
#include "hive/plugins/slipbox/models/Note.hpp"
#include "../../../../../../include/hive-db-sqlite/hive/db/sqlite/queries/slipbox/FindNextSiblingOrderSQLiteQuery.hpp"

namespace hive::plugins::slipbox::triggers
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
        hive::essential::Crudl operation,
        int stack_depth,
        api::OperationResult& validation_result,
        api::OperationResult& action_result,
        const hive::model::ModelDefinition def,
        identification user_id,
        identification id,
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

        identification map_id = new_note.map_id;
        identification parent_note_id = new_note.parent_note_id;

        nlohmann::json req;
        req["map_id"] = map_id;
        req["parent_note_id"] = parent_note_id;
        req["note_id"] = id;
        nlohmann::json res;
        try
        {
            res = call_query(db::sqlite::queries::slipbox::QUERY_FindNextSiblingOrder, req);
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