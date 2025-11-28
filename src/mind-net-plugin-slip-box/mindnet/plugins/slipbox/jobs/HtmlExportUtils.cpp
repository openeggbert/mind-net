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

#include "mindnet/plugins/slipbox/jobs/HtmlExportUtils.hpp"
#include "mindnet/plugins/slipbox/models/Note.hpp"

#include <cmark.h>

namespace mindnet::plugin::slipbox::jobs
{
    using std::string;

    string& normalize_text_for_url(string& title)
    {
        std::replace(title.begin(), title.end(), ' ', '_');
        std::replace(title.begin(), title.end(), '"', '_');
        std::replace(title.begin(), title.end(), '/', '_');
        std::replace(title.begin(), title.end(), '#', ' ');
        return title;
    }

    string& replace_underscore_by_space(string& title)
    {
        std::replace(title.begin(), title.end(), '_', ' ');
        return title;
    }

    std::string replace_wikilinks(const std::string& input)
    {
        std::string out;
        out.reserve(input.size());

        for (size_t i = 0; i < input.size();)
        {
            if (i + 1 < input.size() && input[i] == '[' && input[i + 1] == '[')
            {
                size_t end = input.find("]]", i + 2);
                if (end != std::string::npos)
                {
                    std::string inner = input.substr(i + 2, end - (i + 2));
                    out += "<u style=\"color:blue\">" + inner + "</u>";
                    i = end + 2;
                    continue;
                }
            }
            out += input[i++];
        }

        return out;
    }

    std::string replace_placeholders(
        const std::string& text,
        const std::map<std::string, std::string>& values)
    {
        std::string result = text;

        for (const auto& [key, value] : values)
        {
            std::string placeholder = "{" + key + "}";
            size_t pos = 0;

            while ((pos = result.find(placeholder, pos)) != std::string::npos)
            {
                result.replace(pos, placeholder.length(), value);
                pos += value.length();
            }
        }

        return result;
    }

    string generate_breadcrumb(std::vector<string>& parents, string& current)
    {
        string result = "<a id href=\"index.html\">Home</a>\n";
        string path;
        string current_for_url = current;
        normalize_text_for_url(current_for_url);
        for (auto& p : parents)
        {
            if (p == current_for_url) break;
            if (!path.empty()) path += "/";
            path += p;

            result += "<a href=\"";
            result += path;
            result += "/index.html\">";
            result += replace_underscore_by_space(p);
            result += "</a>";
            result += "\n";
        }
        result += "<span id=\"panel_current\">";
        result += current;
        result += "</span>";
        result += "\n";
        return result;
    }

    string generate_hierarchy_panel(bool go_up, std::string& path,
                                    std::vector<plugins::slipbox::models::Note> children)
    {
        string result;
        if (go_up)
        {
            result += "<a href=\"";
            std::filesystem::path p(path);
            auto parent_path = p.parent_path().string();
            if (parent_path.empty()) parent_path = ".";
            result += parent_path;
            result += "/index.html\">Go Up</a>";
            result += "\n";
        }
        for (auto& note : children)
        {
            result += "<a href=\"";
            if (!path.empty())
            {
                result += path;
                result += "/";
            }
            string title_without_spaces_ = note.title;
            normalize_text_for_url(title_without_spaces_);

            result += title_without_spaces_;
            result += "/index.html\">";
            result += note.title;
            result += "</a>";
            result += "\n";
        }
        return result;
    }

    std::vector<identification> split_path_numbers(const std::string& s)
    {
        std::vector<identification> result;
        std::stringstream ss(s);
        std::string item;

        while (std::getline(ss, item, '/'))
        {
            if (item.empty()) continue;
            result.push_back(std::stoll(item));
        }

        return result;
    }

    std::string markdown_to_html(const std::string& md)
    {
        std::string step1 = replace_wikilinks(md);
        char* html = cmark_markdown_to_html(
            step1.c_str(),
            step1.size(),
            CMARK_OPT_DEFAULT
        );

        std::string result(html);
        free(html);

        return result;
    }

