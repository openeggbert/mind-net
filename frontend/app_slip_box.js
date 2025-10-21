// ========================================
// Imports & Globals
// ========================================
import {delete_entity, list_all_entities, post_entity, put_entity, read_entity} from "./api.js";
import {
    makeEnum, sleep_for_seconds, hide_element, hide_elements, get_element, set_value, copy_to_clipboard,
    chooseOption, show_elements, show_or_hide_elements, show_or_hide_element
} from "./dom.js";

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

    function startDrag(x, y) {
        const rect = el.getBoundingClientRect();
        dragging = true;
        wasDragged = true;
        offsetX = x - rect.left;
        offsetY = y - rect.top;
        el.style.transform = "none";
        el.style.position = "fixed";
    }

    const doDrag = (x, y) => {
        if (!dragging) return;
        el.style.left = `${x - offsetX}px`;
        el.style.top = `${y - offsetY}px`;
    };

    const stopDrag = () => {
        dragging = false
    };

    // mouse
    header.addEventListener('mousedown', e => {
        startDrag(e.clientX, e.clientY);
        e.preventDefault();
    });
    document.addEventListener('mousemove', e => doDrag(e.clientX, e.clientY));
    document.addEventListener('mouseup', stopDrag);

    // touch
    header.addEventListener('touchstart', e => {
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
    win.style.display = 'none';
    window.closeWindow = closeWindow;
}

export function showWindow() {
    const win = document.getElementById("window_container");
    win.style.display = "block";

    if (!wasDragged) {
        Object.assign(win.style, {
            left: "50%",
            top: "50%",
            transform: "translate(-50%, -50%)",
            position: "fixed"
        });
    }
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

export function refresh_page() {
    render(); // redraws current state
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

export function navigate_to(params) {
    suppressPopstate = true;
    set_url_params(params);
    render();
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

let beforeUnloadAttached = false;

function link_to(a, params) {
    // always update href
    a.href = "?" + new URLSearchParams(params).toString();

    // add listener only once, but don't store 'params' in closure
    if (!a._hasLinkListener) {
        a.addEventListener("click", (event) => {
            event.preventDefault();

            // get current href
            const url = new URL(a.href, window.location.origin);
            const p = Object.fromEntries(url.searchParams.entries());
            navigate_to(p);
        });
        a._hasLinkListener = true;
    }
}

async function render() {
    // TODO split into renderParent(), renderCurrent(), renderMeta(), renderChildren().
    console.log("render() called", performance.now());

    init_from_http_parameters();

    let mode_root_or_notes = mode_root || mode_notes;

    // Header
    show_or_hide_elements(mode_root_or_notes, "button_previous", "button_next", "button_focus")

    // Parent
    show_or_hide_element(mode_root_or_notes, "parent")

    if (mode_root_or_notes) {
        original_note = mode_notes ? await read_entity("note", note_id) : null;
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
            link_to(parent_title, {}); // go to homepage without parameters
        }
        if (mode_notes) {
            parent_title.innerText = has_parent ? parent_note.title : map.name;
            link_to(parent_title, has_parent ? { note_id: note.parent_note_id } : { map_id: note.map_id });
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
                link_to(parent_title, has_parent ? { note_id: note.parent_note_id } : { map_id: note.map_id });
            }
        }
    }

    // Current
    show_or_hide_element(mode_root_or_notes, "current")

    if (mode_root_or_notes) {
        set_value("current_label", mode_root ? "Map" : "Note")
        set_value("current_id", mode_root ? map_id : note_id)

        get_element("current_title").onclick = function () {
            if (mode_root) showWindowFrom("Detail of map #" + map_id, "index.html?entity=map&action=read&id=" + map_id)
            if (mode_notes) showWindowFrom("Detail of note #" + note_id, "index.html?entity=note&action=read&id=" + note_id)
        }

        if (mode_root) map = await read_entity("map", map_id);
        set_value("current_title", mode_root ? map.name : note.title);
        let current_title = document.getElementById("current_title");
        current_title.style.display = "inline-block";
        current_title.style.minWidth = "50px";

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
            }
        }

        get_element("current_button_copy").onclick = function () {
            copy_to_clipboard(mode_root ? map_id : note_id)
        }

        set_value("current_textarea", "");
        if (mode_root) set_value("current_textarea", map.description);

        let content = mode_notes ? (note.content_id === 0 ? null : await read_entity("content", note.content_id)) : null;
        original_content_value = mode_notes ? (content === null ? null : content.value) : null;
        if (mode_notes) set_value("current_textarea", content === null ? "" : content.value);

        let has_parent = mode_root ? false : note.parent_note_id !== "0";

        get_element("current_button_delete").onclick = async function () {
            if (mode_root) {
                show_error("Not yet implemented");
                return;
            }
            // mode_notes
            if (!confirm("Are you sure you want to delete this note?")) return;
            try {
                let response = await delete_entity("note", note_id);
                console.info(response)
                show_toast(response)
                await sleep_for_seconds(4)
                navigate_to(has_parent ? { note_id: note.parent_note_id } : { map_id: note.map_id });


            } catch (err) {
                show_error("Delete failed: " + err.message);
            }
        }

        get_element("current_button_cancel").onclick = function () {

            show_toast("Cancelling changes");
            refresh_page()
        }

        if (mode_root) get_element("current_button_save").onclick = function () {
            map.description = get_element("current_textarea").value;
            put_entity("map", map_id, map)
        }
        if (mode_notes) get_element("current_button_save").onclick = async function () {
            let content = note.content_id === 0 ? null : await read_entity("content", note.content_id);
            //alert("content" + JSON.stringify(content))
            let content_id = content === null ? 0 : content.id;
            if (content === null) {
                const new_content = JSON.parse("{\"value\":\"\",\"format\":\"md\",\"version\":1,\"created_at\":0,\"updated_at\":0}")
                let content_created = await post_entity("content", new_content);
                //alert("content_id=" + content_created.id)
                note.content_id = content_created.id;
            }
            if (JSON.stringify(note) !== JSON.stringify(original_note)) {
                await put_entity("note", note_id, note)
                original_note = structuredClone(note)
                show_info("Note changes were saved")
            } else {
                show_warn("Note was not changed")
            }

            if (content === null) content = await read_entity("content", note.content_id);
            content.value = get_element("current_textarea").value;
            if (original_content_value !== content.value) {
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

        assign_meta_list_function("links", "Links", "link")
        assign_meta_list_function("urls", "Urls", "url")
        assign_meta_list_function("terms", "Terms", "term")
        assign_meta_list_function("sources", "Sources", "source")
        assign_meta_list_function("ideas", "Ideas", "idea")
        assign_meta_list_function("questions", "Questions", "question")

        // assign_meta_list_function("backlinks", "Backlinks", "backlink")
        // assign_meta_list_function("siblings", "Siblings", "sibling")
        hide_element("meta_button_backlinks")
        hide_element("meta_button_siblings")
        assign_meta_list_function("wanted_notes", "Wanted notes", "wanted_note")
        assign_meta_list_function("properties", "Properties", "property")
        assign_meta_list_function("tags", "Tags", "tag")
        assign_meta_list_function("collections", "Collections", "collection")
//
        assign_meta_list_function("alert", "Alerts", "alert")
        assign_meta_list_function("flags", "Flags", "flag")
        assign_meta_list_function("projects", "Projects", "project")
        assign_meta_list_function("tasks", "Tasks", "task")
        assign_meta_list_function("pinned_notes", "Pinned notes", "pinned_note")
        //
        assign_meta_list_function("visited", "Visited", "visited")
        assign_meta_list_function("history", "History", "history")
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

    if (mode_root) document.getElementById("children_button_add").onclick = async function () {
        let title = prompt("Title of new note");

        if (title !== undefined && title !== null) {

            const new_note = JSON.parse("{\"importance\":0,\"sibling_order\":0,\"source_id\":0,\"title\":\"\",\"content_id\":0,\"parent_note_id\":0,\"difficulty\":0,\"map_id\":0,\"created_at\":0,\"alias_for_note_id\":0,\"updated_at\":0}")

            new_note.title = title
            new_note.map_id = map_id
            await post_entity("note", new_note)
        }
    };

    if (mode_notes) document.getElementById("children_button_add").onclick = async function () {
        let title = prompt("Title of new note");

        if (title !== undefined && title !== null) {

            const new_note = JSON.parse("{\"importance\":0,\"sibling_order\":0,\"source_id\":0,\"title\":\"\",\"content_id\":0,\"parent_note_id\":0,\"difficulty\":0,\"map_id\":0,\"created_at\":0,\"alias_for_note_id\":0,\"updated_at\":0}")

            new_note.title = title
            new_note.map_id = note.map_id
            new_note.parent_note_id = note.id
            await post_entity("note", new_note)
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
            link_to(a, { map_id: map.id });
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

    if (mode_root) {

        for (const e of notes) {

            const li = document.createElement("li");
            children.appendChild(li);
            li.innerText = "#" + e.id + " ";
            let a = document.createElement("a")
            li.appendChild(a);
            a.innerText = e.title;
            link_to(a, { note_id: e.id });
            a.style.display = "inline-block";
            a.style.minWidth = "20px";
            a.style.marginRight = "10px";


            //http://localhost:8888/web/index.html?entity=map&action=create#
            const button_copy = document.createElement("button");
            li.appendChild(button_copy);
            button_copy.innerText = simple ? "Copy" : "📋 Copy";
            button_copy.onclick = function () {
                copy_to_clipboard(e.id);
            };

        }

    }

    if (mode_notes) {

        for (const e of notes) {

            const li = document.createElement("li");
            children.appendChild(li);
            li.innerText = "#" + e.id + " ";
            let a = document.createElement("a")
            li.appendChild(a);
            a.innerText = e.title;
            link_to(a, { note_id: e.id });
            a.style.display = "inline-block";
            a.style.minWidth = "20px";
            a.style.marginRight = "10px";


            //http://localhost:8888/web/index.html?entity=map&action=create#
            const button_copy = document.createElement("button");
            li.appendChild(button_copy);
            button_copy.innerText = simple ? "Copy" : "📋 Copy";
            button_copy.onclick = function () {
                copy_to_clipboard(e.id);
            };
        }
    }

    //document.getElementById("children_li_example").remove();

    document.getElementById("loading_screen").style.display = "none";
    document.getElementById("slip_box").style.display = "block";

    if (mode_notes && !beforeUnloadAttached) window.addEventListener("beforeunload", async function (event) {
        let note_changed = JSON.stringify(note) !== JSON.stringify(original_note);
        let current_textarea = get_element("current_textarea").value;
        console.log("current_textarea=" + current_textarea)
        console.log("original_content_value=" + original_content_value)
        let content_changed = original_content_value === null ? current_textarea !== "" : original_content_value !== current_textarea;

        console.log("note_changed=" + note_changed)
        console.log("content_changed=" + content_changed)

        if (note_changed || content_changed) {

            event.preventDefault();
            event.returnValue = "";
            return "";
        }
    });
    beforeUnloadAttached = true;


}
