import {API_BASE, apiFetch} from "./api.js";

export function togglePanel(element, id) {
    const el = document.getElementById(id);
    const isCollapsed = el.classList.toggle('collapsed');

    element.textContent = isCollapsed ? "▶" : "▼";

    if (id === "meta_content") {
        const metaPanel = document.getElementById("meta_panel");
        const metaLabel = document.getElementById("meta_label");
        if(window.innerWidth > 800)
        {
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

export function closeModal() {
    document.getElementById('collections-modal').style.display = 'none';
}

window.closeModal = closeModal;

export function openModal() {
    document.getElementById('collections-modal').style.display = 'block';
}

window.openModal = openModal;

function hide_element(id) {
    document.getElementById(id).style.display = "none";
}

function set_value(id, value) {
    document.getElementById(id).innerHTML = value;
}

const PANEL_NOTE__LABEL = "panel_note__label";
const PANEL_NOTE__NOTE_ID = "panel_note__note_id";
const PANEL_NOTE__NOTE_TITLE = "panel_note__note_title";
const PANEL_SUBNOTES__LABEL = "panel_subnotes__label";

let map_id = "";
let note_id = "";
let mode_list_maps = false;
let mode_list_root_notes = false;
let mode_list_notes = false;

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

document.addEventListener('DOMContentLoaded', () => {
    document.querySelectorAll('.actions button').forEach(btn => {
        btn.addEventListener('click', () => {
            const label = btn.textContent.trim();
            if (label.includes('Copy')) showToast('Copied to clipboard');
            if (label.includes('Save')) showToast('Note saved');
            if (label.includes('Delete')) showToast('Note deleted');
        });
    });


    document.querySelectorAll('.add').forEach(btn => {
        btn.addEventListener('click', () => showToast('Subnote added'));
    });
});

document.addEventListener("DOMContentLoaded", async function () {
    //init_state();
    if (mode_list_maps && false) {
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
            button_copy.innerText = "Copy";

        }
    }
});