    std::string get_html_template()
    {
        return R"*****(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{map_name} - {note_title}</title>
    <meta name="description" content="{description}"/>
    <meta name="keywords" content="{keywords}"/>
    <meta name="author" content="{author}">

    <base href="{base_href}" target="_self">
    <link rel="stylesheet" href="styles.css">
    <link rel="icon" href="favicon.png" type="image/x-icon" sizes="32x32">
    <script type="text/javascript" src="script.js"></script>
</head>
<body onload="loadContent()">
<header>
    <div id="main_banner">{map_name}</div>
</header>

<!-- Combined Breadcrumb and Hierarchical Navigation Panel -->
<div id="breadcrumb_hierarchy_panel">
    <div id="breadcrumb">
        {breadcrumb}
    </div>
    <div id="hierarchy_panel">
        {hierarchy_panel}
    </div>
</div>

<main>
    <section>
        <h1>{note_title}</h1>

        <div id="tocButton"></div>
        <div id="toc"></div>

        {html_content}
    </section>
</main>

<footer>
    <p>Content is available under <a href="https://creativecommons.org/licenses/by-sa/4.0/" target="_blank"
                                     rel="noopener noreferrer">Creative Commons Attribution-ShareAlike 4.0 International
        License</a> unless otherwise noted.</p>
</footer>
</body>
</html>

)*****";
    }

    std::string get_styles_css()
    {
        return R"(
:root {
  --blue-bg-soft: #eef5ff;
  --blue-panel: #dae9ff;
  --blue-accent: #2f6fed;
  --blue-accent-hover: #1e5ddd;
  --blue-accent-deep: #0e3fa8;
  --blue-border: #c0d6f7;

  --text-dark: #1f1f1f;
  --text-medium: #4a4a4a;
}

/* Page */
body {
  padding: 20px;
  background-color: var(--blue-bg-soft);
  font-family: 'Segoe UI', Tahoma, Verdana, sans-serif;
  color: var(--text-medium);
  line-height: 1.65;
  margin: auto;
  max-width: 1200px;
}

/* Main banner */
#main_banner {
  background-color: var(--blue-panel);
  margin: 0;
  padding: 18px;
  text-align: center;
  border-top-left-radius: 12px;
  border-top-right-radius: 12px;
  box-shadow: 0 4px 10px rgba(0,0,0,0.12);
  font-size: 2em;
  font-weight: bold;
  color: var(--blue-accent-deep);
}

/* Breadcrumb panel */
#breadcrumb_hierarchy_panel {
  padding: 12px;
  background-color: #ffffff;
  font-size: 0.9em;
  margin-bottom: 20px;
  border-radius: 12px;
  border: 1px solid var(--blue-border);
  box-shadow: 0 2px 8px rgba(0,0,0,0.08);
}

#breadcrumb_hierarchy_panel a {
  color: var(--blue-accent);
  text-decoration: none;
}

#breadcrumb_hierarchy_panel a:hover {
  color: var(--blue-accent-hover);
  text-decoration: underline;
}

#breadcrumb {
  padding-bottom: 8px;
  border-bottom: 1px solid var(--blue-border);
  font-weight: bold;
}

#breadcrumb a {
  display: inline-block;
  padding: 6px 12px;
  background-color: var(--blue-panel);
  border-radius: 6px;
  color: var(--blue-accent-deep);
  text-decoration: none;
  margin: 4px;
  box-shadow: 0 2px 6px rgba(0,0,0,0.06);
  transition: 0.2s;
}

#breadcrumb a:hover {
  background-color: var(--blue-accent);
  color: #ffffff;
}

/* Hierarchy panel */
#hierarchy_panel {
  display: flex;
  flex-wrap: wrap;
  gap: 10px;
  padding-top: 10px;
}

#hierarchy_panel a {
  padding: 6px 12px;
  background-color: var(--blue-panel);
  border-radius: 6px;
  color: var(--blue-accent-deep);
  font-weight: bold;
  box-shadow: 0 2px 6px rgba(0,0,0,0.06);
  text-decoration: none;
  transition: 0.2s;
}

#hierarchy_panel a:hover {
  background-color: var(--blue-accent);
  color: #ffffff;
}

#breadcrumb::before {
  content: "Path: ";
  padding-right:42px;
  font-weight: bold;
}

#hierarchy_panel::before {
  padding-top:5px;
  content: "Subpages: ";
  font-weight: bold;
}

/* Navigation menu */
nav {
  padding: 15px;
  background-color: var(--blue-bg-soft);
  border-bottom-left-radius: 8px;
  border-bottom-right-radius: 8px;
  border-bottom: 3px solid var(--blue-border);
  box-shadow: 0 2px 6px rgba(0,0,0,0.05);
  margin-bottom: 20px;
}

nav a {
  color: var(--blue-accent);
  text-decoration: none;
  font-weight: bold;
  font-size: 110%;
}

nav a:hover {
  color: var(--blue-accent-hover);
  text-decoration: underline;
}

/* Main content */
main {
  padding: 20px;
  background-color: #ffffff;
  border-radius: 12px;
  box-shadow: 0 4px 10px rgba(0,0,0,0.12);
  min-height: 300px;
}

