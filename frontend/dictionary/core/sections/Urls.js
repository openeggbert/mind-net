// ========================================
// Imports
// ========================================
import {
    delete_entity,
    post_entity, put_entity,
    read_entity
} from "../../../api.js";
import {
    chooseOption,
    get_element,
    showError,
    showInfo, showWarn,
} from "../../../dom.js";
import {Autocomplete} from "../../../common.js";
import {_CrudSection} from "../_CrudSection.js";
import {Entities} from "../../entities/Entities.js";
import {showWindowFromUrl} from "../../window/VirtualWindow.js";

export class Urls extends _CrudSection {
    constructor(get_selected_map_id_callback) {
        super(
            {
                model: "url",
                models: "urls",
                table: "dictionary_url",
                filter: termId => "&dictionary_term_id=" + termId + "&sort=position",
                input: true,
                resolveTitle: async item => {
                    let url_type = await read_entity(Entities.dictionary_url_type, item.dictionary_url_type_id)
                    if (!url_type) {
                        showError("Loading url type failed: " + item.dictionary_url_type_id)
                        return null
                    }
                    return url_type.url
                },
                createAutocomplete: input => {
                    return new Autocomplete(
                        input,
                        3,
                        "dictionary_url_type_fulltext",
                        "&dictionary_map_id=" + get_selected_map_id_callback(),
                        "url",
                        "url_part",
                        "div_search_url_end"
                    )
                },
                autocompleteCallback: async (item, termId) => {
                    if(!get_element("input_checkbox_assign_flag").checked) return
                    showInfo("Found url: " + item.url)
                    let dictionary_url_type_id = item.id
                    let new_url = {
                        dictionary_map_id: get_selected_map_id_callback(),
                        dictionary_term_id: termId,
                        dictionary_url_type_id: dictionary_url_type_id
                    }

                    let url_created = await post_entity(Entities.dictionary_url, new_url)
                    if (url_created === null || url_created === undefined) {
                        showError("Creating url failed: " + item.url)
                        return
                    }
                    showInfo("New url was assigned: " + item.url)
                    this.addItem(item.url, url_created.id)
                }
            },get_selected_map_id_callback);
    }

    async afterRender(dictionary_term_id) {
        let button_add_url = get_element("button_add_url")
        button_add_url.onclick = async () => {
            get_element("div_search_url").style.display = "block"
            let url = this.get_input_value()
            if (url === "") {
                return;
            }

            let new_url_type = {
                dictionary_map_id: this.get_selected_map_id_callback(),
                url: url,
                title: "",
                type: 0
            }
            let new_url_type_created = await post_entity(Entities.dictionary_url_type, new_url_type)
            if (!new_url_type_created) {
                showError("Creating new url type failed: " + url)
                return
            }
            let input_checkbox_assign_flag = get_element("input_checkbox_assign_flag").checked

            if(!input_checkbox_assign_flag)
            {
                showInfo("New url type was created: " + url)
                return
            }

            let new_url = {
                dictionary_map_id: this.get_selected_map_id_callback(),
                dictionary_term_id: dictionary_term_id,
                dictionary_url_type_id: new_url_type_created.id
            }

            let url_created = await post_entity(Entities.dictionary_url, new_url)
            if (url_created === null || url_created === undefined) {
                showError("Creating url failed: " + url)
                return
            }
            showInfo("New url was assigned: " + url)
            this.addItem(url, url_created.id)
        }
        get_element("button_show_urls").onclick = () => {
            let url = "index.html?entity=dictionary_url_type&action=list"
            showWindowFromUrl("Show urls", url)
        }
    }

