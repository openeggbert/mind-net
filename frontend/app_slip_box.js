// ========================================
// Imports & Globals
// ========================================
import {
    delete_entity,
    getTitleCache, getUserId,
    list_all_entities,
    post_entity,
    put_entity,
    read_entity,
    setTitleCache
} from "./api.js";
import {
    makeEnum, sleep_for_seconds, hide_element, hide_elements, get_element, set_value, copy_to_clipboard,
    chooseOption, show_elements, show_or_hide_elements, show_or_hide_element, show_element, saveToLocalStorage,
    formatDateTimeHM, formatDateTime, showInfo, showError
} from "./dom.js";
import {Autocomplete} from "./common.js";

let map_id = "";
let note_id = "";
let map = null;
let note = null;
let original_map = null;
let original_note = null;
let original_content_value = null;
let mode_maps = false;
let mode_root = false;
let mode_notes = false;
let wasDragged = false;
let suppressPopstate = false;
const simple = document.getElementById("app").dataset.version === "simple";
const rich = !simple
const panels = ["parent", "current", "meta", "children"]


// ========================================
// Markdown Renderer (syntax highlight + emoji)
// ========================================
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

md.use(function (md) {
    const defaultRender = md.renderer.rules.link_open || function (tokens, idx, options, env, self) {
        return self.renderToken(tokens, idx, options);
    };

    md.renderer.rules.link_open = function (tokens, idx, options, env, self) {
        const hrefIndex = tokens[idx].attrIndex('href');
        if (hrefIndex >= 0) {
            const href = tokens[idx].attrs[hrefIndex][1];
            if (href.startsWith('#wanted:')) {
                tokens[idx].attrPush(['class', 'note-wanted']);

                const title = decodeURIComponent(href.substring('#wanted:'.length));
                tokens[idx].attrPush(['data-title', title]);

                tokens[idx].attrs[hrefIndex][1] = '#';

            } else if (href.startsWith('#unknown:')) {
                tokens[idx].attrPush(['class', 'note-unknown']);

                const title = decodeURIComponent(href.substring('#unknown:'.length));
                tokens[idx].attrPush(['data-title', title]);

                tokens[idx].attrs[hrefIndex][1] = '#';
            }

        }
        return defaultRender(tokens, idx, options, env, self);
    };
});

document.addEventListener("click", e => {
    const a = e.target.closest("a.note-wanted, a.note-unknown");
    if (!a) return;

    e.preventDefault();

    const title = a.getAttribute("data-title") || a.textContent || "";

    if (a.classList.contains("note-wanted")) {
        show_error(`Note "${title}" does not yet exist.`);
        copy_to_clipboard(title);
    } else if (a.classList.contains("note-unknown")) {
        show_error(`Note "${title}" maybe exists, but is not yet linked. Save and reload to update.`);
    }
});


// configure highlight.js appearance
window.hljs.configure({languages: ['cpp', 'js', 'json', 'html', 'sql', 'python']});


// ========================================
// Panels
// ========================================

const set_flag = (key, value) => value ? localStorage.setItem(key, "true") : localStorage.removeItem(key);
const get_flag = key => localStorage.getItem(key) === "true";
const setPanelCollapsed = (name, value) => set_flag(`slip_box.${name}_panel_collapsed`, value);
const getPanelCollapsed = name => get_flag(`slip_box.${name}_panel_collapsed`);


export function togglePanel(element, id) {
    const el = get_element(id);
    const isCollapsed = el.classList.toggle('collapsed');
    let panel = id.replace("_content", "")
    panels.includes(panel) ? setPanelCollapsed(panel, isCollapsed) : console.warn("togglePanel() does not know id: " + id)

    if (element) element.textContent = simple
        ? (isCollapsed ? "▶ Expand" : "▼ Collapse")
        : (isCollapsed ? "▶" : "▼");

    if (id === "meta_content" && !simple && window.innerWidth > 800) {
        const metaPanel = document.getElementById("meta");
        const metaLabel = document.getElementById("meta_label");
        if (isCollapsed) {
            Object.assign(metaPanel.style, {
                maxWidth: "50px", minWidth: "50px", overflow: "hidden"
            });
            metaLabel.style.display = "none";
        } else {
            ["max-width", "min-width", "overflow"].forEach(p => metaPanel.style.removeProperty(p));
            metaLabel.style.display = "inline";
        }
    }
}

window.togglePanel = togglePanel;
export const button_focus_onclick = () => document.body.classList.toggle("focus-mode");
window.button_focus_onclick = button_focus_onclick;

// ========================================
// Window
// ========================================

function makeDraggable(el) {
    const header = el.querySelector('.window_container-header');
    let offsetX = 0, offsetY = 0, dragging = false;

    function startDrag(x, y, ev) {
        // stop fake drags
        if (ev && ev.buttons !== 1) return;

        console.log("START DRAG", {display: el.style.display, left: el.style.left, top: el.style.top});

        if (el.style.display === "none") return;

        const rect = el.getBoundingClientRect();
        dragging = true;
        wasDragged = true;

        offsetX = x - rect.left;
        offsetY = y - rect.top;

        el.style.position = "fixed";
        el.style.transform = "none";
    }


    function doDrag(x, y, ev) {
        if (document.body._forceStopDragging) {
            dragging = false;
            document.body._forceStopDragging = false;
            return;
        }
        if (!dragging) return;
        if (ev && ev.buttons !== 1) {
            dragging = false;
            return;
        }
        el.style.left = `${x - offsetX}px`;
        el.style.top = `${y - offsetY}px`;
    }


    function stopDrag() {
        console.log("STOP DRAG");
        dragging = false;
    }

    // --- Mouse support ---
    header.addEventListener('mousedown', e => startDrag(e.clientX, e.clientY, e));
    document.addEventListener('mousemove', e => doDrag(e.clientX, e.clientY, e));
    document.addEventListener('mouseup', stopDrag);

    // --- Touch support ---
    header.addEventListener('touchstart', e => {
        if (e.target.closest('.window-close')) return;

        const t = e.touches[0];
        startDrag(t.clientX, t.clientY);
        e.preventDefault();
    }, {passive: false});

    document.addEventListener('touchmove', e => {
        const t = e.touches[0];
        doDrag(t.clientX, t.clientY);
    }, {passive: false});

    document.addEventListener('touchend', stopDrag);
}


window.closeWindow = closeWindow;


export function closeWindow() {
    const win = document.getElementById('window_container');

    console.log("CLOSE WINDOW before", {
        display: win.style.display,
        left: win.style.left,
        top: win.style.top,
        transform: win.style.transform
    });

    wasDragged = false;
    win.hidden = true;

    win.style.display = 'none';
    win.style.left = "";
    win.style.top = "";
    win.style.transform = "translate(-50%, -50%)";
    win.style.position = "fixed";

    setTimeout(() => win.hidden = false, 50);
}


export function showWindow() {
    const win = document.getElementById("window_container");
    document.body._forceStopDragging = true;

    win.style.display = "block";
    win.style.position = "fixed";
    win.style.left = "50%";
    win.style.top = "50%";
    win.style.transform = "translate(-50%, -50%)";

    requestAnimationFrame(() => {
        const rect = win.getBoundingClientRect();

        win.style.setProperty("--win-init-w", rect.width + "px");
        win.style.setProperty("--win-init-h", rect.height + "px");

        if (!wasDragged) {
            win.style.left = "calc(50% - (var(--win-init-w) / 2))";
            win.style.top = "calc(50% - (var(--win-init-h) / 2))";
            win.style.transform = "none";
        }
    });
}