/* Footer */
footer {
  padding: 15px;
  background-color: var(--blue-panel);
  text-align: center;
  border-top: 3px solid var(--blue-border);
  margin-top: 20px;
  font-size: 0.9em;
  color: var(--blue-accent-deep);
}

/* Figures */
figure {
  border: 2px solid var(--blue-border);
  padding: 10px;
  max-width: 600px;
}

img { max-width: 100%; }

/* Tables */
table, th, td {
  border: 1px solid var(--blue-border);
}
th {
  background: var(--blue-panel);
  color: var(--blue-accent-deep);
}
th, td {
  padding: 6px;
}

/* Tags */
#tags li {
  background-color: var(--blue-accent);
  color: white;
  padding: 6px 12px;
  border-radius: 16px;
  font-size: 14px;
}

/* Code blocks */
pre {
  background: #eee;
  color: black;
  border: 2px solid #1f3b77;
  padding: 5px;
  font-size: 110%;
}

/* Toggle button */
#toggleButton {
  background-color: var(--blue-accent);
  color: white;
  padding: 10px 20px;
  font-size: 18px;
  border-radius: 6px;
}

#toggleButton:hover {
  background-color: var(--blue-accent-hover);
  transform: scale(1.04);
}

/* Links */
section a {
  color: var(--blue-accent);
}
section a:hover {
  color: var(--blue-accent-hover);
}

/* Ref links */
a.ref {
  color: var(--blue-accent-deep);
  background-color: var(--blue-bg-soft);
  border: 1px solid var(--blue-border);
  padding: 3px 7px;
  border-radius: 4px;
}

/* Blockquote */
blockquote {
  background: var(--blue-bg-soft);
  border: 2px solid var(--blue-panel);
  padding: 12px;
  border-radius: 8px;
}

)";
    }

    std::string get_script_js()
    {
        return R"(
function getCurrentRelativePath() {
    const url = window.location.pathname;
    return url.startsWith('/') ? url.substring(1) : url;
}

function setCookie(name, value, days) {
    const expires = new Date();
    expires.setDate(expires.getDate() + days);
    document.cookie = `${name}=${value}; expires=${expires.toUTCString()}; path=/`;
}

function getCookie(name) {
    const cookies = document.cookie.split(';').map(cookie => cookie.trim().split('='));
    const cookie = cookies.find(([key]) => key === name);
    return cookie ? cookie[1] : null;
}

function loadContent() {
    const toc = document.getElementById('toc');
    const tocButton = document.getElementById('tocButton');
    const headings = document.querySelectorAll('h2, h3, h4');

    const toggleButton = document.createElement('button');
    toggleButton.textContent = getCookie('tocShown') === 'true' ? 'Hide Content' : 'Show Content';
    //toggleButton.style.fontSize = "110%";
    toggleButton.style.padding = "5px";
    toggleButton.style.margin = "0px";
    toggleButton.id= "toggleButton"

    toggleButton.onclick = () => {
        const isShown = toc.style.display === 'block';
        toc.style.display = isShown ? 'none' : 'block';
        setCookie('tocShown', !isShown, 30);
        toggleButton.textContent = isShown ? 'Show Content' : 'Hide Content';
    };

    if (getCookie('tocShown') === 'true') {
        toc.style.display = 'block';
    } else {
        toc.style.display = 'none';
    }

    toc.innerHTML = '<h2 class="ignore-counter">Content:</h2><ul></ul>';
    const tocList = toc.querySelector('ul');
    let lastH2 = null, lastH3 = null;

    let someHeaders = false;

    headings.forEach((heading, index) => {
        if(!someHeaders) someHeaders = true;
        const listItem = document.createElement('li');
        const link = document.createElement('a');

        heading.id = `heading-${index}`;
        //link.href = "file:///" + getCurrentRelativePath() + `#heading-${index}`;
        link.href = `/${getCurrentRelativePath()}#heading-${index}`;
        link.textContent = heading.textContent;

        listItem.appendChild(link);

        if (heading.tagName === 'H2') {
            tocList.appendChild(listItem);
            lastH2 = document.createElement('ul');
            listItem.appendChild(lastH2);
            lastH3 = null;
        } else if (heading.tagName === 'H3' && lastH2) {
            lastH2.appendChild(listItem);
            lastH3 = document.createElement('ul');
            listItem.appendChild(lastH3);
        } else if (heading.tagName === 'H4' && lastH3) {
            lastH3.appendChild(listItem);
        }
    });
    if(someHeaders) {
    tocButton.appendChild(toggleButton);
    } else {
    toc.style.display = 'none';

}

    //a.ref
        document.querySelectorAll('a.ref').forEach(function (link) {
          link.setAttribute('target', '_blank');
        });

}
)";
    }
}