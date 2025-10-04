import {API_BASE, apiFetch} from "./api.js";
import {showError, showInfo} from "./dom.js";

export function togglePanel(element, id) {
    const el = document.getElementById(id);
    if (el === null) alert(id);
    const isCollapsed = el.classList.toggle('collapsed');

    let simple = document.getElementById("app").dataset.version === "simple";
    if (element !== null) {
        if (simple) {
            element.textContent = isCollapsed ? "▶ Expand" : "▼ Collapse";
        } else {
            element.textContent = isCollapsed ? "▶" : "▼";
        }

    }


    if (id === "meta_content" && !simple) {
        const metaPanel = document.getElementById("meta");
        const metaLabel = document.getElementById("meta_label");
        if (window.innerWidth > 800) {
            if (isCollapsed) {

                metaPanel.style.maxWidth = "50px";
                metaPanel.style.minWidth = "50px";
                metaPanel.style.overflow = "hidden";
                metaLabel.style.display = "none";

            } else {
                metaPanel.style.removeProperty("max-width");
                metaPanel.style.removeProperty("min-width");
                metaPanel.style.removeProperty("overflow");
                metaLabel.style.display = "inline";
            }
        }
    }


}

window.togglePanel = togglePanel;

export function button_focus_onclick() {
    document.body.classList.toggle('focus-mode');
}

window.button_focus_onclick = button_focus_onclick;


let wasDragged = false;


function makeDraggable(el) {
    const header = el.querySelector('.window_container-header');
    let offsetX = 0, offsetY = 0, dragging = false;

    function startDrag(x, y) {
        const rect = el.getBoundingClientRect();
        dragging = true;
        wasDragged = true;   // uživatel pohnul
        offsetX = x - rect.left;
        offsetY = y - rect.top;
        el.style.transform = "none";
        el.style.position = "fixed";
    }

    function doDrag(x, y) {
        if (!dragging) return;
        el.style.left = (x - offsetX) + "px";
        el.style.top = (y - offsetY) + "px";
    }

    function stopDrag() {
        dragging = false;
    }

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
    const win = document.getElementById('window_container');
    win.style.display = 'block';

    if (!wasDragged) {
        win.style.left = '50%';
        win.style.top = '50%';
        win.style.transform = 'translate(-50%, -50%)';
        win.style.position = 'fixed';
    }
}


window.showWindow = showWindow;

export function clearWindow() {
    document.getElementById('window_container_content').innerHTML = "";
}

window.clearWindow = clearWindow;

export function setWindowTitle(windowTitle) {
    document.getElementById('window_container_title').innerText = windowTitle;
}

export function getWindowContent() {
    return document.getElementById('window_container_content');
}

export function setWindowContent(text) {
    getWindowContent().innerText = text;
}

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

function hide_element(id) {
    let el = document.getElementById(id);
    if (el === null) return;
    el.style.display = "none";
}

function get_element(id) {
    return document.getElementById(id);
}

function set_value(id, value) {
    document.getElementById(id).innerHTML = value;
}

function copy_to_clipboard(text) {
    navigator.clipboard.writeText(text);
    showInfo("Copied to clipboard: " + text);
}

let map_id = "";
let note_id = "";
let map = null;
let note = null;
let mode_maps = false;
let mode_root = false;
let mode_notes = false;

export function showToast(message) {
    const toast = document.createElement('div');
    toast.className = 'toast';
    toast.textContent = message;
    document.body.appendChild(toast);

    requestAnimationFrame(() => toast.classList.add('show'));

    setTimeout(() => {
        toast.classList.remove('show');
        setTimeout(() => toast.remove(), 300);
    }, 2000);
}

window.showToast = showToast;

function init_from_http_parameters() {
    const params = new URLSearchParams(window.location.search);
    let has_map_id = params.has("map_id");
    let has_note_id = params.has("note_id");
    if (has_map_id) {
        map_id = params.get("map_id");
        mode_root = true;
    } else {
        if (has_note_id) {
            note_id = params.get("note_id");
            mode_notes = true;
        } else {
            mode_maps = true;
        }
    }
}