window.showWindow = showWindow;
export const clearWindow = () => document.getElementById("window_container_content").innerHTML = "";
window.clearWindow = clearWindow;
export const setWindowTitle = title => document.getElementById("window_container_title").innerText = title;
export const getWindowContent = () => document.getElementById("window_container_content");
export const setWindowContent = text => getWindowContent().textContent = text;


export function setWindowContentByUrl(url) {
    clearWindow();
    let iframe = document.createElement("iframe");
    iframe.src = url;
    iframe.scroling = "no"
    iframe.style.display = "block";
    iframe.style.width = "100%";
    iframe.style.height = "100%";

    getWindowContent().appendChild(iframe);
    getWindowContent().style.height = "100%";
}

export function showWindowFrom(title, url) {
    clearWindow()
    setWindowTitle(title)
    if (url === undefined) {
        alert("url is required");
        return;
    }
    setWindowContentByUrl(url)
    showWindow();
}

// ========================================
// Utils
// ========================================

// --- Difficulty ---
export const Difficulty = makeEnum({
    None: 0,
    Easy: 1,
    Medium: 2,
    Hard: 3,
    Expert: 4
});

// --- Importance ---
export const Importance = makeEnum({
    None: 0,
    Low: 1,
    Medium: 2,
    High: 3
});

function ensure_toast_container() {
    let container = document.getElementById("toast_container");
    if (!container) {
        container = document.createElement("div");
        container.id = "toast_container";
        document.body.appendChild(container);
    }
    return container;
}

export function show_toast(message, type = "info") {
    const container = ensure_toast_container();

    const toast = document.createElement("div");
    toast.className = `toast ${type}`;
    toast.textContent = message;
    container.appendChild(toast);

    requestAnimationFrame(() => toast.classList.add("show"));

    setTimeout(() => {
        toast.classList.remove("show");
        setTimeout(() => toast.remove(), 300);
    }, 3000);
}

window.showToast = show_toast;

const makeShow = type => msg => show_toast(msg, type);

const show_info = makeShow("info");
const show_warn = makeShow("warn");
const show_error = makeShow("error");

export async function refresh_page() {
    await render(); // redraws current state
}

function init_from_http_parameters() {
    const params = parse_url_params();
    mode_maps = mode_root = mode_notes = false;

    if (params.map_id) {
        map_id = params.map_id;
        mode_root = true;
    } else if (params.note_id) {
        note_id = params.note_id;
        mode_notes = true;
    } else {
        mode_maps = true;
    }
}

function parse_url_params() {
    const params = new URLSearchParams(window.location.search);
    return Object.fromEntries(params.entries());
}

function set_url_params(params, replace = false) {
    const query = new URLSearchParams(params).toString();
    const new_url = `${window.location.pathname}?${query}`;
    if (replace) history.replaceState(params, "", new_url);
    else history.pushState(params, "", new_url);
}

export async function navigate_to(params) {
    suppressPopstate = true;
    set_url_params(params);
    await render();
    // suppress popstate for a short time (ca 100ms)
    setTimeout(() => suppressPopstate = false, 200);
}


window.navigate_to = navigate_to;

window.addEventListener("popstate", () => {
    if (!suppressPopstate) render();
});


// ========================================
// Main (DOMContentLoaded)
// ========================================

document.addEventListener('DOMContentLoaded', async () => {
    init_dom();
    await render();
});

function init_dom() {
    get_element("slipbox_header").title = "Go to list of all maps"
    get_element("slipbox_header").style.cursor = "pointer"
    get_element("button_mindnet").addEventListener("click", () => {
        window.location.href = 'index.html'
    });
    get_element("button_mindnet").title = "Go to Mind Net generic frontend"
    get_element("button_previous").title = "Previous sibling by order";
    get_element("button_next").title = "Next sibling by order";
    if (!simple) {
        get_element("button_focus").addEventListener("click", button_focus_onclick);
        get_element("button_focus").title = "Turn on/off focus mode"
    }
    get_element("button_jump_to_note").addEventListener("click", () => {
        const input = prompt("Enter note ID");
        if (input === null) return; // canceled

        const id = String(input).trim();
        if (!/^\d+$/.test(id)) {
            show_error("Note ID must be a whole number.");
            return;
        }

        navigate_to({note_id: id}); // SPA navigation → immediately re-renders the given note
    });


    get_element("button_theme").addEventListener("click", () => {
        document.body.classList.toggle("dark");
    });
    get_element("button_theme").title = "Switch dark/light theme"


    panels.forEach(panel => {
        if (rich || panel !== "parent") {
            get_element("collapsible-toggle-" + panel)
                .addEventListener("click", (e) => togglePanel(e.target, panel + "_content"));
        }
    })

    let toggle_parent = document.getElementById("collapsible-toggle-parent");
    let toggle_current = document.getElementById("collapsible-toggle-current");
    let toggle_meta = document.getElementById("collapsible-toggle-meta");
    let toggle_children = document.getElementById("collapsible-toggle-children");

    let meta_collapsed = false;
    if (window.innerWidth <= 800) {
        meta_collapsed = true;
        togglePanel(toggle_meta, 'meta_content');
    }

    if (!simple && getPanelCollapsed("parent")) togglePanel(toggle_parent, 'parent_content')
    if (getPanelCollapsed("current") && !meta_collapsed) togglePanel(toggle_current, 'current_content')
    if (getPanelCollapsed("meta")) togglePanel(toggle_meta, 'meta_content')
    if (getPanelCollapsed("children")) togglePanel(toggle_children, 'children_content')

    get_element("children_button_refresh").addEventListener("click", refresh_page)
    // document.querySelectorAll('.add').forEach(btn => {
    //     btn.addEventListener('click', () => show_toast('New child added'));
    // });
    //
    const win = document.getElementById('window_container');
    makeDraggable(win);
}

let beforeUnloadHandler = null;

async function link_to(a, params) {
    // always update href
    a.href = "?" + new URLSearchParams(params).toString();

    // add listener only once, but don't store 'params' in closure
    if (!a._hasLinkListener) {
        a.addEventListener("click", async (event) => {
            event.preventDefault();

            // get current href
            const url = new URL(a.href, window.location.origin);
            const p = Object.fromEntries(url.searchParams.entries());
            await navigate_to(p);
        });
        a._hasLinkListener = true;
    }
}

let render_number = 0

