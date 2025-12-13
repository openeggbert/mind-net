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
    formatDateTimeHM, formatDateTime, showInfo, showError, showWarn
} from "./dom.js";
import {Autocomplete} from "./common.js";

let wasDragged = false;
let suppressPopstate = false;

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
                    window.hljs.highlight(str, { language: lang, ignoreIllegals: true }).value +
                    '</code></pre>';
            } catch (__) {}
        }
        return '<pre class="hljs"><code>' + md.utils.escapeHtml(str) + '</code></pre>';
    }
});

// plugin for emoji (:smile:, :rocket:, etc.)
md.use(window.markdownitEmoji);

// configure highlight.js appearance
window.hljs.configure({languages: ['cpp', 'js', 'json', 'html', 'sql', 'python']});

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
        const t = e.touches[0];
        startDrag(t.clientX, t.clientY);
        e.preventDefault();
    }, { passive: false });

    document.addEventListener('touchmove', e => {
        const t = e.touches[0];
        doDrag(t.clientX, t.clientY);
    }, { passive: false });

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

document.addEventListener('DOMContentLoaded', async () => {
    await init_dom();
});

class DictionaryApp {
    #input_search_term = document.getElementById("input_search_term")
    #autocomplete_term_title = null
    #term_container = new TermContainer()

    constructor() {
        this.select_map = new SelectMap()
        this.select_map.init()

        get_element("dictionary_header").title = "Go to home"
        get_element("dictionary_header").style.cursor = "pointer"
        get_element("button_mindnet").addEventListener("click", () => {
            window.location.href = 'index.html'
        });
        get_element("button_mindnet").title = "Go to Mind Net generic frontend"

        this.#autocomplete_term_title = new Autocomplete(this.#input_search_term, 1, "dictionary_term_fulltext", "&dictionary_map_id=" + this.select_map.get_selected_map_id(), "title", "title_part")
        this.#autocomplete_term_title.addCallback(async () => {
            let item = this.#autocomplete_term_title.get_item()
            showInfo("Found term: " + item.title + (item.disambiguation === "" ? "" : ("(" + item.disambiguation + ")")))
            await this.#term_container.render(item.id)
            this.#term_container.show()
        })
        get_element("button_add_term").onclick = async () => {
            if (this.#input_search_term.value === "") {
                showError("Could not create term, the title must not be empty.");
                return;
            }
            let new_term = {
                dictionary_map_id: this.select_map.get_selected_map_id(),
                title: this.#input_search_term.value,
            }
            let new_term_created = await post_entity("dictionary_term", new_term)
            if (new_term_created === null) {
                showError("Creating new term failed.")
                return;
            }
            showInfo("Created new term: " + new_term_created.title)
            await this.#term_container.render(new_term_created.id)
            this.#term_container.show()

        }

        //todo remove me
        this.#term_container.render(1)
        sleep_for_seconds(2)
        this.#term_container.show()
    }

    refresh_autocomplete_term_title() {
        this.#autocomplete_term_title.destroy()
        this.#autocomplete_term_title = new Autocomplete(this.#input_search_term, 1, "dictionary_term_fulltext", "&dictionary_map_id=" + this.select_map.get_selected_map_id(), "title", "title_part")
        this.#autocomplete_term_title.addCallback(async () => {
            let item = this.#autocomplete_term_title.get_item()
            showInfo("Found term: " + item.title + (item.disambiguation === "" ? "" : ("(" + item.disambiguation + ")")))
            await this.#term_container.show()
            this.#term_container.render(item.id)
        })
    }

    get_selected_map_id() {
        return this.select_map.get_selected_map_id()
    }
}
class SelectMap {
    #element
    #selected_map_id = 1
    #map = new Map();

    async list_maps_from_backend() {
        return await list_all_entities("dictionary_map", "&sort=position");
    }
    constructor() {
        this.#element = get_element("select_map");
        this.#element.innerHTML = ""

        this.#element.addEventListener("change", () => {
            this.#selected_map_id = this.#element.value
            dictionary_app.refresh_autocomplete_term_title()
            showInfo("Map changed to: " + this.#map.get(this.#element.value))
        });

        let button_add_map = get_element("button_add_map").onclick = async () => {
            const name = prompt("Enter map name");
            if (name === null) return;
            let new_map = {
                name: name,
                description: "",
                owner_id: getUserId(),
                team_id: 0,
                owner_rights: 7,
                team_rights: 0,
                other_rights: 0
            }
            let new_map_created = await post_entity("dictionary_map", new_map)

            if (new_map_created === null) return
            this.add_map(new_map_created.id, new_map_created.name)
        }
    }
    get_selected_map_id() {
        return this.#selected_map_id
    }
    async init() {
        const maps = await this.list_maps_from_backend();
        maps.forEach(e => {
            this.add_map(e.id, e.name);
        });
    }

    add_map(id, name) {
        let option = document.createElement("option")
        option.value = id
        option.innerText = name
        this.#element.appendChild(option)
        this.#map.set(option.value, option.innerText)
    }
}
class TermContainer {
    #element
    #dictionary_term_json
    dictionary_term_id = 0

