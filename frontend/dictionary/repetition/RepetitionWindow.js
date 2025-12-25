import {VirtualWindow} from "../window/VirtualWindow.js";
import {Form} from "../dom/elements/Form.js";
import {Label} from "../dom/elements/Label.js";
import {Span} from "../dom/elements/Span.js";
import {Select} from "../dom/elements/Select.js";
import {Input} from "../dom/elements/Input.js";
import {Checkbox} from "../dom/elements/Checkbox.js";
import {Button} from "../dom/elements/Button.js";
import {_10PX, _20PX, _5PX} from "../styles/Styles.js";
import {list_all_entities, QueryParams} from "../../api.js";
import {Entities} from "../entities/Entities.js";
import {defined} from "../../common.js";
import {showError} from "../../dom.js";
import {Option} from "../dom/elements/Option.js";
import {SearchAutocomplete} from "../search/SearchAutocomplete.js";
import {EventType} from "../dom/attributes/EventType.js";
import {FormRow} from "../search/FormRow.js";
import {Div} from "../dom/elements/Div.js";
import {translate} from "../globals/Globals.js";
import {Br} from "../dom/elements/Br.js";
import {markdownToHtml} from "../markdown/Markdown.js";
import {Color} from "../styles/properties/Color.js";
import {Display} from "../styles/properties/Display.js";

let centred = false

class RepetitionModel {
    constructor() {
        this.map_id = 0
        this.search_id = 0
        this.search_name = ""
        this.due = true
        this.not_due = false
        this.never = true
        this.has_definition = true
    }

    to_json() {
        return {
            map_id: this.map_id,
            search_id: this.search_id,
            search_name: this.search_name,
            due: this.due ? 1 : 0,
            not_due: this.not_due ? 1 : 0,
            never: this.never ? 1 : 0,
            has_definition: this.has_definition ? 1 : 0
        }
    }
}

class AskUserForGradeModel {

}

export class RepetitionWindow extends VirtualWindow {
    constructor(get_selected_map_id_callback, render_term_callback) {
        super({
            title: "🔁 " + translate("dictionary.repetition.repetition"),
            width: screen.width > 1000 ? 1000 : screen.width - 100,
            height: 600
        })
        this.get_selected_map_id_callback = get_selected_map_id_callback
        this.render_term_callback = render_term_callback
        this.set_content_padding()
    }

    async init() {
        let model = null
        while (true) {
            model = await this.ask_user_for_model(model)
            let searches = await this.list_term_searches(model)
            // alert(JSON.stringify(searches))
            if (!searches) searches = await this.list_term_searches(model)
            if (!searches) {
                showError(translate("dictionary.repetition.error.loading_terms_for_review_failed"))
                continue
            }
            for (let i = 0; i < searches.length; i++) {
                let search = searches[i]
                await this.ask_user_for_grade(search, i + 1, searches.length)
            }
            // console.log("User confirmed repetition:", model)
            // let c = confirm("Continue");
            // if (!c) break;
        }

    }

    async ask_user_for_grade(term_for_review, number, size) {
        return new Promise(async (resolve, reject) => {
            class RepetitionButton extends Button {
                constructor(text) {
                    super(text)
                        .add_class("repetition_button")
                        .add_class("common-repetition_button")
                    this.styles().backgroundColor("rgb(85, 85, 85)").end()

                }
            }
            class RatingButton extends Button {
                constructor(text) {
                    super(text)
                        .add_class("repetition_button")
                        .add_class("rating-btn")
                }
            }
            class ActionButton extends Button {
                constructor(text) {
                    super(text)
                        .add_class("repetition_button")
                        .add_class("action-btn")
                }
            }
            let content = new Div().add_class("repetition_div")
            this.set_content(content.element())

            let card = new Div().add_class("repetition_card")
            content.appendChild(card)

            let item = new Label("i18n.dictionary.repetition.item")
            item.styles().display(Display.Inline).end()
            card
                .appendChild(new Span(item, new Span(number + " / " + size).styles().fontWeight("bold").end()))
                .appendChild(new Br())


            card.appendChild(new Label("❓ " + term_for_review.title).styles().fontSize("150%").margin(_10PX).end())
            card.appendChild(new Div(new RepetitionButton("Show definition")))
            let definition_div =new Div()
                .styles()
                .padding(_10PX)
                .minHeight("100px")
                .color(Color.Black)
                .end()
                .add_class("definition_div")
            card.appendChild(definition_div.set_html_unsafe(markdownToHtml(term_for_review.definition)))
            card.appendChild(new Div("How well did you recall the note?").styles().margin(_10PX).end())
            card.appendChild(new Div(
                new RatingButton("0"),
                new RatingButton("1"),
                new RatingButton("2"),
                new RatingButton("3"),
                new RatingButton("4"),
                new RatingButton("5")
            ))
            card.appendChild(new RepetitionButton("Show legend")).appendChild(new Br())
            card.appendChild(new Div("Next action").styles().margin(_10PX).end())
            let send_button = new ActionButton("Send").on("click", () => {
                resolve(3)
            })
            card.appendChild(new Div(send_button, new ActionButton("Skip")))
        })
    }

