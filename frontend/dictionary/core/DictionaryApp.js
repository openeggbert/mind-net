import {getTitleCache, list_entities, post_entity, QueryParams, read_entity, setTitleCache} from "../../api.js";
import {formatDateTime, get_element, showError, showInfo, showSuccess, showWarn, sleep_for_seconds} from "../../dom.js";
import {Autocomplete, null_or_undefined} from "../../common.js";
import {SelectMap} from "./SelectMap.js";
import TermContainer from "./TermContainer.js";
import {SearchWindow} from "../search/SearchWindow.js";
import {Entities} from "../entities/Entities.js";
import {showWindowFromUrl, VirtualWindow} from "../window/VirtualWindow.js";
import {RepetitionWindow} from "../repetition/RepetitionWindow.js";
import {I18n} from "../i18n/I18n.js";
import {LanguageObject, SUPPORTED_LANGUAGES} from "../i18n/Language.js";
import {get_next_visit_source, set_i18n, set_next_visit_source, translate, USER_ID} from "../globals/Globals.js";
import {Div} from "../dom/elements/Div.js";
import {get_params, set_params} from "./Utils.js";
import {VisitSource} from "../enums/VisitSource.js";
import {enumValue} from "../enums/EnumFunctions.js";

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
            set_next_visit_source(VisitSource.Random)
            this.#autocomplete_term_title.set_selected_item(0)
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
            let win = new VirtualWindow({
                title: translate("dictionary.term.container.visit_window.term_visit_history"),
                width: 1000,
                height: 600,
            })

            win.show()
            let content = new Div()

            let button = document.createElement("button")
            button.innerText = translate("dictionary.term.container.visit_window.button_show_all_visits")
            button.dataset.i18n = "dictionary.term.container.visit_window.button_show_all_visits"
            button.onclick = () => {
                let url = "index.html?entity=dictionary_term_visit&action=list&user_id=" + USER_ID
                showWindowFromUrl(translate("dictionary.term.container.visit_window.all_visits"), url)
            }
            button.style.margin = "20px;"
            button.style.textAlign = "center"

            content.appendChild(button)

            let table = document.createElement("table");
            content.appendChild(table);
            win.set_content(content.element())

            let visits_result = await list_entities(
                Entities.dictionary_term_visit,
                new QueryParams()
                    .add("user_id", USER_ID)
                    .sort("created_at", "desc")
                    .build(),
                1,
                100
            )
            if (null_or_undefined(visits_result)) {
                showError(translate("dictionary.term.container.visit_window.error.loading_visits_failed"))
                return;
            }
            let visits = visits_result.items
            const disambiguation_map = new Map()

            table.style.borderCollapse = "collapse";
            table.style.margin = "0 auto";
            let tr_first = document.createElement("tr");
            table.appendChild(tr_first);
            let th_number = document.createElement("th");
            th_number.innerText = "#"
            let th_id = document.createElement("th");
            th_id.innerText = translate("dictionary.common.term_id")
            th_id.dataset.i18n = "dictionary.common.term_id"
            let th_title = document.createElement("th");
            th_title.innerText = translate("dictionary.common.title")
            th_title.dataset.i18n = "dictionary.common.title"

            let th_source = document.createElement("th");
            th_source.innerText = "Source"

            let th_timestamp = document.createElement("th");
            th_timestamp.innerText = translate("dictionary.common.timestamp")
            th_timestamp.dataset.i18n = "dictionary.common.timestamp"

            tr_first.appendChild(th_number)
            tr_first.appendChild(th_id)
            tr_first.appendChild(th_title)
            tr_first.appendChild(th_source)
            tr_first.appendChild(th_timestamp)
            for (const el of [th_number, th_id, th_title, th_source, th_timestamp]) {
                el.style.minWidth = "20px"
                el.style.padding = "10px";
                el.style.border = "1px solid black";
                el.style.background = "#ccc"
            }
            th_title.style.minWidth = "200px"

            let history_entry_number = 0;
            for (const entry of visits.slice()) {
                history_entry_number++

                let visited_term_id = entry.dictionary_term_id

                if (!disambiguation_map.has(visited_term_id)) {
                    let term = await read_entity(Entities.dictionary_term, visited_term_id)
                    if (term === null || term === undefined) {
                        disambiguation_map.set(visited_term_id, "")
                        showWarn(translate("dictionary.term.container.error.reading_term_failed") + ": " + visited_term_id);
                    } else {
                        disambiguation_map.set(visited_term_id, term.disambiguation)
                    }
                }

                let tr = document.createElement("tr");
                table.appendChild(tr)
                let td_number = document.createElement("td");
                let td_id = document.createElement("td");
                let td_title = document.createElement("td");
                let td_source = document.createElement("td");
                let td_timestamp = document.createElement("td");
                for (const el of [td_number, td_id, td_title, td_source, td_timestamp]) {
                    el.style.padding = "10px";
                    el.style.border = "1px solid black";
                }

                tr.appendChild(td_number)
                tr.appendChild(td_id)
                tr.appendChild(td_title)
                tr.appendChild(td_source)
                tr.appendChild(td_timestamp)
                td_number.innerText = String(history_entry_number);
                td_id.innerText = visited_term_id;
                let a = document.createElement("a");
                a.href = "?";
                {
                    let title = getTitleCache(Entities.dictionary_term, visited_term_id)
                    if (title === null || title === undefined) {
                        let x = visited_term_id
                        let term_ = await read_entity(Entities.dictionary_term, x)
                        if (term_ === null) {
                            showWarn(translate("dictionary.term.container.error.reading_term_failed") + x);
                            title = "??? (# " + x + ")"
                        } else {
                            title = term_.title
                            setTitleCache(Entities.dictionary_term, x, title);
                        }
                    }
                    let finalTitle = title
                    if (disambiguation_map.has(visited_term_id)) {
                        let value = disambiguation_map.get(visited_term_id)
                        if (value !== "") finalTitle = title + " (" + disambiguation_map.get(visited_term_id) + ")"
                    }
                    a.title = finalTitle

                    a.innerText = finalTitle;
                }

                let callback = async (term_id) => await this.render(term_id, true)
                a.onclick = async function () {
                    event.preventDefault();
                    set_next_visit_source(VisitSource.VisitedHistory)
                    await callback(visited_term_id)
                }
                td_title.appendChild(a)
                let source = enumValue(VisitSource, entry.source)
                td_source.innerText = source == null ? VisitSource.Unknown.label : source.label
                td_timestamp.innerText = formatDateTime(entry.created_at, true, true, true)
            }

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
        await this.#term_container.render_last_visited_term()
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