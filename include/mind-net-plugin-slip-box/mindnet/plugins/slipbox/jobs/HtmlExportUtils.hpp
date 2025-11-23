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