    #tags
    #flags
    constructor() {
        this.#element = get_element("term_container");
        this.#tags = new Tags()
        this.#flags = new Flags()
    }
    show() {
        this.#element.style.display = "block"
    }
    hide() {
        this.#element.style.display = "none"
    }
    async render(dictionary_term_id) {
        if (this.dictionary_term_id === dictionary_term_id) {
            showWarn("This term is already shown.")
            return
        }
        if (dictionary_term_id === 0) return;

        await this.render_term(dictionary_term_id);
        this.#tags.render(dictionary_term_id)
        this.#flags.render(dictionary_term_id)
    }
    async render_term(dictionary_term_id) {
        this.dictionary_term_id = dictionary_term_id
        let dictionary_term = await read_entity("dictionary_term", dictionary_term_id)
        this.#dictionary_term_json = dictionary_term
        get_element("term_container_h2").innerText = "Term #" + dictionary_term.id
        get_element("input_title").value = dictionary_term.title
        get_element("input_disambiguation").value = dictionary_term.disambiguation
        get_element("textarea_definition").innerText = dictionary_term.definition

        let importance = dictionary_term.importance
        let difficulty = dictionary_term.difficulty
        let id1 = "input_importance_" + (importance === 1 ? "low" : (importance === 2 ? "medium" : "high"));

        let id2 = "input_difficulty_" + (difficulty === 1 ? "easy" : (difficulty === 2 ? "medium" : "hard"))

        let input_importance= get_element(id1)
        if(input_importance === null) {
            showWarn("There is no id: " + id1)
        }
        let input_difficulty= get_element(id2)
        input_importance.checked=true;
        input_difficulty.checked=true;

        get_element("textarea_definition").innerText = dictionary_term.definition

        get_element("button_delete_term").onclick = async () => {
            if (!confirm("Do you really want to delete this term and all its tags, flags, links, notes, sources and aliases?")) return;

            async function delete_rows(model_name, entities) {
                for (const e of entities) {
                    let delete_result = await delete_entity(model_name, e.id)
                    if(!delete_result) {
                        showError("Deleting " + model_name + " failed.");
                    }
                }
            }

            let flags = await list_all_entities(
                "dictionary_flag",
                "&dictionary_term_id=" + dictionary_term_id + "&user_id=" + getUserId())
            let links1 = await list_all_entities(
                "dictionary_link",
                "&from_dictionary_term_id=" + dictionary_term_id)
            let links2 = await list_all_entities(
                "dictionary_link",
                "&to_dictionary_term_id=" + dictionary_term_id)
            let notes = await list_all_entities(
                "dictionary_note",
                "&dictionary_term_id=" + dictionary_term_id)
            let reviews = await list_all_entities(
                "dictionary_review",
                "&dictionary_term_id=" + dictionary_term_id + "&user_id" + getUserId())
            let sources = await list_all_entities(
                "dictionary_source",
                "&dictionary_term_id=" + dictionary_term_id)
            let states4 = await list_all_entities(
                "dictionary_state_4",
                "&dictionary_term_id=" + dictionary_term_id + "&user_id" + getUserId())
            let tags = await list_all_entities(
                "dictionary_tag",
                "&dictionary_term_id=" + dictionary_term_id)
            let aliases = await list_all_entities(
                "dictionary_term_alias",
                "&dictionary_term_id=" + dictionary_term_id)
            let visits = await list_all_entities(
                "dictionary_term_visit",
                "&dictionary_term_id=" + dictionary_term_id + "&user_id" + getUserId())
            await delete_rows("dictionary_flag", flags)
            await delete_rows("dictionary_link", links1)
            await delete_rows("dictionary_link", links2)
            await delete_rows("dictionary_note", notes)
            await delete_rows("dictionary_review", reviews)
            await delete_rows("dictionary_source", sources)
            await delete_rows("dictionary_state_4", states4)
            await delete_rows("dictionary_tag", tags)
            await delete_rows("dictionary_term_alias", aliases)
            await delete_rows("dictionary_term_visit", visits)
            let delete_dictionary_term = await delete_entity("dictionary_term", dictionary_term_id)
            if(delete_dictionary_term !== null && delete_dictionary_term !== undefined) {
                showInfo("Deleted dictionary term: " + dictionary_term.title)
                this.hide()
            } else {
                showError("Deleting dictionary term failed: " + dictionary_term.title)
            }
        }

        get_element("button_save_term").onclick = async () => {
            let new_term =structuredClone(this.#dictionary_term_json)
            new_term.title=get_element("input_title").value
            new_term.disambiguation=get_element("input_disambiguation").value
            new_term.definition=get_element("textarea_definition").value

            let importance = 0
            let difficulty = 0
            let input_importance_low= get_element("input_importance_low")
            let input_importance_medium= get_element("input_importance_medium")
            let input_importance_high= get_element("input_importance_high")
            let input_difficulty_easy= get_element("input_difficulty_easy")
            let input_difficulty_medium= get_element("input_difficulty_medium")
            let input_difficulty_hard= get_element("input_difficulty_hard")
            if(input_importance_low.checked) importance=1
            if(input_importance_medium.checked) importance=2
            if(input_importance_high.checked) importance=3
            if(input_difficulty_easy.checked) difficulty=1
            if(input_difficulty_medium.checked) difficulty=2
            if(input_difficulty_hard.checked) difficulty=3
            new_term.importance = importance
            new_term.difficulty = difficulty
            let updated = put_entity("dictionary_term", dictionary_term_id, new_term)
            if(updated !== null && updated !== undefined) {
                showInfo("Dictionary term was successfully updated.")
                this.#dictionary_term_json = new_term
            } else {
                showError("Updating dictionary term failed.")
            }
        }
        get_element("button_show_backlinks").onclick = () => {
            let url = "index.html?entity=dictionary_link&action=list&to_dictionary_term_id=" + dictionary_term_id
            showWindowFrom("Backlinks", url)
        }
        get_element("button_show_visited").onclick = () => {
            let url = "index.html?entity=dictionary_term_visit&action=list&dictionary_term_id=" + dictionary_term_id + "&user_id=" + getUserId()
            showWindowFrom("Backlinks", url)
        }
        let new_visit = {
            dictionary_term_id: dictionary_term_id,
            user_id: getUserId()
        }
        let created_dictionary_term_visit = post_entity("dictionary_term_visit", new_visit)
        if(created_dictionary_term_visit === null || created_dictionary_term_visit === undefined) {
            showError("Creating new term visit failed.")
        }
    }
}

class Tags {
    #element
    #input_search_tag = document.getElementById("input_search_tag")
    #autocomplete_tag_title = null
    constructor() {
        this.#element = get_element("tags");
        this.#element.innerHTML = ""
    }
    show() {
        this.#element.style.display = "block"
    }
    hide() {
        this.#element.style.display = "none"
    }

