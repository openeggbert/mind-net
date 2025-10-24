//
// Created by robertvokac on 10/23/25.
//

#include "mindnet/plugins/slipbox/triggers/LinkResolver.h"

#include <set>
#include <unordered_set>

#include "mindnet/essential/ConsolePrinter.h"
#include "mindnet/essential/Global.h"
#include "../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindNotesInMapSQLiteQuery.h"

namespace mindnet::plugins::slipbox::triggers
{
    LinkResolution LinkResolver::resolve(
        i64 map_id,
        const std::vector<std::string>& wikilinks,
        std::function<nlohmann::json(const std::string&, nlohmann::json&)>& call)
    {
        std::vector<std::string> new_links;
        std::vector<std::string> new_wanted_notes;
        std::unordered_map<std::string, i64> title_to_id;

        // --- handle SQLite 900-parameter limit -----------------------------
        std::unordered_set<std::string> uniq_titles;
        uniq_titles.reserve(wikilinks.size());
        for (auto& t : wikilinks) uniq_titles.insert(t);

        const size_t MAX_SQLITE_IN = 900;
        std::vector<std::string> all_titles(uniq_titles.begin(), uniq_titles.end());
        std::set<std::string> found_note_titles;


        if (all_titles.empty())
        {
            essential::info << "No wikilinks to resolve for map_id=" << map_id << essential::commit;
        }
        else
        {
            for (size_t i = 0; i < all_titles.size(); i += MAX_SQLITE_IN)
            {
                size_t end = std::min(i + MAX_SQLITE_IN, all_titles.size());
                std::vector<std::string> chunk(all_titles.begin() + i, all_titles.begin() + end);

                nlohmann::json req;
                req["map_id"] = map_id;
                req["note_titles"] = chunk;

                try
                {
                    nlohmann::json res = call(db::sqlite::queries::QUERY_FindNotesInMap, req);
                    for (auto& el : res.at("found_note_titles"))
                        found_note_titles.insert(el.get<std::string>());


                    for (auto& [title, id] : res.at("found_note_ids").items()) {
                        title_to_id[title] = id.get<i64>();
                    }


                    size_t chunk_index = (i / MAX_SQLITE_IN) + 1;
                    size_t total_chunks = (all_titles.size() + MAX_SQLITE_IN - 1) / MAX_SQLITE_IN;
                    essential::info << "FindNotesInMapQuery OK (chunk " << chunk_index << "/" << total_chunks
                        << "), map_id=" << map_id << essential::commit;
                }
                catch (const std::exception& e)
                {
                    essential::err << "FindNotesInMapQuery failed on chunk " << (i / MAX_SQLITE_IN + 1)
                        << ": " << e.what() << essential::commit;
                    return LinkResolution{};
                }
            }
        }


        for (auto& e : wikilinks) {
            if (title_to_id.contains(e))
                new_links.push_back(e);
            else
                new_wanted_notes.push_back(e);
        }


        essential::info << "Found " << new_links.size() << " existing links, "
            << new_wanted_notes.size() << " wanted notes, "
            << "in map_id=" << map_id << essential::commit;

        LinkResolution link_resolution;
        link_resolution.existing = new_links;
        link_resolution.missing = new_wanted_notes;
        link_resolution.title_to_id = title_to_id;
        return link_resolution;
    }
}
