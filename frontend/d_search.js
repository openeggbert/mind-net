import {Entities} from "./d_entities.js";
import {
    TermStatus, Importance, Difficulty, DictionaryItem, TimeRange, RepetitionMode, Sort, Order,
    enumValue, enumValues, gen_enum_id, find_by_enum_id, humanizeEnumKey
} from "./d_enums.js";
import {Autocomplete, defined} from "./common.js";
import {ActionType, Button, Div, EventType, Form, Input, Label, Span} from "./d_dom.js";
import {_10PX} from "./d_styles.js";
import {showInfo} from "./dom.js";

/*
 * ------------------------------------------------------------------
 * class SearchModel
 * ------------------------------------------------------------------
 * PURPOSE:
 *   - Single source of truth for search state
 *   - NO DOM access
 *   - NO REST calls
 *
 * CONTENT:
 *   - Fields correspond EXACTLY to query_json keys:
 *
 * METHODS:
 *   - toJSON()
 *       -> returns object identical to current query_json
 *   - static fromJSON(json)
 *       -> used when loading saved searches
 *
 * RULE:
 *   - Backend contract MUST NOT change.
*/
export class SearchModel {
    constructor() {
        this.title_contains = "";
        this.title_starts_with = "";
        this.definition_contains = "";
        this.statuses = [];
        this.pinned_only = false;
        this.importance_low = true
        this.importance_medium = true
        this.importance_high = true
        this.difficulty_easy = true
        this.difficulty_medium = true
        this.difficulty_hard = true
        this.tag_id = 0
        this.flag_title = ""
        this.link_from_term_id = 0
        this.link_to_term_id = 0
        this.note_contains = ""
        this.index_id = 0
        this.source_id = 0
        this.alias_alias = ""
        this.missing_items = []
        this.has_items = []
        this.created = TimeRange.Any
        this.updated = TimeRange.Any
        this.visited = TimeRange.Any
        this.reviewed = TimeRange.Any
        this.repetition_due = true
        this.repetition_not_due = false
        this.repetition_never = true
        this.sort = Sort.None
        this.order = Order.None
        // …
    }

    reset() {
        const def = new SearchModel();
        Object.assign(this, def);
    }

    to_json() {
        return {
            title_contains: this.title_contains,
            title_starts_with: this.title_starts_with,
            definition_contains: this.definition_contains,

            statuses: this.statuses.length === 1 && this.statuses[0] === -1 ? [] : this.statuses.map(e => {
                return e.id
            }),
            pinned_only: this.pinned_only,

            importance_low: this.importance_low,
            importance_medium: this.importance_medium,
            importance_high: this.importance_high,

            difficulty_easy: this.difficulty_easy,
            difficulty_medium: this.difficulty_medium,
            difficulty_hard: this.difficulty_hard,

            tag_id: this.tag_id,
            flag_title: this.flag_title,

            link_from_term_id: this.link_from_term_id,
            link_to_term_id: this.link_to_term_id,

            note_contains: this.note_contains,
            index_id: this.index_id,
            source_id: this.source_id,
            alias_alias: this.alias_alias,

            missing_items: this.missing_items.map(e => {
                return e.id
            }),
            has_items: this.has_items.map(e => {
                return e.id
            }),

            created: this.created.id,
            updated: this.updated.id,
            visited: this.visited.id,
            reviewed: this.reviewed.id,

            repetition_due: this.repetition_due,
            repetition_not_due: this.repetition_not_due,
            repetition_never: this.repetition_never,

            sort: this.sort === null ? Sort.None.id : this.sort.id,
            order: this.order === null ? Sort.None.id : this.order.id
        };
    }

