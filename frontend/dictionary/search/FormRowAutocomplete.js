import {FormRow} from "./FormRow.js";
import {Span} from "../dom/elements/Span.js";
import {Div} from "../dom/elements/Div.js";
import {SearchAutocomplete} from "./SearchAutocomplete.js";
import {Input} from "../dom/elements/Input.js";

export class FormRowAutocomplete extends FormRow {
    static #create_container(label, input) {
        return new Span(
            input,
            new Div()
                .set_id("search_end_" + label.toLowerCase())
        )
            .css({
                position: "relative"
            })
    }

    #autocomplete

    constructor(win, search_form, label, entity, query_params = "", input = new Input()) {
        super(
            label,
            FormRowAutocomplete.#create_container(label, input)
        );
        this.input = input
        search_form.add_control(this)
        this.#autocomplete = new SearchAutocomplete(
            win,
            label.toLowerCase(),
            input.element(),
            1,
            entity,
            query_params)

        if (label === "Flags") alert("this.#autocomplete=" + this.#autocomplete)
    }

    set_query_params(query_params) {
        this.#autocomplete.set_query_params(query_params)
    }
    get_title() {
        return this.#autocomplete.get_item() === null ? "" : this.#autocomplete.get_item().title
    }

    reset() {
        this.#autocomplete.reset()
    }

    get_item_id() {
        return this.#autocomplete.get_item_id()
    }

    async set_from_title(title, id) {
        await this.#autocomplete.set_from_title(title, id)
    }

}