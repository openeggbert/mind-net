// ========================================
// Imports
// ========================================
import {
    delete_entity,
    post_entity
} from "../../../api.js";
import {
    get_element,
    showError,
    showInfo,
    showWarn
} from "../../../dom.js";
import {Entities} from "../../entities/d_entities.js";

import {USER_ID} from "../../globals/d_globals.js";

import {_CrudSection} from "../CrudSection.js";

export class Aliases extends _CrudSection {
    constructor(get_selected_map_id_callback) {
        super({
            model: "alias",
            models: "aliases",
            table: "dictionary_term_alias",
            filter: termId => "&dictionary_term_id=" + termId + "&is_public=0" + "&user_id=" + USER_ID,
            input: false,
            resolveTitle: async item => {
                return item.alias
            },
        },get_selected_map_id_callback);
    }

    async afterRender(dictionary_term_id) {
        let button_add_alias = get_element("button_add_alias")

        button_add_alias.onclick = async () => {
            const title = prompt("Enter alias title");
            if (title === null || title === "") return;

            let new_alias = {
                dictionary_term_id: dictionary_term_id,
                dictionary_map_id: this.get_selected_map_id_callback(),
                alias: title
            }

            let alias_created = await post_entity(Entities.dictionary_term_alias, new_alias)
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
            let alias_deleted = await delete_entity(Entities.dictionary_term_alias, id)
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