async function list_entities(entity, additional_params = "", page_number = 1, page_size = 20) {
    const url = new URL(`${API_BASE}/${entity}`);

    url.searchParams.set("page_number", page_number.toString());
    url.searchParams.set("page_size", page_size.toString());

    let finalUrl = url.toString() + additional_params;

    const json = await apiFetch(finalUrl);
    const total_pages = json?.total_pages || 1;

    const items = json?.items || [];

    return items;
}


//entity, additional_params = "", page_number = 1, page_size = 20
async function list_all_entities(entity, additional_params = "") {
        let result = [];
    let page_size = 100;
    let page_number = 1;

    while(true) {
        let items = await list_entities(entity, additional_params, page_number, page_size);
        page_number++;
        if(items.length > 0) {
            items.forEach((item) => {
                result.push(item);
            })

        } else {
            break;
        }
        if(result.length >= 1000) {
            showToast("Omitting some results: 1000 or more results. ")
            break;
        }
    }
    return result;
}
async function read_entity(entity, id) {
    const url = new URL(`${API_BASE}/${entity}/${id}`);

    return await apiFetch(url.toString());
}

async function put_entity(model_name, id, json) {
    const url = new URL(`${API_BASE}/${model_name}/${id}`);
    let response = await apiFetch(url.toString(), {
        method: "PUT",
        headers: {"Content-Type": "application/json"},
        body: JSON.stringify(json)
    });

}

async function post_entity(model_name, json) {
    const url = new URL(`${API_BASE}/${model_name}`);
    let response = await apiFetch(url.toString(), {
        method: "POST",
        headers: {"Content-Type": "application/json"},
        body: JSON.stringify(json)
    });

}

export function refresh_page() {
    let current_url = window.location.href;
    if (current_url !== undefined) {

        if (current_url.endsWith("#")) {
            current_url = current_url.slice(0, -1);
        }

    }

    window.location.href = current_url;
}
window.refresh_page = refresh_page;

function refresh_page_to(url) {
    window.location.href = url;
}

