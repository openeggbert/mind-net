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


#include "mindnet/plugins/slipbox/triggers/AfterUpdateContentTrigger.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/plugins/slipbox/models/Content.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_set>

#include "mindnet/plugins/slipbox/models/Link.hpp"
#include "mindnet/plugins/slipbox/models/Note.hpp"
#include "mindnet/plugins/slipbox/models/Url.hpp"
#include "mindnet/plugins/slipbox/models/WantedNote.hpp"
#include "mindnet/plugins/slipbox/triggers/ContentLinkParser.hpp"
#include "mindnet/plugins/slipbox/triggers/LinkResolver.hpp"
#include "mindnet/plugins/slipbox/triggers/LinkSynchronizer.hpp"
#include "mindnet/util/Utils.hpp"



namespace mindnet::plugins::slipbox::triggers
{
    using_loggers()

    AfterUpdateContentTrigger::AfterUpdateContentTrigger()
        : Trigger(
            "AfterUpdateContentTrigger",
            "Parses links after update of a content.",
            1000,
            {essential::Crudl::Update},
            api::TriggerPhase::After,
            "content"
        )
    {
    }

    //     int main()
    //     {
    //         std::string text = R"(This is a test. Visit https://example.com/page?x=1, or http://sub.domain.cz.
    // There can also be a link with an ending: https://example.org/test).
    // And here are wiki links: [[Something]], [[Another page|display text]], and [[Nested [[bad]]]] )";
    //
    //         std::cout << text << std::endl;
    //
    //         auto urls = find_urls(text);
    //         std::cout << "Found URLs:\n";
    //         for (auto& m : urls)
    //         {
    //             std::cout << "  '" << m.text << "' at pos " << m.pos << " len " << m.len << "\n";
    //         }
    //
    //         auto dbs = find_double_brackets(text);
    //         std::cout << "\nFound [[...]] links:\n";
    //         for (auto& m : dbs)
    //         {
    //             // if you want the inner content without square brackets:
    //             std::string inner = m.text.substr(2, m.len - 4);
    //             std::cout << "  '" << m.text << "' -> inner: '" << inner << "' at pos " << m.pos << "\n";
    //         }
    //         return 0;
    //     }

    // Found URLs:
    //   'https://example.com/page?x=1,' at pos 22 len 29
    //   'http://sub.domain.cz.' at pos 55 len 21
    //   'https://example.org/test' at pos 118 len 24
    //
    // Found [[...]] links:
    //   '[[Something]]' -> inner: 'Something' at pos 170
    //   '[[Another page|display text]]' -> inner: 'Another page|display text' at pos 185
    //   '[[Nested [[bad]]' -> inner: 'Nested [[bad' at pos 220


