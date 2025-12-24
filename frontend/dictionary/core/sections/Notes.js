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
} from "../../../dom.js";
import {_CrudSection} from "../_CrudSection.js";
import {Entities} from "../../entities/Entities.js";
import {attachMarkdownEditor} from "../../markdown/Markdown.js";

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