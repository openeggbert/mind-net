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
    showWarn
} from "../../../dom.js";
import {Autocomplete} from "../../../common.js";
import {_CrudSection} from "../_CrudSection";
import {Entities} from "../../entities/Entities";
import {showWindowFromUrl} from "../../window/VirtualWindow";

export class Indexes extends _CrudSection {
    constructor(get_selected_map_id_callback) {
        super(
            {
                model: "index",
                models: "indexes",
                table: "dictionary_index",
                filter: termId => "&dictionary_term_id=" + termId + "&sort=position",
                input: true,
                resolveTitle: async item => {
                    let index_type = await read_entity(Entities.dictionary_index_type, item.dictionary_index_type_id)
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
                        "&dictionary_map_id=" + this.get_selected_map_id_callback(),
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

                    let index_created = await post_entity(Entities.dictionary_index, new_index)
                    if (index_created === null || index_created === undefined) {
                        showError("Creating index failed: " + item.title)
                        return
                    }
                    showInfo("New index was assigned: " + item.title)
                    this.addItem(item.title, index_created.id)
                }
            },get_selected_map_id_callback);
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
                dictionary_map_id: this.get_selected_map_id_callback(),
                title: title
            }
            let new_index_type_created = await post_entity(Entities.dictionary_index_type, new_index_type)
            if (!new_index_type_created) {
                showError("Creating new index type failed: " + title)
                return
            }
            let new_index = {
                dictionary_term_id: dictionary_term_id,
                dictionary_index_type_id: new_index_type_created.id
            }

            let index_created = await post_entity(Entities.dictionary_index, new_index)
            if (index_created === null || index_created === undefined) {
                showError("Creating index failed: " + title)
                return
            }
            showInfo("New index was assigned: " + title)
            this.addItem(title, index_created.id)
        }
        get_element("button_show_indexes").onclick = () => {
            let url = "index.html?entity=dictionary_index_type&action=list"
            showWindowFromUrl("Show indexes", url)
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
                showWindowFromUrl("Editing Source", url)

            }
            if (result === "Index Type") {
                let read_index = await read_entity(Entities.dictionary_index, id)
                if (read_index === null || read_index === undefined) {
                    showError("Reading index failed: " + title)
                }
                let url = "index.html?entity=dictionary_index_type&action=update&id=" + read_index.dictionary_index_type_id
                showInfo(url)
                showWindowFromUrl("Editing Index type", url)
            }
        }
        div_buttons.appendChild(edit_button)

        let delete_button = document.createElement("button")
        delete_button.innerHTML = "🗑️ Delete"
        delete_button.onclick = () => {
            if (!confirm("Do you really want to delete this index?")) return;
            let index_deleted = delete_entity(Entities.dictionary_index, id)
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
