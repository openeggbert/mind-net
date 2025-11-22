//
// Created by robertvokac on 11/15/25.
//

#include "mindnet/plugins/slipbox/jobs/HtmlExportJob.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/util/Utils.hpp"

#include <filesystem>

namespace mindnet::plugins::slipbox::jobs
{
    using_loggers()

    HtmlExportJob::HtmlExportJob()
        : Job(
            "HtmlExportJob",
            "HtmlExportJob",
            "@startup",
            true,
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

        return "";
    }

    std::string HtmlExportJob::get_html_template()
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
    <meta name="author" content="Robert Vokáč">

    <base href="{base_href}" target="_self">
    <link rel="stylesheet" href="styles.css">
    <link rel="icon" href="favicon.ico" type="image/x-icon" sizes="32x32">
    <script type="text/javascript" src="script.js"></script>
</head>
<body onload="loadContent()">
<header>
    <div id="main_banner">{map_name}</div>
    <nav>
        <ul>
            {main_banner}
        </ul>
    </nav>

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
    <h1>{note_title}</h1>

    <div id="tocButton"></div>
    <div id="toc"></div>

    {html_content}
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

    std::string HtmlExportJob::get_styles_css()
    {
               return R"(
:root {
  --dark-yellow-color: #f57f17;
  --light-orange-color: #fff3e0;
  --yellow-color: #ffeb3b;

}

/* Basic style for the entire page */
body {
  padding: 20px;
  background-color: #fff9e6; /* Light yellow background for the page */
  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; /* Modern and readable font */
  color: #333; /* Dark gray text color for good contrast */
  line-height: 1.6; /* Improved text readability */
  margin: auto;
  max-width: 1200px;
}

/* Style for the main banner */
#main_banner {
  background-color: var(--yellow-color); /* Light yellow for a prominent banner */
  margin: 0;
  padding: 15px; /* More space for better appearance */
  text-align: center; /* Center text */
  border-top-left-radius: 12px; /* Rounded corners for a modern look */
  border-top-right-radius: 12px; /* Rounded corners for a modern look */

  box-shadow: 0 6px 12px rgba(0,0,0,0.2); /* Soft shadow for emphasis */
  font-size: 2em; /* Increased font size for importance */
  font-weight: bold; /* Highlight text */
  color: #333; /* Dark gray text color for contrast */
}









/* Combined Breadcrumb and Hierarchical Navigation Panel */
#breadcrumb_hierarchy_panel {
  padding: 10px;
  background-color: #ffffff; /* Light yellow background */
  font-size: 0.9em;
  margin-bottom: 20px;
  border-radius: 8px; /* Rounded corners for a modern look */
  font-weight: normal;
  border: 0px solid #ffa967;
  box-shadow: 0 6px 12px rgba(0,0,0,0.2); /* Soft shadow for emphasis */
}

#panel_current {
  font-weight: bold;
}

#breadcrumb_hierarchy_panel a {
  color: var(--dark-yellow-color); /* Dark yellow color for links */
  text-decoration: none;
}

#breadcrumb_hierarchy_panel a:hover {
  text-decoration: underline;
  color: #e65100; /* Darker yellow color on hover */
}

#breadcrumb_hierarchy_panel span {
  color: #795548; /* Brown color for the current page */
}

#breadcrumb {
  padding-bottom: 8px;
  border-bottom: 1px solid var(--yellow-color);
  font-weight: bold;
}

#breadcrumb a {
  display: inline-block;
  padding: 6px 12px;
  background-color: var(--yellow-color);
  border-radius: 4px;
  color: var(--dark-yellow-color);
  text-decoration: none;
  font-weight: bold;
  margin-left: 4px;
  margin-right: 4px;
  margin-bottom: 8px;
  box-shadow: 0 4px 8px rgba(0,0,0,0.15);
  transition: background-color 0.3s, color 0.3s;
}

#breadcrumb a:hover {
  background-color: #e65100;
  color: #ffffff;
  text-decoration: none;
}


/* Hierarchical Navigation Panel */
#hierarchy_panel {
  display: flex; /* Flex layout for folder-like appearance */
  flex-wrap: wrap;
  gap: 10px; /* Space between items */
  padding-top: 10px;
}

#hierarchy_panel a {
  display: inline-block;
  padding: 6px 12px;
  background-color: var(--yellow-color); /* Light folder-like color */
  border-radius: 4px; /* Rounded corners for folder-like style */
  color: var(--dark-yellow-color); /* Dark yellow color for links */
  text-decoration: none;
  font-weight: bold;
  box-shadow: 0 4px 8px rgba(0,0,0,0.15); /* Soft shadow for depth */
}

#hierarchy_panel a:hover {
  background-color: #e65100; /* Darker yellow on hover */
  color: #ffffff; /* White text on hover */
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
  background-color: var(--light-orange-color); /* Light orange background for navigation */
  border-bottom-left-radius: 8px; /* Rounded corners for a modern look */
  border-bottom-right-radius: 8px; /* Rounded corners for a modern look */
  border-bottom: 3px solid #ffca28; /* Light yellow bottom border */
  margin-bottom: 20px; /* Space below navigation */
  box-shadow: 0 4px 8px rgba(0,0,0,0.1); /* Soft shadow for separation */
}

/* Navigation list style */
nav ul {
  list-style: none; /* Remove default bullet points */
  padding: 0; /* Remove default padding */
  margin: 0; /* Remove default margin */
  display: flex; /* Display list items in a row */
  justify-content: center; /* Center align list items */
}

