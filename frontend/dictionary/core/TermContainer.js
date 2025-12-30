import {
    get_element,
    loadFromLocalStorage,
    saveToLocalStorage,
    showError,
    showInfo,
    showSuccess,
    showWarn
} from "../../dom.js";
import {delete_entity, list_all_entities, post_entity, put_entity, QueryParams, read_entity} from "../../api.js";
import {defined} from "../../common.js";
import {Tags} from "./sections/Tags.js";
import {Flags} from "./sections/Flags.js";
import {Links} from "./sections/Links.js";
import {Backlinks} from "./sections/Backlinks.js";
import {Notes} from "./sections/Notes.js";
import {Urls} from "./sections/Urls.js";
import {Indexes} from "./sections/Indexes.js";
import {Sources} from "./sections/Sources.js";
import {Aliases} from "./sections/Aliases.js";
import {Entities} from "../entities/Entities.js";
import {attachMarkdownEditor} from "../markdown/Markdown.js";
import {
    clear_next_visit_source,
    get_next_visit_source,
    set_next_visit_source,
    translate,
    USER_ID
} from "../globals/Globals.js";
import {Button} from "../dom/elements/Button.js";
import {_10PX} from "../styles/Styles.js";
import {set_params} from "./Utils.js";
import {VirtualWindow} from "../window/VirtualWindow.js";
import {Div} from "../dom/elements/Div.js";
import {Span} from "../dom/elements/Span.js";
import {B} from "../dom/elements/B.js";
import {VisitSource} from "../enums/VisitSource.js";

const COLON_SPACE = ": "

class TermContainer {
    #element
    #dictionary_term_json
    dictionary_term_id = 0
    #sections = null

    constructor(
        get_selected_map_id_callback,
        set_selected_map_id_callback,
        render_term_id_callback) {
        this.#element = get_element("term_container");

        this.get_selected_map_id_callback = get_selected_map_id_callback
        this.set_selected_map_id_callback = set_selected_map_id_callback
        this.render_term_id_callback = render_term_id_callback
        this.#sections = [
            new Tags(get_selected_map_id_callback),
            new Flags(get_selected_map_id_callback),
            new Links(get_selected_map_id_callback, render_term_id_callback),
            new Backlinks(get_selected_map_id_callback, render_term_id_callback),
            new Notes(get_selected_map_id_callback),
            new Urls(get_selected_map_id_callback),
            new Indexes(get_selected_map_id_callback),
            new Sources(get_selected_map_id_callback),
            new Aliases(get_selected_map_id_callback)
        ]

        let term_container_h2 = get_element("term_container_h2")
        term_container_h2.style.backgroundColor = "rgba(213,215,221,0.6)"
        term_container_h2.style.color = "#2c3e50";
        term_container_h2.style.padding = "10px"
        term_container_h2.style.borderBottom = "1px solid #dcdfe3"

        function attach_onclick_to_label(section, models, self) {
            let label = get_element("label_" + models)
            let container = get_element("container_" + models)
            label.style.cursor = "pointer"
            label.style.backgroundColor = "rgba(213,215,221,0.6)"
            label.style.color = "#2c3e50";
            label.style.borderBottom = "1px solid #dcdfe3"
            label.style.padding = "10px"
            let expand_button = new Button("▶️ Expand")
            let collapse_button = new Button("▼ Collapse")

            if(models === "notes") {
                expand_button.element().onclick = (e)=> {
                    console.debug("expand_button clicked")
                    section.execute_action("expand");
                    // label.click()
                }
                collapse_button.element().onclick = (e)=> {
                    console.debug("collapse_button clicked")
                    section.execute_action("collapse");
                }
                label.appendChild(expand_button.element())
                label.appendChild(collapse_button.element())
                Array.from([expand_button, collapse_button]).forEach((e)=>{
                    e.styles().marginLeft(_10PX).end()
                })
            }
            label.onclick = (e) => {
                if (e.target === expand_button.element()) return;
                if (e.target === collapse_button.element()) return;
                e.preventDefault()
                e.stopPropagation()

                let current_display = container.style.display;
                let shown = current_display === "block" || current_display === "";
                container.style.display = shown ? "none" : "block"
                let containerParent = container.parentElement
                let tmp_id = "label_" + models + "_tmp"
                if (shown) {
                    let start = models.charAt(0).toUpperCase() + models.slice(1)
                    let text = translate("dictionary.term.container.click_note", {models: start})

                    let tmp_span = document.createElement("span")
                    tmp_span.innerText = text
                    tmp_span.dataset.i18n = "dictionary.term.container.click_note"
                    tmp_span.dataset.i18nParams = "models::" + "dictionary.sections." + models + ".label." + models

                    tmp_span.style.color = "grey"
                    tmp_span.style.fontSize = "75%"
                    tmp_span.style.fontWeight = "normal"
                    tmp_span.id = tmp_id
                    tmp_span.style.marginLeft = "10px"
                    label.appendChild(tmp_span)
                    containerParent.style.margin = "0px 0 0px 0"
                    // label.style.display = "inline-block"
                    label.style.whiteSpace = "nowrap"
                    expand_button.hide()
                    collapse_button.hide()
                } else {
                    get_element(tmp_id).remove()
                    containerParent.style.margin = "10px 0 10px 0"
                    // label.style.display = "block"
                    label.style.whiteSpace = "normal"
                    expand_button.show("inline-block")
                    collapse_button.show("inline-block")
                }

            }

            // const array = ["flags", "backlinks", "indexes", "sources", "aliases"]
            // array.forEach(e => {
            //     if (models === e) {
            //         label.click()
            //     }
            // })
        }

