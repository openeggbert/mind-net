//
// Created by robertvokac on 10/23/25.
//

#include "mindnet/plugins/slipbox/triggers/LinkSynchronizer.h"

#include <unordered_set>

#include "mindnet/api/IService.h"
#include "mindnet/essential/Global.h"
#include "mindnet/plugins/slipbox/models/Link.h"
#include "mindnet/plugins/slipbox/models/Url.h"
#include "mindnet/plugins/slipbox/models/WantedNote.h"

namespace mindnet::plugins::slipbox::triggers
{
    using_loggers()

    LinkSynchronizer::LinkSynchronizer(
        api::AccessTokenContext& token,
        int stack_depth,
        i64 note_id,
        std::function<api::OperationResult(
            const model::ModelDefinition&,
            api::AccessTokenContext&,
            int,
            int)> run_delete,
        std::function<std::pair<int, api::OperationResult>(
            const model::ModelDefinition&,
            api::AccessTokenContext&,
            entity_fields&,
            int)> run_create)
        : token(token),
          stack_depth(stack_depth),
          note_id(note_id),
          run_delete_(std::move(run_delete)),
          run_create_(std::move(run_create_))
    {
    }


    // Generic sync
    template <typename Entity, typename MakeFn>
    void LinkSynchronizer::sync_entities(const std::vector<std::string>& old_items,
                                         const std::map<std::string, i64>& old_ids,
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
                i64 id = it->second;

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
                                     const std::map<std::string, i64>& old_urls_ids,
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
    void LinkSynchronizer::sync_links(const std::vector<std::string>& old_links,
                                      const std::map<std::string, i64>& old_links_ids,
                                      const std::vector<std::string>& new_links,
                                      const std::unordered_map<std::string, i64>& title_to_id)
    {
        sync_entities<models::Link>(
            old_links, old_links_ids, new_links,
            models::LINK_DEFINITION,
            [&](const std::string& title)
            {
                models::Link m;
                m.from_note_id = note_id;
                if (auto it = title_to_id.find(title); it != title_to_id.end())
                    m.to_note_id = it->second;
                m.to_note_title = title;
                return m;
            });
    }

    // --- WANTED_NOTE ---
    void LinkSynchronizer::sync_wanted_notes(const std::vector<std::string>& old_wanted,
                                             const std::map<std::string, i64>& old_wanted_ids,
                                             const std::vector<std::string>& new_wanted)
    {
        sync_entities<models::WantedNote>(
            old_wanted, old_wanted_ids, new_wanted,
            models::WANTED_NOTE_DEFINITION,
            [&](const std::string& title)
            {
                models::WantedNote m;
                m.from_note_id = note_id;
                m.to_note_title = title;
                return m;
            });
    }
}
