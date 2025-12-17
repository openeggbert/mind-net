// ========================================
// Imports & Globals
// ========================================
import {
    delete_entity,
    getTitleCache, getUserId,
    list_all_entities, list_entities,
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
import {Autocomplete, null_or_undefined} from "./common.js";

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

        this.#autocomplete_term_title = new Autocomplete(this.#input_search_term, 1, "dictionary_term_fulltext", "&dictionary_map_id=" + this.select_map.get_selected_map_id(), "title", "title_part", "div_search_term_end")
        this.#autocomplete_term_title.addCallback(async () => {
            let item = this.#autocomplete_term_title.get_item()
            showInfo("Found term: " + item.title)
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
        // this.#term_container.render(1)
        // sleep_for_seconds(2)
        // this.#term_container.show()
    }

    refresh_autocomplete_term_title() {
        this.#autocomplete_term_title.destroy()
        this.#autocomplete_term_title = new Autocomplete(this.#input_search_term, 1, "dictionary_term_fulltext", "&dictionary_map_id=" + this.select_map.get_selected_map_id(), "title", "title_part", "div_search_term_end")
        this.#autocomplete_term_title.addCallback(async () => {
            let item = this.#autocomplete_term_title.get_item()
            showInfo("Found term: " + item.title)
            await this.#term_container.show()
            this.#term_container.render(item.id)
        })
    }

    get_selected_map_id() {
        return this.select_map.get_selected_map_id()
    }

    async render(dictionary_term_id) {
        await this.#term_container.render(dictionary_term_id)
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

    #sections = null

    constructor() {
        this.#element = get_element("term_container");
        this.#sections = [
            new Tags(),
            new Flags(),
            new Links(),
            new Notes(),
            new Indexes(),
            new Sources(),
            new Aliases()
        ]

        let term_container_h2 = get_element("term_container_h2")
        term_container_h2.style.backgroundColor = "rgba(213,215,221,0.6)"
        term_container_h2.style.color = "#2c3e50";
        term_container_h2.style.padding = "10px"
        term_container_h2.style.borderBottom = "1px solid #dcdfe3"

        function attach_onclick_to_label(models) {
            let label = get_element("label_" + models)
            label.style.cursor = "pointer"
            label.style.backgroundColor = "rgba(213,215,221,0.6)"
            label.style.color = "#2c3e50";
            label.style.borderBottom = "1px solid #dcdfe3"
            label.style.padding = "10px"
            label.onclick = () => {
                let container = get_element("container_" + models)
                let current_display = container.style.display;
                let shown = current_display === "block" || current_display === "";
                container.style.display = shown ? "none" : "block"
                let tmp_id = "label_" + models + "_tmp"
                if (shown) {
                    let tmp_div = document.createElement("div")
                    let start = models.charAt(0).toUpperCase() + models.slice(1)
                    tmp_div.innerText = start + " are hidden. " + "Click \"" + start + ":\" to show them again."

                    tmp_div.style.color = "grey"
                    //tmp_div.style.fontStyle = "italic"
                    tmp_div.id = tmp_id
                    label.after(tmp_div)
                } else {
                    get_element(tmp_id).remove()
                }

            }
        }

        this.#sections.forEach(section => {
            let models = section.get_configuration().models
            attach_onclick_to_label(models)
        })

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
        this.#sections.forEach(
            section => {
                section.render(dictionary_term_id)
            }
        )
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

        let input_importance = get_element(id1)
        if (input_importance === null) {
            showWarn("There is no id: " + id1)
        }
        let input_difficulty = get_element(id2)
        input_importance.checked = true;
        input_difficulty.checked = true;

        get_element("textarea_definition").innerText = dictionary_term.definition

        get_element("button_delete_term").onclick = async () => {
            if (!confirm("Do you really want to delete this term and all its tags, flags, links, notes, sources and aliases?")) return;

            async function delete_rows(model_name, entities) {
                for (const e of entities) {
                    let delete_result = await delete_entity(model_name, e.id)
                    if (!delete_result) {
                        showError("Deleting " + model_name + " failed.");
                    }
                }
            }

            let tags = await list_all_entities(
                "dictionary_tag",
                "&dictionary_term_id=" + dictionary_term_id)
            let private_flags = await list_all_entities(
                "dictionary_flag",
                "&is_public=0&dictionary_term_id=" + dictionary_term_id + "&user_id=" + getUserId())
            let public_flags = await list_all_entities(
                "dictionary_flag",
                "&is_public=1&dictionary_term_id=" + dictionary_term_id)
            let links1 = await list_all_entities(
                "dictionary_link",
                "&from_dictionary_term_id=" + dictionary_term_id)
            let links2 = await list_all_entities(
                "dictionary_link",
                "&to_dictionary_term_id=" + dictionary_term_id)
            let notes = await list_all_entities(
                "dictionary_note",
                "&dictionary_term_id=" + dictionary_term_id)
            let indexes = await list_all_entities(
                "dictionary_index",
                "&dictionary_term_id=" + dictionary_term_id)
            let sources = await list_all_entities(
                "dictionary_source",
                "&dictionary_term_id=" + dictionary_term_id)
            let aliases = await list_all_entities(
                "dictionary_term_alias",
                "&dictionary_term_id=" + dictionary_term_id)
            let visits = await list_all_entities(
                "dictionary_term_visit",
                "&dictionary_term_id=" + dictionary_term_id + "&user_id=" + getUserId())
            let reviews = await list_all_entities(
                "dictionary_review",
                "&dictionary_term_id=" + dictionary_term_id + "&user_id=" + getUserId())
            let states4 = await list_all_entities(
                "dictionary_state_4",
                "&dictionary_term_id=" + dictionary_term_id + "&user_id=" + getUserId())
            await delete_rows("dictionary_flag", private_flags)
            await delete_rows("dictionary_flag", public_flags)
            await delete_rows("dictionary_link", links1)
            await delete_rows("dictionary_link", links2)
            await delete_rows("dictionary_note", notes)
            await delete_rows("dictionary_review", reviews)
            await delete_rows("dictionary_index", indexes)
            await delete_rows("dictionary_source", sources)
            await delete_rows("dictionary_state_4", states4)
            await delete_rows("dictionary_tag", tags)
            await delete_rows("dictionary_term_alias", aliases)
            await delete_rows("dictionary_term_visit", visits)
            let delete_dictionary_term = await delete_entity("dictionary_term", dictionary_term_id)
            if (delete_dictionary_term !== null && delete_dictionary_term !== undefined) {
                showInfo("Deleted dictionary term: " + dictionary_term.title)
                this.hide()
            } else {
                showError("Deleting dictionary term failed: " + dictionary_term.title)
            }
        }

        get_element("button_save_term").onclick = async () => {
            let new_term = structuredClone(this.#dictionary_term_json)
            new_term.title = get_element("input_title").value
            new_term.disambiguation = get_element("input_disambiguation").value
            new_term.definition = get_element("textarea_definition").value

            let importance = 0
            let difficulty = 0
            let input_importance_low = get_element("input_importance_low")
            let input_importance_medium = get_element("input_importance_medium")
            let input_importance_high = get_element("input_importance_high")
            let input_difficulty_easy = get_element("input_difficulty_easy")
            let input_difficulty_medium = get_element("input_difficulty_medium")
            let input_difficulty_hard = get_element("input_difficulty_hard")
            if (input_importance_low.checked) importance = 1
            if (input_importance_medium.checked) importance = 2
            if (input_importance_high.checked) importance = 3
            if (input_difficulty_easy.checked) difficulty = 1
            if (input_difficulty_medium.checked) difficulty = 2
            if (input_difficulty_hard.checked) difficulty = 3
            new_term.importance = importance
            new_term.difficulty = difficulty
            let updated = put_entity("dictionary_term", dictionary_term_id, new_term)
            if (updated !== null && updated !== undefined) {
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
        get_element("button_show_visited").onclick = async () => {
            let visits_result = await list_entities("dictionary_term_visit", "&user_id=" + getUserId() + "&sort=created_at&order=desc", 1, 100)
            if (null_or_undefined(visits_result)) {
                showError("Loading visits failed.")
                return;
            }
            let visits = visits_result.items
            const disambiguation_map = new Map()

            let table = document.createElement("table");
            table.style.borderCollapse = "collapse";
            table.style.margin = "0 auto";
            let tr_first = document.createElement("tr");
            table.appendChild(tr_first);
            let th_number = document.createElement("th");
            th_number.innerText = "#"
            let th_id = document.createElement("th");
            th_id.innerText = "Term ID"
            let th_title = document.createElement("th");
            th_title.innerText = "Title"
            let th_timestamp = document.createElement("th");
            th_timestamp.innerText = "Timestamp"
            tr_first.appendChild(th_number)
            //tr_first.appendChild(th_id)
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
            for (const entry of visits.slice()) {
                history_entry_number++

                let visited_term_id = entry.dictionary_term_id

                if (!disambiguation_map.has(visited_term_id)) {
                    let term = await read_entity("dictionary_term", visited_term_id)
                    if (term === null || term === undefined) {
                        showWarn("Loading dictionary_term with id " + visited_term_id + " failed.");
                        disambiguation_map.set(visited_term_id, "")
                    } else {
                        disambiguation_map.set(visited_term_id, term.disambiguation)
                    }
                }

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
                //tr.appendChild(td_id)
                tr.appendChild(td_title)
                tr.appendChild(td_timestamp)
                td_number.innerText = history_entry_number;
                td_id.innerText = visited_term_id;
                let a = document.createElement("a");
                a.href = "?";
                {
                    let title = getTitleCache("dictionary_term", visited_term_id)
                    if (title === null || title === undefined) {

                        let x = entry.id
                        let note_ = await read_entity("dictionary_term", x)
                        if (note_ === null) {
                            showWarn("Loading dictionary_term with id " + x + " failed.");
                            title = "Unknown (#" + x + ")"
                        } else {
                            title = note_.title
                            setTitleCache("dictionary_term", x, title);
                        }
                    }
                    let finalTitle = title
                    if (disambiguation_map.has(visited_term_id)) {
                        let value = disambiguation_map.get(visited_term_id)
                        if (value !== "") finalTitle = title + " (" + disambiguation_map.get(visited_term_id) + ")"
                    }
                    a.title = finalTitle

                    a.innerText = finalTitle;
                }

                a.onclick = async function () {
                    event.preventDefault();
                    await dictionary_app.render(visited_term_id)
                }
                td_title.appendChild(a)
                td_timestamp.innerText = formatDateTime(entry.created_at, true, true, true)
            }
            clearWindow()

            let button = document.createElement("button")
            button.innerText = "Show all visits"
            button.onclick = () => {
                let url = "index.html?entity=dictionary_term_visit&action=list&user_id=" + getUserId()
                showWindowFrom("All visits", url)
            }
            button.style.margin = "20px;"
            button.style.textAlign = "center"

            getWindowContent().appendChild(button)

            getWindowContent().appendChild(table);
            getWindowContent().style.height = "100%";
            setWindowTitle("Term Visit History (Last 100)")

            showWindow()

        }
        let new_visit = {
            dictionary_term_id: dictionary_term_id,
            user_id: getUserId(),
            dictionary_map_id: dictionary_app.get_selected_map_id()
        }
        let created_dictionary_term_visit = post_entity("dictionary_term_visit", new_visit)
        if (created_dictionary_term_visit === null || created_dictionary_term_visit === undefined) {
            showError("Creating new term visit failed.")
        }
    }
}

function defined(value) {
    return value !== null && value !== undefined
}

class CrudConfiguration {
    //string
    model
    //string
    models
    //string
    table
    //bool
    filter
    input
    resolveTitle
    createAutocomplete
    autocompleteCallback
}

function validate_cfg(cfg) {
    let to_be_validated = [
        cfg.model, cfg.models, cfg.table, cfg.input, cfg.resolveTitle
    ]
    to_be_validated.forEach(e => {
        if(!defined(e)) return false
    })
    return true
}

class CrudSection {
    #cfg
    #element;
    #input= null
    #autocomplete = null

    constructor(cfg) {
        if(!validate_cfg(cfg)) throw "Configuration is not valid for model: " + cfg.model
        this.#cfg = cfg
        this.#element = get_element(cfg.models);
        this.#input = cfg.input ? get_element("input_search_" + this.#cfg.model) : null;
        this.autocomplete = null;
        this.#reset()
    }

    get_configuration() {
        return this.#cfg
    }

    #setVisible(visible) {
        this.#element.style.display = visible ? "block" : "none";
    }

    show() {
        this.#setVisible(true);
    }

    hide() {
        this.#setVisible(false);
    }

    #reset() {
        this.#element.innerHTML = "";
    }

    get _element() {
        return this.#element;
    }

    async render(dictionary_term_id) {
        this.#reset();

        const items = await this.loadItems(dictionary_term_id);
        if (items === null || items === undefined) {
            showError("Listing " + this.#cfg.models + " failed.")
            return
        }
        for (const item of items) {
            if (item === null) throw "item is null"
            if (item === undefined) throw "item is undefined"

            let title = await this.#cfg.resolveTitle(item)
            if (title === null || title === undefined) {
                showError("Loading title failed for model: " + this.#cfg.model + " and id " + item.id)
                continue
            }
            this.addItem(title, item.id, item);
        }
        if(this.#cfg.input) get_element("div_search_" + this.#cfg.model).style.display = "none"
        this.#setupAutocomplete(dictionary_term_id);
        this.afterRender(dictionary_term_id);
    }

    async loadItems(dictionary_term_id) {
        let filter = this.#cfg.filter
        if(filter === null || filter === undefined) throw "Not implemented";

        return await list_all_entities(
            this.#cfg.table,
            this.#cfg.filter(dictionary_term_id)
        );
    }

    addItem(title, id, item) {
        throw "Not implemented";
    }

    afterRender(dictionary_term_id) {
        // optional hook
    }
    get_input_value() {
        return this.#input.value
    }
    clear_input_value() {
        this.#input.value = ""
    }

    #setupAutocomplete(termId) {
        if (!defined(this.#cfg.createAutocomplete)) return
        if (!defined(this.#cfg.autocompleteCallback)) return

        if (this.#autocomplete) this.#autocomplete.destroy();

        this.#autocomplete = this.#cfg.createAutocomplete(this.#input)

        this.#autocomplete.addCallback(async () => {
            const item = this.#autocomplete.get_item();
            if (!item) {
                showWarn("Autocomplete returned null item");
                return;
            }
            await this.#cfg.autocompleteCallback(item, termId)
        });
    }
}

class Tags extends CrudSection {
    constructor() {
        super({
            model: "tag",
            models: "tags",
            table: "dictionary_tag",
            filter: termId => "&dictionary_term_id=" + termId,
            input: true,
            resolveTitle: async item => {
                let tag_type = await read_entity("dictionary_tag_type", item.dictionary_tag_type_id)
                if (!tag_type) {
                    showError("Loading tag type failed: " + item.dictionary_tag_type_id)
                    return null
                }
                return tag_type.title
            },
            createAutocomplete: input => {
                return new Autocomplete(
                    input,
                    1,
                    "dictionary_tag_type_fulltext",
                    "&dictionary_map_id=" + dictionary_app.get_selected_map_id(),
                    "title",
                    "title_part",
                    "div_search_tag_end"
                )
            },
            autocompleteCallback: async (item, termId) => {
                let new_tag = {
                    dictionary_term_id: termId,
                    dictionary_tag_type_id: item.id
                }

                let tag_created = await post_entity("dictionary_tag", new_tag);
                if (tag_created === null || tag_created === undefined) {
                    showError("Creating tag failed: " + item.title)
                    return
                }
                showInfo("New tag was assigned: " + item.title)
                this.addItem(item.title, tag_created.id)
            }
        });
    }

    afterRender(dictionary_term_id) {
        let button_add_tag = get_element("button_add_tag")
        button_add_tag.onclick = async () => {
            get_element("div_search_tag").style.display = "block"
            let title = this.get_input_value()
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
            this.addItem(title, tag_created.id)
        }
        get_element("button_show_tags").onclick = () => {
            let url = "index.html?entity=dictionary_tag_type&action=list&dictionary_map_id=" + dictionary_app.get_selected_map_id()
            showWindowFrom("Show tags", url)
        }
    }

    addItem(title, id, item) {
        let div = document.createElement("div")
        div.classList.add("tag")
        this._element.appendChild(div)
        div.innerText = title
        let button = document.createElement("button")
        button.innerHTML = "&times;"
        button.onclick = async () => {
            if (!confirm("Do you really want to delete this tag?")) return;
            let tag_deleted = await delete_entity("dictionary_tag", id)
            let deleted = tag_deleted !== null && tag_deleted !== undefined
            if (deleted) {
                showInfo("Tag was successfully deleted: " + title)
                div.remove()
            } else {
                showError("Deleting tag failed: " + title)
            }
        }
        div.appendChild(button)

        this.clear_input_value()
    }
}

class Flags extends CrudSection {
    constructor() {
        super({
            model: "flag",
            models: "flags",
            table: "dictionary_flag",
            input: false,
            resolveTitle: async item => {
                return item.title
            },
        });
    }

    async loadItems(dictionary_term_id) {
        let result = []
        let private_flags_result = await list_all_entities("dictionary_flag", "&dictionary_term_id=" + dictionary_term_id + "&is_public=0" + "&user_id=" + getUserId())
        if (!private_flags_result) {
            showError("Listing private flags failed.")
            return [];
        } else {
            private_flags_result.forEach(e => {
                result.push(e)
            })

        }
        let public_flags_result = await list_all_entities("dictionary_flag", "&dictionary_term_id=" + dictionary_term_id + "&is_public=1")
        if (!public_flags_result) {
            showError("Listing public flags failed.")
            return [];
        } else {
            let user_id = getUserId();
            public_flags_result.forEach(e => {
                if (e.user_id !== user_id) result.push(e)
            });
        }
        return result;
    }

    afterRender(dictionary_term_id) {
        let button_add_flag = get_element("button_add_flag")

        let input_checkbox_public_flag = get_element("input_checkbox_public_flag")
        input_checkbox_public_flag.checked = false

        button_add_flag.onclick = async () => {
            const title = prompt("Enter flag title");
            if (title === null || title === "") return;

            let is_public = input_checkbox_public_flag.checked
            let new_flag = {
                dictionary_term_id: dictionary_term_id,
                user_id: getUserId(),
                title: title,
                is_public: is_public ? 1 : 0
            }

            let flag_created = await post_entity("dictionary_flag", new_flag)
            if (flag_created === null || flag_created === undefined) {
                showError("Creating flag failed: " + title)
                return
            }
            showInfo("New flag was assigned: " + title)
            this.addItem(title, flag_created.id, flag_created)

        }
    }

    addItem(title, id, item) {
        let is_public = item.is_public

        let div = document.createElement("div")
        div.classList.add("tag")
        this._element.appendChild(div)
        div.innerText = title
        div.style.backgroundColor = "#e0e0e0"
        div.style.color = "#2b2b2b"
        if (!is_public) {
            div.style.border = "1px dashed #8fa3b8"
            div.innerText = "🔒 " + title;
        }
        let button = document.createElement("button")
        button.innerHTML = "&times;"
        button.onclick = async () => {
            if (!confirm("Do you really want to delete this flag?")) return;
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

class Links extends CrudSection {
    constructor() {
        super({
            model: "link",
            models: "links",
            table: "dictionary_link",
            filter: termId => "&from_dictionary_term_id=" + termId,
            input: true,
            resolveTitle: async item => {
                let another_dictionary_term = await read_entity("dictionary_term", item.to_dictionary_term_id)
                if (!another_dictionary_term) {
                    showError("Loading term failed: " + item.to_dictionary_term_id)
                    return null
                }
                return another_dictionary_term.title
            },
            createAutocomplete: input => {
                return new Autocomplete(
                    input,
                    1,
                    "dictionary_term_fulltext",
                    "&dictionary_map_id=" + dictionary_app.get_selected_map_id(),
                    "title",
                    "title_part",
                    "div_search_link_end"
                )
            },
            autocompleteCallback: async (item, termId) => {
                let another_dictionary_term_id = item.id
                let new_link = {
                    from_dictionary_term_id: termId,
                    to_dictionary_term_id: another_dictionary_term_id
                }

                let link_created = await post_entity("dictionary_link", new_link)
                if (link_created === null || link_created === undefined) {
                    showError("Creating link failed: " + item.title)
                    return
                }
                showInfo("New link was created: " + item.title)
                this.addItem(item.title, link_created.id, link_created.to_dictionary_term_id)
            }

        });
    }

    async afterRender(dictionary_term_id) {
        let button_add_link = get_element("button_add_link")
        button_add_link.onclick = async () => {
            let was_hidden = get_element("div_search_link").style.display === "none"
            get_element("div_search_link").style.display = "block"
            let title = this.get_input_value()
            if (title === "") {
                if (!was_hidden) showWarn("Term title must not be empty")
                return;
            }

            let new_term = {
                dictionary_map_id: dictionary_app.get_selected_map_id(),
                title: title,
            }

            let term_created = await post_entity("dictionary_term", new_term)
            if (term_created === null || term_created === undefined) {
                showError("Creating term failed: " + title)
                return
            }

            let new_flag = {
                dictionary_term_id: term_created.id,
                user_id: getUserId(),
                title: "stub",
                is_public: 1
            }

            let flag_created = await post_entity("dictionary_flag", new_flag)
            if (flag_created === null || flag_created === undefined) {
                showError("Creating tag failed: " + title)
                return
            }

            let new_link = {
                from_dictionary_term_id: dictionary_term_id,
                to_dictionary_term_id: term_created.id
            }

            let link_created = await post_entity("dictionary_link", new_link)
            if (link_created === null || link_created === undefined) {
                showError("Creating link failed: " + title)
                return
            }
            showInfo("New link was assigned: " + title)
            this.addItem(title, link_created.id, link_created.to_dictionary_term_id)
        }
    }

    addItem(title, id, item) {
        let to_dictionary_term_id = item.to_dictionary_term_id
        let div = document.createElement("div")
        div.classList.add("item")
        this._element.appendChild(div)

        let spanElement = document.createElement("span")
        div.appendChild(spanElement)
        let a = document.createElement("a")
        a.onclick = async () => {
            await dictionary_app.render(to_dictionary_term_id)
        }
        a.innerText = title
        spanElement.appendChild(a)
        let span = document.createElement("span")
        spanElement.appendChild(span)

        function termRelationTypeToString(type) {
            switch (type) {
                case 0: return "Not defined";   // NotDefined
                case 1: return "Is a";          // IsA
                case 2: return "Part of";       // PartOf
                case 3: return "Uses";          // Uses
                case 4: return "Depends on";    // DependsOn
                case 5: return "Implements";    // Implements
                case 6: return "Related";       // Related
                case 7: return "Contrasts";     // Contrasts
                case 8: return "Alternative to";// AlternativeTo
                default:
                    return "Not defined";
            }
        }

        function stringToTermRelationType(str) {
            switch (str) {
                case "Not defined":    return 0; // NotDefined
                case "Is a":           return 1; // IsA
                case "Part of":        return 2; // PartOf
                case "Uses":           return 3; // Uses
                case "Depends on":     return 4; // DependsOn
                case "Implements":     return 5; // Implements
                case "Related":        return 6; // Related
                case "Contrasts":      return 7; // Contrasts
                case "Alternative to": return 8; // AlternativeTo
                default:
                    return 0; // NotDefined
            }
        }

        span.innerText = " (" + termRelationTypeToString(item.type) + ")"
        span.style.color = "black"
        span.style.marginLeft = "10px"

        let div_buttons = document.createElement("div")

        let edit_button = document.createElement("button")
        edit_button.innerHTML = "📝 Edit"
        edit_button.onclick = async () => {
            let option = await chooseOption([
                "Not defined",
                "Is a",
                "Part of",
                "Uses",
                "Depends on",
                "Implements",
                "Related",
                "Contrasts",
                "Alternative to"
            ])
            if(option === null || option === undefined) return
            let type = stringToTermRelationType(option)

            let read_link = await read_entity("dictionary_link", id)

            let read = read_link !== null && read_link !== undefined
            if (!read) {
                showError("Reading link failed: " + title)
            }
            read_link.type = type
            let updated = await put_entity("dictionary_link", id, read_link)
            if(updated) {
                showInfo("Updating link type to " + option + " was successful.");
                span.innerText = " (" + option + ")"
            } else {
                showError("Updating link type to " + option + " failed.");
            }
        }
        div_buttons.appendChild(edit_button)

        let delete_button = document.createElement("button")
        delete_button.style.marginLeft = "10px"
        delete_button.innerHTML = "🗑️ Delete"
        delete_button.onclick = () => {
            if (!confirm("Do you really want to delete this link?")) return;
            let link_deleted = delete_entity("dictionary_link", id)
            let deleted = link_deleted !== null && link_deleted !== undefined
            if (deleted) {
                showInfo("Link was successfully deleted: " + title)
                div.remove()
            } else {
                showError("Deleting link failed: " + title)
            }
        }
        div_buttons.appendChild(delete_button)

        div.appendChild(div_buttons)
        this.clear_input_value()
    }
}

class Notes extends CrudSection {

    constructor() {
        super({
            model: "note",
            models: "notes",
            table: "dictionary_note",
            filter: termId => "&dictionary_term_id=" + termId + "&sort=position",
            input: false,
            resolveTitle: async item => {
                return item.title
            },
        });
    }

    async afterRender(dictionary_term_id) {
        let button_add_note = get_element("button_add_note")

        button_add_note.onclick = async () => {
            const title = prompt("Enter note title");
            if (title === null || title === "") return;

            let new_note = {
                dictionary_term_id: dictionary_term_id,
                title: title,
                position: 0
            }

            let note_created = await post_entity("dictionary_note", new_note)
            if (note_created === null || note_created === undefined) {
                showError("Creating note failed: " + title)
                return
            }
            showInfo("New note was created: " + title)
            this.addItem(title, note_created.id)
        }

    }

    addItem(title, id) {
        let div = document.createElement("div")
        div.classList.add("item")
        this._element.appendChild(div)

        let span = document.createElement("span")
        span.innerText = title
        div.appendChild(span)
        div.onclick = async (e) => {
            let div_id = "notes_" + id;
            let note_details = get_element(div_id)

            let note_details_exist = note_details !== null && note_details !== undefined;

            if (e.target.tagName === "BUTTON") {
                if (note_details_exist) note_details.remove()
                return;
            }

            let id_title = "input_note_title_" + id
            let id_position = "input_note_position_" + id
            let id_content = "text_area_content_" + id

            let read_note = await read_entity("dictionary_note", id)
            if (read_note === null || read_note === undefined) {
                showError("Loading note failed: " + title)
                return;
            }

            if (note_details_exist) {
                let unsaved_changes = false
                let title_changed = false
                let position_changed = false
                let content_changed = false
                title_changed = get_element(id_title).value !== read_note.title
                position_changed = Number(get_element(id_position).value) !== read_note.position
                content_changed = get_element(id_content).value !== read_note.content

                unsaved_changes = title_changed || position_changed || content_changed
                if (unsaved_changes) {
                    if (!confirm("Do you really want to collapse this note? Unsaved changes will be lost.")) return;
                }

                note_details.remove()
                return
            }

            note_details = document.createElement("div")
            note_details.id = div_id
            div.after(note_details)
            note_details.style.border = "1px solid #ddd"
            note_details.style.backgroundColor = "#e4e09c"
            note_details.style.padding = "10px"

            function make_label(forId, innerText) {
                let label = document.createElement("label")
                label.for = forId
                label.innerText = innerText + ": "
                label.style.display = "inline-block"
                label.style.marginTop = "10px"
                label.style.marginRight = "10px"
                label.style.minWidth = "100px"
                note_details.appendChild(label)
            }


            make_label(id_title, "Title")

            let input_title = document.createElement("input")
            input_title.type = "text"
            input_title.value = read_note.title
            input_title.style.maxWidth = "200px"
            input_title.id = id_title

            note_details.appendChild(input_title)
            note_details.appendChild(document.createElement("br"))

            make_label(id_position, "Position")

            let input_position = document.createElement("input")
            input_position.type = "text"
            input_position.value = read_note.position
            input_position.style.maxWidth = "200px"
            input_position.id = id_position

            note_details.appendChild(input_position)
            note_details.appendChild(document.createElement("br"))

            make_label(id_content, "Content")
            note_details.appendChild(document.createElement("br"))

            let text_area_content = document.createElement("textarea")
            text_area_content.value = read_note.content
            text_area_content.style.width = "100%"
            text_area_content.style.boxSizing = "border-box"
            text_area_content.id = id_content
            text_area_content.style.height = "200px"

            note_details.appendChild(text_area_content)
            note_details.appendChild(document.createElement("br"))

            let save_button = document.createElement("button")
            save_button.id = "button_save_term_note_" + id;
            save_button.classList.add("save-btn")
            save_button.innerText = "Save"
            note_details.appendChild(save_button)

            save_button.onclick = async () => {
                read_note.title = input_title.value
                read_note.position = input_position.value
                read_note.content = text_area_content.value
                let updated = await put_entity("dictionary_note", read_note.id, read_note)
                if (updated === null || updated === undefined) {
                    showError("Updating note failed: " + read_note.title)
                    return
                }
                span.innerText = read_note.title
                showInfo("Note was successfully updated: " + read_note.title)
            }

        }

        let div_buttons = document.createElement("div")
        let edit_button = document.createElement("button")
        edit_button.innerHTML = "📝 Edit"
        edit_button.style.marginRight = "10px"
        edit_button.onclick = async () => {
            div.click()
        }
        div_buttons.appendChild(edit_button)

        let delete_button = document.createElement("button")
        delete_button.innerHTML = "🗑️ Delete"
        delete_button.onclick = async () => {
            if (!confirm("Do you really want to delete this note?")) return;
            let note_deleted = await delete_entity("dictionary_note", id)
            let deleted = note_deleted !== null && note_deleted !== undefined
            if (deleted) {
                showInfo("Note was successfully deleted: " + title)
                div.remove()
            } else {
                showError("Deleting note failed: " + title)
            }
        }
        div_buttons.appendChild(delete_button)
        div.appendChild(div_buttons)
    }
}

class Indexes extends CrudSection {
    constructor() {
        super({
            model: "index",
            models: "indexes",
            table: "dictionary_index",
            filter: termId => "&dictionary_term_id=" + termId + "&sort=position",
            input: true,
            resolveTitle: async item => {
                let index_type = await read_entity("dictionary_index_type", item.dictionary_index_type_id)
                if (!index_type) {
                    showError("Loading index type failed: " + item.dictionary_index_type_id)
                    return null;
                }
                return index_type.title
            },
            createAutocomplete: input => {
                return new Autocomplete(
                    input,
                    1,
                    "dictionary_index_type_fulltext",
                    "&dictionary_map_id=" + dictionary_app.get_selected_map_id(),
                    "title",
                    "title_part",
                    "div_search_index_end"
                )
            },
            autocompleteCallback: async (item, termId) => {
                showInfo("Found index: " + item.title)
                let dictionary_index_type_id = item.id
                let new_index = {
                    dictionary_index_type_id: dictionary_index_type_id,
                    dictionary_term_id: termId
                }

                let index_created = await post_entity("dictionary_index", new_index)
                if (index_created === null || index_created === undefined) {
                    showError("Creating index failed: " + item.title)
                    return
                }
                showInfo("New index was assigned: " + item.title)
                this.addItem(item.title, index_created.id)
            }
        });
    }

    async afterRender(dictionary_term_id) {
        let button_add_index = get_element("button_add_index")
        button_add_index.onclick = async () => {
            get_element("div_search_index").style.display = "block"
            let title = this.get_input_value()
            if (title === "") {
                return;
            }

            let new_index_type = {
                dictionary_map_id: dictionary_app.get_selected_map_id(),
                title: title
            }
            let new_index_type_created = await post_entity("dictionary_index_type", new_index_type)
            if (!new_index_type_created) {
                showError("Creating new index type failed: " + title)
                return
            }
            let new_index = {
                dictionary_term_id: dictionary_term_id,
                dictionary_index_type_id: new_index_type_created.id
            }

            let index_created = await post_entity("dictionary_index", new_index)
            if (index_created === null || index_created === undefined) {
                showError("Creating index failed: " + title)
                return
            }
            showInfo("New index was assigned: " + title)
            this.addItem(title, index_created.id)
        }
        get_element("button_show_indexes").onclick = () => {
            let url = "index.html?entity=dictionary_index_type&action=list"
            showWindowFrom("Show indexes", url)
        }
    }

    addItem(title, id) {
        let div = document.createElement("div")
        div.classList.add("item")
        this._element.appendChild(div)
        let span = document.createElement("span")
        span.innerText = title
        div.appendChild(span)

        let div_buttons = document.createElement("div")
        div.appendChild(div_buttons)

        let edit_button = document.createElement("button")
        edit_button.innerHTML = "📝 Edit"
        edit_button.style.marginRight = "10px"
        edit_button.onclick = async () => {
            const result = await chooseOption(["Index", "Index Type"]);
            if (result === null || result === undefined) return
            if (result === "Index") {

                let url = "index.html?entity=dictionary_index&action=update&id=" + id
                showWindowFrom("Editing Source", url)

            }
            if (result === "Index Type") {
                let read_index = await read_entity("dictionary_index", id)
                if (read_index === null || read_index === undefined) {
                    showError("Reading index failed: " + title)
                }
                let url = "index.html?entity=dictionary_index_type&action=update&id=" + read_index.dictionary_index_type_id
                showInfo(url)
                showWindowFrom("Editing Index type", url)
            }
        }
        div_buttons.appendChild(edit_button)

        let delete_button = document.createElement("button")
        delete_button.innerHTML = "🗑️ Delete"
        delete_button.onclick = () => {
            if (!confirm("Do you really want to delete this index?")) return;
            let index_deleted = delete_entity("dictionary_index", id)
            let deleted = index_deleted !== null && index_deleted !== undefined
            if (deleted) {
                showInfo("Source was successfully deleted: " + title)
                div.remove()
            } else {
                showError("Deleting index failed: " + title)
            }
        }
        div_buttons.appendChild(delete_button)

        this.clear_input_value()
    }
}

class Sources extends CrudSection {
    constructor() {
        super({
            model: "source",
            models: "sources",
            table: "dictionary_source",
            filter: termId => "&dictionary_term_id=" + termId,
            input: true,
            resolveTitle: async item => {
                let source_type = await read_entity("dictionary_source_type", item.dictionary_source_type_id)
                if (!source_type) {
                    showError("Loading source type failed: " + item.dictionary_source_type_id)
                    return null
                }
                return source_type.title
            },
            createAutocomplete: input => {
                return new Autocomplete(
                    input,
                    1,
                    "dictionary_source_type_fulltext",
                    "",
                    "title",
                    "title_part",
                    "div_search_source_end"
                )
            },
            autocompleteCallback: async (item, termId) => {
                showInfo("Found source: " + item.title)
                let dictionary_source_type_id = item.id
                let new_source = {
                    dictionary_term_id: termId,
                    dictionary_source_type_id: dictionary_source_type_id
                }

                let source_created = await post_entity("dictionary_source", new_source)
                if (source_created === null || source_created === undefined) {
                    showError("Creating source failed: " + item.title)
                    return
                }
                showInfo("New source was assigned: " + item.title)
                this.addItem(item.title, source_created.id)
            }
        });
    }

    async afterRender(dictionary_term_id) {
        let button_add_source = get_element("button_add_source")
        button_add_source.onclick = async () => {
            get_element("div_search_source").style.display = "block"
            let title = this.get_input_value()
            if (title === "") {
                return;
            }

            let new_source_type = {
                title: title,
                type: 0
            }
            let new_source_type_created = await post_entity("dictionary_source_type", new_source_type)
            if (!new_source_type_created) {
                showError("Creating new source type failed: " + title)
                return
            }
            let new_source = {
                dictionary_term_id: dictionary_term_id,
                dictionary_source_type_id: new_source_type_created.id
            }

            let source_created = await post_entity("dictionary_source", new_source)
            if (source_created === null || source_created === undefined) {
                showError("Creating source failed: " + title)
                return
            }
            showInfo("New source was assigned: " + title)
            this.addItem(title, source_created.id)
        }
        get_element("button_show_sources").onclick = () => {
            let url = "index.html?entity=dictionary_source_type&action=list"
            showWindowFrom("Show sources", url)
        }
    }

    addItem(title, id) {
        let div = document.createElement("div")
        div.classList.add("item")
        this._element.appendChild(div)
        let span = document.createElement("span")
        span.innerText = title
        div.appendChild(span)

        let div_buttons = document.createElement("div")
        div.appendChild(div_buttons)

        let edit_button = document.createElement("button")
        edit_button.innerHTML = "📝 Edit"
        edit_button.style.marginRight = "10px"
        edit_button.onclick = async () => {
            const result = await chooseOption(["Source", "Source Type"]);
            if (result === null || result === undefined) return
            if (result === "Source") {

                let url = "index.html?entity=dictionary_source&action=update&id=" + id
                showWindowFrom("Editing Source", url)

            }
            if (result === "Source Type") {
                let read_source = await read_entity("dictionary_source", id)
                if (read_source === null || read_source === undefined) {
                    showError("Reading source failed: " + title)
                }
                let url = "index.html?entity=dictionary_source_type&action=update&id=" + read_source.dictionary_source_type_id
                showInfo(url)
                showWindowFrom("Editing Source type", url)
            }
        }
        div_buttons.appendChild(edit_button)

        let delete_button = document.createElement("button")
        delete_button.innerHTML = "🗑️ Delete"
        delete_button.onclick = () => {
            if (!confirm("Do you really want to delete this source?")) return;
            let source_deleted = delete_entity("dictionary_source", id)
            let deleted = source_deleted !== null && source_deleted !== undefined
            if (deleted) {
                showInfo("Source was successfully deleted: " + title)
                div.remove()
            } else {
                showError("Deleting source failed: " + title)
            }
        }
        div_buttons.appendChild(delete_button)

        this.clear_input_value()
    }
}


class Aliases extends CrudSection {
    constructor() {
        super({
            model: "alias",
            models: "aliases",
            table: "dictionary_term_alias",
            filter: termId => "&dictionary_term_id=" + termId + "&is_public=0" + "&user_id=" + getUserId(),
            input: false,
            resolveTitle: async item => {
                return item.alias
            },
        });
    }

    async afterRender(dictionary_term_id) {
        let button_add_alias = get_element("button_add_alias")

        button_add_alias.onclick = async () => {
            const title = prompt("Enter alias title");
            if (title === null || title === "") return;

            let new_alias = {
                dictionary_term_id: dictionary_term_id,
                alias: title
            }

            let alias_created = await post_entity("dictionary_term_alias", new_alias)
            if (alias_created === null || alias_created === undefined) {
                showError("Creating alias failed: " + title)
                return
            }
            showInfo("New alias was assigned: " + title)
            this.addItem(title, alias_created.id)
        }
    }

    addItem(title, id) {
        let div = document.createElement("div")
        div.classList.add("tag")
        this._element.appendChild(div)
        div.innerText = title
        div.style.backgroundColor = "rgba(151,112,207,0.71)"
        div.style.color = "#2b2b2b"

        let button = document.createElement("button")
        button.innerHTML = "&times;"
        button.onclick = async () => {
            if (!confirm("Do you really want to delete this alias?")) return;
            let alias_deleted = await delete_entity("dictionary_term_alias", id)
            let deleted = alias_deleted !== null && alias_deleted !== undefined
            if (deleted) {
                showInfo("Alias was successfully deleted: " + title)
                div.remove()
            } else {
                showError("Deleting alias failed: " + title)
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