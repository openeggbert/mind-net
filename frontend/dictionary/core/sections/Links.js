// ========================================
// Imports
// ========================================
import {
    delete_entity,
    post_entity,
    put_entity,
    read_entity
} from "../../../api.js";
import {
    chooseOption,
    get_element,
    showError,
    showInfo,
    showWarn
} from "../../../dom.js";
import {Autocomplete, defined} from "../../../common.js";
import {_CrudSection} from "../_CrudSection.js";
import {Entities} from "../../entities/Entities.js";
import {set_next_visit_source, USER_ID} from "../../globals/Globals.js";
import {VisitSource} from "../../enums/VisitSource.js";

export class Links extends _CrudSection {
    constructor(get_selected_map_id_callback, render_term_id_callback) {
        super( {
                model: "link",
                models: "links",
                table: "dictionary_link",
                filter: termId => "&from_dictionary_term_id=" + termId,
                input: true,
                resolveTitle: async item => {
                    let another_dictionary_term = await read_entity(Entities.dictionary_term, item.to_dictionary_term_id)
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
                        "&alias=1&dictionary_map_id=0&default_dictionary_map_id=" + get_selected_map_id_callback(),
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

                    let link_created = await post_entity(Entities.dictionary_link, new_link)
                    if (link_created === null || link_created === undefined) {
                        showError("Creating link failed: " + item.title)
                        return
                    }
                    showInfo("New link was created: " + item.title)
                    this.addItem(item.title, link_created.id, link_created)
                }

            },
            get_selected_map_id_callback,
            render_term_id_callback);
        if(!defined(this.render_term_id_callback)) {
            throw new Error ("render_term_id_callback is null or undefined")
        }
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
                dictionary_map_id: this.get_selected_map_id_callback(),
                title: title,
                status: 1
            }

            let term_created = await post_entity(Entities.dictionary_term, new_term)
            if (term_created === null || term_created === undefined) {
                showError("Creating term failed: " + title)
                return
            }

            let new_flag = {
                dictionary_term_id: term_created.id,
                dictionary_map_id: this.get_selected_map_id_callback(),
                user_id: USER_ID,
                title: "stub",
                is_public: 1
            }

            let flag_created = await post_entity(Entities.dictionary_flag, new_flag)
            if (flag_created === null || flag_created === undefined) {
                showError("Creating flag failed: " + title)
                return
            }

            let new_link = {
                from_dictionary_term_id: dictionary_term_id,
                to_dictionary_term_id: term_created.id
            }

            let link_created = await post_entity(Entities.dictionary_link, new_link)
            if (link_created === null || link_created === undefined) {
                showError("Creating link failed: " + title)
                return
            }
            showInfo("New link was assigned: " + title)
            this.addItem(title, link_created.id, link_created)
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
            if(!defined(this.render_term_id_callback)) {
                throw new Error ("render_term_id_callback is null or undefined")
            } else {
                set_next_visit_source(VisitSource.Link)
                await this.render_term_id_callback(to_dictionary_term_id)
            }
        }
        a.innerText = title
        spanElement.appendChild(a)
        let span = document.createElement("span")
        spanElement.appendChild(span)

        function termRelationTypeToString(type) {
            switch (type) {
                case 0:
                    return "None";   // None
                case 1:
                    return "Is a";          // IsA
                case 2:
                    return "Part of";       // PartOf
                case 3:
                    return "Uses";          // Uses
                case 4:
                    return "Depends on";    // DependsOn
                case 5:
                    return "Implements";    // Implements
                case 6:
                    return "Related";       // Related
                case 7:
                    return "Contrasts";     // Contrasts
                case 8:
                    return "Alternative to";// AlternativeTo
                default:
                    return "None";
            }
        }

        function stringToTermRelationType(str) {
            switch (str) {
                case "None":
                    return 0; // None
                case "Is a":
                    return 1; // IsA
                case "Part of":
                    return 2; // PartOf
                case "Uses":
                    return 3; // Uses
                case "Depends on":
                    return 4; // DependsOn
                case "Implements":
                    return 5; // Implements
                case "Related":
                    return 6; // Related
                case "Contrasts":
                    return 7; // Contrasts
                case "Alternative to":
                    return 8; // AlternativeTo
                default:
                    return 0; // None
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
                "None",
                "Is a",
                "Part of",
                "Uses",
                "Depends on",
                "Implements",
                "Related",
                "Contrasts",
                "Alternative to"
            ])
            if (option === null || option === undefined) return
            let type = stringToTermRelationType(option)

            let read_link = await read_entity(Entities.dictionary_link, id)

            let read = read_link !== null && read_link !== undefined
            if (!read) {
                showError("Reading link failed: " + title)
            }
            read_link.type = type
            let updated = await put_entity(Entities.dictionary_link, id, read_link)
            if (updated) {
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
            let link_deleted = delete_entity(Entities.dictionary_link, id)
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