/* Navigation link style */
nav li {
  margin: 0 15px; /* Space between list items */
  /*margin-bottom: 12px;*/ /* Space between list items */
  font-size: 1.2em; /* Increased font size for better readability */
}

/* Style for links in the navigation menu */
nav a {
  color: #ff6f00; /* Light orange color for links */
  text-decoration: none; /* No underline for a modern look */
  font-weight: bold; /* Highlight links */
  font-size: 110%;
}

nav a:hover {
  text-decoration: underline; /* Underline on hover */
  color: #e64a19; /* Darker orange color on hover */
}

/* Style for main content area */
main {
  padding: 20px;
  background-color: #ffffff; /* White background for contrasting content */
  border-radius: 12px; /* Rounded corners for a modern look */
  box-shadow: 0 6px 12px rgba(0,0,0,0.2); /* Soft shadow for emphasis */
  min-height:300px;
}

/* Style for lists */
section ul {
  margin-left: 20px; /* Space on the left side of the list */
}



/* Style for the footer */
footer {
  padding: 15px;
  background-color: var(--yellow-color); /* Light yellow for the footer */
  text-align: center; /* Center text */
  border-top: 3px solid #ffca28; /* Light yellow top border */
  margin-top: 20px; /* Space above footer */
  box-shadow: 0 -4px 8px rgba(0,0,0,0.1); /* Soft shadow for separation */
  font-size: 0.9em; /* Smaller font size for the footer */
  color: #333; /* Dark gray text color for contrast */
}

/* Responsive design for navigation menu and banner on small screens */
@media (max-width: 768px) {
  #main_banner {
    font-size: 1.5em; /* Smaller font size for the banner on smaller screens */
    padding: 10px; /* Reduce padding for better fit */
  }

  nav {
    padding: 10px; /* Reduce padding for navigation on smaller screens */
  }

  nav a {
    font-size: 0.9em; /* Smaller font size for navigation links */
  }

  nav ul {
    flex-direction: column; /* Stack list items vertically on smaller screens */
    align-items: center; /* Center align list items */
  }

  nav li {
    margin: 10px 0; /* Space between list items when stacked */
  }
  body {
     padding: 5px;
  }
}

figure {border: 2px solid rgba(140,140,140,0.2);padding:10px;max-width: 600px;margin-left:0px;}

img {max-width:100%!important;}

/* Tables */
table, th, td {
  border: 1px solid;
}
table {
   border-collapse: collapse;
   /*display: block;*/
   overflow-x: auto;
   margin-top: 20px;
   }

th {
  color: var(--dark-yellow-color);
  background: var(--light-orange-color);
}
th,td {
  padding: 5px;
}

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
    background-color: #007acc;
    color: white;
    padding: 6px 12px;
    border-radius: 16px;
    font-size: 14px;
    font-family: Arial, sans-serif;
    white-space: nowrap;
}

code,pre {
background: black;
color: white;
font-weight: bold;
padding: 10px;
border: 2px solid green;
margin-top: 5px;
display: block;
font-size: 125%;
}

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

h2{font-size:180%;}
h3{font-size:120%;}
/*h4{}*/


/*h1{}*/
h2{color:#4169E1;}
h3{color:#228B22;}
h4{color:#FF8C00;}
h5{color: #a600ff;}
h6{color: #c3b903;}

/*
body {
    counter-reset: h2-counter;
}

h2 {
    counter-reset: h3-counter;
    counter-increment: h2-counter;
}

h2.ignore-counter {
    counter-increment: none;
}


h2::before {
    content: counter(h2-counter) ". ";
}

h3 {
    counter-reset: h4-counter;
    counter-increment: h3-counter;
}

h3::before {
    content: counter(h2-counter) "." counter(h3-counter) " ";
}

h4 {
    counter-increment: h4-counter;
}

h4::before {
    content: counter(h2-counter) "." counter(h3-counter) "." counter(h4-counter) " ";
}

h5 {
  counter-increment: h5-counter;
}

h5::before {
  content: counter(h2-counter) "." counter(h3-counter) "." counter(h4-counter) "." counter(h5-counter) " ";
}

h6 {
  counter-increment: h6-counter;
}

h6::before {
  content: counter(h2-counter) "." counter(h3-counter) "." counter(h4-counter) "." counter(h5-counter) "." counter(h6-counter) " ";
}
/*/

.infobox {
float:right;border:1px solid black;text-align:left;margin-left:10px;
font-size:75%;
}
br {margin-bottom:10px;}

.infobox tr:first-child th:first-child {
text-align:center;font-size:125%;
}
section a:hover {
  background: #fffd8d;
  transition: background 0.3s ease;
}


#toggleButton {
  background-color: #ff9800;
  color: white;
  padding: 10px 20px;
  font-size: 18px;
  border: none;
  border-radius: 5px;
  cursor: pointer;
  transition: all 0.3s ease-in-out;
}

#toggleButton:hover {
  background-color: #e65100;
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
  color: #0056b3;
  background-color: #eef5ff;
  padding: 0 8px 0 8px;
  border-radius: 3px;
  text-decoration: underline;
  font-weight: bold;
  border: 1px solid blue;
  margin-left: 5px;
  margin-right: 5px;
}
a.ref::before {
  content: "Ref: ";
}
blockquote {
background: #fff3e0; border: 2px solid orange; padding: 10px;
}

)";
    }

    std::string HtmlExportJob::get_script_css()
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

    using_loggers()
}
