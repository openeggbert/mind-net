// ========================================
// Imports
// ========================================
import {
    delete_entity,
    list_all_entities,
    post_entity,
    put_entity, QueryParams,
    read_entity
} from "../../api.js";
import {
    chooseOption,
    get_element,
    showError,
    showInfo,
    showWarn
} from "../../dom.js";
import {Autocomplete, defined} from "../../common.js";
import {attachMarkdownEditor} from "../markdown/d_markdown.js";
import {Entities} from "../entities/d_entities.js";

import {USER_ID} from "../globals/d_globals.js";
import {
    showWindowFromUrl
} from "../window/d_window.js";

import {_CrudSection} from "./CrudSection.js";

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
                    "&dictionary_map_id=" + this.get_selected_map_id_callback(),
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
                    return "Not defined";   // NotDefined
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
                    return "Not defined";
            }
        }

        function stringToTermRelationType(str) {
            switch (str) {
                case "Not defined":
                    return 0; // NotDefined
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

export class Notes extends _CrudSection {

    constructor(get_selected_map_id_callback) {
        super(
            {
            model: "note",
            models: "notes",
            table: "dictionary_note",
            filter: termId => "&dictionary_term_id=" + termId + "&sort=position",
            input: false,
            resolveTitle: async item => {
                return item.title
            },
        },get_selected_map_id_callback);
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

            let note_created = await post_entity(Entities.dictionary_note, new_note)
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

            let read_note = await read_entity(Entities.dictionary_note, id)
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
                return label
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

            let content_label = make_label(id_content, "Content")
            let edit_button = document.createElement("button")
            edit_button.innerText = "✏️ Edit"
            let read_button = document.createElement("button")
            read_button.innerText = "📖 Read"
            content_label.after(edit_button)
            edit_button.after(read_button)

            let text_area_content = document.createElement("textarea")
            text_area_content.value = read_note.content
            text_area_content.style.width = "100%"
            text_area_content.style.boxSizing = "border-box"
            text_area_content.id = id_content
            text_area_content.style.height = "200px"

            note_details.appendChild(text_area_content)

            attachMarkdownEditor({
                textarea: text_area_content,
                buttonEdit: edit_button,
                buttonRead: read_button
            });

            let save_button = document.createElement("button")
            save_button.id = "button_save_term_note_" + id;
            save_button.classList.add("save-btn")
            save_button.innerText = "Save"
            note_details.appendChild(save_button)

            save_button.onclick = async () => {
                read_note.title = input_title.value
                read_note.position = input_position.value
                read_note.content = text_area_content.value
                let updated = await put_entity(Entities.dictionary_note, read_note.id, read_note)
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
            let note_deleted = await delete_entity(Entities.dictionary_note, id)
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