        this.#sections.forEach(section => {
            let models = section.get_configuration().models
            attach_onclick_to_label(section, models, this)
        })

    }

    show() {
        this.#element.style.display = "block"
    }

    hide() {
        this.#element.style.display = "none"
    }

    async render_last_visited_term() {
        let term_id = loadFromLocalStorage("last_visited_term_id")
        if(!term_id) return

        set_next_visit_source(VisitSource.LastVisited)
        this.render(term_id)
        this.show()
    }
    async render(dictionary_term_id) {
        if (this.dictionary_term_id === dictionary_term_id) {
            showWarn(translate("dictionary.term.container.info.term_already_shown"))
            return
        }
        if (dictionary_term_id === 0) return;
        // if (dictionary_term_id === null) return;
        // if (dictionary_term_id === undefined) return;

        await this.render_term(dictionary_term_id);
        for(let i = 0;i< this.#sections.length;i++) {
            let section = this.#sections[i];
            //await
                section.render(dictionary_term_id)
        }
    }

    async render_term(dictionary_term_id) {
        this.dictionary_term_id = dictionary_term_id
        let dictionary_term = await read_entity(Entities.dictionary_term, dictionary_term_id)
        if(!defined(dictionary_term)) {
            showError(translate("dictionary.term.container.error.reading_term_failed") + ": " + dictionary_term_id)
            // throw new Error ("Reading term failed: " + dictionary_term_id)
            return;
        }
        set_params(null, dictionary_term_id)
        if(this.get_selected_map_id_callback() !== dictionary_term.dictionary_map_id) {
            this.set_selected_map_id_callback(dictionary_term.dictionary_map_id)
        }
        document.title = translate("dictionary.title.dictionary_app") + " - " + dictionary_term.title
        this.#dictionary_term_json = dictionary_term
        get_element("button_older").onclick = async () => {
            let older = await read_entity("dictionary_older_term", dictionary_term.id)
            if(!older) {
                showError("Reading older term failed.")
                return
            }
            let older_term_id = older.older_term_id
            if(older_term_id === 0) {
                showWarn("This map has no older term.")
                return
            }
            showInfo(translate("dictionary.term.info.found_term") + ": " + older_term_id)
            set_next_visit_source(VisitSource.Older)
            this.render_term_id_callback(older_term_id, true)
        }
        get_element("button_newer").onclick = async ()=> {
            let newer = await read_entity("dictionary_newer_term", dictionary_term.id)
            if(!newer) {
                showError("Reading newer term failed.")
                return
            }
            let newer_term_id = newer.newer_term_id
            if(newer_term_id === 0)
            {
                showWarn("This map has no newer term.")
                return;
            }
            showInfo(translate("dictionary.term.info.found_term") + ": " + newer_term_id)
            set_next_visit_source(VisitSource.Newer)
            this.render_term_id_callback(newer_term_id, true)        }
        get_element("h2_term_id").innerText = dictionary_term.id
        get_element("input_title").value = dictionary_term.title
        get_element("input_disambiguation").value = dictionary_term.disambiguation
        get_element("textarea_definition").value = dictionary_term.definition

        attachMarkdownEditor({
            textarea: get_element("textarea_definition"),
            buttonEdit: get_element("button_edit_definition"),
            buttonRead: get_element("button_read_definition")
        });

        get_element("select_status").selectedIndex = dictionary_term.status

        let list_understandings = await list_all_entities(
            Entities.dictionary_term_understanding,
            new QueryParams().add_user_id().add(Entities.dictionary_term_understanding.dictionary_term_id, dictionary_term_id).build()
        )
        if (!list_understandings) {
            showError("Loading dictionary_term_understanding failed")
            this.hide()
            return
        } else {
            let understanding = list_understandings.length === 0 ? null : list_understandings[0]
            get_element("select_understanding").selectedIndex = understanding === null ? 0 : understanding.level
        }
        let button_show_understanding_legend = get_element("button_show_understanding_legend")
        if(!button_show_understanding_legend.onclick) {
            button_show_understanding_legend.onclick=()=> {
                let window = new VirtualWindow(
                    {
                        title: "Understanding legend",
                        width: 600,
                        height: 400
                    }
                )
                let content = new Div()

                function appendUnderstanding(content, title, description) {
                    content.appendChild(new Div(new B(title)))
                    content.appendChild(new Div(
                        new Span(" -- "),
                        new Span(description)
                    ))
                }

                appendUnderstanding(content, "Unknown",
                    "I know the term exists, but nothing more.")

                appendUnderstanding(content, "Recognized",
                    "I recognize the name and its general context.")

                appendUnderstanding(content, "Understood",
                    "I understand the definition and core idea.")

                appendUnderstanding(content, "Applied",
                    "I can correctly use it in practice.")

                appendUnderstanding(content, "Internalized",
                    "Using it is automatic and requires no conscious effort.")


                window.set_content(content.element())
                window.show()
            }
        }

        let checkbox_pinned = get_element("checkbox_pinned")
        let pinned_terms = await list_all_entities(
            Entities.dictionary_pinned_term,
            new QueryParams()
                .add_user_id()
                .add(Entities.dictionary_pinned_term.dictionary_term_id, dictionary_term_id)
                .build()
        )
        if (!defined(pinned_terms)) {
            showError(translate("dictionary.term.container.error.loading_pinned_terms_failed"))
        } else {
            checkbox_pinned.checked = pinned_terms.length > 0
        }

        let importance = dictionary_term.importance
        let difficulty = dictionary_term.difficulty
        let id1 = "input_importance_" + (importance === 1 ? "low" : (importance === 2 ? "medium" : "high"));

        let id2 = "input_difficulty_" + (difficulty === 1 ? "easy" : (difficulty === 2 ? "medium" : "hard"))

        let input_importance = get_element(id1)
        if (input_importance === null) {
            showWarn("There is no id: " + id1)
        }
        let input_difficulty = get_element(id2)
        input_importance.checked = true;
        input_difficulty.checked = true;


        let checkbox_repetition = get_element("checkbox_repetition")
        checkbox_repetition.checked = dictionary_term.repetition !== 0

        get_element("textarea_definition").innerText = dictionary_term.definition

        get_element("button_delete_term").onclick = async () => {
            if (!confirm(translate("dictionary.term.container.prompt.delete_term"))) return;

            async function delete_rows(model_name, entities) {
                for (const e of entities) {
                    let delete_result = await delete_entity(model_name, e.id)
                    if (!delete_result) {
                        showError(translate("dictionary.term.container.error.deleting_model_failed", {model_name: model_name}));
                    }
                }
            }

            let table = null;

            table = Entities.dictionary_pinned_term
            let pinned_terms = await list_all_entities(
                table,
                new QueryParams()
                    .add_user_id()
                    .add(table.dictionary_term_id, dictionary_term_id)
                    .build()
            )

            table = Entities.dictionary_tag
            let tags = await list_all_entities(
                table,
                new QueryParams(table.dictionary_term_id, dictionary_term_id).build())

            table = Entities.dictionary_flag
            let private_flags = await list_all_entities(
                table,
                new QueryParams()
                    .add(table.is_public, 0)
                    .add(table.dictionary_term_id, dictionary_term_id)
                    .add_user_id()
                    .build())

            table = Entities.dictionary_flag
            let public_flags = await list_all_entities(
                table,
                new QueryParams()
                    .add(table.is_public, 1)
                    .add(table.dictionary_term_id, dictionary_term_id)
                    .build()
            )

            table = Entities.dictionary_link
            let links_from = await list_all_entities(
                table,
                new QueryParams(table.from_dictionary_term_id, dictionary_term_id).build())

            table = Entities.dictionary_link
            let links_to = await list_all_entities(
                table,
                new QueryParams(table.to_dictionary_term_id, dictionary_term_id).build())

            table = Entities.dictionary_note
            let notes = await list_all_entities(
                table,
                new QueryParams(table.dictionary_term_id, dictionary_term_id).build())

            table = Entities.dictionary_index
            let indexes = await list_all_entities(
                table,
                new QueryParams(table.dictionary_term_id, dictionary_term_id).build())

            table = Entities.dictionary_source
            let sources = await list_all_entities(
                table,
                new QueryParams(table.dictionary_term_id, dictionary_term_id).build())

            table = Entities.dictionary_term_alias
            let aliases = await list_all_entities(
                table,
                new QueryParams(table.dictionary_term_id, dictionary_term_id).build())

            table = Entities.dictionary_term_visit
            let visits = await list_all_entities(
                table,
                new QueryParams(table.dictionary_term_id, dictionary_term_id).add_user_id().build())

            table = Entities.dictionary_url
            let urls = await list_all_entities(
                table,
                new QueryParams(table.dictionary_term_id, dictionary_term_id).build())

            table = Entities.dictionary_term_understanding
            let understandings = await list_all_entities(
                table,
                new QueryParams(table.dictionary_term_id, dictionary_term_id).add_user_id().build())

            table = Entities.dictionary_review
            let reviews = await list_all_entities(
                table,
                new QueryParams(table.dictionary_term_id, dictionary_term_id).add_user_id().build())

            table = Entities.dictionary_state_18
            let states_18 = await list_all_entities(
                table,
                new QueryParams(table.dictionary_term_id, dictionary_term_id).add_user_id().build())

            dictionary_term.status = 6 //deleted
            let updated = await put_entity(Entities.dictionary_term, dictionary_term_id, dictionary_term)
            if (!defined(updated)) {
                showError(translate("dictionary.term.container.error.setting_status_to_deleted_failed"))
                return;
            }

            await delete_rows(Entities.dictionary_pinned_term, pinned_terms)
            await delete_rows(Entities.dictionary_flag, private_flags)
            await delete_rows(Entities.dictionary_flag, public_flags)
            await delete_rows(Entities.dictionary_link, links_from)
            await delete_rows(Entities.dictionary_link, links_to)
            await delete_rows(Entities.dictionary_note, notes)
            await delete_rows(Entities.dictionary_review, reviews)
            await delete_rows(Entities.dictionary_index, indexes)
            await delete_rows(Entities.dictionary_source, sources)
            await delete_rows(Entities.dictionary_state_18, states_18)
            await delete_rows(Entities.dictionary_tag, tags)
            await delete_rows(Entities.dictionary_term_alias, aliases)
            await delete_rows(Entities.dictionary_term_visit, visits)
            await delete_rows(Entities.dictionary_url, urls)
            await delete_rows(Entities.dictionary_term_understanding, understandings)

            let delete_dictionary_term = await delete_entity(Entities.dictionary_term, dictionary_term_id)
            if (delete_dictionary_term !== null && delete_dictionary_term !== undefined) {
                showSuccess( translate("dictionary.term.container.info.deleting_term_successful")+ ": " + dictionary_term.title)
                this.hide()
                set_params(this.get_selected_map_id_callback(), null)
                saveToLocalStorage("last_visited_term_id",null)

            } else {
                showError( translate("dictionary.term.container.error.deleting_term_failed")+ ": " + dictionary_term.title)
            }
        }

        get_element("button_save_term").onclick = async () => {
            let new_term = structuredClone(this.#dictionary_term_json)
            new_term.title = get_element("input_title").value
            new_term.disambiguation = get_element("input_disambiguation").value
            new_term.definition = get_element("textarea_definition").value

            let checkbox_pinned = get_element("checkbox_pinned")
            let pinned_now = checkbox_pinned.checked
            let pinned_terms = await list_all_entities(
                Entities.dictionary_pinned_term,
                new QueryParams()
                    .add(Entities.dictionary_pinned_term.dictionary_term_id, dictionary_term_id)
                    .add_user_id()
                    .build()
            )
            if (!defined(pinned_terms)) {
                showError(translate("dictionary.term.container.error.loading_pinned_terms_failed"))
            } else {
                let pinned_in_db = pinned_terms.length > 0
                if (pinned_now !== pinned_in_db) {
                    if (pinned_now) {
                        let new_pinned_term = {
                            dictionary_term_id: dictionary_term_id,
                            user_id: USER_ID,
                            dictionary_map_id: this.get_selected_map_id_callback()
                        }
                        let created = await post_entity(Entities.dictionary_pinned_term, new_pinned_term)
                        if (defined(created)) {
                            showSuccess(translate("dictionary.term.container.info.creating_pinned_term_successful"))
                            checkbox_pinned.checked = true
                        } else {
                            showError(translate("dictionary.term.container.error.creating_pinned_term_failed"))
                        }
                    }
                    if (!pinned_now) {
                        for (const e of pinned_terms) {
                            let deleted = await delete_entity(Entities.dictionary_pinned_term, e.id)
                            if (deleted) {
                                showSuccess(translate("dictionary.term.container.info.deleting_pinned_term_successful") + COLON_SPACE + e.id)
                                checkbox_pinned.checked = false
                            } else {
                                showError(translate("dictionary.term.container.error.deleting_pinned_term_failed") + COLON_SPACE + e.id)
                            }
                        }
                    }
                }
            }

            new_term.status = get_element("select_status").selectedIndex


            let understanding_level = get_element("select_understanding").selectedIndex

            let list_understandings = await list_all_entities(
                Entities.dictionary_term_understanding,
                new QueryParams().add_user_id().add(Entities.dictionary_term_understanding.dictionary_term_id, dictionary_term_id).build()
            )
            if (!list_understandings) {
                showError("Saving understanding failed")
            } else {
                let understanding = list_understandings.length === 0 ? null : list_understandings[0]
                let to_be_created = understanding === null;
                if(to_be_created) {
                    understanding = {
                        dictionary_term_id: dictionary_term_id,
                        user_id: USER_ID,
                        dictionary_map_id: this.get_selected_map_id_callback(),
                        level: understanding_level
                    }
                } else {
                    understanding.level = understanding_level
                }

                if (to_be_created) {

                    let created = await post_entity(Entities.dictionary_term_understanding, understanding)
                    if (defined(created)) {
                        showSuccess("Creating new term understanding was successful.")
                    } else {
                        showError("Creating new term understanding failed.")
                    }
                }
                if (!to_be_created) {

                    let updated = await put_entity(Entities.dictionary_term_understanding, understanding.id, understanding)
                    if (defined(updated)) {
                        showSuccess("Updating term understanding was successful.")
                    } else {
                        showError("Updating term understanding failed.")
                    }
                }
            }

            let importance = 0
            let difficulty = 0
            let input_importance_low = get_element("input_importance_low")
            let input_importance_medium = get_element("input_importance_medium")
            let input_importance_high = get_element("input_importance_high")
            let input_difficulty_easy = get_element("input_difficulty_easy")
            let input_difficulty_medium = get_element("input_difficulty_medium")
            let input_difficulty_hard = get_element("input_difficulty_hard")
            if (input_importance_low.checked) importance = 1
            if (input_importance_medium.checked) importance = 2
            if (input_importance_high.checked) importance = 3
            if (input_difficulty_easy.checked) difficulty = 1
            if (input_difficulty_medium.checked) difficulty = 2
            if (input_difficulty_hard.checked) difficulty = 3
            new_term.importance = importance
            new_term.difficulty = difficulty
            new_term.repetition = get_element("checkbox_repetition").checked ? 1 : 0
            let updated = await put_entity(Entities.dictionary_term, dictionary_term_id, new_term)
            if (updated !== null && updated !== undefined) {
                showSuccess(translate("dictionary.term.container.info.updating_term_successful"))
                this.#dictionary_term_json = new_term
            } else {
                showError(translate("dictionary.term.container.error.updating_term_failed"))
            }
        }

        let new_visit = {
            dictionary_term_id: dictionary_term_id,
            user_id: USER_ID,
            dictionary_map_id: this.get_selected_map_id_callback(),
            source: get_next_visit_source().id
        }
        clear_next_visit_source()

        let created_dictionary_term_visit = post_entity(Entities.dictionary_term_visit, new_visit)
        if (created_dictionary_term_visit === null || created_dictionary_term_visit === undefined) {
            showError(translate("dictionary.term.container.error.creating_term_visit_failed"))
        }
        const MILLISECONDS_PER_365_DAYS = 1000 * 3600 * 24 * 365;
        saveToLocalStorage("last_visited_term_id",dictionary_term_id, MILLISECONDS_PER_365_DAYS)
    }
}

export default TermContainer