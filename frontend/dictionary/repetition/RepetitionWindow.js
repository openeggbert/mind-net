import {VirtualWindow} from "../window/VirtualWindow.js";
import {Form} from "../dom/elements/Form.js";
import {Label} from "../dom/elements/Label.js";
import {Span} from "../dom/elements/Span.js";
import {Select} from "../dom/elements/Select.js";
import {Input} from "../dom/elements/Input.js";
import {Checkbox} from "../dom/elements/Checkbox.js";
import {Button} from "../dom/elements/Button.js";
import {_20PX} from "../styles/Styles.js";
import {list_all_entities, QueryParams} from "../../api.js";
import {Entities} from "../entities/Entities.js";
import {defined} from "../../common.js";
import {showError} from "../../dom.js";
import {Option} from "../dom/elements/Option.js";
import {SearchAutocomplete} from "../search/SearchAutocomplete.js";
import {EventType} from "../dom/attributes/EventType.js";
import {FormRow} from "../search/FormRow.js";
import {Div} from "../dom/elements/Div.js";

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
}

export class RepetitionWindow extends VirtualWindow {
    constructor(get_selected_map_id_callback, render_term_callback) {
        super({
            title: "🔁 Repetition",
            width: screen.width > 1000 ? 1000 : screen.width - 100,
            height: 600
        })
        this.get_selected_map_id_callback = get_selected_map_id_callback
        this.render_term_callback = render_term_callback
        this.now = Date.now()
    }

    async init() {
        let model = null
        while (true) {
            model = await this.ask_user_for_model(model)
            console.log("User confirmed repetition:", model)
            let c = confirm("Continue");
            if (!c) break;
        }

    }

    async ask_user_for_model(model = null) {
        return new Promise(async (resolve, reject) => {

            let content = new Div()
            this.set_content(content.element())

            const repetition_form = new Form()
            content.appendChild(repetition_form.element())

            let maps = await list_all_entities(Entities.dictionary_map, new QueryParams().sort(Entities.dictionary_map.position).build());
            if (!defined(maps)) {
                showError("Loading maps failed.")
                return
            }

            function add_control(label, control) {
                let row = new FormRow(label, control)
                row.styles().paddingTop("10px").end()
                repetition_form.append(row)
            }

            let map_select = new Select()
            map_select.add_option(new Option(0, "Any"))

            maps.forEach(map => {
                let option = new Option(map.id, map.name)
                console.log("map to be enabled: " + map.id + " " + map.name + " " + this.get_selected_map_id_callback())
                let map_id_number = Number(map.id)
                if (map_id_number === (defined(model) ? Number(model.map_id) : Number(this.get_selected_map_id_callback()))) {
                    option.set_selected()
                }
                map_select.add_option(option)
            })
            add_control("Map", map_select)

            let search_input = new Input().set_placeholder("Load a search")
            let search_end_repetition = new Span().set_id("search_end_repetition_" + this.now)

            add_control("Search", new Span(search_input, search_end_repetition))
            let search_autocomplete = new SearchAutocomplete(this, "repetition_" + this.now, search_input.element(), 1, "dictionary_search_fulltext", "&dictionary_map_id=" + map_select.get_selected_values()[0])
            if(defined(model) && model.search_id !== 0 && model.search_name.length > 0)
            {
                console.log("set_from_title(" + model.search_name +")")
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

            let due_checkbox = new ModePair("Due", defined(model) ? model.due : true)
            let not_due_checkbox = new ModePair("Not Due", defined(model) ? model.not_due : false)
            let never_checkbox = new ModePair("Never", defined(model) ? model.never : true)
            let modes = new Span(
                due_checkbox, not_due_checkbox, never_checkbox
            )
            add_control("Mode", modes)

            let has_definition_checkbox = new Checkbox().set_checked(defined(model) ? model.has_definition : true)
            add_control("Has definition", has_definition_checkbox)

            let start_button = new Button("Start").styles().marginTop(_20PX).marginLeft(_20PX).transform("scale(1.5)").end()
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
}