    async render(dictionary_term_id) {
        this.#element.innerHTML = ""
        get_element("div_search_tag").style.display = "none"
        if(this.#autocomplete_tag_title !== null) {
            this.#autocomplete_tag_title.destroy()
            this.#autocomplete_tag_title = null
        }
        this.#autocomplete_tag_title = new Autocomplete(
            this.#input_search_tag,
            1,
            "dictionary_tag_type_fulltext",
            "&dictionary_map_id=" + dictionary_app.get_selected_map_id(),
            "title",
            "title_part",
        "div_search_tag_end"
        )

        this.#autocomplete_tag_title.addCallback(async () => {

            let item = this.#autocomplete_tag_title.get_item()
            showInfo("Found tag: " + item.title)
            let dictionary_tag_type_id = item.id
            let new_tag = {
                dictionary_term_id: dictionary_term_id,
                dictionary_tag_type_id: dictionary_tag_type_id
            }

            let tag_created = await post_entity("dictionary_tag", new_tag)
            if(tag_created === null || tag_created === undefined) {
                showError("Creating tag failed: " + item.title)
                return
            }
            showInfo("New tag was assigned: " + item.title)
            this.add_tag(item.title, tag_created.id)
            //get_element("div_search_tag").style.display = "none"
        })

        let tags_result = await list_all_entities("dictionary_tag", "&dictionary_term_id=" + dictionary_term_id)
        if(!tags_result) {
            showError("Listing tags failed.")
            return;
        }
        showInfo("Found " + tags_result.length + " tags")
        for (const dictionary_tag_json of tags_result) {

            let tag_type = await read_entity("dictionary_tag_type", dictionary_tag_json.dictionary_tag_type_id)
            if(!tag_type) {
                showError("Loading tag type failed: " + dictionary_tag_json.dictionary_tag_type_id)
                continue
            }
            let title = tag_type.title

            this.add_tag(title, dictionary_tag_json.id)
        }

        let button_add_tag = get_element("button_add_tag")
        button_add_tag.onclick = async () => {
            get_element("div_search_tag").style.display = "block"
            let title = this.#input_search_tag.value
            if (title === "") {
                return;
            }

            let new_tag_type = {
                dictionary_map_id: dictionary_app.get_selected_map_id(),
                title: title
            }
            let new_tag_type_created = await post_entity("dictionary_tag_type", new_tag_type)
            if (!new_tag_type_created) {
                showError("Creating new tag type failed: " + title)
                return
            }
            let new_tag = {
                dictionary_term_id: dictionary_term_id,
                dictionary_tag_type_id: new_tag_type_created.id
            }

            let tag_created = await post_entity("dictionary_tag", new_tag)
            if (tag_created === null || tag_created === undefined) {
                showError("Creating tag failed: " + title)
                return
            }
            showInfo("New tag was assigned: " + title)
            this.add_tag(title, tag_created.id)
            //get_element("div_search_tag").style.display = "none"
        }
        get_element("button_show_tags").onclick = () => {
            let url = "index.html?entity=dictionary_tag_type&action=list&dictionary_map_id=" + dictionary_app.get_selected_map_id()
            showWindowFrom("Show tags", url)
        }
    }
    add_tag(title, id) {
        let div = document.createElement("div")
        div.classList.add("tag")
        this.#element.appendChild(div)
        div.innerText = title
        let button = document.createElement("button")
        button.innerHTML = "&times;"
        button.onclick = () => {
            let tag_deleted = delete_entity("dictionary_tag", id)
            let deleted = tag_deleted !== null && tag_deleted !== undefined
            if(deleted) {
                showInfo("Tag was successfully deleted: " + title)
                div.remove()
            } else {
                showError("Deleting tag failed: " + title)
            }
        }
        div.appendChild(button)

        this.#input_search_tag.value = ""
    }
}