async function render() {
    // TODO split into renderParent(), renderCurrent(), renderMeta(), renderChildren().
    render_number++;
    console.log("render() #" + render_number + " called", performance.now());

    note = null;
    original_note = null;
    original_content_value = null;
    console.log("render note_id", note_id, "note=", note);

    init_from_http_parameters();
    const local_note_id = note_id;

    let mode_root_or_notes = mode_root || mode_notes;

    let note_navigation = mode_notes ? await read_entity("note_navigation", local_note_id) : null;
    //if(mode_notes) alert(JSON.stringify(note_navigation));

    get_element("button_previous").onclick = async () => {
        if (!mode_notes) return
        if (note_navigation === null || note_navigation === undefined) return;

        if (note_navigation.prev_note_id === 0) {
            //show_warn("There is no previous note in the map.")
            await navigate_to({map_id: map_id});
            return;
        }

        await navigate_to({note_id: note_navigation.prev_note_id});
    }
    get_element("button_next").onclick = async () => {

        if (!mode_notes) {
            let children_url = get_element("children_ul")

            const firstLi = children_ul.querySelector("li");
            if (!firstLi) {
                show_warn("This map has no note")
                return
            }

            const text = firstLi.innerText.trim();

            let sliced = text.slice(1);

            const spaceIndex = sliced.indexOf(" ");
            sliced = sliced.substring(0, spaceIndex);
            await navigate_to({note_id: sliced});

        }

        if (note_navigation === null || note_navigation === undefined) return;

        if (note_navigation.next_note_id === 0) {
            show_warn("There is no next note in the map.")
            return;
        }

        await navigate_to({note_id: note_navigation.next_note_id});
    }

    // Header
    show_or_hide_elements(mode_root_or_notes, "button_previous", "button_next", "button_focus")

    // Breadcrumb
    let breadcrumb = get_element("breadcrumb")
    breadcrumb.innerHTML = ""
    let dir = document.createElement("span");
    dir.innerText = "📂 ";
    breadcrumb.appendChild(dir);

    let home = document.createElement("a");
    home.href = "?";
    home.innerText = "Home";
    breadcrumb.appendChild(home);
    home.onclick = async () => {
        event.preventDefault();
        await navigate_to({});
    }


//     <div class="breadcrumb">
//             📂 <a href="?">Home</a> › <a href="#">C++</a> › <a href="#">Constructors</a> › <strong>Note #43453</strong>
// </div>

    // Parent
    show_or_hide_element(mode_root_or_notes, "parent")

    if (mode_root_or_notes) {
        original_note = mode_notes ? await read_entity("note", local_note_id) : null;
        note = mode_notes ? structuredClone(original_note) : null;
        let parent_note = mode_notes ? (note.parent_note_id === 0 ? null : await read_entity("note", note.parent_note_id)) : null;
        let has_parent = mode_notes ? note.parent_note_id !== 0 : null;

        if (mode_root) set_value("parent_label", "All maps")
        if (mode_notes) set_value("parent_label", has_parent ? "Parent Note" : "Parent Map")

        show_or_hide_elements(mode_notes, "parent_id_label", "parent_id")
        map = mode_notes ? await read_entity("map", note.map_id) : null;
        if (mode_notes) set_value("parent_id", has_parent ? note.parent_note_id : note.map_id);

        let parent_title = document.getElementById("parent_title");
        if (mode_root) {
            parent_title.innerText = "All maps";
            await link_to(parent_title, {}); // go to homepage without parameters
        }
        if (mode_notes) {
            parent_title.innerText = has_parent ? parent_note.title : map.name;
            await link_to(parent_title, has_parent ? {note_id: note.parent_note_id} : {map_id: note.map_id});
        }

        get_element("parent_button_copy").onclick = function () {
            if (mode_root) copy_to_clipboard(parent_title.href);
            if (mode_notes) copy_to_clipboard(has_parent ? note.parent_note_id : note.map_id);
        }

        get_element("parent_button_edit").onclick = async function () {
            if (mode_root) {
                showWindowFrom("List of maps", "index.html?entity=map&action=list")
                return;
            }
            //mode_notes
            if (!has_parent && false) {
                let confirmed = confirm("Are you sure you want to set a parent for this note? Now it has now parent.");
                if (!confirmed) {
                    return;
                }
            }
            let new_parent_note_id = prompt("Enter new parent note ID");

            if (new_parent_note_id !== undefined && new_parent_note_id !== null) {
                note.parent_note_id = new_parent_note_id;

                parent_note = note.parent_note_id === "0" ? null : await read_entity("note", note.parent_note_id);
                has_parent = note.parent_note_id !== "0";

                set_value("parent_label", has_parent ? "Parent Note" : "Parent Map")
                set_value("parent_id", has_parent ? note.parent_note_id : note.map_id);

                let parent_title = document.getElementById("parent_title");

                let map_ = await read_entity("map", note.map_id);
                parent_title.innerText = has_parent ? parent_note.title : map_.name;
                await link_to(parent_title, has_parent ? {note_id: note.parent_note_id} : {map_id: note.map_id});
            }
        }
    }

    // Current
    show_or_hide_element(mode_root_or_notes, "current")

    if (mode_root_or_notes) {
        set_value("current_label", mode_root ? "Map" : "Note")
        set_value("current_id", mode_root ? map_id : local_note_id)

        get_element("current_title").onclick = function () {
            if (mode_root) showWindowFrom("Detail of map #" + map_id, "index.html?entity=map&action=read&id=" + map_id)
            if (mode_notes) showWindowFrom("Detail of note #" + local_note_id, "index.html?entity=note&action=read&id=" + local_note_id)
        }

        if (mode_root) map = await read_entity("map", map_id);
        set_value("current_title", mode_root ? map.name : note.title);
        let current_title = document.getElementById("current_title");
        current_title.style.display = "inline-block";
        current_title.style.minWidth = "50px";

        //breadcrumb
        {
            let separator = document.createElement("span");
            separator.innerText = " › "
            breadcrumb.appendChild(separator);

            let breadcrumb_map = document.createElement("a");
            breadcrumb_map.innerText = map.name;
            breadcrumb_map.href = "?";
            breadcrumb.appendChild(breadcrumb_map);
            breadcrumb_map.onclick = async function () {
                event.preventDefault();
                await navigate_to({map_id: map.id});
            }
        }
        if (mode_notes) {
            const path_as_ids = note.path
                .split('/')
                .filter(x => x.trim() !== "")
                .map(x => Number(x));

            for (const x of path_as_ids) {
                let title = getTitleCache("note", x)
                if (title === null || title === undefined) {

                    let note_ = await read_entity("note", x)
                    if (note_ === null) {
                        show_warn("Loading note with id " + x + " failed.");
                        title = "Unknown (#" + x + ")"
                    } else {
                        title = note_.title
                        setTitleCache("note", x, title);
                    }
                }

                let separator = document.createElement("span");
                separator.innerText = " › "
                breadcrumb.appendChild(separator);

                let breadcrumb_note = document.createElement("a");
                breadcrumb_note.innerText = title;
                breadcrumb_note.href = "?";
                breadcrumb.appendChild(breadcrumb_note);
                breadcrumb_note.onclick = async function () {
                    event.preventDefault();
                    await navigate_to({note_id: x});
                }
            }
        }

        get_element("current_button_rename").onclick = function () {
            if (mode_root) {
                show_error("Not yet implemented");
                return;
            }
            // mode_notes
            let new_name = prompt("Enter new name", note.title);
            if (new_name !== undefined && new_name !== null) {
                note.title = new_name;
                set_value("current_title", note.title);
                setTitleCache("note", note.id, note.title);
            }
        }

        get_element("current_button_copy").onclick = function () {
            copy_to_clipboard(mode_root ? map_id : local_note_id)
        }

        show_or_hide_element(mode_notes, "current_button_edit")
        hide_element("current_button_read")

        show_or_hide_element(mode_notes, "current_hint_label")
        show_or_hide_element(mode_notes, "current_hint")
        show_or_hide_element(mode_notes, "current_button_edit_hint")
        if (mode_notes) {
            get_element("current_hint").innerText = note.hint
        }
        get_element("current_button_edit_hint").onclick = function () {
            if (!mode_notes) return;

            let new_hint = prompt("Enter new hint", note.hint);
            if (new_hint !== undefined && new_hint !== null) {
                note.hint = new_hint;
                set_value("current_hint", note.hint);
            }
        }


        // Always recreate a fresh <textarea> to avoid value persistence between notes
        {
            const oldTextarea = get_element("current_textarea");
            if (oldTextarea) {
                const newTextarea = document.createElement("textarea");
                newTextarea.id = "current_textarea";
                newTextarea.className = oldTextarea.className;
                newTextarea.placeholder = oldTextarea.placeholder;
                newTextarea.rows = oldTextarea.rows;
                newTextarea.cols = oldTextarea.cols;
                newTextarea.style.width = oldTextarea.style.width;
                newTextarea.style.height = oldTextarea.style.height;
                oldTextarea.parentNode.replaceChild(newTextarea, oldTextarea);
            }
        }

        {
            const oldToolbar = document.getElementById("markdown_toolbar");
            if (oldToolbar) oldToolbar.remove();
        }

        const textarea = get_element("current_textarea");

// Markdown toolbar
        function createMarkdownToolbar(textarea) {
            let toolbar = document.getElementById("markdown_toolbar");

            // if already exists, just show
            if (toolbar) {
                toolbar.style.display = "flex";
                return;
            }

            toolbar = document.createElement("div");
            toolbar.id = "markdown_toolbar";
            toolbar.style.display = "flex";
            toolbar.style.gap = "4px";
            toolbar.style.marginBottom = "6px";

            const buttons = [
                {label: "B", title: "Bold", before: "**", after: "**"},
                {label: "I", title: "Italic", before: "*", after: "*"},
                {label: "H1", title: "Heading 1", before: "# ", after: ""},
                {label: "H2", title: "Heading 2", before: "## ", after: ""},
                {label: "H3", title: "Heading 3", before: "### ", after: ""},
                {label: "Code", title: "Inline code", before: "`", after: "`"},
                {label: "Code block", title: "Code block", before: "```\n", after: "\n```"},
                {label: "Quote", title: "Blockquote", before: "> ", after: ""},
                {label: "Link", title: "Insert link", before: "[", after: "](url)"},
                {label: "[[...]]", title: "Wiki link", before: "[[", after: "]]"},
                {label: "List", title: "List item", before: "- ", after: ""},
                {label: "[ ]", title: "Checkbox", before: "- [ ] ", after: ""},
                {label: "---", title: "Divider", before: "\n---\n", after: ""},
            ];

            buttons.forEach(cfg => {
                const btn = document.createElement("button");
                btn.type = "button";
                btn.textContent = cfg.label;
                btn.title = cfg.title;
                btn.className = "markdown-btn";
                btn.addEventListener("click", () => insertMarkdown(textarea, cfg.before, cfg.after));
                toolbar.appendChild(btn);
            });

            textarea.parentNode.insertBefore(toolbar, textarea);
        }

// helper
        function insertMarkdown(textarea, before, after) {
            const start = textarea.selectionStart;
            const end = textarea.selectionEnd;
            const selected = textarea.value.substring(start, end);
            const newText = before + selected + after;
            textarea.setRangeText(newText, start, end, "end");
            textarea.focus();
        }


        let markdownPreviewDiv = get_element("markdown_preview");
        if (markdownPreviewDiv != null) {
            markdownPreviewDiv.remove();
            markdownPreviewDiv = null;
        }

        async function convert_wikilinks_to_markdown(markdown_text) {

            const links = await list_all_entities("link", "&from_note_id=" + local_note_id);
            const wanted_notes = await list_all_entities("wanted_note", "&from_note_id=" + local_note_id);

            const regex = /\[\[\s*([^\|\]]+?)(?:\|([^\]]+?))?\s*\]\]/g;

            return markdown_text.replace(regex, (match, title, display) => {
                title = title.trim();
                display = display ? display.trim() : title;
                const encoded = encodeURIComponent(title);

                const link = links.find(l => l.to_note_title === title);
                const wanted_note = wanted_notes.find(wn => wn.to_note_title === title);

                if (link) {
                    return `[${display}](?note_id=${link.to_note_id})`;
                } else if (wanted_note) {
                    return `[${display}](#wanted:${encoded} "Note does not yet exist.")`;
                } else {
                    return `[${display}](#unknown:${encoded} "Note maybe exists, but is not yet linked. Save and reload to update.")`;
                }
            });
        }


        async function render_markdown() {
            // switch to read mode = render Markdown
            const markdownText = await convert_wikilinks_to_markdown(textarea.value);
            const html = md.render(markdownText);

            markdownPreviewDiv = document.createElement("div");
            markdownPreviewDiv.id = "markdown_preview";
            markdownPreviewDiv.innerHTML = html;
            markdownPreviewDiv.style.border = "1px solid #ccc";
            markdownPreviewDiv.style.padding = "8px";
            markdownPreviewDiv.style.background = "var(--bg, #fafafa)";
            markdownPreviewDiv.style.whiteSpace = "normal";
            //markdownPreviewDiv.style.overflow = "auto";

            textarea.style.display = "none";
            textarea.parentNode.insertBefore(markdownPreviewDiv, textarea);

            hide_element("current_button_read");
            show_element("current_button_edit");
        }

        get_element("current_button_edit").onclick = function () {
            // switch back to edit mode
            if (markdownPreviewDiv) {
                markdownPreviewDiv.remove();
                markdownPreviewDiv = null;
            }
            textarea.style.display = "block";

            // creates / shows toolbar
            createMarkdownToolbar(textarea);

            hide_element("current_button_edit");
            show_element("current_button_read");
        };

        hide_element("markdown_toolbar")

        get_element("current_button_read").onclick = async function () {
            if (mode_notes) {
                await render_markdown()
                hide_element("markdown_toolbar")
            } else {
                hide_element("current_button_read");
                show_element("current_button_edit");
                hide_element("markdown_toolbar")
            }
        };

        set_value("current_textarea", "");
        if (mode_root) set_value("current_textarea", map.description);
        if (mode_root) hide_element("markdown_preview")
        if (mode_root) show_element("current_textarea")

        let content = mode_notes ? (note.content_id === 0 ? null : await read_entity("content", note.content_id)) : null;
        original_content_value = mode_notes ? (content === null ? null : content.value) : null;

        if (mode_notes) {
            set_value("current_textarea", content === null ? "" : content.value);
            textarea.style.display = "none";
            hide_element("markdown_toolbar");
            hide_element("current_button_read");
            show_element("current_button_edit");

            markdownPreviewDiv = document.createElement("div");
            markdownPreviewDiv.id = "markdown_preview";
            markdownPreviewDiv.innerHTML = "<em>Loading…</em>";
            textarea.parentNode.insertBefore(markdownPreviewDiv, textarea);

            (async () => {
                const markdownText = await convert_wikilinks_to_markdown(textarea.value);
                const html = md.render(markdownText);
                markdownPreviewDiv.innerHTML = html;
            })();
        }


        let has_parent = mode_root ? false : note.parent_note_id !== "0";

        get_element("current_button_delete").onclick = async function () {
            if (mode_root) {
                show_error("Not yet implemented");
                return;
            }
            // mode_notes
            if (!confirm("Are you sure you want to delete this note?")) return;
            try {
                let response = await delete_entity("note", local_note_id);
                console.info(response)
                show_toast(response)
                await sleep_for_seconds(4)
                await navigate_to(has_parent ? {note_id: note.parent_note_id} : {map_id: note.map_id});


            } catch (err) {
                show_error("Delete failed: " + err.message);
            }
        }

        get_element("current_button_cancel").onclick = function () {

            show_toast("Cancelling changes");
            refresh_page()
        }

        if (mode_root) get_element("current_button_save").onclick = function () {
            console.log("saving note_id", local_note_id, "textarea=", get_element("current_textarea").value);
            map.description = get_element("current_textarea").value;
            put_entity("map", map_id, map)
        }
        if (mode_notes) get_element("current_button_save").onclick = async function () {
            let content = note.content_id === 0 ? null : await read_entity("content", note.content_id);
            //alert("content" + JSON.stringify(content))
            let content_id = content === null ? 0 : content.id;
            let new_content_created = false;
            if (content === null) {
                const new_content = JSON.parse("{\"value\":\"\",\"format\":\"md\",\"version\":1,\"created_at\":0,\"updated_at\":0}")
                let content_created = await post_entity("content", new_content);
                new_content_created = true;
                original_content_value = ""
                //alert("content_id=" + content_created.id)
                note.content_id = content_created.id;
            }
            if (JSON.stringify(note) !== JSON.stringify(original_note)) {
                await put_entity("note", local_note_id, note)
                original_note = structuredClone(note)
                show_info("Note changes were saved")
            } else {
                show_warn("Note was not changed")
            }


            if (content === null) content = await read_entity("content", note.content_id);
            content.value = get_element("current_textarea").value;
            if (original_content_value !== content.value) {
                content.version = content.version + 1
                let put_content_response = await put_entity("content", content.id, content);
                show_toast("Content changes were saved");
                original_content_value = content.value;
            } else {
                show_warn("Content was not changed")
            }


        }


    }

    // Meta
    show_or_hide_element(mode_root_or_notes, "meta")

    if (mode_root_or_notes) {
        show_or_hide_element(mode_notes, "meta_start")
        if (mode_notes) {
            get_element("meta_order").innerText = note.sibling_order;
            get_element("current_button_edit_order").onclick = async function () {

                const input = prompt("Enter new sibling order", note.sibling_order);
                if (input !== null) {
                    let num = Number(input);
                    if (isNaN(num)) {
                        alert("This is not number! " + input);
                    } else {
                        note.sibling_order = num;
                        get_element("meta_order").innerText = note.sibling_order;
                        show_toast("Sibling order updated to " + note.sibling_order);
                    }
                }

            }
            let meta_importance = get_element("meta_importance")

            function refresh_importance_span() {
                switch (note.importance) {
                    case 0:
                        meta_importance.className = "tag none";
                        break;
                    case 1:
                        meta_importance.className = "tag low";
                        break;
                    case 2:
                        meta_importance.className = "tag medium";
                        break;
                    case 3:
                        meta_importance.className = "tag high";
                        break;
                    default:
                        console.warn("Unknown importance: " + note.importance)
                }
                meta_importance.innerText = Importance.fromNumber(note.importance);
            }

            refresh_importance_span()

            get_element("current_button_edit_importance").onclick = async function () {

                const result = await chooseOption(Importance.getTexts());
                if (result !== undefined && result !== null) {
                    note.importance = Importance.toNumber(result);
                    refresh_importance_span()
                    show_toast("Importance updated to " + result);
                }
            }

            let meta_difficulty = get_element("meta_difficulty")

            function refresh_difficulty_span() {
                switch (note.difficulty) {
                    case 0:
                        meta_difficulty.className = "tag none";
                        break;
                    case 1:
                        meta_difficulty.className = "tag easy";
                        break;
                    case 2:
                        meta_difficulty.className = "tag medium";
                        break;
                    case 3:
                        meta_difficulty.className = "tag hard";
                        break;
                    case 4:
                        meta_difficulty.className = "tag expert";
                        break;
                    default:
                        console.warn("Unknown difficulty: " + note.difficulty)
                }
                meta_difficulty.innerText = Difficulty.fromNumber(note.difficulty);
            }

            refresh_difficulty_span()

            get_element("current_button_edit_difficulty").onclick = async function () {

                const result = await chooseOption(Difficulty.getTexts());
                if (result !== undefined && result !== null) {
                    note.difficulty = Difficulty.toNumber(result);
                    refresh_difficulty_span()
                    show_toast("Difficulty updated to " + result);
                }
            }
        }

        function assign_meta_list_function(models, Models, model) {
            get_element("meta_button_" + models).onclick = function () {
                showWindowFrom(Models, "index.html?entity=" + model + "&action=list");
            }
        }

        get_element("meta_button_links").onclick = function () {
            let Models = "Links";
            let model = "link"
            let url = "index.html?entity=" + model + "&action=list"
            if (mode_notes) url = url + "&from_note_id=" + note.id
            showWindowFrom(Models, url);
        }

        get_element("meta_button_urls").onclick = function () {
            let Models = "Urls";
            let model = "url"
            let url = "index.html?entity=" + model + "&action=list"
            if (mode_notes) url = url + "&from_note_id=" + note.id
            showWindowFrom(Models, url);
        }

        get_element("meta_button_terms").onclick = async function () {
            const result = await chooseOption(["List", "Add", "Fulltext"]);
            if (result === null || result === undefined) return

            let Models = "Terms";
            let model = "term"
            let url;
            if (result === "List") url = "index.html?entity=" + model + "&action=list&map_id=" + map.id;
            if (result === "Add") url = "index.html?entity=" + model + "&action=create&map_id=" + map.id;
            if (result === "Fulltext") {
                clearWindow()
                let div = document.createElement("div");
                div.style.padding = "10px"
                let inputElement = document.createElement("input");
                inputElement.type = "text";
                inputElement.id = "term_input"
                inputElement.placeholder = "Search term ..."
                inputElement.autocomplete = "off";
                inputElement.style.fontSize = "150%";
                let suggestions = document.createElement("div")
                suggestions.className = "suggestions"
                suggestions.id = "suggestions"
                div.appendChild(inputElement);
                // div.appendChild(suggestions);
                getWindowContent().appendChild(div);

                const input = document.getElementById("term_input");

                let ac = new Autocomplete(input, 3, "term_fulltext", "&map_id=" + map.id, "title", "title_part")
                let button = document.createElement("button");
                button.innerText = "Add"
                button.style.marginLeft = "10px";
                input.parentNode.appendChild(button);
                button.onclick = async function () {
                    let term = {
                        map_id: map.id,
                        note_id: 0,
                        title: input.value,
                    }
                    let created = await post_entity("term", term)
                    if (created === null) {
                        showError("Saving term " + input.value + " failed.")
                    } else {
                        showInfo("Saving term " + input.value + " was successful.")
                    }
                }

                getWindowContent().style.height = "100%";
                setWindowTitle("Term Fulltext")

                showWindow()
            } else {
                showWindowFrom(Models, url);
            }

        }

        assign_meta_list_function("sources", "Sources", "source")

        get_element("meta_button_ideas").onclick = async function () {
            const result = await chooseOption(["List", "Add"]);
            if (result === null || result === undefined) return

            let Models = "Ideas";
            let model = "idea"
            let url;
            if (result === "List") url = "index.html?entity=" + model + "&action=list&user_id=" + getUserId()
            if (result === "Add") url = "index.html?entity=" + model + "&action=create&user_id=" + getUserId()
            showWindowFrom(Models, url);
        }
        get_element("meta_button_questions").onclick = async function () {
            const result = await chooseOption(["List", "List all", "Add"]);
            if (result === null || result === undefined) return

            let Models = "Questions";
            let model = "question"
            let url;
            if (result === "List") url = "index.html?entity=" + model + "&action=list&note_id=" + note.id
            if (result === "List all") url = "index.html?entity=" + model + "&action=list"
            if (result === "Add") url = "index.html?entity=" + model + "&action=create&note_id=" + note.id
            showWindowFrom(Models, url);
        }

        get_element("meta_button_backlinks").onclick = function () {
            let Models = "Backlinks";
            let model = "link"
            let url = "index.html?entity=" + model + "&action=list"
            if (mode_notes) url = url + "&to_note_id=" + note.id
            showWindowFrom(Models, url);
        }

        get_element("meta_button_siblings").onclick = function () {
            let Models = "Siblings";
            let model = "note"
            let url = "index.html?entity=" + model + "&action=list&parent_note_id=" + note.parent_note_id
            showWindowFrom(Models, url);
        }

        show_or_hide_elements(mode_notes, "meta_button_backlinks")
        show_or_hide_elements(mode_notes, "meta_button_siblings")

        get_element("meta_button_wanted_notes").onclick = function () {
            let Models = "Wanted notes";
            let model = "wanted_note"
            let url = "index.html?entity=" + model + "&action=list";
            if (mode_notes) url = url + "&from_note_id=" + note.id
            showWindowFrom(Models, url);
        }

        get_element("meta_button_properties").onclick = async function () {
            const result = await chooseOption(["List", "Add"]);
            if (result === null || result === undefined) return

            let Models = "Properties";
            let model = "property"
            let url;
            if (result === "List") {
                url = "index.html?entity=" + model + "&action=list&map_id=" + map.id;
                if (mode_notes) url = url + "&note_id=" + note.id
            }
            if (result === "Add") {
                url = "index.html?entity=" + model + "&action=create&map_id=" + map.id;
                if (mode_notes) url = url + "&note_id=" + note.id

            }
            showWindowFrom(Models, url);
        }

        get_element("meta_button_tags").onclick = async function () {
            const result = await chooseOption(["List", "Add", "List types", "Add type", "Fulltext"]);
            if (result === null || result === undefined) return

            let Models = "Tags";
            let model = "tag"
            let url;
            if (result === "List") {
                url = "index.html?entity=" + model + "&action=list"
                if (mode_notes) url = url + "&note_id=" + note.id
            }
            if (result === "Add") {
                url = "index.html?entity=" + model + "&action=create"
                if (mode_notes) url = url + "&note_id=" + note.id
            }
            if (result === "List types") {
                url = "index.html?entity=" + "tag_type" + "&action=list&map_id=" + map.id;
            }
            if (result === "Add type") {
                url = "index.html?entity=" + "tag_type" + "&action=create&map_id=" + map.id;
            }
            if (result === "Fulltext") {
                clearWindow()
                let div = document.createElement("div");
                div.style.padding = "10px"
                let inputElement = document.createElement("input");
                inputElement.type = "text";
                inputElement.id = "tag_type_input"
                inputElement.placeholder = "Search tag type ..."
                inputElement.autocomplete = "off";
                inputElement.style.fontSize = "150%";
                let suggestions = document.createElement("div")
                suggestions.className = "suggestions"
                suggestions.id = "suggestions"
                div.appendChild(inputElement);
                // div.appendChild(suggestions);
                getWindowContent().appendChild(div);

                const input = document.getElementById("tag_type_input");

                let ac = new Autocomplete(input, 2, "tag_type_fulltext", "&map_id=" + map.id, "title", "title_part")
                let button = document.createElement("button");
                button.innerText = "Add"
                button.style.marginLeft = "10px";
                input.parentNode.appendChild(button);
                button.onclick = async function () {
                    let tag_type_exists = ac.get_item() !== null;
                    let tag_type_id = null;
                    if (!tag_type_exists) {
                        let tag_type = {
                            map_id: map.id,
                            title: input.value,
                        }

                        let created = await post_entity("tag_type", tag_type)
                        if (created === null) {
                            showError("Saving tag_type " + input.value + " failed.")
                            return
                        } else {
                            showInfo("Saving tag_type " + input.value + " was successful.")
                            tag_type_id = created.id
                        }
                    } else {
                        tag_type_id = ac.get_item().id
                    }
                    if (mode_notes) {
                        let tag = {
                            note_id: note.id,
                            tag_type_id: tag_type_id,
                        }
                        let created = await post_entity("tag", tag)
                        if (created === null) {
                            showError("Saving tag " + input.value + " failed.")
                        } else {
                            showInfo("Saving tag " + input.value + " was successful.")
                        }
                    }
                }

                getWindowContent().style.height = "100%";
                setWindowTitle("Tag Type Fulltext")

                showWindow()
            } else {
                showWindowFrom(Models, url);
            }
        }

        get_element("meta_button_collections").onclick = async function () {
            const result = await chooseOption(["List collections", "Create collection", "Add to collection"]);
            if (result === null || result === undefined) return

            let Models = "Collections";
            let model = "collection"
            let url;
            if (result === "List collections") {
                url = "index.html?entity=" + model + "&action=list"
            }
            if (result === "Create collection") {
                url = "index.html?entity=" + model + "&action=create"
            }
            if (result === "Add to collection") {
                url = "index.html?entity=" + "collection_item" + "&action=create"
                if (mode_notes) url = url + "&note_id=" + note.id
            }
            showWindowFrom(Models, url);
        }
//
        assign_meta_list_function("alert", "Alerts", "alert")

        get_element("meta_button_flags").onclick = async function () {
            const result = await chooseOption(["List", "Add"]);
            if (result === null || result === undefined) return

            let Models = "Flags";
            let model = "flag"
            let url;
            if (result === "List") {
                url = "index.html?entity=" + model + "&action=list"
                if (mode_notes) url = url + "&note_id=" + note.id
            }
            if (result === "Add") {
                url = "index.html?entity=" + model + "&action=create"
                if (mode_notes) url = url + "&note_id=" + note.id
            }
            showWindowFrom(Models, url);
        }

        get_element("meta_button_annotations").onclick = async function () {
            const result = await chooseOption(["List", "Add"]);
            if (result === null || result === undefined) return

            let Models = "Annotations";
            let model = "annotation"
            let url;
            if (result === "List") {
                url = "index.html?entity=" + model + "&action=list"
                if (mode_notes) url = url + "&note_id=" + note.id
            }
            if (result === "Add") {
                url = "index.html?entity=" + model + "&action=create"
                if (mode_notes) url = url + "&note_id=" + note.id
            }
            showWindowFrom(Models, url);
        }

        get_element("meta_button_tests").onclick = async function () {
            const result = await chooseOption(["List", "Add"]);
            if (result === null || result === undefined) return

            let Models = "Tests";
            let model = "test"
            let url;
            if (result === "List") {
                url = "index.html?entity=" + model + "&action=list&map_id=" + map.id;
            }
            if (result === "Add") {
                url = "index.html?entity=" + model + "&action=create&map_id=" + map.id;
                if (mode_notes) url = url + "&under_note_id=" + note.id
            }
            showWindowFrom(Models, url);
        }
        get_element("meta_button_test_attempts").onclick = async function () {
            const result = await chooseOption(["List", "Add", "Run"]);
            if (result === null || result === undefined) return

            let Models = "Test attempts";
            let model = "test_attempt"
            let url;
            if (result === "List") {
                url = "index.html?entity=" + model + "&action=list&user_id=" + getUserId();
                showWindowFrom(Models, url);
            }
            if (result === "Add") {
                url = "index.html?entity=" + model + "&action=create&user_id=" + getUserId();
                showWindowFrom(Models, url);
            }
            if (result === "Run") {
                let test_attempt_id = prompt("Test attempt ID");

                if (test_attempt_id === undefined || test_attempt_id === null) {
                    show_warn("No Test attempt ID was entered")
                    return
                }
                let test_attempt = await read_entity("test_attempt", test_attempt_id)
                let test = await read_entity("test", test_attempt.test_id)
                // alert(JSON.stringify(test_attempt, null, 2));
                // alert(JSON.stringify(test, null, 2));
                let started_at = test_attempt.started_at;
                let finished_at = test_attempt.finished_at;
                if (finished_at !== 0) {
                    show_warn("This test attempt is already finished.")
                    return;
                }
                let time_limit_in_seconds = test.time_limit_in_seconds;
                let expires_at = started_at + time_limit_in_seconds * 1000;
                let expired = Date.now() > expires_at
                if (expired) {
                    show_warn("This test attempt is expired.")
                    return;
                }
                const question_ids = test_attempt.question_ids.split(",");
                let any_question = false

                for (const question_id of question_ids) {
                    let test_attempt_answers = await list_all_entities(
                        "test_attempt_answer",
                        "&test_attempt_id=" + test_attempt_id + "&question_id=" + question_id
                    )
                    if (test_attempt_answers.length !== 0) {
                        continue;
                    }
                    any_question = true

                    let question = await read_entity("question", question_id)

                    const answers = question.answers.split("::::");
                    let question_text = question.question_text + "?"

                    let result = ""
                    let correct = false

                    if (answers.length > 1) {
                        result = await chooseOption(answers.slice().map(e => {
                            e.length === 0 ? e : e.substr(0)
                        }), question_text);
                        if (result === null || result === undefined) {
                            show_warn("You exited this test.")
                            break;
                        }
                        for (const answer of answers) {
                            if (answer === result) {
                                correct = true;
                                break;
                            }
                        }
                        correct = result.length !== 0 && result[0] !== "-";
                    } else {
                        result = await chooseOption(["Show answer"], question_text);
                        show_info(answers[0])
                        result = await chooseOption(["I know", "I don't know"], "Did you know?");
                        correct = result !== null && result !== undefined && result === "I know";
                        if (correct) result = answers[0];
                    }
                    let test_attempt_answer = {
                        test_attempt_id: test_attempt.id,
                        question_id: question_id,
                        user_answer: result,
                        is_correct: correct ? 1 : 0,
                    }
                    let post_test_attempt_answer = post_entity("test_attempt_answer", test_attempt_answer)
                    if (correct) show_info("Correct answer")
                    if (!correct) show_error("Incorrect answer")
                }
                if (!any_question) {
                    show_warn("All questions were already answered.");
                }

                await sleep_for_seconds(1)
                let test_attempt2 = await read_entity("test_attempt", test_attempt_id)
                alert(JSON.stringify(test_attempt2, null, 2));

                function score_to_text(score) {
                    if (score >= 90) return "Excellent"
                    if (score >= 80) return "Very Good"
                    if (score >= 70) return "Good"
                    if (score >= 60) return "Satisfactory"
                    if (score >= 50) return "Poor"
                    return "Insufficient"
                }

                if (test_attempt2.finished_at !== 0) {
                    let score = test_attempt2.score_times_100
                    show_info("Test finished. Your score: " + score + "%. " + score_to_text(score))
                    if (score >= 60) {
                        show_info("You passed.")
                    } else {
                        show_error("You failed.")
                    }
                }
            }

        }

        assign_meta_list_function("projects", "Projects", "project")
        assign_meta_list_function("tasks", "Tasks", "task")


        get_element("meta_button_pinned_notes").onclick = async function () {
            const result = await chooseOption(["List", "Pin", "Pinned"]);
            if (result === null || result === undefined) return

            let Models = "Pinned notes";
            let model = "pinned_note"
            let url;
            if (result === "List") {
                url = "index.html?entity=" + model + "&action=list&user_id=" + getUserId()
                if (mode_notes) url = url + "&note_id=" + note.id
            }
            if (result === "Pin") {
                url = "index.html?entity=" + model + "&action=create&user_id=" + getUserId()
                if (mode_notes) url = url + "&note_id=" + note.id
            }
            if (result === "Pinned") {
                url = "index.html?entity=" + model + "&action=list&user_id=" + getUserId()
            }
            showWindowFrom(Models, url);
        }
        //

        // save the id of the visited note to the local storage
        function save_visited_note_id(id) {
            const key = "visited_note_ids";
            let ids = JSON.parse(localStorage.getItem(key)) || [];
            const last = ids.length === 0 ? null : ids[ids.length - 1];

            if (last && last.id === id) {
                last.ts = Date.now();
            } else {
                ids.push({
                    id: id,
                    ts: Date.now()
                });
            }

            const MAX = 200;

            if (ids.length > MAX) {
                ids = ids.slice(ids.length - MAX);
            }

            localStorage.setItem(key, JSON.stringify(ids));
        }

        if (mode_notes) save_visited_note_id(note.id)

        get_element("meta_button_visited").onclick = async function () {
            let visited_note_ids = localStorage.getItem("visited_note_ids")
            if (visited_note_ids === null || visited_note_ids === undefined) {
                show_warn("History is empty");
                return;
            }

            let visited_note_ids_json = JSON.parse(visited_note_ids)
            let table = document.createElement("table");
            table.style.borderCollapse = "collapse";
            table.style.margin = "0 auto";
            let tr_first = document.createElement("tr");
            table.appendChild(tr_first);
            let th_number = document.createElement("th");
            th_number.innerText = "#"
            let th_id = document.createElement("th");
            th_id.innerText = "ID"
            let th_title = document.createElement("th");
            th_title.innerText = "Title"
            let th_timestamp = document.createElement("th");
            th_timestamp.innerText = "Timestamp"
            tr_first.appendChild(th_number)
            tr_first.appendChild(th_id)
            tr_first.appendChild(th_title)
            tr_first.appendChild(th_timestamp)
            for (const el of [th_number, th_id, th_title, th_timestamp]) {
                el.style.minWidth = "20px"
                el.style.padding = "10px";
                el.style.border = "1px solid black";
                el.style.background = "#ccc"
            }
            th_title.style.minWidth = "200px"

            let history_entry_number = 0;
            for (const entry of visited_note_ids_json.slice().reverse()) {
                history_entry_number++

                let visited_note_id = entry.id
                let tr = document.createElement("tr");
                table.appendChild(tr)
                let td_number = document.createElement("td");
                let td_id = document.createElement("td");
                let td_title = document.createElement("td");
                let td_timestamp = document.createElement("td");
                for (const el of [td_number, td_id, td_title, td_timestamp]) {
                    el.style.padding = "10px";
                    el.style.border = "1px solid black";
                }

                tr.appendChild(td_number)
                tr.appendChild(td_id)
                tr.appendChild(td_title)
                tr.appendChild(td_timestamp)
                td_number.innerText = history_entry_number;
                td_id.innerText = visited_note_id;
                let a = document.createElement("a");
                a.href = "?";
                {
                    let title = getTitleCache("note", visited_note_id)
                    if (title === null || title === undefined) {

                        let x = entry.id
                        let note_ = await read_entity("note", x)
                        if (note_ === null) {
                            show_warn("Loading note with id " + x + " failed.");
                            title = "Unknown (#" + x + ")"
                        } else {
                            title = note_.title
                            setTitleCache("note", x, title);
                        }
                    }
                    a.title = title
                    a.innerText = title;
                }
                a.onclick = async function () {
                    event.preventDefault();
                    await navigate_to({note_id: visited_note_id});
                }
                td_title.appendChild(a)
                td_timestamp.innerText = formatDateTime(entry.ts, true, true, true)
            }
            clearWindow()
            getWindowContent().appendChild(table);
            getWindowContent().style.height = "100%";
            setWindowTitle("History")

            showWindow()
        }

        get_element("meta_button_history").onclick = async function () {
            const result = await chooseOption(["History", "Content history"]);
            if (result === null || result === undefined) return

            let Models = "History";
            let model = "history"
            let url;
            url = "index.html?entity=" + model + "&action=list"
            if (result === "History") {
                if (mode_notes) url = url + "&table_name=note&record_id=" + note.id
                if (mode_root) url = url + "&table_name=map&record_id=" + map.id
            }
            if (result === "Content history") {
                if (mode_notes) url = url + "&table_name=content&record_id=" + note.content_id
                if (mode_root) url = url + "&table_name=map&record_id=" + map.id
            }

            showWindowFrom(Models, url);
        }
    }

    // Children
    if (mode_maps) set_value("children_label", "All maps");
    if (mode_root) set_value("children_label", "Root notes")
    if (mode_notes) set_value("children_label", "Subnotes")

    show_or_hide_element(mode_root_or_notes, "collapsible-toggle-children");


    show_or_hide_element(mode_root_or_notes, "children_button_go_up");

    let parent_title = document.getElementById("parent_title");

    if (mode_root_or_notes) document.getElementById("children_button_go_up").onclick = function () {

        parent_title.click()
    };


    if (mode_maps) document.getElementById("children_button_add").onclick = function () {
        setWindowTitle("Maps");
        setWindowContentByUrl("index.html?entity=map&action=create");
        showWindow();
    };

    async function add_note_to_children(e) {
        let children = document.getElementById("children_ul");

        const li = document.createElement("li");
        children.appendChild(li);
        li.innerText = "#" + e.id + " ";
        let a = document.createElement("a")
        li.appendChild(a);
        a.innerText = e.title;
        await link_to(a, {note_id: e.id});
        a.style.display = "inline-block";
        a.style.minWidth = "20px";
        a.style.marginRight = "10px";

        const button_copy = document.createElement("button");
        li.appendChild(button_copy);
        button_copy.innerText = simple ? "Copy" : "📋 Copy";
        button_copy.onclick = function () {
            copy_to_clipboard(e.id);
        };
    }

    if (mode_root) document.getElementById("children_button_add").onclick = async function () {
        let title = prompt("Title of new note");

        if (title !== undefined && title !== null) {

            const new_note = JSON.parse("{\"importance\":0,\"sibling_order\":0,\"source_id\":0,\"title\":\"\",\"content_id\":0,\"parent_note_id\":0,\"difficulty\":0,\"map_id\":0,\"created_at\":0,\"alias_for_note_id\":0,\"updated_at\":0}")

            new_note.title = title
            new_note.map_id = map_id
            let result = await post_entity("note", new_note)
            if (result === undefined || result === null) return;
            await add_note_to_children(result)
        }
    };

    if (mode_notes) document.getElementById("children_button_add").onclick = async function () {
        let title = prompt("Title of new note");

        if (title !== undefined && title !== null) {

            const new_note = JSON.parse("{\"importance\":0,\"sibling_order\":0,\"source_id\":0,\"title\":\"\",\"content_id\":0,\"parent_note_id\":0,\"difficulty\":0,\"map_id\":0,\"created_at\":0,\"alias_for_note_id\":0,\"updated_at\":0}")

            new_note.title = title
            new_note.map_id = note.map_id
            new_note.parent_note_id = note.id
            let result = await post_entity("note", new_note)
            if (result === undefined || result === null) return;
            await add_note_to_children(result)
        }
    };

    let children = document.getElementById("children_ul");
    children.replaceChildren(); // 🧮 destroys all children along with event listeners

    let maps = null
    let notes = null
    if (mode_maps) maps = await list_all_entities("map", "&fields=id,name");
    if (mode_root) notes = await list_all_entities(
        "note",
        "&sort=sibling_order&order=asc&fields=id,title&parent_note_id=0&map_id=" + map_id
    );
    if (mode_notes) notes = await list_all_entities(
        "note"
        , "&sort=sibling_order&order=asc&fields=id,title&parent_note_id=" + note.id + "&map_id=" + note.map_id
    );

    if (mode_maps) {

        for (const map of maps) {

            const li = document.createElement("li");
            children.appendChild(li);
            li.innerText = "#" + map.id + " ";
            let a = document.createElement("a")
            li.appendChild(a);
            a.innerText = map.name;
            link_to(a, {map_id: map.id});
            a.style.display = "inline-block";
            a.style.minWidth = "50px";

            const button_copy = document.createElement("button");
            li.appendChild(button_copy);
            button_copy.innerText = simple ? "Copy" : "📋 Copy";
            button_copy.onclick = function () {
                copy_to_clipboard(map.id);
            };
        }
    }

    if (mode_root || mode_notes) {

        for (const e of notes) {
            await add_note_to_children(e)
        }

    }

    //document.getElementById("children_li_example").remove();

    document.getElementById("loading_screen").style.display = "none";
    document.getElementById("slip_box").style.display = "block";


    // Remove old listener if it exists
    if (beforeUnloadHandler) {
        window.removeEventListener("beforeunload", beforeUnloadHandler);
        beforeUnloadHandler = null;
    }

// Add event listener for current note
    if (mode_notes) {
        beforeUnloadHandler = function (event) {
            const current_textarea = get_element("current_textarea").value;

            const note_changed = JSON.stringify(note) !== JSON.stringify(original_note);
            const content_changed =
                original_content_value === null
                    ? current_textarea !== ""
                    : original_content_value !== current_textarea;

            console.log("note_changed", note_changed, "content_changed", content_changed);

            if (note_changed || content_changed) {
                event.preventDefault();
                event.returnValue = "";
                return "";
            }
        };

        window.addEventListener("beforeunload", beforeUnloadHandler);
    }


    console.log("render() #" + render_number + " ended");
}