    void AfterUpdateContentTrigger::run_before_or_after(
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
        models::Content new_content;
        models::Content old_content;
        new_content.from_values(fields);
        old_content.from_values(old_fields);

        if (new_content.value.size() > 16 * 1024 * 1024)
        {
            warn << "Content too large to parse (>" << new_content.value.size() << " bytes)" << commit;
            return;
        }

        if (new_content.value == old_content.value)
        {
            info << "Content value unchanged; skipping link parse for content_id="
                << new_content.get_id() << commit;
            return;
        }

        api::AccessTokenContext token = user_id == 0
                                            ? api::AccessTokenContext(user_id, "system", 403)
                                            : api::AccessTokenContext(user_id, "", 200);

        bool success{false};
        auto update_content = [&]()
        {
            new_content.last_parsed_success_at = 0;
            new_content.last_parsed_fail_at = 0;
            if (success) new_content.last_parsed_success_at = util::Utils::current_unix_timestamp_ms();
            if (!success) new_content.last_parsed_fail_at = util::Utils::current_unix_timestamp_ms();
            auto v = new_content.to_values();
            auto run_content_update = run_update(models::CONTENT_DEFINITION, token, new_content.get_id(), v,
                                                 stack_depth);
            if (run_content_update.ko())
            {
                err << "Failed to update content: " << run_content_update.error << commit;
            }
        };

        orm::QueryParams params;
        params.add_filter("content_id", new_content.get_id());
        auto list_notes_result = run_list(models::NOTE_DEFINITION, token, params, stack_depth);
        if (auto& result = list_notes_result.second; result.ko())
        {
            err << "Failed to list notes: " << result.error << commit;
            update_content();
            return;
        }
        auto& notes = list_notes_result.first;
        if (notes.empty() || notes.size() > 1)
        {
            err << "Unexpected note count (" << notes.size()
                << ") for content_id=" << new_content.get_id() << commit;

            update_content();
            return;
        }

        models::Note note;
        note.from_values(list_notes_result.first[0]);
        identification map_id = note.map_id;

        auto parsed = ContentLinkParser::parse_links(new_content.value);
        auto& new_urls = parsed.urls;
        auto& new_links_or_wanted_notes = parsed.wikilinks;

        info << "Parsed " << new_urls.size() << " URLs and "
            << new_links_or_wanted_notes.size() << " wikilinks "
            << "for content_id=" << new_content.get_id() << commit;

        if (new_links_or_wanted_notes.size() > 500)
        {
            warn << "Content " << new_content.get_id()
                << " has unusually many wikilinks ("
                << new_links_or_wanted_notes.size() << ")" << commit;
        }

        std::function<nlohmann::json(const std::string&, nlohmann::json&)> call_query_lambda = [this
            ](const std::string& query_name, nlohmann::json& request)
        {
            return call_query(query_name, request);
        };

        LinkResolution link_resolution = LinkResolver::resolve(
            map_id,
            new_links_or_wanted_notes,
            call_query_lambda);

        std::vector<std::string> old_urls;
        std::vector<std::string> old_links;
        std::vector<std::string> old_wanted_notes;
        std::map<std::string, i64> old_urls_ids;
        std::map<std::string, i64> old_links_ids;
        std::map<std::string, i64> old_wanted_notes_ids;


        // --- load persisted state for this note_id (URL, LINK, WANTED_NOTE) ----
        {
            // 1) URL
            {
                orm::QueryParams p;
                p.add_filter("from_note_id", note.get_id());
                auto r = run_list(models::URL_DEFINITION, token, p, stack_depth);
                if (auto& res = r.second; res.ko())
                {
                    err << "Failed to list URL for note_id=" << note.get_id()
                        << ": " << res.error << commit;

                    update_content();
                    return;
                }
                else
                {
                    for (auto& row : r.first)
                    {
                        models::Url m;
                        m.from_values(row);
                        // Assumption: models::URL has field/getter 'url'
                        old_urls.emplace_back(m.to_url);
                        old_urls_ids.emplace(m.to_url, m.get_id());
                    }
                }
            }

            // 2) LINK (internal links to existing notes)
            {
                orm::QueryParams p;
                p.add_filter("from_note_id", note.get_id());
                auto r = run_list(models::LINK_DEFINITION, token, p, stack_depth);
                if (auto& res = r.second; res.ko())
                {
                    err << "Failed to list LINK for note_id=" << note.get_id()
                        << ": " << res.error << commit;

                    update_content();
                    return;
                }
                else
                {
                    for (auto& row : r.first)
                    {
                        models::Link m;
                        m.from_values(row);
                        // Assumption: models::Link has text field for target (e.g. title/target_title)
                        old_links.emplace_back(m.to_note_title);
                        old_links_ids.emplace(m.to_note_title, m.get_id());
                    }
                }
            }

            // 3) WANTED_NOTE (non-existing targets; requested notes)
            {
                orm::QueryParams p;
                p.add_filter("from_note_id", note.get_id());
                auto r = run_list(models::WANTED_NOTE_DEFINITION, token, p, stack_depth);
                if (auto& res = r.second; res.ko())
                {
                    err << "Failed to list WANTED_NOTE for note_id=" << note.get_id()
                        << ": " << res.error << commit;

                    update_content();
                    return;
                }
                else
                {
                    for (auto& row : r.first)
                    {
                        models::WantedNote m;
                        m.from_values(row);
                        // Assumption: models::WantedNote has name/identifier (e.g. title/name)
                        old_wanted_notes.emplace_back(m.to_note_title);
                        old_wanted_notes_ids.emplace(m.to_note_title, m.get_id());
                    }
                }
            }

            info << "Loaded from DB for note_id=" << note.get_id()
                << ": URLs=" << old_urls.size()
                << ", LINKS=" << old_links.size()
                << ", WANTED_NOTES=" << old_wanted_notes.size() << commit;
        }


        std::function<api::OperationResult(
            const model::ModelDefinition&,
            api::AccessTokenContext&,
            int,
            int)> run_delete_lambda = [this](const model::ModelDefinition& def,
                                             api::AccessTokenContext& token,
                                             identification id,
                                             int depth) -> api::OperationResult
        {
            return run_delete(def, token, id, depth);
        };


        std::function<std::pair<int, api::OperationResult>(
            const model::ModelDefinition&,
            api::AccessTokenContext&,
            entity_fields&,
            int)> run_create_lambda = [this](
            const model::ModelDefinition& def,
            api::AccessTokenContext& token,
            entity_fields& fields,
            int depth) -> std::pair<int, api::OperationResult>
        {
            return run_create(def, token, fields, depth);
        };


        LinkSynchronizer sync(
            token,
            stack_depth,
            note.get_id(),
            run_delete_lambda,
            run_create_lambda
        );
        sync.sync_urls(old_urls, old_urls_ids, parsed.urls);
        sync.sync_links(old_links, old_links_ids, link_resolution.existing, link_resolution.title_to_id);
        sync.sync_wanted_notes(old_wanted_notes, old_wanted_notes_ids, link_resolution.missing);

        success = true;
        update_content();
    }
}
