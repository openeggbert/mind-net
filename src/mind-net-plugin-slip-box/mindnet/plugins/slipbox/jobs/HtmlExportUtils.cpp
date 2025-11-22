//
// Created by robertvokac on 11/22/25.
//
#include "mindnet/plugins/slipbox/jobs/HtmlExportUtils.hpp"

#include "mindnet/plugins/slipbox/models/Note.hpp"

namespace mindnet::plugin::slipbox::jobs
{
    using std::string;

    string& title_without_spaces(string& title)
    {
        std::replace(title.begin(), title.end(), ' ', '_');
        return title;
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
        return "";
    }

    string generate_hierarchy_panel(std::string& go_up, std::string& path,
                                    std::vector<plugins::slipbox::models::Note> children)
    {
        string result;
        if (!go_up.empty())
        {
            result += "<a href=\"";
            std::filesystem::path p(path);
            result += p.parent_path().string();
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
            auto title_without_spaces_ = title_without_spaces(note.title);
            result += title_without_spaces_;
            result += "/index.html\">";
            result += note.title;
            result += "</a>";
            result += "\n";
        }
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
    <title>{note_title}</title>
    <meta name="description" content="{description}"/>
    <meta name="keywords" content="{keywords}"/>
    <meta name="author" content="{author}">

    <base href="{base_href}" target="_self">
    <link rel="stylesheet" href="styles.css">
    <link rel="icon" href="favicon.ico" type="image/x-icon" sizes="32x32">
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
  --sky-dark: #1E90FF;       /* Dark blue for links */
  --sky-medium: #87CEEB;     /* Main banner / highlight */
  --sky-light: #E3F6FF;      /* Light blue background */
  --sky-hover: #0A66C2;      /* Hover deep blue */
  --sky-border: #B5E5FF;     /* Soft border blue */
}

/* Basic style for the entire page */
body {
  padding: 20px;
  background-color: var(--sky-light);
  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  color: #333;
  line-height: 1.6;
  margin: auto;
  max-width: 1200px;
}

/* Style for the main banner */
#main_banner {
  background-color: var(--sky-medium);
  margin: 0;
  padding: 15px;
  text-align: center;
  border-top-left-radius: 12px;
  border-top-right-radius: 12px;
  box-shadow: 0 6px 12px rgba(0,0,0,0.2);
  font-size: 2em;
  font-weight: bold;
  color: #003366;
}

/* Combined Breadcrumb and Hierarchical Navigation Panel */
#breadcrumb_hierarchy_panel {
  padding: 10px;
  background-color: #ffffff;
  font-size: 0.9em;
  margin-bottom: 20px;
  border-radius: 8px;
  font-weight: normal;
  border: 0px solid var(--sky-border);
  box-shadow: 0 6px 12px rgba(0,0,0,0.2);
}

#panel_current {
  font-weight: bold;
}

#breadcrumb_hierarchy_panel a {
  color: var(--sky-dark);
  text-decoration: none;
}

#breadcrumb_hierarchy_panel a:hover {
  text-decoration: underline;
  color: var(--sky-hover);
}

#breadcrumb_hierarchy_panel span {
  color: #003366;
}

#breadcrumb {
  padding-bottom: 8px;
  border-bottom: 1px solid var(--sky-medium);
  font-weight: bold;
}

#breadcrumb a {
  display: inline-block;
  padding: 6px 12px;
  background-color: var(--sky-medium);
  border-radius: 4px;
  color: var(--sky-dark);
  text-decoration: none;
  font-weight: bold;
  margin-left: 4px;
  margin-right: 4px;
  margin-bottom: 8px;
  box-shadow: 0 4px 8px rgba(0,0,0,0.15);
  transition: background-color 0.3s, color 0.3s;
}

#breadcrumb a:hover {
  background-color: var(--sky-hover);
  color: #ffffff;
  text-decoration: none;
}

/* Hierarchical Navigation Panel */
#hierarchy_panel {
  display: flex;
  flex-wrap: wrap;
  gap: 10px;
  padding-top: 10px;
}

#hierarchy_panel a {
  display: inline-block;
  padding: 6px 12px;
  background-color: var(--sky-medium);
  border-radius: 4px;
  color: var(--sky-dark);
  text-decoration: none;
  font-weight: bold;
  box-shadow: 0 4px 8px rgba(0,0,0,0.15);
}

#hierarchy_panel a:hover {
  background-color: var(--sky-hover);
  color: #ffffff;
  text-decoration: none;
}

#breadcrumb::before, #hierarchy_panel::before {
  font-weight: normal;
  color: #666;
  font-style: normal;
}

#breadcrumb::before {
  content: "Path: ";
  padding-right:42px;
}

#hierarchy_panel::before {
  padding-top:5px;
  content: "Subpages: ";
}