document.addEventListener('DOMContentLoaded', async () => {

    let disable_rest = false;
    //disable_rest = true;

    let el = null;
    let simple = document.getElementById("app").dataset.version === "simple";

    el = document.getElementById("collapsible-toggle-meta");

    if (window.innerWidth <= 800) togglePanel(el, 'meta_content')

    // document.querySelectorAll('.actions button').forEach(btn => {
    //     btn.addEventListener('click', () => {
    //         const label = btn.textContent.trim();
    //         if (label.includes('Copy')) showToast('Copied to clipboard');
    //         if (label.includes('Save')) showToast('Note saved');
    //         if (label.includes('Delete')) showToast('Note deleted');
    //     });
    // });


    document.querySelectorAll('.add').forEach(btn => {
        btn.addEventListener('click', () => showToast('New child added'));
    });

    const win = document.getElementById('window_container');
    makeDraggable(win);

    init_from_http_parameters();

    if (disable_rest) {
        return;
    }

    if (mode_maps) {
        hide_element("button_previous");
        hide_element("button_next");
        hide_element("button_focus");
        hide_element("parent");
        hide_element("current");
        hide_element("meta");
        set_value("children_label", "All maps");
        hide_element("collapsible-toggle-children");

        document.getElementById("children_button_add").onclick = function () {
            setWindowTitle("Maps");
            setWindowContentByUrl("index.html?entity=map&action=create");
            showWindow();
        };
        let children = document.getElementById("children_ul");

        let maps = await list_all_entities("map", "&fields=id,name");
        for (const map of maps) {

            const li = document.createElement("li");
            children.appendChild(li);
            li.innerText = "#" + map.id + " ";
            let a = document.createElement("a")
            li.appendChild(a);
            a.innerText = map.name;
            a.href = "?map_id=" + map.id;
            a.style.display = "inline-block";
            a.style.minWidth = "50px";

            //http://localhost:8888/web/index.html?entity=map&action=create#
            const button_copy = document.createElement("button");
            li.appendChild(button_copy);
            button_copy.innerText = simple ? "Copy" :"📋 Copy";
            button_copy.onclick = function () {
                copy_to_clipboard(map.id);
            };


        }
    }

    if (mode_root) {
        set_value("parent_label", "All maps")
        hide_element("parent_id_label")
        hide_element("parent_id")
        let parent_title = document.getElementById("parent_title");
        parent_title.innerText = "All maps";
        parent_title.href = "?";
        get_element("parent_button_copy").onclick = function () {
            copy_to_clipboard(parent_title.href);
        }
        get_element("parent_button_edit").onclick = function () {
            showWindowFrom("List of maps", "index.html?entity=map&action=list")
        }
        set_value("current_label", "Map")
        set_value("current_id", map_id)

        get_element("current_title").onclick = function () {
            showWindowFrom("Detail of map #" + map_id, "index.html?entity=map&action=read&id=" + map_id)
        }

        map = await read_entity("map", map_id);
        set_value("current_title", map.name);

        let current_title = document.getElementById("current_title");
        current_title.style.display = "inline-block";
        current_title.style.minWidth = "50px";

        get_element("current_button_rename").onclick = function () {
            showError("Not yet implemented");
        }

        get_element("current_button_copy").onclick = function () {
            copy_to_clipboard(map_id)
        }

        set_value("current_textarea", map.description);

        get_element("current_button_delete").onclick = function () {
            showError("Not yet implemented");
        }
        get_element("current_button_cancel").onclick = function () {
            refresh_page()
        }
        //{"team_id":0,"owner_id":1,"category":"","other_rights":7,"team_rights":7,"description":"aaaa","name":"aa","created_at":1759583934,"owner_rights":7,"updated_at":1759601801,"id":1}

        get_element("current_button_save").onclick = function () {
            map.description = get_element("current_textarea").value;
            put_entity("map", map_id, map)
        }
        hide_element("meta_start")

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

        set_value("children_label", "Root notes")

        document.getElementById("children_button_add").onclick = async function () {
            let title = prompt("Title of new note");

            if (title !== undefined && title !== null) {

                const new_note = JSON.parse("{\"importance\":0,\"sibling_order\":0,\"source_id\":0,\"title\":\"\",\"content_id\":0,\"parent_note_id\":0,\"difficulty\":0,\"map_id\":0,\"created_at\":0,\"alias_for_note_id\":0,\"updated_at\":0}")

                new_note.title = title
                new_note.map_id = map_id
                await post_entity("note", new_note)
            }
        };


        let children = document.getElementById("children_ul");

        let notes = await list_all_entities(
            "note",
            "&sort=sibling_order&order=asc&fields=id,title&parent_note_id=0&map_id="+map_id
        );
        for (const e of notes) {

            const li = document.createElement("li");
            children.appendChild(li);
            li.innerText = "#" + e.id + " ";
            let a = document.createElement("a")
            li.appendChild(a);
            a.innerText = e.title;
            a.href = "?note_id=" + e.id;
            a.style.display = "inline-block";
            a.style.minWidth = "20px";
            a.style.marginRight = "10px";


            //http://localhost:8888/web/index.html?entity=map&action=create#
            const button_copy = document.createElement("button");
            li.appendChild(button_copy);
            button_copy.innerText = simple ? "Copy" :"📋 Copy";
            button_copy.onclick = function () {
                copy_to_clipboard(e.id);
            };

        }

    }

    if(mode_notes) {
        alert("mode_notes is not yet supported")
        throw "mode_notes is not yet supported"
    }

    document.getElementById("children_li_example").remove();


});
