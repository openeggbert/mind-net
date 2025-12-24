import {get_element, showInfo} from "../../dom.js";
import {list_all_entities, post_entity, QueryParams} from "../../api.js";
import {Entities} from "../entities/Entities";
import {USER_ID} from "../globals/Globals";

export class SelectMap {
    #element
    #selected_map_id = 1
    #maps = new Map();

    async list_maps_from_backend() {
        return await list_all_entities(Entities.dictionary_map, new QueryParams().sort(Entities.dictionary_map.position).build());
    }

    constructor(refresh_autocomplete_term_title_callback) {
        this.#element = get_element("select_map");
        this.#element.innerHTML = ""

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

            if (new_map_created === null) return
            this.add_map(new_map_created.id, new_map_created.name)
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
}
