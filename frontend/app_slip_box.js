import {API_BASE, apiFetch} from "./api.js";

export function toggle_section(id, toggleId) {
    const content = document.getElementById(id);
    const toggle = document.getElementById(toggleId);

    const isCollapsed = content.classList.toggle("collapsed");

    toggle.textContent = isCollapsed ? "▶ Expand" : "▼ Collapse";
}
window.toggle_section = toggle_section;

function openModal() {
    document.getElementById("collections-modal").style.display = "block";
}

function closeModal() {
    document.getElementById("collections-modal").style.display = "none";
}

function button_previous() {

}

function button_next() {

}

function button_theme() {

}

function parent_note_title_link() {

}

function note_title_link() {

}

function hide_element(id) {
    document.getElementById(id).style.display="none";
}
function set_value(id, value) {
    document.getElementById(id).innerHTML = value;
}
const PANEL_NOTE__LABEL = "panel_note__label";
const PANEL_NOTE__NOTE_ID = "panel_note__note_id";
const PANEL_NOTE__NOTE_TITLE = "panel_note__note_title";
const PANEL_SUBNOTES__LABEL = "panel_subnotes__label";

function set_note_id(id) {
    set_value("note_id", id)
}
function set_note_title(id) {
    set_value("note_title", id)
}

let map_id = "";
let note_id = "";
let mode_list_maps = false;
let mode_list_root_notes = false;
let mode_list_notes = false;

function init_state() {
    const params = new URLSearchParams(window.location.search);
    let has_map_id = params.has("map_id");
    let has_note_id = params.has("note_id");
    if (has_map_id) {
        map_id = params.get("map_id");
        mode_list_root_notes = true;
    } else {
        if (has_note_id) {
            note_id = params.get("note_id");
            mode_list_notes = true;
        } else {
            mode_list_maps = true;
        }
    }
}

async function list_entity(entity, page_number = 1, page_size = 20) {
  const url = new URL(`${API_BASE}/${entity}`);

    url.searchParams.set("page_number", page_number);
    url.searchParams.set("page_size", page_size);

  const json = await apiFetch(url.toString());
  const total_pages = json?.total_pages || 1;

  const items = json?.items || [];

  return items;


}
document.addEventListener("DOMContentLoaded", async function () {
    init_state();
    if (mode_list_maps) {
        document.getElementById("panel_parent").style.display = "none";
        document.getElementById("button_copy_note_id").style.display = "none";

        hide_element(PANEL_NOTE__NOTE_ID);
        hide_element(PANEL_NOTE__NOTE_TITLE);
        set_value(PANEL_NOTE__LABEL, "All maps");
        set_value(PANEL_SUBNOTES__LABEL, "Maps:")

        //document.getElementById("child_example").remove();

        let children = document.getElementById("panel_subnotes__children");

        let maps = await list_entity("map");
        for (const map of maps) {

            const li = document.createElement("li");
            children.appendChild(li);
            li.innerText = "#" + map.id + " ";
            let a = document.createElement("a")
            li.appendChild(a);
            a.innerText = map.name;
            a.href = "index.html?entity=map&action=read&id=" + map.id;
            a.target = "_blank";


            //http://localhost:8888/web/index.html?entity=map&action=create#
            const button_copy = document.createElement("button");
            li.appendChild(button_copy);
            button_copy.innerText="Copy";

        }
    }
});