class Flags {
    #element
    constructor() {
        this.#element = get_element("flags");
        this.#element.innerHTML = ""
    }
    show() {
        this.#element.style.display = "block"
    }
    hide() {
        this.#element.style.display = "none"
    }

    async render(dictionary_term_id) {
        let private_flags_result = await list_all_entities("dictionary_flag", "&dictionary_term_id=" + dictionary_term_id + "&is_public=0" + "&user_id=" + getUserId())
        if(!private_flags_result) {
            showError("Listing private flags failed.")
            return;
        }
        let public_flags_result = await list_all_entities("dictionary_flag", "&dictionary_term_id=" + dictionary_term_id + "&is_public=1")
        if(!public_flags_result) {
            showError("Listing public flags failed.")
            return;
        }
        for (const e of private_flags_result) {
            this.add_flag(e.title, e.id, false)
        }
        for (const e of public_flags_result) {
            if(e.user_id === getUserId()) continue
            this.add_flag(e.title, e.id, true)
        }

        let button_add_flag = get_element("button_add_flag")
        button_add_flag.onclick = async () => {
            const title = prompt("Enter map name");
            if (title === null || title === "") return;

            let is_public = confirm("Should be the flag public?")

            let new_flag = {
                dictionary_term_id: dictionary_term_id,
                user_id: getUserId(),
                title: title,
                is_public: is_public? 1 : 0
            }

            let flag_created = await post_entity("dictionary_flag", new_flag)
            if (flag_created === null || flag_created === undefined) {
                showError("Creating flag failed: " + title)
                return
            }
            showInfo("New flag was assigned: " + title)
            this.add_flag(title, flag_created.id, is_public)
            //get_element("div_search_tag").style.display = "none"

        }
    }
    add_flag(title, id, is_public = true) {
        let div = document.createElement("div")
        div.classList.add("tag")
        this.#element.appendChild(div)
        div.innerText = title
        div.style.backgroundColor = "#e0e0e0"
        div.style.color = "#2b2b2b"
        if(!is_public) {
            div.style.border = "1px dashed #8fa3b8"
            div.innerText = "🔒 " + title;
        }
        let button = document.createElement("button")
        button.innerHTML = "&times;"
        button.onclick = async () => {
            let flag_deleted = await delete_entity("dictionary_flag", id)
            let deleted = flag_deleted !== null && flag_deleted !== undefined
            if (deleted) {
                showInfo("Flag was successfully deleted: " + title)
                div.remove()
            } else {
                showError("Deleting flag failed: " + title)
            }
        }
        let button_background = div.style.background;
        button.onmouseenter = () => {
            button.style.background = "#bbb";
        };

        button.onmouseleave = () => {
            button.style.background = button_background;
        };
        div.appendChild(button)
    }
}

let dictionary_app = null
async function init_dom() {
    dictionary_app = new DictionaryApp()

    const win = document.getElementById('window_container');
    makeDraggable(win);
}