/* Style for navigation menu */
nav {
  padding: 15px;
  background-color: var(--sky-light);
  border-bottom-left-radius: 8px;
  border-bottom-right-radius: 8px;
  border-bottom: 3px solid var(--sky-medium);
  margin-bottom: 20px;
  box-shadow: 0 4px 8px rgba(0,0,0,0.1);
}

nav ul {
  list-style: none;
  padding: 0;
  margin: 0;
  display: flex;
  justify-content: center;
}

nav li {
  margin: 0 15px;
  font-size: 1.2em;
}

nav a {
  color: var(--sky-dark);
  text-decoration: none;
  font-weight: bold;
  font-size: 110%;
}

nav a:hover {
  text-decoration: underline;
  color: var(--sky-hover);
}

/* Style for main content area */
main {
  padding: 20px;
  background-color: #ffffff;
  border-radius: 12px;
  box-shadow: 0 6px 12px rgba(0,0,0,0.2);
  min-height:300px;
}

/* Lists */
section ul {
  margin-left: 20px;
}

/* Footer */
footer {
  padding: 15px;
  background-color: var(--sky-medium);
  text-align: center;
  border-top: 3px solid var(--sky-border);
  margin-top: 20px;
  box-shadow: 0 -4px 8px rgba(0,0,0,0.1);
  font-size: 0.9em;
  color: #003366;
}

/* Responsive */
@media (max-width: 768px) {
  #main_banner {
    font-size: 1.5em;
    padding: 10px;
  }

  nav {
    padding: 10px;
  }

  nav a {
    font-size: 0.9em;
  }

  nav ul {
    flex-direction: column;
    align-items: center;
  }

  nav li {
    margin: 10px 0;
  }

  body {
     padding: 5px;
  }
}

/* Images & Figures */
figure {
  border: 2px solid var(--sky-border);
  padding:10px;
  max-width: 600px;
  margin-left:0px;
}

img {max-width:100%!important;}

/* Tables */
table, th, td {
  border: 1px solid;
}

table {
   border-collapse: collapse;
   overflow-x: auto;
   margin-top: 20px;
}

th {
  color: #003366;
  background: var(--sky-light);
}

th,td {
  padding: 5px;
}

/* Tags */
#tags {
    list-style: none;
    padding: 0;
    display: flex;
    gap: 8px;
    border-top: 2px solid #ddd;
    padding-top: 20px;
    margin-top: 60px;
}

#tags::before {
  content: "Tags: ";
  font-weight: bold;
  padding-top: 5px;
}

#tags li {
    background-color: var(--sky-dark);
    color: white;
    padding: 6px 12px;
    border-radius: 16px;
    font-size: 14px;
    font-family: Arial, sans-serif;
    white-space: nowrap;
}

/* Code blocks */
code,pre {
  background: #000;
  color: #fff;
  font-weight: bold;
  padding: 10px;
  border: 2px solid var(--sky-dark);
  margin-top: 5px;
  display: block;
  font-size: 125%;
}

/* TOC */
#toc {
	border: 1px solid #ccc;
	padding: 5px;
	margin-bottom: 20px;
	background-color: #f9f9f9;
	font-size: 90%;
}

#tocButton {
	margin-bottom: 20px;
}

/* Headers */
h1{font-size:250%; border-bottom:0;}
main section h1{border-bottom: 2px solid #bbb;}
h2{font-size:180%; color:#4169E1;}
h3{font-size:120%; color:#228B22;}
h4{color:#FF8C00;}
h5{color: #a600ff;}
h6{color: #c3b903;}

/* Infobox */
.infobox {
  float:right;
  border:1px solid #000;
  text-align:left;
  margin-left:10px;
  font-size:75%;
}

.infobox tr:first-child th:first-child {
  text-align:center;
  font-size:125%;
}

section a:hover {
  background: #dff3ff;
  transition: background 0.3s ease;
}

/* Toggle Button */
#toggleButton {
  background-color: var(--sky-dark);
  color: white;
  padding: 10px 20px;
  font-size: 18px;
  border: none;
  border-radius: 5px;
  cursor: pointer;
  transition: all 0.3s ease-in-out;
}

#toggleButton:hover {
  background-color: var(--sky-hover);
  transform: scale(1.05);
}

section a {
  text-decoration: none;
  color: #0645AD;
}

section a:hover {
  text-decoration: underline;
  color: #0645AD;
}

a.ref {
  color: var(--sky-dark);
  background-color: #eef5ff;
  padding: 0 8px;
  border-radius: 3px;
  text-decoration: underline;
  font-weight: bold;
  border: 1px solid var(--sky-dark);
  margin-left: 5px;
  margin-right: 5px;
}

a.ref::before {
  content: "Ref: ";
}

/* Blockquote */
blockquote {
  background: var(--sky-light);
  border: 2px solid var(--sky-medium);
  padding: 10px;
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
