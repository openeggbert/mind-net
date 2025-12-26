import {get_element, showError, showInfo, showSuccess, showWarn} from "../../dom.js";
import {delete_entity, list_all_entities, post_entity, put_entity, QueryParams, read_entity} from "../../api.js";
import {Entities} from "../entities/Entities.js";
import {translate, USER_ID} from "../globals/Globals.js";
import {showWindowFromUrl} from "../window/VirtualWindow.js";
import {defined} from "../../common.js";

export class SelectMap {
    #element
    #selected_map_id = 1
    #maps = new Map();
    #hide_term_container_callback

    async list_maps_from_backend() {
        return await list_all_entities(Entities.dictionary_map, new QueryParams().sort(Entities.dictionary_map.position).build());
    }

    constructor(refresh_autocomplete_term_title_callback, hide_term_container_callback) {
        this.#element = get_element("select_map");
        this.#element.innerHTML = ""
        this.#hide_term_container_callback = hide_term_container_callback

        this.#element.addEventListener("change", () => {
            this.#selected_map_id = this.#element.value
            refresh_autocomplete_term_title_callback()
            this.#hide_term_container_callback()
            showInfo(translate("dictionary.select_map.info.map_changed") + ": " + this.#maps.get(this.#element.value))
        });

        let button_add_map = get_element("button_add_map").onclick = async () => {
            const name = prompt(translate( "dictionary.select_map.button.add_map.prompt.enter_map_name"));
            if (name === null) return;
            let new_map = {
                name: name,
                description: "",
                owner_id: USER_ID,
                team_id: 0,
                owner_rights: 7,
                team_rights: 0,
                other_rights: 0
            }
            let new_map_created = await post_entity(Entities.dictionary_map, new_map)

            if (new_map_created === null) {
                showError(translate("dictionary.select_map.button.add_map.error.creating_map_failed") + ": " + name)
                return
            } else {
                showSuccess( translate("dictionary.select_map.button.add_map.info.map_created")+ ": " + name)
            }
            this.add_map(new_map_created.id, new_map_created.name)
            this.select_map(new_map_created.id)
        }
        let button_show_map = get_element("button_show_map").onclick = async () => {
            if(this.#selected_map_id === 0) return
            let url = "index.html?entity=dictionary_map&action=read&id=" + this.get_selected_map_id()
            showWindowFromUrl(translate("dictionary.select_map.button.show_map"), url)
        }
        let button_rename_map = get_element("button_rename_map").onclick = async () => {
            if(this.#selected_map_id === 0) return
            let read_map = await read_entity(Entities.dictionary_map, this.#selected_map_id)
            if(!defined(read_map)) {
                showError(translate("dictionary.select_map.button.rename_map.error.reading_map_failed") + ": " + this.#selected_map_id)
                return
            }
            const old_name = read_map.name
            const new_name = prompt(translate("dictionary.select_map.button.rename_map.prompt_enter_new_map_name"), read_map.name);
            if (new_name === null) return;
            if (new_name === old_name) {
                showWarn(translate("dictionary.select_map.button.rename_map.warn.no_change"))
                return;
            }
            read_map.name = new_name
            let map_updated = await put_entity(Entities.dictionary_map, this.#selected_map_id, read_map)

            if (map_updated === null) {
                showError(translate("dictionary.select_map.button.rename_map.error.update_failed") + ": " + new_name)
                return
            } else {
                showSuccess(translate("dictionary.select_map.button.rename_map.info.update_successful") + ": " + new_name)
            }
            this.rename_map(this.#selected_map_id, new_name)
        }
        let button_delete_map = get_element("button_delete_map").onclick = async () => {
            if(this.#selected_map_id === 0) return
            if (!confirm(translate("dictionary.select_map.button.delete_map.confirm.text"))) return;

            let deleted = await delete_entity(Entities.dictionary_map, this.#selected_map_id)
            if(!defined(deleted)) {
                showError(translate("dictionary.select_map.button.delete_map.error.deleting_failed") + this.#selected_map_id)
                return
            }

            showSuccess(translate("dictionary.select_map.button.delete_map.error.deleting_successful") + ": " + this.#selected_map_id)

            this.remove_map(this.#selected_map_id)
        }
    }

    get_selected_map_id() {
        return this.#selected_map_id
    }
    set_selected_map_id(map_id) {
        this.select_map(map_id)
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
        this.#maps.set(option.value, option.innerText)
    }
    remove_map(id) {
        let options = Array.from(this.#element.options)
        options.forEach(e=>{
            if(Number(e.value) === Number(id)) {
                this.#maps.delete(e.value)
                if(Number(this.#selected_map_id) === Number(e.value)) {
                    this.#element.selected_index = 0

                }
                e.remove()
                this.#hide_term_container_callback()
            }
        })
        if(options.length >0) {
            this.#selected_map_id = options[0].value
        }

    }
    rename_map(id, new_label) {
        Array.from(this.#element.options).forEach(e=>{
            if(Number(e.value) === Number(id)) {
                this.#maps.set(e.value, new_label)
                e.innerText = new_label
            }
        })
    }
    select_map(map_id) {
        Array.from(this.#element.options).forEach(e=>{
            if(Number(e.value) === Number(map_id)) {
                e.selected = true
                this.#selected_map_id = map_id
            }
        })
    }
}
