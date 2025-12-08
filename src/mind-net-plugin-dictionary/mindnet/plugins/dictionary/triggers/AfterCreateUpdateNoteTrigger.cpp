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

#include "mindnet/plugins/dictionary/triggers/AfterCreateUpdateNoteTrigger.hpp"

#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/dictionary/models/Link.hpp"
#include "mindnet/plugins/dictionary/models/Note.hpp"
#include "mindnet/plugins/dictionary/models/WantedNote.hpp"

namespace mindnet::plugins::dictionary::triggers
{
    using_loggers()

    AfterCreateUpdateNoteTrigger::AfterCreateUpdateNoteTrigger()
        : Trigger(
            "AfterCreateUpdateNoteTrigger",
            "Updates links and wanted notes after creation or update of a note.",
            1000,
            {essential::Crudl::Create, essential::Crudl::Update},
            api::TriggerPhase::After,
            "note"
        )
    {
    }

    void AfterCreateUpdateNoteTrigger::run_before_or_after(
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
        if (action_result.ko())
        {
            return;
        }
        models::Note new_note;
        new_note.from_values(fields);

        models::Note old_note;
        if (operation == essential::Crudl::Update) old_note.from_values(old_fields);

        std::string new_title = new_note.title;
        std::string old_title = old_note.title;

        if (operation == essential::Crudl::Update && new_title == old_title)
        {
            info << "Note title unchanged, skipping link/wanted-note sync" << commit;
            return;
        }

        identification map_id = new_note.map_id;

        api::AccessTokenContext token = user_id == 0
                                            ? api::AccessTokenContext(user_id, "system", 403)
                                            : api::AccessTokenContext(user_id, "", 200);

        auto has_map_this_note = [&](identification note_id)
        {
            orm::QueryParams params_notes;
            params_notes.add_filter("id", note_id);
            params_notes.add_filter("map_id", map_id);
            auto list_notes = run_list(models::NOTE_DEFINITION, token, params_notes, stack_depth);
            if (list_notes.second.ko())
            {
                err << "Listing notes failed " << list_notes.second.error << commit;
                return false;
            }
            return !list_notes.first.empty();
        };

        auto find_wanted_notes_for_title = [&](const std::string& title)
        {
            orm::QueryParams params_wanted_notes;
            params_wanted_notes.add_filter("to_note_title", title);
            auto list_wanted_notes = run_list(models::WANTED_NOTE_DEFINITION, token, params_wanted_notes, stack_depth);
            std::set<models::WantedNote> wanted_notes;
            if (list_wanted_notes.second.ko())
            {
                err << "Listing wanted notes failed";
                return wanted_notes;
            }

            for (auto& e : list_wanted_notes.first)
            {
                models::WantedNote wn;
                wn.from_values(e);
                if (has_map_this_note(wn.from_note_id)) wanted_notes.insert(wn);
            }
            return wanted_notes;
        };

        auto find_links_for_title = [&](const std::string& title)
        {
            orm::QueryParams params_links;
            params_links.add_filter("to_note_title", title);
            auto list_links = run_list(models::LINK_DEFINITION, token, params_links, stack_depth);

            std::set<models::Link> links;
            if (list_links.second.ko())
            {
                err << "Listing links failed";
                return links;
            }
            for (auto& e : list_links.first)
            {
                models::Link l;
                l.from_values(e);
                links.insert(l);
            }
            return links;
        };

        auto convert_link_to_wanted_note = [&](const models::Link& link)
        {
            models::WantedNote wanted_note;
            wanted_note.from_note_id = link.from_note_id;
            wanted_note.to_note_title = link.to_note_title;
            return wanted_note;
        };
        auto convert_wanted_note_to_link = [&](const models::WantedNote& wanted_note, identification note_id)
        {
            models::Link link;
            link.from_note_id = wanted_note.from_note_id;
            link.to_note_id = note_id;
            link.to_note_title = wanted_note.to_note_title;
            return link;
        };
        auto delete_link = [&](const models::Link& link)
        {
            auto delete_result = run_delete(models::LINK_DEFINITION, token, link.get_id(), stack_depth);
            if (delete_result.ko()) err << "Deleting link failed " << delete_result.error << commit;
        };
        auto delete_wanted_note = [&](const models::WantedNote& wanted_note)
        {
            auto delete_result = run_delete(models::WANTED_NOTE_DEFINITION, token, wanted_note.get_id(), stack_depth);
            if (delete_result.ko()) err << "Deleting wanted_note failed " << delete_result.error << commit;
        };
        auto save_link = [&](const models::Link& link)
        {
            auto v = link.to_values();
            auto create_result = run_create(models::LINK_DEFINITION, token, v, stack_depth);
            if (create_result.second.ko()) err << "Saving link failed " << create_result.second.error << commit;
        };
        auto save_wanted_note = [&](const models::WantedNote& wanted_note)
        {
            auto v = wanted_note.to_values();
            auto create_result = run_create(models::WANTED_NOTE_DEFINITION, token, v, stack_depth);
            if (create_result.second.ko()) err << "Saving wanted_note failed " << create_result.second.error << commit;
        };
        if (operation == essential::Crudl::Create || operation == essential::Crudl::Update)
        {
            auto wanted_notes = find_wanted_notes_for_title(new_title);
            for (const auto& wn : wanted_notes)
            {
                auto link = convert_wanted_note_to_link(wn, id);
                delete_wanted_note(wn);
                save_link(link);
                info << "Converted WantedNote→Link: from=" << wn.from_note_id
                    << " to=" << new_title << commit;
            }
        }
        if (operation == essential::Crudl::Update)
        {
            auto links = find_links_for_title(old_title);
            for (const auto& l : links)
            {
                auto wn = convert_link_to_wanted_note(l);
                delete_link(l);
                save_wanted_note(wn);
                info << "Converted Link→WantedNote: from=" << l.from_note_id
                    << " old_title=" << old_title << commit;
            }
        }
    }
}