    async ask_user_for_model(model = null) {
        return new Promise(async (resolve, reject) => {

            let content = new Div()
            this.set_content(content.element())
            if(!centred) {
                this.center()
                centred = true
            }

            const repetition_form = new Form()
            content.appendChild(repetition_form.element())

            let maps = await list_all_entities(Entities.dictionary_map, new QueryParams().sort(Entities.dictionary_map.position).build());
            if (!defined(maps)) {
                showError(translate("dictionary.crud_section.error.listing_models_failed", {models: translate("dictionary.common.maps").toLowerCase()}))
                return
            }

            function add_control(label, control) {
                let row = new FormRow(label, control)
                row.styles().paddingTop("10px").end()
                repetition_form.append(row)
            }

            let map_select = new Select()
            let o0 = new Option(0, translate("dictionary.common.any"))
            o0.element().dataset.i18n = "dictionary.common.any"
            map_select.add_option(o0)

            maps.forEach(map => {
                let option = new Option(map.id, map.name)
                console.log("map to be enabled: " + map.id + " " + map.name + " " + this.get_selected_map_id_callback())
                let map_id_number = Number(map.id)
                if (map_id_number === (defined(model) ? Number(model.map_id) : Number(this.get_selected_map_id_callback()))) {
                    option.set_selected()
                }
                map_select.add_option(option)
            })
            add_control("i18n.dictionary.common.map", map_select)

            let search_input = new Input().set_placeholder(translate("dictionary.common.load_a_search"))
            search_input.element().dataset.i18nPlaceholder = "dictionary.common.load_a_search"
            let search_end_repetition = new Span().set_id("search_end_repetition_" + this.get_created_at())

            add_control("i18n.dictionary.common.search", new Span(search_input, search_end_repetition))
            let search_autocomplete = new SearchAutocomplete(this, "repetition_" + this.get_created_at(), search_input.element(), 1, "dictionary_search_fulltext", "&dictionary_map_id=" + map_select.get_selected_values()[0])
            if (defined(model) && model.search_id !== 0 && model.search_name.length > 0) {
                console.log("set_from_title(" + model.search_name + ")")
                await search_autocomplete.set_from_title(model.search_name)
            }

            map_select.on(EventType.Change.label, () => {
                search_autocomplete.set_query_params("&dictionary_map_id=" + map_select.get_selected_values()[0])
            })

            class ModePair extends Span {
                constructor(label, checked = true) {
                    super(
                        new Checkbox().set_checked(checked).styles().marginRight(_20PX).end(),
                        new Label(label, "").styles().marginRight(0).end(),
                    );
                }

                get_checkbox() {
                    return this.children()[0]._object
                }
            }

            let due_checkbox = new ModePair("i18n.dictionary.common.due", defined(model) ? model.due : true)
            let not_due_checkbox = new ModePair("i18n.dictionary.common.not_due", defined(model) ? model.not_due : false)
            let never_checkbox = new ModePair("i18n.dictionary.common.never", defined(model) ? model.never : true)
            let modes = new Span(
                due_checkbox, not_due_checkbox, never_checkbox
            )
            add_control("i18n.dictionary.common.mode", modes)

            let has_definition_checkbox = new Checkbox().set_checked(defined(model) ? model.has_definition : true)
            add_control("i18n.dictionary.common.has_definition", has_definition_checkbox)

            let start_button = new Button(translate("dictionary.common.start")).styles().marginTop(_20PX).marginLeft(_20PX).transform("scale(1.5)").end()
            start_button.element().dataset.i18n = "dictionary.common.start"
            content.appendChild(start_button)

            search_autocomplete.addCallback(() => {
                due_checkbox.get_checkbox().element().disabled = true
                not_due_checkbox.get_checkbox().element().disabled = true
                never_checkbox.get_checkbox().element().disabled = true
                has_definition_checkbox.element().disabled = true
            })
            search_autocomplete.addResetCallback(() => {
                due_checkbox.get_checkbox().element().disabled = false
                not_due_checkbox.get_checkbox().element().disabled = false
                never_checkbox.get_checkbox().element().disabled = false
                has_definition_checkbox.element().disabled = false
            })
            if (defined(model)) {
                let search = model.search_id !== 0
                due_checkbox.get_checkbox().element().disabled = search
                not_due_checkbox.get_checkbox().element().disabled = search
                never_checkbox.get_checkbox().element().disabled = search
                has_definition_checkbox.element().disabled = search
            }
            start_button.on(EventType.Click.label, () => {
                const model = new RepetitionModel()

                model.map_id = Number(map_select.get_selected_values()[0] ?? 0)
                model.search_id = search_autocomplete.get_item_id() ?? 0
                model.search_name = search_autocomplete.get_item_id() === 0 ? "" : search_input.get_value()

                model.due = due_checkbox.get_checkbox().is_checked()
                model.not_due = not_due_checkbox.get_checkbox().is_checked()
                model.never = never_checkbox.get_checkbox().is_checked()

                model.has_definition = has_definition_checkbox.is_checked()
                content.clear_html()
                resolve(model)
            })
        })
    }

    async list_term_searches(model) {
        let json = model.to_json()
        return await list_all_entities(
            "dictionary_term_for_review",
            new QueryParams()
                .add_user_id()
                .add("dictionary_map_id", json.map_id)
                .add("dictionary_search_id", json.search_id)
                .add("is_due", json.due)
                .add("is_not_due", json.not_due)
                .add("is_never", json.never)
                .add("has_definition", json.has_definition)
                .build()
        )
    }
}
