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
#include "mindnet/plugins/slipbox/triggers/LinkSynchronizer.hpp"

#include <unordered_set>
#include <unordered_map>

#include "mindnet/api/IService.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/slipbox/models/Link.hpp"
#include "mindnet/plugins/slipbox/models/Url.hpp"
#include "mindnet/plugins/slipbox/models/WantedNote.hpp"
#include "mindnet/util/Utils.hpp"


namespace mindnet::plugins::slipbox::triggers
{
    using_loggers()

    LinkSynchronizer::LinkSynchronizer(
        api::AccessTokenContext& token,
        int stack_depth,
        identification note_id,
        std::function<api::OperationResult(
            const model::ModelDefinition&,
            api::AccessTokenContext&,
            identification,
            int)> run_delete,
        std::function<std::pair<identification, api::OperationResult>(
            const model::ModelDefinition&,
            api::AccessTokenContext&,
            entity_fields&,
            int)> run_create)
        : token(token),
          stack_depth(stack_depth),
          note_id(note_id),
          run_delete_(std::move(run_delete)),
          run_create_(std::move(run_create))
    {
    }


    // Generic sync for items represented as string titles
    template <typename Entity, typename MakeFn>
    void LinkSynchronizer::sync_entities(const std::vector<std::string>& old_items,
                                         const std::map<std::string, identification>& old_ids,
                                         const std::vector<std::string>& new_items,
                                         const mindnet::model::ModelDefinition& def,
                                         MakeFn make_entity_fn)
    {
        std::unordered_set<std::string> old_set(old_items.begin(), old_items.end());
        std::unordered_set<std::string> new_set(new_items.begin(), new_items.end());
        const std::string& name = def.get_model_name();

        // --- DELETE missing ---
        for (const auto& val : old_set)
        {
            if (!new_set.contains(val))
            {
                auto it = old_ids.find(val);
                if (it == old_ids.end()) continue;
                identification id = it->second;

                auto del_result = run_delete_(def, token, id, stack_depth);
                if (del_result.ko())
                    warn << "Failed to delete " << name << "='" << val << "' for note_id=" << note_id
                         << ": " << del_result.error << commit;
                else
                    info << "Deleted " << name << " '" << val << "' for note_id=" << note_id << commit;
            }
        }

        // --- INSERT new ---
        for (const auto& val : new_set)
        {
            if (!old_set.contains(val))
            {
                Entity entity = make_entity_fn(val);
                auto v = entity.to_values();
                v[1] = mindnet::util::Utils::current_unix_timestamp_ms();
                v[2] = mindnet::util::Utils::current_unix_timestamp_ms();
                auto result = run_create_(def, token, v, stack_depth);
                if (result.second.ko())
                    warn << "Failed to insert " << name << "='" << val << "' for note_id=" << note_id
                         << ": " << result.second.error << commit;
                else
                    info << "Inserted " << name << " '" << val << "' for note_id=" << note_id << commit;
            }
        }
    }



    // --- URL ---
    void LinkSynchronizer::sync_urls(const std::vector<std::string>& old_urls,
                                     const std::map<std::string, identification>& old_urls_ids,
                                     const std::vector<std::string>& new_urls)
    {
        sync_entities<models::Url>(
            old_urls, old_urls_ids, new_urls,
            models::URL_DEFINITION,
            [&](const std::string& url)
            {
                models::Url m;
                m.from_note_id = note_id;
                m.to_url = url;
                return m;
            });
    }


    // --- LINK ---
    void LinkSynchronizer::sync_links(
        const std::vector<std::string>& old_links,
        const std::map<std::string, identification>& old_links_ids,
        const std::vector<WikiLink>& new_links,
        const std::unordered_map<std::string, identification>& title_to_id)
    {
        // Build lookup: title -> display
        std::unordered_map<std::string, std::string> display_map;
        display_map.reserve(new_links.size());
        for (auto& w : new_links)
            display_map[w.title] = w.display;

        // Extract only titles for generic sync
        std::vector<std::string> new_titles;
        new_titles.reserve(new_links.size());
        for (auto& w : new_links)
            new_titles.emplace_back(w.title);

        sync_entities<models::Link>(
            old_links, old_links_ids, new_titles,
            models::LINK_DEFINITION,
            [&](const std::string& title)
            {
                models::Link m;
                m.from_note_id = note_id;
                m.to_note_title = title;

                // O(1) lookup, no linear scan
                if (auto it = display_map.find(title); it != display_map.end())
                    m.label = it->second;
                else
                    m.label = title;

                if (auto it = title_to_id.find(title); it != title_to_id.end())
                    m.to_note_id = it->second;

                return m;
            }
        );
    }


    // --- WANTED_NOTE ---
    void LinkSynchronizer::sync_wanted_notes(
        const std::vector<std::string>& old_wanted,
        const std::map<std::string, identification>& old_wanted_ids,
        const std::vector<WikiLink>& new_wanted)
    {
        // Build lookup: title -> display
        std::unordered_map<std::string, std::string> display_map;
        display_map.reserve(new_wanted.size());
        for (auto& w : new_wanted)
            display_map[w.title] = w.display;

        // Extract only titles
        std::vector<std::string> new_titles;
        new_titles.reserve(new_wanted.size());
        for (auto& w : new_wanted)
            new_titles.emplace_back(w.title);

        sync_entities<models::WantedNote>(
            old_wanted, old_wanted_ids, new_titles,
            models::WANTED_NOTE_DEFINITION,
            [&](const std::string& title)
            {
                models::WantedNote m;
                m.from_note_id = note_id;
                m.to_note_title = title;

                // No linear scan
                if (auto it = display_map.find(title); it != display_map.end())
                    m.label = it->second;
                else
                    m.label = title;

                return m;
            }
        );
    }

}
