//
// Created by robertvokac on 11/15/25.
//

#include "mindnet/plugins/slipbox/jobs/HtmlExportJob.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/util/Utils.hpp"
#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/orm/QueryParams.hpp"

#include <filesystem>
#include <fstream>

#include "mindnet/plugins/slipbox/jobs/HtmlExportUtils.hpp"

namespace mindnet::plugins::slipbox::jobs
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
        try {
            std::filesystem::create_directories(export_dir);
        } catch (const std::exception& ex) {
            return std::string("Failed to create export directory: ") + ex.what();
        }

        api::AccessTokenContext token = api::AccessTokenContext(0, "system", 403);

        mindnet::orm::QueryParams maps_query_params;
        auto maps = run_list(mindnet::plugins::slipbox::models::MAP_DEFINITION, token, maps_query_params, 0 );
        if (maps.second.ko()) return maps.second.error;

        for (auto& map_values : maps.first)
        {
            models::Map map;
            map.from_values(map_values);
            auto export_map_dir = export_dir / map.name;
            try {
                std::filesystem::create_directories(export_map_dir);
            } catch (const std::exception& ex) {
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

    std::string HtmlExportJob::generate_map(std::filesystem::path& export_map_dir, models::Map& map, api::AccessTokenContext& token)
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
        auto notes = run_list(mindnet::plugins::slipbox::models::NOTE_DEFINITION, token, query_params, 0 );
        if (notes.second.ko()) return notes.second.error;

        std::vector<models::Note> root_notes;
        for (auto& note_values:notes.first)
        {
            models::Note note;
            note.from_values(note_values);
            root_notes.push_back(note);
        }

        {
            string index_html = plugin::slipbox::jobs::get_html_template();

            std::string empty_string;
            string hierarchy_panel_html = plugin::slipbox::jobs::generate_hierarchy_panel(empty_string, empty_string, root_notes);
            std::map<string,string> placeholder_map =
            {
                {note_title, map.name},
                {description_, map.name},
                {keywords, map.name},
                {author, author_.display_name.empty() ? author_.username : author_.display_name},
                {map_name, map.name},
                {base_href, "."},
                {breadcrumb, "<span id=\"panel_current\">Home</span>"},
                {hierarchy_panel, hierarchy_panel_html},
                {html_content, map.description},
                            };
            index_html = plugin::slipbox::jobs::replace_placeholders(index_html, placeholder_map);
            std::ofstream index_html_file(export_map_dir / "index.html");
            index_html_file << index_html;
        }

        for (auto& note:root_notes)
        {
            auto result = generate_page(note, token);
            if (!result.empty()) return result;
        }


        return "";
    }

    std::string HtmlExportJob::generate_page(models::Note& note, api::AccessTokenContext& token)
    {
        return "";
    }

}
