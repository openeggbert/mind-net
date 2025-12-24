import {get_element, showError, showInfo, showWarn} from "../../dom.js";
import {delete_entity, list_all_entities, post_entity, put_entity, QueryParams, read_entity} from "../../api.js";
import {Entities} from "../entities/Entities.js";
import {USER_ID} from "../globals/Globals.js";
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
            showInfo("Map changed to: " + this.#maps.get(this.#element.value))
        });

        let button_add_map = get_element("button_add_map").onclick = async () => {
            const name = prompt("Enter map name");
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
                showError("Creating new map failed: " + name)
                return
            } else {
                showInfo("New map was successfully created: " + name)
            }
            this.add_map(new_map_created.id, new_map_created.name)
            this.select_map(new_map_created.id)
        }
        let button_show_map = get_element("button_show_map").onclick = async () => {
            if(this.#selected_map_id === 0) return
            let url = "index.html?entity=dictionary_map&action=read&id=" + this.get_selected_map_id()
            showWindowFromUrl("Show map", url)
        }
        let button_rename_map = get_element("button_rename_map").onclick = async () => {
            if(this.#selected_map_id === 0) return
            let read_map = await read_entity(Entities.dictionary_map, this.#selected_map_id)
            if(!defined(read_map)) {
                showError("Reading map failed: " + this.#selected_map_id)
                return
            }
            const old_name = read_map.name
            const new_name = prompt("Enter new map name", read_map.name);
            if (new_name === null) return;
            if (new_name === old_name) {
                showWarn("You did not change the name. Nothing to be updated.")
                return;
            }
            read_map.name = new_name
            let map_updated = await put_entity(Entities.dictionary_map, this.#selected_map_id, read_map)

            if (map_updated === null) {
                showError("Updating new failed: " + new_name)
                return
            } else {
                showInfo("Map was successfully updated: " + new_name)
            }
            this.rename_map(this.#selected_map_id, new_name)
        }
        let button_delete_map = get_element("button_delete_map").onclick = async () => {
            if(this.#selected_map_id === 0) return
            if (!confirm("Do you really want to delete this map?")) return;

            let deleted = await delete_entity(Entities.dictionary_map, this.#selected_map_id)
            if(!defined(deleted)) {
                showError("Deleting map failed. " + this.#selected_map_id)
                return
            }

            showInfo("Map was successfully deleted: " + this.#selected_map_id)

            this.remove_map(this.#selected_map_id)
        }
    }

    get_selected_map_id() {
        return this.#selected_map_id
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
