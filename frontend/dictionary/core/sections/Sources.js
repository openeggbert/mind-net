// ========================================
// Imports
// ========================================
import {
    delete_entity,
    post_entity,
    read_entity
} from "../../../api.js";
import {
    chooseOption,
    get_element,
    showError,
    showInfo,
} from "../../../dom.js";
import {Autocomplete} from "../../../common.js";
import {_CrudSection} from "../_CrudSection";
import {Entities} from "../../entities/Entities";
import {showWindowFromUrl} from "../../window/VirtualWindow";

export class Sources extends _CrudSection {
    constructor(get_selected_map_id_callback) {
        super(
            {
                model: "source",
                models: "sources",
                table: "dictionary_source",
                filter: termId => "&dictionary_term_id=" + termId,
                input: true,
                resolveTitle: async item => {
                    let source_type = await read_entity(Entities.dictionary_source_type, item.dictionary_source_type_id)
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

                    let source_created = await post_entity(Entities.dictionary_source, new_source)
                    if (source_created === null || source_created === undefined) {
                        showError("Creating source failed: " + item.title)
                        return
                    }
                    showInfo("New source was assigned: " + item.title)
                    this.addItem(item.title, source_created.id)
                }
            },get_selected_map_id_callback);
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
            let new_source_type_created = await post_entity(Entities.dictionary_source_type, new_source_type)
            if (!new_source_type_created) {
                showError("Creating new source type failed: " + title)
                return
            }
            let new_source = {
                dictionary_term_id: dictionary_term_id,
                dictionary_source_type_id: new_source_type_created.id
            }

            let source_created = await post_entity(Entities.dictionary_source, new_source)
            if (source_created === null || source_created === undefined) {
                showError("Creating source failed: " + title)
                return
            }
            showInfo("New source was assigned: " + title)
            this.addItem(title, source_created.id)
        }
        get_element("button_show_sources").onclick = () => {
            let url = "index.html?entity=dictionary_source_type&action=list"
            showWindowFromUrl("Show sources", url)
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
                showWindowFromUrl("Editing Source", url)

            }
            if (result === "Source Type") {
                let read_source = await read_entity(Entities.dictionary_source, id)
                if (read_source === null || read_source === undefined) {
                    showError("Reading source failed: " + title)
                }
                let url = "index.html?entity=dictionary_source_type&action=update&id=" + read_source.dictionary_source_type_id
                showInfo(url)
                showWindowFromUrl("Editing Source type", url)
            }
        }
        div_buttons.appendChild(edit_button)

        let delete_button = document.createElement("button")
        delete_button.innerHTML = "🗑️ Delete"
        delete_button.onclick = () => {
            if (!confirm("Do you really want to delete this source?")) return;
            let source_deleted = delete_entity(Entities.dictionary_source, id)
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
