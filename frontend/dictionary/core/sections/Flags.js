// ========================================
// Imports
// ========================================
import {
    delete_entity,
    list_all_entities,
    post_entity,
    QueryParams,
} from "../../../api.js";
import {
    get_element,
    showError,
    showInfo
} from "../../../dom.js";
import {Entities} from "../../entities/d_entities.js";

import {USER_ID} from "../../globals/d_globals.js";

import {_CrudSection} from "../CrudSection.js";


export class Flags extends _CrudSection {
    constructor(get_selected_map_id_callback) {
        super(
            {
                model: "flag",
                models: "flags",
                table: "dictionary_flag",
                input: false,
                resolveTitle: async item => {
                    return item.title
                },
            },get_selected_map_id_callback);
    }

    async loadItems(dictionary_term_id) {
        let result = []
        let private_flags_result = await list_all_entities(
            Entities.dictionary_flag,
            new QueryParams()
                .add(Entities.dictionary_flag.dictionary_term_id, dictionary_term_id)
                .add(Entities.dictionary_flag.is_public, 0)
                .add_user_id()
                .build()
        )
        if (!private_flags_result) {
            showError("Listing private flags failed.")
            return [];
        } else {
            private_flags_result.forEach(e => {
                result.push(e)
            })

        }
        let public_flags_result = await list_all_entities(
            Entities.dictionary_flag,
            new QueryParams()
                .add(Entities.dictionary_flag.dictionary_term_id, dictionary_term_id)
                .add(Entities.dictionary_flag.is_public, 1)
                .build()
        )
        if (!public_flags_result) {
            showError("Listing public flags failed.")
            return [];
        } else {
            let user_id = USER_ID;
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
                user_id: USER_ID,
                dictionary_map_id: this.get_selected_map_id_callback(),
                title: title,
                is_public: is_public ? 1 : 0
            }

            let flag_created = await post_entity(Entities.dictionary_flag, new_flag)
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
            let flag_deleted = await delete_entity(Entities.dictionary_flag, id)
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
