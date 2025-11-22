//
// Created by robertvokac on 11/22/25.
//

#pragma once
#include <map>
#include <string>

#include "mindnet/plugins/slipbox/models/Note.hpp"

namespace mindnet::plugin::slipbox::jobs
{
    using std::string;
    string& normalize_text_for_url(string& title);
    string& normalize_underscore_by_space(string& title);
    std::string replace_wikilinks(const std::string& input);

    string replace_placeholders(
        const std::string& text,
        const std::map<std::string, std::string>& values);
    string generate_breadcrumb(std::vector<string>& parents, string& current);
    string generate_hierarchy_panel(bool go_up, std::string& path, std::vector<plugins::slipbox::models::Note> children);
    std::vector<int64_t> split_path_numbers(const std::string& s);

    std::string markdown_to_html(const std::string& text);
    string get_html_template();
    string get_styles_css();
    string get_script_js();

}
