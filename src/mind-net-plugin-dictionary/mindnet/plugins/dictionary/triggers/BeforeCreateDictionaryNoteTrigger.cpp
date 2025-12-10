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

#include "mindnet/plugins/dictionary/triggers/BeforeCreateDictionaryNoteTrigger.hpp"

#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryNote.hpp"
#include "../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindNextDictionaryNotePositionSQLiteQuery.hpp"

namespace mindnet::plugins::dictionary::triggers
{
    using_loggers()

    BeforeCreateDictionaryNoteTrigger::BeforeCreateDictionaryNoteTrigger()
        : Trigger(
            "BeforeCreateDictionaryNoteTrigger",
            "Set the value of the position.",
            1000,
            {essential::Crudl::Create},
            api::TriggerPhase::Before,
            "dictionary_note"
        )
    {
    }

    void BeforeCreateDictionaryNoteTrigger::run_before_or_after(
        mindnet::essential::Crudl operation,
        int stack_depth,
        api::OperationResult& validation_result,
        api::OperationResult& action_result,
        const mindnet::model::ModelDefinition def,
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
        models::DictionaryNote new_note;
        new_note.from_values(fields);

        identification dictionary_term_id = new_note.dictionary_term_id;

        nlohmann::json req;
        req["dictionary_term_id"] = dictionary_term_id;
        nlohmann::json res;
        try
        {
            res = call_query(db::sqlite::queries::QUERY_FindNextDictionaryNotePosition, req);
            i64 next_position = res["next_position"];
            new_note.position = next_position;
            auto v = new_note.to_values();
            fields = v;
            info << "Query FindNextDictionaryNotePosition successful" << commit;
        }
        catch (std::exception& e)
        {
            err << "Query FindNextDictionaryNotePosition failed" << commit;
        }
    }
}