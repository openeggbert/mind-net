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

#include "mindnet/plugins/dictionary/jobs/HtmlExportJob.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/util/Utils.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/orm/QueryParams.hpp"

#include <filesystem>
#include <fstream>
#include <map>

#include "mindnet/plugins/dictionary/jobs/HtmlExportUtils.hpp"

namespace mindnet::plugins::dictionary::jobs
{
    using_loggers()
    using std::string;

    HtmlExportJob::HtmlExportJob()
        : Job(
            "HtmlExportJob",
            "HtmlExportJob",
            "@startup",
            false,
            false
        )
    {
    }

    std::string HtmlExportJob::run(api::cronq::JobConfig& job_config)
    {
        essential::info << "HtmlExportJob started" << essential::commit;

        auto current_path = std::filesystem::current_path();
        auto export_dir = current_path /
        ("html_export_" + mindnet::util::Utils::unixtime_to_short_string(
            util::Utils::current_unix_timestamp_ms()
        ));
        try
        {
            std::filesystem::create_directories(export_dir);
        }
        catch (const std::exception& ex)
        {
            return std::string("Failed to create export directory: ") + ex.what();
        }

        api::AccessTokenContext token = api::AccessTokenContext(0, "system", 403);

        mindnet::orm::QueryParams maps_query_params;
        auto maps = run_list(mindnet::plugins::dictionary::models::MAP_DEFINITION, token, maps_query_params, 0);
        if (maps.second.ko()) return maps.second.error;

        for (auto& map_values : maps.first)
        {
            models::Map map;
            map.from_values(map_values);
            if (map.name == "C++") map.name = "cppforever.com";
            auto export_map_dir = export_dir / map.name;
            try
            {
                std::filesystem::create_directories(export_map_dir);
            }
            catch (const std::exception& ex)
            {
                return std::string("Failed to create export map directory: ") + ex.what();
            }
            auto result = generate_map(export_map_dir, map, token);
            if (!result.empty()) return result;
        }

        return "";
    }

    static const string note_title = "note_title";
    static const string description_ = "description";
    static const string keywords = "keywords";
    static const string author = "author";
    static const string map_name = "map_name";
    static const string base_href = "base_href";
    static const string breadcrumb = "breadcrumb";
    static const string hierarchy_panel = "hierarchy_panel";
    static const string html_content = "html_content";

    std::string HtmlExportJob::generate_map(std::filesystem::path& export_map_dir, models::Map& map,
                                            api::AccessTokenContext& token)
    {
        {
            std::ofstream styles_css(export_map_dir / "styles.css");
            styles_css << plugin::slipbox::jobs::get_styles_css();
        }
        {
            std::ofstream script_js(export_map_dir / "script.js");
            script_js << plugin::slipbox::jobs::get_script_js();
        }

        auto author_result = run_read(core::models::USER_DEFINITION, token, map.owner_id, 0);
        if (author_result.second.ko()) return author_result.second.error;
        core::models::User author_;
        author_.from_values(author_result.first);

        mindnet::orm::QueryParams query_params;
        query_params.add_filter("map_id", map.get_id());
        query_params.add_filter("parent_note_id", 0);
        query_params.sort = "sibling_order";
        query_params.order = mindnet::orm::Order::Asc;
        auto notes = run_list(mindnet::plugins::dictionary::models::NOTE_DEFINITION, token, query_params, 0);
        if (notes.second.ko()) return notes.second.error;

        std::vector<models::Note> root_notes;
        for (auto& note_values : notes.first)
        {
            models::Note note;
            note.from_values(note_values);
            root_notes.push_back(note);
        }
        auto author_display_name = author_.display_name.empty() ? author_.username : author_.display_name;
        {
            string index_html = plugin::slipbox::jobs::get_html_template();

            std::string empty_string;
            string hierarchy_panel_html = plugin::slipbox::jobs::generate_hierarchy_panel(
                false, empty_string, root_notes);
            std::map<string, string> placeholder_map =
            {
                {note_title, "Home"},
                {description_, map.name},
                {keywords, map.name},
                {author, author_display_name},
                {map_name, map.name},
                {base_href, "."},
                {breadcrumb, "<span id=\"panel_current\">Home</span>"},
                {hierarchy_panel, hierarchy_panel_html},
                {html_content, plugin::slipbox::jobs::markdown_to_html(map.description)},
            };
            index_html = plugin::slipbox::jobs::replace_placeholders(index_html, placeholder_map);
            std::ofstream index_html_file(export_map_dir / "index.html");
            index_html_file << index_html;
        }

        for (auto& note : root_notes)
        {
            auto result = generate_page(note, token, author_display_name, map, export_map_dir);
            if (!result.empty()) return result;
        }

        return "";
    }