    from_json(json) {
        if (!json || typeof json !== "object") {
            this.reset();
            return;
        }

        this.title_contains = json.title_contains ?? "";
        this.title_starts_with = json.title_starts_with ?? "";
        this.definition_contains = json.definition_contains ?? "";

        this.statuses = Array.isArray(json.statuses)
            ? json.statuses
                .map(id => enumValue(TermStatus, id))
                .filter(Boolean)
            : [];

        this.pinned_only = !!json.pinned_only;

        this.importance_low = json.importance_low ?? true;
        this.importance_medium = json.importance_medium ?? true;
        this.importance_high = json.importance_high ?? true;

        this.difficulty_easy = json.difficulty_easy ?? true;
        this.difficulty_medium = json.difficulty_medium ?? true;
        this.difficulty_hard = json.difficulty_hard ?? true;

        this.tag_id = json.tag_id ?? 0;
        this.flag_title = json.flag_title ?? "";

        this.link_from_term_id = json.link_from_term_id ?? 0;
        this.link_to_term_id = json.link_to_term_id ?? 0;

        this.note_contains = json.note_contains ?? "";
        this.index_id = json.index_id ?? 0;
        this.source_id = json.source_id ?? 0;
        this.alias_alias = json.alias_alias ?? "";

        this.missing_items = Array.isArray(json.missing_items)
            ? json.missing_items
                .map(id => enumValue(DictionaryItem, id))
                .filter(Boolean)
            : [];

        this.has_items = Array.isArray(json.has_items)
            ? json.has_items
                .map(id => enumValue(DictionaryItem, id))
                .filter(Boolean)
            : [];

        this.created = enumValue(TimeRange, json.created) ?? TimeRange.Any;
        this.updated = enumValue(TimeRange, json.updated) ?? TimeRange.Any;
        this.visited = enumValue(TimeRange, json.visited) ?? TimeRange.Any;
        this.reviewed = enumValue(TimeRange, json.reviewed) ?? TimeRange.Any;

        this.repetition_due = json.repetition_due ?? true;
        this.repetition_not_due = json.repetition_not_due ?? false;
        this.repetition_never = json.repetition_never ?? true;

        this.sort = json.sort === null ? Sort.None : enumValue(Sort, json.sort)
        this.order = json.order === null ? Order.None : enumValue(Order, json.order);
    }
}

export class CloseButton extends Button {
    constructor(model, input, autocomplete = null) {
        super()

        this
            .set_title("Clear " + model)
            .set_html_unsafe("&times;")
            .styles().marginLeft(_10PX).end()
            .on(EventType.Click.label, (e => {
                event.preventDefault();
                if (defined(autocomplete)) {
                    autocomplete.reset()
                } else {
                    input.clear_value()
                }
            }))
        input._object.insert_after(this)
    }
}

export class SearchAutocomplete extends Autocomplete {
    constructor(win, model, input, input_min_length, entity, query_params = "") {
        super(
            input,
            input_min_length,
            entity,
            query_params,
            "title",
            "title_part",
            "search_end_" + model)

        this.clear_after_click = false
        this.box.style.positon = "fixed"
        this.box.style.willChange = "transform";
        this.win = win

        new CloseButton(model, input, this)
    }

    after_render(box, input) {
        box.style.position = "fixed";
        box.style.left = "0px";
        box.style.top = "0px";
        box.style.willChange = "transform";

        let i = 0
        runWhileShown(box, () => {
            const r = input.getBoundingClientRect();
            const b = box.getBoundingClientRect();
            box.style.transform =
                `translate3d(${r.left}px, ${r.top + 46}px, 0)`;
            // showInfo("win: " + this.win.left() + " " + this.win.top())
            // showInfo("box: " + b.x + " " + b.y)
            let boxY = Number(b.y)
            let winY = this.win.top()

            let wHeight = Number(this.win.height().slice(0, -2))
            let wTop = Number(this.win.top().slice(0, -2))
            let wBottom = wHeight + wTop

            // showInfo(wHeight + " " + wTop + " " + wBottom + " " + this.win.top())
            let over = boxY < (wTop + 40)
            let under = boxY > (wBottom)
            box.style.visibility = over || under ? "hidden" : "visible"
        });
    }


}

function runWhileShown(el, callback) {
    let running = true;

    function loop() {
        if (!running || el.style.display !== "block") return;

        callback();
        requestAnimationFrame(loop);
    }

    requestAnimationFrame(loop);

    return () => running = false; // optional destroy
}

export class SearchForm extends Form {
    #controls = []

    constructor() {
        super();
        this
            .set_id("form_search")
    }

    add_control(control) {
        super.appendChild(control)
        this.#controls.push(control)
        this.add_action_handler(ActionType.Reset, (self, ...args) => {
            let success = true
            this.#controls.forEach(e => {
                let success_e = e.set_default_values()
                if (!success_e) success = false
            })
            return success
        })

    }

    appendChild(child) {
        throw new Error("Function appendChild is not supported. Use add_control instead.")
    }

}

export class FormRow extends Div {
    constructor(label_text, control) {
        super(new Label(label_text), control);
        this.add_class("form-row")
        this.set_id("form-row_" + label_text.toLowerCase())
        this.control = control
        this.add_action_handler(ActionType.Reset, (self, ...args) => {
            self.control.set_default_values();
            return true;
        })

    }

    get_control() {
        return this.control
    }
}

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
    }

    get_title() {
        return this.#autocomplete.get_item() === null ? "" : this.autocomplete.get_item().title
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

