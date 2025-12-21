// ========================================
// Markdown Renderer (syntax highlight + emoji)
// ========================================

import {get_element} from "./dom.js";
import {defined} from "./common.js";

// MD

const md = window.markdownit({
    html: false,
    linkify: true,
    typographer: true,
    highlight: function (str, lang) {
        if (lang && window.hljs.getLanguage(lang)) {
            try {
                return '<pre class="hljs"><code>' +
                    window.hljs.highlight(str, {language: lang, ignoreIllegals: true}).value +
                    '</code></pre>';
            } catch (__) {
            }
        }
        return '<pre class="hljs"><code>' + md.utils.escapeHtml(str) + '</code></pre>';
    }
});

// plugin for emoji (:smile:, :rocket:, etc.)
md.use(window.markdownitEmoji);

// configure highlight.js appearance
window.hljs.configure({languages: ['cpp', 'js', 'json', 'html', 'sql', 'python']});

export function attachMarkdownEditor({
                                         textarea,
                                         buttonEdit,
                                         buttonRead
                                     }) {
    textarea.parentNode
        .querySelectorAll(".markdown_toolbar, .definition_md_rendered")
        .forEach(e => e.remove());
    {
        let div_height_10px = get_element("div_height_10px")
        if(defined(div_height_10px)) div_height_10px.remove()
    }

    if (!textarea) throw "attachMarkdownEditor: textarea is required";

    textarea.style.marginTop = "10px;"
    let div = document.createElement("div")
    div.style.height = "10px"
    div.id = "div_height_10px"
    buttonEdit.after(div)
    buttonRead.after(div)

    // --- create rendered markdown div ---
    const rendered = document.createElement("div");
    rendered.className = "definition_md_rendered";
    rendered.style.display = "none";
    rendered.style.border = "1px solid #ccc";
    rendered.style.padding = "8px";
    rendered.style.background = "#e6e6c5";
    rendered.style.whiteSpace = "normal";
    rendered.style.minHeight = "100px"

    rendered.style.maxHeight = "400px";
    rendered.style.overflowY = "auto";

    textarea.parentNode.insertBefore(rendered, textarea.nextSibling);

    // --- toolbar ---
    const toolbar = document.createElement("div");
    toolbar.className = "markdown_toolbar";
    toolbar.style.display = "none";
    toolbar.style.gap = "4px";
    toolbar.style.marginBottom = "6px";

    const buttons = [
        {
            html: "<strong>B</strong>",
            title: "Bold",
            before: "**",
            after: "**"
        },
        {
            html: "<em>I</em>",
            title: "Italic",
            before: "*",
            after: "*"
        },
        {
            html: "<u>U</u>",
            title: "Underline",
            before: "__",
            after: "__"
        },
        {
            html: "<span style='font-weight:500'>P</span>",
            title: "Paragraph",
            before: "\n\n",
            after: ""
        },
        {
            html: "<span style='font-weight:700'>H1</span>",
            title: "Heading 1",
            before: "# ",
            after: ""
        },
        {
            html: "<span style='font-weight:600'>H2</span>",
            title: "Heading 2",
            before: "## ",
            after: ""
        },
        {
            html: "<span style='font-weight:500'>H3</span>",
            title: "Heading 3",
            before: "### ",
            after: ""
        },
        {
            html: "<span>• List</span>",
            title: "List item",
            before: "- ",
            after: ""
        },
        {
            html: "<code>{ }</code>",
            title: "Inline code",
            before: "`",
            after: "`"
        },
        {
            html: "<code>```</code>",
            title: "Code block",
            before: "```\n",
            after: "\n```"
        },
        {
            html: "<span style='font-style:italic'>❝</span>",
            title: "Quote",
            before: "> ",
            after: ""
        },
        {
            html: "<span style='text-decoration:underline'>Link</span>",
            title: "Link",
            before: "[",
            after: "](url)"
        },
        {
            html: "<span style='letter-spacing:2px'>—</span>",
            title: "Divider",
            before: "\n---\n",
            after: ""
        }
    ];


    function insertMarkdown(before, after) {
        const start = textarea.selectionStart;
        const end = textarea.selectionEnd;
        const selected = textarea.value.substring(start, end);
        const text = before + selected + after;
        textarea.setRangeText(text, start, end, "end");
        textarea.focus();
    }

    buttons.forEach(cfg => {
        const btn = document.createElement("button");
        btn.type = "button";
        btn.title = cfg.title;
        btn.className = "markdown-btn";
        btn.innerHTML = cfg.html;
        btn.onclick = () => insertMarkdown(cfg.before, cfg.after);
        toolbar.appendChild(btn);
    });


    textarea.parentNode.insertBefore(toolbar, textarea);

    // --- modes ---
    function renderMarkdown() {
        rendered.innerHTML = md.render(textarea.value || "");
        textarea.style.display = "none";
        toolbar.style.display = "none";
        rendered.style.display = "block";

        if (buttonEdit) buttonEdit.style.display = "inline-block";
        if (buttonRead) buttonRead.style.display = "none";
    }

    function editMarkdown() {
        rendered.style.display = "none";
        textarea.style.display = "block";
        toolbar.style.display = "flex";

        if (buttonEdit) buttonEdit.style.display = "none";
        if (buttonRead) buttonRead.style.display = "inline-block";
    }

    // --- wire buttons ---
    if (buttonRead) buttonRead.onclick = renderMarkdown;
    if (buttonEdit) buttonEdit.onclick = editMarkdown;

    // --- initial state: READ ---
    if (textarea.value === "") {
        editMarkdown()
    } else {
        renderMarkdown();
    }

    // --- return handles (optional) ---
    const api = {
        renderMarkdown,
        editMarkdown,
        renderedDiv: rendered,
        toolbar
    };

    return api;
}
