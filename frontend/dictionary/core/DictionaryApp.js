import {post_entity} from "../../api.js";
import {get_element, showError, showInfo, showSuccess, showWarn} from "../../dom.js";
import {Autocomplete} from "../../common.js";
import {SelectMap} from "./SelectMap.js";
import TermContainer from "./TermContainer.js";
import {SearchWindow} from "../search/SearchWindow.js";
import {Entities} from "../entities/Entities.js";
import {showWindowFromUrl} from "../window/VirtualWindow.js";
import {RepetitionWindow} from "../repetition/RepetitionWindow.js";
import {I18n} from "../i18n/I18n.js";
import {LanguageObject, SUPPORTED_LANGUAGES} from "../i18n/Language.js";
import {get_next_visit_source, set_i18n, set_next_visit_source, translate} from "../globals/Globals.js";
import {get_params, set_params} from "./Utils.js";
import {VisitSource} from "../enums/VisitSource.js";
import {TermVisitHistoryWindow} from "./dom/TermVisitHistoryWindow.js";

export class DictionaryApp {
    #input_search_term = document.getElementById("input_search_term")
    #autocomplete_term_title = null
    #term_container = null
    #i18n = null

    constructor() {
        this.#i18n = new I18n("en")
        set_i18n(this.#i18n)

        this.select_map = new SelectMap(
            () => {
                this.refresh_autocomplete_term_title()
            },
            () => {
                this.#term_container.hide()
            }
        )
        let get_selected_map_id_callback = () => {
            return this.select_map.get_selected_map_id()
        }
        let set_selected_map_id_callback = (map_id) => {
            return this.set_selected_map_id(map_id)
        }
        let render_term_id_callback = async (term_id) => {
            await this.render(term_id)
        }
        this.#term_container = new TermContainer(
            get_selected_map_id_callback,
            set_selected_map_id_callback,
            render_term_id_callback
        )

        get_element("dictionary_header").title = "Go to home"
        get_element("dictionary_header").style.cursor = "pointer"
        get_element("button_mindnet").addEventListener("click", () => {
            window.location.href = 'index.html'
        });
        get_element("button_mindnet").title = "Go to Mind Net generic frontend"

        this.#autocomplete_term_title = new Autocomplete(this.#input_search_term, 1, "dictionary_term_fulltext", "&alias=1&dictionary_map_id=" + this.select_map.get_selected_map_id(), "title", "title_part", "div_search_term_end")
        this.#autocomplete_term_title.addCallback(async () => {
            let item = this.#autocomplete_term_title.get_item()
            showInfo(this.translate("dictionary.term.info.found_term") + ": " + item.title)
            if(get_next_visit_source() !== VisitSource.Random) set_next_visit_source(VisitSource.Search)
            await this.#term_container.render(item.id)
            this.#term_container.show()
            this.#input_search_term.value = ""
        })
        this.set_search_all_results_callback()

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
                showError(this.translate("dictionary.term.button.add_term.error.empty_title"));
                return;
            }
            let new_term = {
                dictionary_map_id: this.select_map.get_selected_map_id(),
                title: this.#input_search_term.value,
            }
            let new_term_created = await post_entity(Entities.dictionary_term, new_term)
            if (new_term_created === null) {
                showError(this.translate("dictionary.term.button.add_term.error.creating_failed"))
                return;
            }
            showSuccess(this.translate("dictionary.term.button.add_term.info.creating_was_successful") + new_term_created.title)
            set_next_visit_source(VisitSource.Created)
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

            let has_items = this.#autocomplete_term_title.set_selected_item(0)
            if(!has_items) {
                showWarn("This map has no term.")
                return
            }
            set_next_visit_source(VisitSource.Random)
            set_params(null, this.#autocomplete_term_title.get_item_id())
            //await this.#term_container.render(this.#autocomplete_term_title.get_item_id())
            this.#term_container.show()
        }
        get_element("button_new_repetition").onclick = async () => {
            let dictionary_map_id = this.get_selected_map_id()
            let repetition_window = new RepetitionWindow(
                () => {
                    return this.get_selected_map_id()
                },
                async (term_id) => {
                    set_next_visit_source(VisitSource.Repetition)
                    await this.render(term_id)
                    this.show_term_container()
                }
            )

            repetition_window.show()
            await repetition_window.init()
        }
        get_element("button_show_visited").onclick = async () => {
            let win = new TermVisitHistoryWindow()
            win.show()
            await win.init()
        }

        get_element("button_new_session").onclick = async () => {
            let url = "app_dictionary.html"
            showWindowFromUrl(this.translate("dictionary.title.dictionary_app"), url)
        }
    }

    set_search_all_results_callback() {
        this.#autocomplete_term_title.set_search_all_results_callback((q)=> {
            showInfo(q)
            let dictionary_map_id = this.get_selected_map_id()
            let search_window = new SearchWindow(
                dictionary_map_id,
                async (term_id) => {
                    await this.render(term_id)
                    this.show_term_container()
                },
                {title_contains: q}
            )
            search_window.show()
        })
    }
    translate(key, params = {}) {
        return this.#i18n.t(key, params)
    }
    async init() {
        await this.init_language()
        await this.select_map.init()

        let params = get_params()
        if(params.map_id) {
            this.set_selected_map_id(params.map_id)
        }
        if(params.term_id) {
            set_next_visit_source(VisitSource.External)
            await this.render(params.term_id)
            this.set_selected_map_id(this.get_selected_map_id())
            this.#term_container.show()
        }
        if(params.term_id || params.map_id) return
        await this.#term_container.render_last_visited_term(params.map_id)
    }

    async init_language() {
        let lang = this.#i18n.getLanguage()
        let select_language = get_element("select_language")
        select_language.innerHTML = ""
        SUPPORTED_LANGUAGES.forEach(l=>{
            let option = document.createElement("option")
            option.value = l.iso639
            option.innerText = new LanguageObject((l)).generate_text_for_option()
            if(option.value === lang) option.selected = true
            select_language.appendChild(option)
        })
        select_language.onchange = ()=> {
            let new_lang = select_language.selectedOptions[0].value
            this.#i18n.setLanguage(new_lang)
        }
        await this.#i18n.setLanguage(lang)
    }

    show_term_container() {
        this.#term_container.show()
    }

    refresh_autocomplete_term_title() {
        this.#autocomplete_term_title.destroy()
        this.#autocomplete_term_title = new Autocomplete(this.#input_search_term, 1, "dictionary_term_fulltext", "&alias=1&dictionary_map_id=" + this.select_map.get_selected_map_id(), "title", "title_part", "div_search_term_end")
        this.#autocomplete_term_title.addCallback(async () => {
            let item = this.#autocomplete_term_title.get_item()
            showInfo(this.translate("dictionary.term.info.found_term") + ": " + item.title)
            await this.#term_container.show()
            if(get_next_visit_source() !== VisitSource.Random) set_next_visit_source(VisitSource.Search)
            await this.#term_container.render(item.id)
            this.#input_search_term.value = ""
        })
        this.set_search_all_results_callback()
    }

    get_selected_map_id() {
        return this.select_map.get_selected_map_id()
    }
    set_selected_map_id(map_id) {
        return this.select_map.set_selected_map_id(map_id)
    }

    #rendering_in_progress = false
    async render(dictionary_term_id, show_container = false) {
        if(this.#rendering_in_progress) {
            showWarn(translate("dictionary.term.warn.another_term_is_already_loading"))
            return;
        }
        // get_element("main_container").style.display = "none"
        // get_element("loading_div").style.display = "block"
        this.#rendering_in_progress = true
        await this.#term_container.render(dictionary_term_id)
        this.#rendering_in_progress = false
        // get_element("main_container").style.display = "block"
        // get_element("loading_div").style.display = "none"
        if(show_container) {
            await this.#term_container.show();
        }

    }
}