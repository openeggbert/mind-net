import {post_entity} from "../../api.js";
import {get_element, showError, showInfo} from "../../dom.js";
import {Autocomplete} from "../../common.js";
import {SelectMap} from "./SelectMap.js";
import TermContainer from "./TermContainer.js";
import {SearchWindow} from "../search/Search";
import {Entities} from "../entities/Entities";
import {showWindowFromUrl} from "../window/VirtualWindow";

export class DictionaryApp {
    #input_search_term = document.getElementById("input_search_term")
    #autocomplete_term_title = null
    #term_container = null

    constructor() {
        this.select_map = new SelectMap(
            ()=> {this.refresh_autocomplete_term_title()}
        )
        this.select_map.init()
        let get_selected_map_id_callback = () => {
            return this.select_map.get_selected_map_id()
        }
        let render_term_id_callback = async (term_id) => {
            await this.render(term_id)
        }
        this.#term_container = new TermContainer(get_selected_map_id_callback, render_term_id_callback)

        get_element("dictionary_header").title = "Go to home"
        get_element("dictionary_header").style.cursor = "pointer"
        get_element("button_mindnet").addEventListener("click", () => {
            window.location.href = 'index.html'
        });
        get_element("button_mindnet").title = "Go to Mind Net generic frontend"

        this.#autocomplete_term_title = new Autocomplete(this.#input_search_term, 1, "dictionary_term_fulltext", "&dictionary_map_id=" + this.select_map.get_selected_map_id(), "title", "title_part", "div_search_term_end")
        this.#autocomplete_term_title.addCallback(async () => {
            let item = this.#autocomplete_term_title.get_item()
            showInfo("Found term: " + item.title)
            await this.#term_container.render(item.id)
            this.#term_container.show()
            this.#input_search_term.value = ""
        })

        get_element("button_open_advanced_search").onclick = async () => {
            let dictionary_map_id = this.get_selected_map_id()
            let search_window = new SearchWindow(
                dictionary_map_id,
                async (term_id) => {
                    await this.render(term_id)
                    this.show_term_container()
                }
            )
            search_window.show()
        };

        get_element("button_add_term").onclick = async () => {
            if (this.#input_search_term.value === "") {
                showError("Could not create term, the title must not be empty.");
                return;
            }
            let new_term = {
                dictionary_map_id: this.select_map.get_selected_map_id(),
                title: this.#input_search_term.value,
            }
            let new_term_created = await post_entity(Entities.dictionary_term, new_term)
            if (new_term_created === null) {
                showError("Creating new term failed.")
                return;
            }
            showInfo("Created new term: " + new_term_created.title)
            await this.#term_container.render(new_term_created.id)
            this.#term_container.show()

            if (false) {
                const orig_title = new_term.title
                for (let i = 0; i < 1000; i++) {
                    new_term.title = orig_title + "_" + i
                    await post_entity(Entities.dictionary_term, new_term)
                    showInfo(i)
                }
            }
        }
        get_element("button_find_random_term").onclick = async () => {
            await this.#autocomplete_term_title.search("*", 1, true)

            this.#autocomplete_term_title.set_selected_item(0)
            //await this.#term_container.render(this.#autocomplete_term_title.get_item_id())
            this.#term_container.show()
        }
        get_element("button_new_session").onclick = async () => {
            let url = "app_dictionary.html"
            showWindowFromUrl("Dictionary - App", url)
        }
    }
    show_term_container() {
        this.#term_container.show()
    }

    refresh_autocomplete_term_title() {
        this.#autocomplete_term_title.destroy()
        this.#autocomplete_term_title = new Autocomplete(this.#input_search_term, 1, "dictionary_term_fulltext", "&dictionary_map_id=" + this.select_map.get_selected_map_id(), "title", "title_part", "div_search_term_end")
        this.#autocomplete_term_title.addCallback(async () => {
            let item = this.#autocomplete_term_title.get_item()
            showInfo("Found term: " + item.title)
            await this.#term_container.show()
            await this.#term_container.render(item.id)
            this.#input_search_term.value = ""
        })
    }

    get_selected_map_id() {
        return this.select_map.get_selected_map_id()
    }

    async render(dictionary_term_id) {
        await this.#term_container.render(dictionary_term_id)
    }
}