    addItem(title, id) {
        if(title === undefined)
            title = "!!!undefined!!!"
        let div = document.createElement("div")
        div.classList.add("item")
        this._element.appendChild(div)
        let span = document.createElement("span")

        let a = document.createElement("a")
        a.href = title
        a.innerText = title
        a.target = "_blank"
        span.appendChild(a)

        div.appendChild(span)


        let div_buttons = document.createElement("div")
        div.appendChild(div_buttons)

        let copy_button = document.createElement("button")
        copy_button.innerHTML = "📋 Copy"
        copy_button.style.marginRight = "10px"
        div_buttons.appendChild(copy_button)
        copy_button.onclick = () => {
            navigator.clipboard.writeText(title)
                .then(() => {
                    showInfo("Url was copied to clipboard: " + title)
                })
                .catch(err => {
                    showInfo("Error during copying Url to clipboard:", err);
                });
        }


        async function read_url() {
            return await read_entity(Entities.dictionary_url, id)
        }
        async function read_url_type() {
            let url_type = await read_url()
            let url_type_id = url_type.dictionary_url_type_id
            return await read_entity(Entities.dictionary_url_type, url_type_id)
        }
        async function update_url_type(url_type, new_value=url_type.title) {
            let updated = await put_entity(Entities.dictionary_url_type, url_type.id, url_type)
            if (updated === null || updated === undefined) {
                showError("Updating url_type failed: " + new_value)
                return false
            }
            showInfo("Url type was successfully updated: " + new_value)
            return true
        }
        async function update_url(url, new_value = url.id) {
            let updated = await put_entity(Entities.dictionary_url, url.id, url)
            if (updated === null || updated === undefined) {
                showError("Updating url failed: " + new_value)
                return false
            }
            showInfo("Url was successfully updated: " + new_value)
            return true
        }

        let info_button = document.createElement("button")
        info_button.innerHTML = "ℹ️ Info"
        info_button.style.marginRight = "10px"
        info_button.onclick = async () => {
            let url_ = await read_url()
            let type_ = await read_url_type()
            showInfo("Title: " + type_.title)
            switch(type_.type) {
                case 0: showInfo("Type: Not defined");break;
                case 1: showInfo("Type: Website");break;
                case 2: showInfo("Type: Article");break;
                case 3: showInfo("Type: Video");break;
                case 4: showInfo("Type: Repository");break;
                default: showWarn("Type: Unknown");break;
            }
            showInfo("Position: " + url_.position)
            showInfo("Note: " + url_.note)
        }
        div_buttons.appendChild(info_button)
        let edit_button = document.createElement("button")
        edit_button.innerHTML = "📝 Edit"
        edit_button.style.marginRight = "10px"
        edit_button.onclick = async () => {
            const what = await chooseOption(["Title", "Type", "Position", "Note"]);
            if (what === null || what === undefined) return

            async function prompt_value(question, default_value) {
                const value = prompt(question, default_value);
                if (value === null || value === "" || value === undefined) return null;
                return value
            }
            if (what === "Title") {
                let url_type = await read_url_type()
                const new_title = await prompt_value("Enter new title", url_type.title);
                if (!new_title) return;
                url_type.title = new_title
                if(await update_url_type(url_type)) {

                }
            }
            if (what === "Type") {
                let url_type = await read_url_type()
                let old_type = url_type.type
                switch(old_type) {
                    case 0: showInfo("Old type: Not defined");break;
                    case 1: showInfo("Old type: Website");break;
                    case 2: showInfo("Old type: Article");break;
                    case 3: showInfo("Old type: Video");break;
                    case 4: showInfo("Old type: Repository");break;
                    default: showWarn("Old type: Unknown");break;
                }
                const what2 = await chooseOption(["Not defined", "Website", "Article", "Video", "Repository"]);
                if (what2 === null || what2 === undefined) return

                let new_type = 0
                if(what2 === "Not defined") new_type = 0
                if(what2 === "Website") new_type = 1
                if(what2 === "Article") new_type = 2
                if(what2 === "Video") new_type = 3
                if(what2 === "Repository") new_type = 4

                if(old_type === new_type) {return;}

                url_type.type = new_type
                await update_url_type(url_type, what2)
            }
            if (what === "Position") {
                let url_ = await read_url()
                const new_position = await prompt_value("Enter new position", url_.position);
                if (!new_position) return;
                url_.position = Number(new_position)
                if(await update_url(url_, new_position)) {

                }
            }
            if (what === "Note") {
                let url_ = await read_url()
                const new_note = await prompt_value("Enter new note", url_.note);
                if (!new_note) return;
                url_.note = new_note
                if(await update_url(url_, new_note)) {

                }
            }

        }
        div_buttons.appendChild(edit_button)

        let delete_button = document.createElement("button")
        delete_button.innerHTML = "🗑️ Delete"
        delete_button.onclick = () => {
            if (!confirm("Do you really want to delete this url?")) return;
            let url_deleted = delete_entity(Entities.dictionary_url, id)
            let deleted = url_deleted !== null && url_deleted !== undefined
            if (deleted) {
                showInfo("Source was successfully deleted: " + title)
                div.remove()
            } else {
                showError("Deleting url failed: " + title)
            }
        }
        div_buttons.appendChild(delete_button)

        this.clear_input_value()
    }
}
