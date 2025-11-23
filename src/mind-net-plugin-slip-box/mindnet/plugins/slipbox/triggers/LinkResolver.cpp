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


#include "mindnet/plugins/slipbox/triggers/LinkResolver.hpp"

#include <unordered_set>

#include "mindnet/essential/ConsolePrinter.hpp"
#include "mindnet/essential/Global.hpp"
#include "../../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/FindNotesInMapSQLiteQuery.hpp"


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