    std::string HtmlExportJob::generate_page(models::Note& note, api::AccessTokenContext& token,
                                             string& author_display_name, models::Map& map,
                                             std::filesystem::path& export_map_dir)
    {
        string index_html = plugin::slipbox::jobs::get_html_template();

        mindnet::orm::QueryParams query_params;
        query_params.add_filter("map_id", map.get_id());
        query_params.add_filter("parent_note_id", note.get_id());
        query_params.sort = "sibling_order";
        query_params.order = mindnet::orm::Order::Asc;
        auto children_result = run_list(mindnet::plugins::dictionary::models::NOTE_DEFINITION, token, query_params, 0);
        if (children_result.second.ko()) return children_result.second.error;

        std::vector<models::Note> children;
        for (auto& note_values : children_result.first)
        {
            models::Note note_;
            note_.from_values(note_values);
            children.push_back(note_);
        }

        auto path_ids = plugin::slipbox::jobs::split_path_numbers(note.path);

        string path_;
        std::vector<std::string> parents;
        for (auto& note_id : path_ids)
        {
            auto e = run_read(models::NOTE_DEFINITION, token, note_id, 0);
            if (e.second.ko()) return e.second.error;
            if (!path_.empty()) path_ += "/";
            models::Note n;
            n.from_values(e.first);
            path_ += plugin::slipbox::jobs::normalize_text_for_url(n.title);
            parents.push_back(n.title);
        }
        string breadcrumb_html = plugin::slipbox::jobs::generate_breadcrumb(parents, note.title);
        string hierarchy_panel_html = plugin::slipbox::jobs::generate_hierarchy_panel(true, path_, children);

        string base_href_;
        for (int i = 1; i <= (note.depth + 1); i++)
        {
            if (!base_href_.empty()) base_href_ += "/";
            base_href_ += "..";
        }
        models::Content content;

        if (note.content_id != 0)
        {
            auto read_content = run_read(models::CONTENT_DEFINITION, token, note.content_id, 0);
            if (read_content.second.ko()) return read_content.second.error;
            content.from_values(read_content.first);
        }

        std::map<string, string> placeholder_map =
        {
            {note_title, note.title},
            {description_, note.title},
            {keywords, note.title},
            {author, author_display_name},
            {map_name, map.name},
            {base_href, base_href_},
            {breadcrumb, breadcrumb_html},
            {hierarchy_panel, hierarchy_panel_html},
            {html_content, note.content_id == 0 ? "" : plugin::slipbox::jobs::markdown_to_html(content.value)},
        };
        index_html = plugin::slipbox::jobs::replace_placeholders(index_html, placeholder_map);
        auto dir = export_map_dir / path_;
        std::filesystem::create_directories(dir);
        auto index_html_path = dir / "index.html";
        debug << "index_html_path=" << index_html_path << commit;
        std::ofstream index_html_file(index_html_path);

        index_html_file << index_html;

        for (auto& n : children)
        {
            auto result = generate_page(n, token, author_display_name, map, export_map_dir);
        }
        return "";
    }
}