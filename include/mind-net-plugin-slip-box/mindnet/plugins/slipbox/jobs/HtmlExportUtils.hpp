//
// Created by robertvokac on 11/22/25.
//

#pragma once
#include <map>
#include <string>

namespace mindnet::plugin::slipbox::jobs
{
    using std::string;
    string& title_without_spaces(string& title);
    string replace_placeholders(
        const std::string& text,
        const std::map<std::string, std::string>& values);
    string generate_breadcrumb(std::vector<string>& parents, string& current);
    string generate_hierarchy_panel(std::string& go_up, std::string& path, std::vector<plugins::slipbox::models::Note> children);

    string get_html_template();
    string get_styles_css();
    string get_script_js();

}
