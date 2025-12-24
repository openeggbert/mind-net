// ========================================
// Imports
// ========================================
import {
    delete_entity,
    post_entity,
    read_entity
} from "../../../api.js";
import {
    get_element,
    showError,
    showInfo,
} from "../../../dom.js";
import {Autocomplete} from "../../../common.js";
import {Entities} from "../../entities/d_entities.js";

import {
    showWindowFromUrl
} from "../../window/d_window.js";

import {_CrudSection} from "../CrudSection.js";


export class Tags extends _CrudSection {
    constructor(get_selected_map_id_callback) {
        super(
            {
                model: "tag",
                models: "tags",
                table: "dictionary_tag",
                filter: termId => "&dictionary_term_id=" + termId,
                input: true,
                resolveTitle: async item => {
                    let tag_type = await read_entity(Entities.dictionary_tag_type, item.dictionary_tag_type_id)
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
                        "&dictionary_map_id=" + get_selected_map_id_callback(),
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

                    let tag_created = await post_entity(Entities.dictionary_tag, new_tag);
                    if (tag_created === null || tag_created === undefined) {
                        showError("Creating tag failed: " + item.title)
                        return
                    }
                    showInfo("New tag was assigned: " + item.title)
                    this.addItem(item.title, tag_created.id)
                }
            },get_selected_map_id_callback
        );
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
                dictionary_map_id: this.get_selected_map_id_callback(),
                title: title
            }
            let new_tag_type_created = await post_entity(Entities.dictionary_tag_type, new_tag_type)
            if (!new_tag_type_created) {
                showError("Creating new tag type failed: " + title)
                return
            }
            let new_tag = {
                dictionary_term_id: dictionary_term_id,
                dictionary_tag_type_id: new_tag_type_created.id
            }

            let tag_created = await post_entity(Entities.dictionary_tag, new_tag)
            if (tag_created === null || tag_created === undefined) {
                showError("Creating tag failed: " + title)
                return
            }
            showInfo("New tag was assigned: " + title)
            this.addItem(title, tag_created.id)
        }
        get_element("button_show_tags").onclick = () => {
            let url = "index.html?entity=dictionary_tag_type&action=list&dictionary_map_id=" + this.get_selected_map_id_callback()
            showWindowFromUrl("Show tags", url)
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
            let tag_deleted = await delete_entity(Entities.dictionary_tag, id)
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
