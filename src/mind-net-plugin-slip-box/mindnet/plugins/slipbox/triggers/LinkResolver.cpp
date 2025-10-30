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
        const std::vector<WikiLink>& wikilinks,
        std::function<nlohmann::json(const std::string&, nlohmann::json&)>& call)
    {
        std::vector<WikiLink> existing;
        std::vector<WikiLink> missing;
        std::unordered_map<std::string, i64> title_to_id;

        // Unique titles to avoid redundant DB hits
        std::unordered_set<std::string> uniq_titles;
        uniq_titles.reserve(wikilinks.size());
        for (auto& w : wikilinks)
            uniq_titles.insert(w.title);

        const size_t MAX_SQLITE_IN = 900;
        std::vector<std::string> all_titles(uniq_titles.begin(), uniq_titles.end());

        if (all_titles.empty())
        {
            essential::info << "No wikilinks to resolve for map_id=" << map_id << essential::commit;
        }
        else
        {
            // batch processing for SQLite IN()
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

                    // res["found_note_ids"] expected: {title: id}
                    for (auto& [title, id] : res.at("found_note_ids").items())
                        title_to_id[title] = id.get<i64>();

                    size_t chunk_index = (i / MAX_SQLITE_IN) + 1;
                    size_t total_chunks = (all_titles.size() + MAX_SQLITE_IN - 1) / MAX_SQLITE_IN;

                    essential::info
                        << "FindNotesInMap OK (chunk " << chunk_index << "/" << total_chunks
                        << "), map_id=" << map_id << essential::commit;
                }
                catch (const std::exception& e)
                {
                    essential::err
                        << "FindNotesInMap failed (chunk " << (i / MAX_SQLITE_IN + 1)
                        << "): " << e.what() << essential::commit;
                    return LinkResolution{};
                }
            }
        }

        // assign existing vs missing
        for (auto& w : wikilinks)
        {
            if (title_to_id.contains(w.title))
                existing.push_back(w);
            else
                missing.push_back(w);
        }

        essential::info
            << "Found " << existing.size() << " existing links, "
            << missing.size() << " missing links, map_id=" << map_id
            << essential::commit;

        LinkResolution lr;
        lr.existing = std::move(existing);
        lr.missing = std::move(missing);
        lr.title_to_id = std::move(title_to_id);
        return lr;
    }
}
