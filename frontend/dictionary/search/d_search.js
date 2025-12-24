import {Entities} from "../entities/d_entities.js";
import {
    TermStatus, Importance, Difficulty, DictionaryItem, TimeRange, RepetitionMode, Sort, Order,
    enumValue, enumValues, gen_enum_id, find_by_enum_id, humanizeEnumKey
} from "../enums/d_enums.js";
import {Autocomplete, defined} from "../../common.js";
import {
    ActionType,
    Button,
    ButtonType,
    Checkbox,
    Div,
    EnumOption,
    EventType, find_dom_element, find_enum,
    Form,
    Input,
    Label, Select,
    Span, Table
} from "../dom/d_dom.js";
import {_10PX, _20PX, _40PX} from "../styles/d_styles.js";
import {
    chooseOption,
    formatDateTime,
    formatDateTimeHM,
    formatDateTimeHMS,
    get_element,
    showError,
    showInfo, showWarn
} from "../../dom.js";
import {VirtualWindow} from "../window/d_window.js";
import {
    delete_entity,
    list_all_entities,
    list_entities,
    post_entity,
    put_entity,
    QueryParams,
    read_entity
} from "../../api.js";
import {BorderCollapse, Color, Cursor, Display, TextAlign, TextDecoration} from "../styles_enums/d_styles_enums.js";
import {showDebug, USER_ID} from "../globals/d_globals.js";

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

/*
* =====================================================================================
* TODO: OOP REFACTOR – SEARCH / ADVANCED SEARCH
* =====================================================================================
*
* CONTEXT:
* --------
* This file currently implements Search and Advanced Search as a large procedural
* block tightly coupled to DOM elements.
*
* The code WORKS and is feature-complete, but responsibilities are mixed:
*   - DOM access
*   - search state
*   - query_json construction
*   - REST execution
*   - result rendering
*   - paging logic
*   - load/save of searches
*
* This is intentional for initial development speed.
* The code is NOT a dead end and is fully refactorable later.
*
*
* CURRENT STRUCTURE (IMPORTANT – DO NOT DELETE):
* ----------------------------------------------
*
* 1) DOM is the implicit state holder
*    - All search values live in input/select/checkbox elements
*    - load_query_json_from_form() reads directly from DOM
*
* 2) query_json is built ad-hoc
*    - load_query_json_from_form()
*    - Must remain backward-compatible with backend
*
* 3) Execution is inline
*    - REST call to list_entities("dictionary_term_search", ...)
*    - paging logic mixed with UI code
*
* 4) Saved searches
*    - dictionary_search table stores query_json
*    - load/save logic is interleaved with UI updates
*
*
* PROBLEMS THIS CREATES:
* ---------------------
* - No explicit Search state object
* - Hard to test or reuse search logic
* - Difficult to add alternative UIs (CLI, preset search, API)
* - Refactoring later without guidance would be risky
*
*
* TARGET OOP DESIGN (EXPLICIT):
* ----------------------------
*
* The refactor MUST introduce the following core classes.
* Names are suggestions; responsibility boundaries are NOT optional.
*
*
* ------------------------------------------------------------------
* class SearchForm
* ------------------------------------------------------------------
* PURPOSE:
*   - Encapsulate ALL DOM access related to search
*   - Translate DOM <-> SearchModel
*
* RESPONSIBILITIES:
*   - readModel(): SearchModel
*   - writeModel(model: SearchModel): void
*   - reset(): void
*
* MUST CONTAIN:
*   - References to input/select/checkbox elements
*
* MUST NOT:
*   - Call REST
*   - Render result tables
*
*
* ------------------------------------------------------------------
* class SearchExecutor
* ------------------------------------------------------------------
* PURPOSE:
*   - Execute search against backend
*
* INPUT:
*   - SearchModel
*   - paging parameters (page, pageSize)
*
* OUTPUT:
*   - { items, total_pages, total_items }
*
* IMPLEMENTATION:
*   - Wraps existing list_entities("dictionary_term_search", ...)
*
* NOTE:
*   - All paging math should live here or in a Pagination class
*
*
* ------------------------------------------------------------------
* OPTIONAL / LATER EXTRACTIONS:
* ------------------------------------------------------------------
*
* class SearchResultTable
*   - render(items)
*   - clear()
*
* class SearchPagination
*   - currentPage
*   - pageSize
*   - totalPages
*   - emits page change events
*
*
* REFACTOR STRATEGY (SAFE AND INCREMENTAL):
* ----------------------------------------
*
* STEP 1 (LOW RISK, HIGH VALUE):
*   - Extract load_query_json_from_form() into SearchModel.toJSON()
*   - Compare old vs new JSON with console diff
*
* STEP 2:
*   - Introduce SearchForm.readModel()
*   - Replace direct DOM reads in search button handler
*
* STEP 3:
*   - Introduce SearchExecutor
*   - Replace inline REST call
*
* STEP 4 (OPTIONAL):
*   - Extract result table rendering
*   - Extract pagination
*
*
* FUTURE DEVELOPER NOTE:
* ---------------------
* You do NOT need to understand the entire file to refactor this.
*
* Start by locating:
*   1) where query_json is built
*   2) where list_entities(...) is called
*   3) where results are rendered
*
* Each of these maps directly to one class above.
*
* This refactor is mechanical, local, and safe.
* =====================================================================================
*/

export class SearchWindow extends VirtualWindow {
    constructor(dictionary_map_id, render_term_callback) {
        super({
            title: "🔍 Advanced Search",
            width: screen.width > 1000 ? 1000 : screen.width - 100,
            height: 600
        })
        this.#init(dictionary_map_id, render_term_callback)
    }
    #init(selected_map_id, render_term_callback) {
        let search_window = this

        const window_content = new Div().styles().height("100%").end()
        search_window.set_content(window_content.element()).show();

        // ---------- FORM ----------

        const search_form = new SearchForm()
        window_content.appendChild(search_form);

        const titleContainsInput = new Input()
            .set_placeholder("e.g. mutex, allocator, RAII")

        search_form.add_control(new FormRow("Title contains", titleContainsInput));

        const titleStartsWithInput = new Input().set_placeholder("e.g. mut, allo, C, K");
        search_form.add_control(new FormRow("Title starts with", titleStartsWithInput));

        const definitionInput = new Input().set_placeholder("e.g. mutex, allocator, RAII")
        search_form.add_control(new FormRow("Definition contains", definitionInput));

        class EnumSelect extends Select {
            constructor(enum_object) {
                super();
                enumValues(enum_object).forEach(e => {
                    this.add_option(new EnumOption(e));
                });
            }
        }

        const statusSelect = new EnumSelect(TermStatus).multiple();

        search_form.add_control(new FormRow("Status", statusSelect));

        const pinnedCheckbox = new Checkbox()
            .add_action_handler(ActionType.Reset, (self, ...args) => {
                self.uncheck();
                return true;
            })
        search_form.add_control(new FormRow("Pinned only", pinnedCheckbox))

        class CheckBoxFormRow extends FormRow {
            constructor(label, enum_object, checked = false, fontSize = "100%") {
                super(label, new Span())
                let span = this.get_control()
                let cb_set = new Set()
                enumValues(enum_object).forEach(enum_value => {
                    //console.debug("Found enum " + JSON.stringify(enum_value))
                    const cb = new Checkbox()
                        .set_value(enum_value.id)
                        .set_checked(checked)
                        .set_id(gen_enum_id(label.toLowerCase(), enum_value))
                        .styles().marginLeft(_10PX).end()

                    const l = new Label("", "auto")
                        .styles().marginLeft(_10PX).marginRight(_10PX).fontSize(fontSize).end()
                        .set_text(enum_value.label)

                    cb_set.add(cb)
                    span.append_many(cb, l);
                    //console.log(span.element().parentElement)
                });
                this.add_action_handler(ActionType.Reset, (self, ...args) => {
                    Array.from(cb_set).forEach(e => {
                        e.set_checked(checked)
                    })
                    return true;
                });

            }
        }

        let importance_control = new CheckBoxFormRow("Importance", Importance, true);
        search_form.add_control(importance_control);
        let difficulty_control = new CheckBoxFormRow("Difficulty", Difficulty, true)
        search_form.add_control(difficulty_control);

        class AcControl extends FormRowAutocomplete {
            constructor(label, model, map_in_query = true) {
                super(
                    search_window,
                    search_form,
                    label,
                    "dictionary_" + model + "_fulltext",
                    map_in_query ? ("&dictionary_map_id=" + selected_map_id) : ""
                )
                this.add_action_handler(ActionType.Reset, (self, ...args) => {
                    self.reset()
                    return true;
                });

            }
        }

        const tag_control = new AcControl("Tag", "tag_type")
        const flag_control = new AcControl("Flag", "flag")
        const link_from_control = new AcControl("Link from", "term")
        const link_to_control = new AcControl("Link to", "term")

        const noteInput = new Input().set_placeholder("e.g. mutex, allocator, RAII")
        search_form.add_control(new FormRow("Note contains", noteInput));

        const index_control = new AcControl("Index", "index_type")
        const source_control = new AcControl("Source", "source_type")
        const alias_control = new AcControl("Alias", "term_alias")

        class CheckBoxItemFormRow extends CheckBoxFormRow {
            constructor(label) {
                super(label, DictionaryItem, false, "80%")
            }
        }

        let missing_control = new CheckBoxItemFormRow("Missing")
        search_form.add_control(missing_control);
        let has_control = new CheckBoxItemFormRow("Has")
        search_form.add_control(has_control);

        class TimeRangeSelect extends EnumSelect {
            constructor() {
                super(TimeRange);
            }
        }

        const createdSelect = new TimeRangeSelect()
        search_form.add_control(new FormRow("Created", createdSelect));

        const updatedSelect = new TimeRangeSelect()
        search_form.add_control(new FormRow("Updated", updatedSelect));

        const visitedSelect = new TimeRangeSelect()
        search_form.add_control(new FormRow("Visited", visitedSelect));

        const reviewedSelect = new TimeRangeSelect()
        search_form.add_control(new FormRow("Reviewed", reviewedSelect));

        let repetition_control = new CheckBoxFormRow("Repetition", RepetitionMode, true)
        search_form.add_control(repetition_control);

        // --- Sort ---
        const sortSelect = new EnumSelect(Sort).styles().width("150px").end()
        const orderSelect = new EnumSelect(Order).styles().width("80px").marginLeft("20px").end()
        let sort_order_span = new Span(sortSelect, orderSelect)
        sort_order_span.add_action_handler(ActionType.Reset, (self, ...args) => {
            let success = true
            self.children().forEach(e => {
                if (!e._object.set_default_values()) success = false
            })
            return success
        })
        search_form.add_control(new FormRow("Sort", sort_order_span));

        // --- Buttons ---
        const buttonRow = new Span();
        search_form.add_control(buttonRow);

        const searchBtn = new Button("🔍 Search")
            .set_type(ButtonType.Button)
            .add_class("save-btn")

        // const winBtn = document.createElement("button");
        // winBtn.type = "button";
        // winBtn.innerText = "win";
        //
        // winBtn.onclick = async e => {
        //     const win = new VirtualWindow({
        //         title: "Advanced Search",
        //         width: 600,
        //         height: 400
        //     });
        //     win.show();
        //     await sleep_for_seconds(3)
        //     win.resize(800, 800)
        // };
        // form.appendChild(winBtn);

        class ProgressBar extends Div {
            #fill;
            #loading_data_progress;
            #start_time = Date()

            constructor() {
                super();

                this.add_class("progress");

                this.#fill = new Div()
                    .add_class("progress-fill");

                this.append(this.#fill);

                this.#loading_data_progress = new Span().set_text("0%")
                this.styles()
                    .background("#ddd").padding("0").textAlign(TextAlign.Center)
                    .end()
                let progress_row = new Div()
                    .styles().margin(_10PX).marginTop("-40px").marginBottom(_20PX).end()
                    .append_many(new Span().set_text("Loading data: "), this.#loading_data_progress)
                this.appendChild(progress_row)
            }

            set_progress(value) {
                value = Math.max(0, Math.min(100, value));

                let now = Date.now()
                let elapsed_seconds = (now - this.#start_time) / 1000
                let remains_seconds = Number(elapsed_seconds / (Number(value) / (100.0 - value))).toFixed()
                this.#fill.element().style.width = value + "%";
                this.#loading_data_progress.set_text(value + "%" + " Remains: " + remains_seconds + " second" + (remains_seconds > 1.0 ? "s" : ""))
                return this;
            }

            reset() {
                this.set_progress(0)
                this.#start_time = Date.now()
                return this;
            }
        }

        let progress_bar = new ProgressBar()
            .hide()

        searchBtn.on(EventType.Click.label, async () => {
            let query_json = load_query_model_from_form().to_json()

            console.log("Advanced search values:", {
                query_json: query_json
            });

            let page_number = 5
            try {
                page_number = Number(input_page_number.get_value())
            } catch {

            }

            let page_size = find_dom_element("page_size_select").element().value

            let list_term_searches = await list_entities(
                "dictionary_term_search",
                new QueryParams()
                    .add("dictionary_map_id", selected_map_id)
                    .add("title", JSON.stringify(query_json))
                    .build(),
                page_number,
                page_size
            )
            // alert(JSON.stringify(query_json))
            if (!defined(list_term_searches)) {
                showError("Listing search results failed.")
                return
            }
            showInfo("Finished: list_term_searches")
            // alert(JSON.stringify(list_term_searches))
            let total_items = list_term_searches.total_items
            let total_pages = list_term_searches.total_pages

            find_dom_element("span_pages_toolbar").show(Display.Block)
            find_dom_element("span_total_pages_count").set_text(total_pages)
            find_dom_element("span_total_count_count").set_text(total_items)
            let items = list_term_searches.items
            if (items.length === 0) {
                showInfo("No search results.")
            }
            progress_bar.reset().show()
            let resultTable = find_dom_element("resultTable")
                .clear_html()
                .hide()
                .styles().marginBottom(_40PX).end()
            let space = get_element("space");
            if (defined(space)) find_dom_element("space").remove_element()

            let tr_th = document.createElement("tr")
            resultTable.appendChild(tr_th)

            function create_th(text) {
                let th = document.createElement("th")
                th.innerText = text
                th.style.border = "1px solid black"
                th.style.backgroundColor = "#aaa"
                th.style.height = _40PX
                return th
            }

            function create_td(text) {
                let td = document.createElement("td");
                td.style.border = "1px solid black"
                td.innerText = text
                td.style.paddingLeft = _10PX
                td.style.height = _40PX
                return td
            }

            let details = items.length === 0 ? false : find_dom_element("details_checkbox").is_checked()
            let th_hash = create_th("#")
            let th_id = create_th("ID")
            let th_title = create_th("Title")
            let th_disambiguation = create_th("Disambiguation")
            tr_th.appendChild(th_hash)
            tr_th.appendChild(th_id)
            tr_th.appendChild(th_title)
            tr_th.appendChild(th_disambiguation)
            th_hash.style.width = "50px"
            th_id.style.width = "50px"
            th_title.style.minWidth = "200px"
            th_disambiguation.style.minWidth = "100px"
            let term_map = new Map()
            let state_map = new Map()

            let repetition_all =
                find_by_enum_id("repetition", RepetitionMode.Due).checked === true &&
                find_by_enum_id("repetition", RepetitionMode.NotDue).checked === true &&
                find_by_enum_id("repetition", RepetitionMode.Never).checked === true
            if (details) {
                function append_th(text) {
                    let th = create_th(text)
                    tr_th.appendChild(th)
                }

                append_th("Created at")
                append_th("Updated at")
                append_th("Status")
                append_th("Importance")
                append_th("Difficulty")
                //if(!repetition_all)
                append_th("Next review")
            }
            const item_count = items.length
            let done = 0.0
            for (const item of items) {

                let term_id = item.id
                let read_term = await read_entity(Entities.dictionary_term, term_id)
                if (!defined) {
                    showError("Reading term failed.")
                    continue
                }
                term_map.set(term_id, read_term)

                let progress = Number((done + (details ? 0.5 : 1.0)) / item_count * 100).toFixed()
                progress_bar.set_progress(progress)
                if (details) {
                    let list_states = await list_all_entities(
                        Entities.dictionary_state_18,
                        new QueryParams()
                            .add(Entities.dictionary_state_18.dictionary_term_id, term_id)
                            .add_user_id()
                            .build()
                    )
                    if (defined(list_states) && list_states.length > 0) state_map.set(term_id, list_states[0])
                }
                progress = Number((done + (details ? 0.5 : 1.0)) / item_count * 100).toFixed()
                progress_bar.set_progress(progress)

                document
                    .getElementById("span_pages_toolbar")
                    .scrollIntoView({behavior: "smooth"});
                done++
            }

            let number = 0;
            if (items.length === 0) {
                let tr = document.createElement("tr")
                tr.onmouseover = (e) => tr.style.backgroundColor = "rgba(101,181,237,0.34)"
                tr.onmouseleave = (e) => tr.style.backgroundColor = "white"
                resultTable.appendChild(tr)

                tr_th.remove()
                resultTable.styles().border().end()

                let td = create_td("No results found");
                td.colSpan = 4
                td.style.textAlign = "left"
                td.style.color = "grey"
                td.style.border = ""
                tr.appendChild(td)
            } else items.forEach(e => {
                ++number
                let tr = document.createElement("tr")
                tr.onmouseover = (e) => tr.style.backgroundColor = "rgba(101,181,237,0.34)"
                tr.onmouseleave = (e) => tr.style.backgroundColor = "white"
                resultTable.appendChild(tr)
                let dictionary_term_id = e.dictionary_term_id
                let title = e.title
                let disambiguation = e.disambiguation

                let td_title = create_td(title);
                td_title.innerText = ""
                let a = document.createElement("a")
                a.href = "#"
                a.title = title
                a.innerText = title
                td_title.appendChild(a)

                td_title.onclick = (async e => {
                    await render_term_callback(dictionary_term_id)
                })
                td_title.style.cursor = "pointer"
                tr.appendChild(create_td(number))
                tr.appendChild(create_td(dictionary_term_id))
                tr.appendChild(td_title)
                tr.appendChild(create_td(disambiguation))
                if (details) {
                    function append_td(text) {
                        let td = create_td(text)
                        td.innerText = text
                        tr.appendChild(td)
                    }

                    let term = term_map.has(dictionary_term_id) ? term_map.get(dictionary_term_id) : null
                    if (defined(term)) {
                        append_td(formatDateTimeHMS(term.created_at))
                        append_td(formatDateTimeHMS(term.updated_at))
                        append_td(humanizeEnumKey(enumValue(TermStatus, term.status).label))
                        append_td(enumValue(Importance, term.importance).label)
                        append_td(enumValue(Difficulty, term.difficulty).label)
                        //if(!repetition_all)
                        {
                            if (state_map.has(dictionary_term_id)) {
                                append_td(formatDateTimeHM(state_map.get(dictionary_term_id).next_review))

                            } else {
                                append_td("---")
                            }
                        }
                    }

                }
            })

            resultTable.show(Display.Block)
            progress_bar.hide()

        });
        buttonRow.appendChild(searchBtn);

        function make_button_obsolete(text) {
            const button = document.createElement("button");
            button.type = "button";
            button.innerText = text;
            button.style.height = "40px"
            button.style.marginLeft = "20px"
            return button
        }

        class SearchButton extends Button {
            constructor(text) {
                super(text);
                this
                    .set_type(ButtonType.Button)
                    .styles().height("40px").marginLeft("20px").end()
            }
        }

        const resetBtn = new SearchButton("♻ Reset")

        resetBtn.element().onclick = () => {
            search_form.set_default_values()
            showInfo("Set to default values")
        }
        buttonRow.appendChild(resetBtn);

        let search_json = null

        function load_query_model_from_form() {
            const m = new SearchModel();

            m.title_contains = titleContainsInput.get_value()
            m.title_starts_with = titleStartsWithInput.get_value()
            m.definition_contains = definitionInput.get_value()
            m.statuses = Array
                .from(statusSelect.selectedOptions())
                .map(opt => {
                        let result = enumValue(TermStatus, opt.value)
                        return result
                    }
                )
            m.pinned_only = pinnedCheckbox.is_checked()
            m.importance_low = find_by_enum_id("importance", Importance.Low)._object.is_checked()
            m.importance_medium = find_by_enum_id("importance", Importance.Medium)._object.is_checked()
            m.importance_high = find_by_enum_id("importance", Importance.High)._object.is_checked()

            m.difficulty_easy = find_by_enum_id("difficulty", Difficulty.Easy)._object.is_checked()
            m.difficulty_medium = find_by_enum_id("difficulty", Difficulty.Medium)._object.is_checked()
            m.difficulty_hard = find_by_enum_id("difficulty", Difficulty.Hard)._object.is_checked()

            m.tag_id = tag_control.get_item_id()
            m.flag_title = flag_control.get_title()
            m.link_from_term_id = link_from_control.get_item_id()
            m.link_to_term_id = link_to_control.get_item_id()
            m.note_contains = noteInput.get_value()
            m.index_id = index_control.get_item_id()
            m.source_id = source_control.get_item_id()
            m.alias_alias = alias_control.get_title()
            m.missing_items = enumValues(DictionaryItem)
                .filter(e => find_by_enum_id("missing", e)._object.is_checked())
            m.has_items = enumValues(DictionaryItem)
                .filter(e => find_by_enum_id("has", e)._object.is_checked())
            m.created = Array
                .from(createdSelect.selectedOptions())
                .map(opt => opt.value)
                .map(e => enumValue(TimeRange, e))[0]
            m.updated = Array
                .from(updatedSelect.selectedOptions())
                .map(opt => opt.value)
                .map(e => enumValue(TimeRange, e))[0]
            m.visited = Array
                .from(visitedSelect.selectedOptions())
                .map(opt => opt.value)
                .map(e => enumValue(TimeRange, e))[0]
            m.reviewed = Array
                .from(reviewedSelect.selectedOptions())
                .map(opt => opt.value)
                .map(e => enumValue(TimeRange, e))[0]
            m.repetition_due = find_by_enum_id("repetition", RepetitionMode.Due)._object.is_checked()
            m.repetition_not_due = find_by_enum_id("repetition", RepetitionMode.NotDue)._object.is_checked()
            m.repetition_never = find_by_enum_id("repetition", RepetitionMode.Never)._object.is_checked()

            m.sort = Array
                .from(sortSelect.selectedOptions())
                .map(opt => enumValue(Sort, opt.value))[0]
            m.order = Array
                .from(orderSelect.selectedOptions())
                .map(opt => enumValue(Order, opt.value))[0]

            return m
        }

        const saveBtn = make_button_obsolete("💾 Save");
        saveBtn.onclick = async () => {
            let search_already_exists = search_json !== null
            let name = null
            if (!search_already_exists) {
                name = prompt("Enter search name");
                if (name === null || name === undefined || name === "") name = formatDateTime(new Date(), true, true, true, false);
            } else {
                name = search_json.name
            }

            let query_json = load_query_model_from_form().to_json();

            // alert(JSON.stringify(query_json, null, 2))

            let is_public = false;

            if (search_already_exists) {
                is_public = search_json.is_public === 1
            } else {
                let is_public_option = await chooseOption(["Public", "Private"]);
                is_public = is_public_option === null || is_public_option === undefined ? false : is_public_option === "Public"
            }

            if (!search_already_exists) {
                let new_search = {
                    user_id: USER_ID,
                    dictionary_map_id: this.get_selected_map_id(),
                    name: name,
                    query_json: JSON.stringify(query_json),
                    is_public: is_public ? 1 : 0
                }
                let new_search_created = await post_entity(Entities.dictionary_search, new_search)
                if (!defined(new_search_created)) {
                    showError("Creating new search failed.")
                    return
                } else {
                    showInfo("New search was successfully created.")
                    search_json = new_search_created
                    find_dom_element("span_search_id_value").set_text(new_search_created.id)
                    unloadBtn.disabled = ""
                    load_input.disabled = ""
                    deleteBtn.disabled = ""

                }
            }
            if (search_already_exists) {
                search_json.query_json = JSON.stringify(query_json)

                let search_updated = await put_entity(Entities.dictionary_search, search_json.id, search_json)
                if (!defined(search_updated)) {
                    showError("Updating search failed.")
                    return
                } else {
                    showInfo("Search was successfully updated.")
                }
            }
        }
        buttonRow.appendChild(saveBtn);

        const unloadBtn = make_button_obsolete("📂 Unload");
        unloadBtn.disabled = true
        unloadBtn.onclick = () => {
            let loaded = search_json !== null
            if (!loaded) return
            search_json = null

            unloadBtn.disabled = true
            load_input.disabled = false
            deleteBtn.disabled = true
            find_dom_element("span_search_id_value").clear_text()
        }
        buttonRow.appendChild(unloadBtn);

        const load_input = new Input()
            .set_id("load_input")
            .set_placeholder("Load a search")
            .css({
                marginLeft: "10px",
                width: "150px"
            })

        buttonRow.appendChild(load_input)
        buttonRow.appendChild(new Span().set_id("search_end_search"))

        let search_autocomplete = new SearchAutocomplete(
            search_window,
            "search",
            load_input.element(),
            1,
            "dictionary_search_fulltext",
            "&dictionary_map_id=" + selected_map_id
        )

        search_autocomplete.addCallback(async e => {
            let old_search_id = search_json === null ? 0 : search_json.id
            let new_search_id = search_autocomplete.get_item_id()
            showDebug("old_search_id=" + old_search_id)
            showDebug("new_search_id=" + new_search_id)
            showDebug(JSON.stringify(search_autocomplete.get_item()))
            search_form.element().classList.add("loading");
            let read_search = await read_entity(Entities.dictionary_search, new_search_id)
            if (!defined(read_search)) {
                showError("Reading search failed: " + new_search_id)
                return
            }
            resetBtn.click()
            let query = JSON.parse(read_search.query_json)
            titleContainsInput.set_value(query.title_contains ?? "")
            titleStartsWithInput.set_value(query.title_starts_with ?? "")
            definitionInput.set_value(query.definition_contains ?? "")
            statusSelect.set_selected_values(query.statuses)
            pinnedCheckbox.set_checked(query.pinned_only ?? false)

            find_enum("importance", Importance.Low).set_checked(query.importance_low ?? true)
            find_enum("importance", Importance.Medium).set_checked(query.importance_medium ?? true)
            find_enum("importance", Importance.High).set_checked(query.importance_high ?? true)

            find_enum("difficulty", Difficulty.Easy).set_checked(query.difficulty_easy ?? true)
            find_enum("difficulty", Difficulty.Medium).set_checked(query.difficulty_medium ?? true)
            find_enum("difficulty", Difficulty.Hard).set_checked(query.difficulty_hard ?? true)

            if ((query.tag_id ?? 0) !== 0) {
                let read_tag = await read_entity(Entities.dictionary_tag, query.tag_id)
                if (!defined(read_tag)) {
                    showError("Reading tag failed: " + query.tag_id)
                } else {
                    let read_tag_type = await read_entity(Entities.dictionary_tag_type, read_tag.dictionary_tag_type_id)
                    if (!defined(read_tag_type)) {
                        showError("Reading tag type failed: " + read_tag.dictionary_tag_type_id)
                    } else {
                        await tag_control.set_from_title(read_tag_type.title, query.tag_id)
                    }
                }
            }

            if ((query.flag_title ?? "") !== "") {
                await flag_control.set_from_title(query.flag_title)
            }
            if ((query.link_from_term_id ?? 0) !== 0) {
                let read_term = await read_entity(Entities.dictionary_term, query.link_from_term_id)
                if (!defined(read_term)) {
                    showError("Reading term failed: " + query.link_from_term_id)
                } else {
                    await link_from_control.set_from_title(read_term.title, query.link_from_term_id)
                }
            }
            if ((query.link_to_term_id ?? 0) !== 0) {
                let read_term = await read_entity(Entities.dictionary_term, query.link_to_term_id)
                if (!defined(read_term)) {
                    showError("Reading term failed: " + query.link_to_term_id)
                } else {
                    await link_to_control.set_from_title(read_term.title, query.link_to_term_id)
                }
            }
            noteInput.set_value(query.note_contains ?? "")

            if ((query.index_id ?? 0) !== 0) {
                let read_index = await read_entity(Entities.dictionary_index, query.index_id)
                if (!defined(read_index)) {
                    showError("Reading index failed: " + query.index_id)
                } else {
                    let read_index_type = await read_entity(Entities.dictionary_index_type, read_index.dictionary_index_type_id)
                    if (!defined(read_index_type)) {
                        showError("Reading index type failed: " + read_index_type.dictionary_index_type_id)
                    } else {
                        await index_control.set_from_title(read_index_type.title, query.index_id)
                    }
                }
            }

            if ((query.source_id ?? 0) !== 0) {
                let read_source = await read_entity(Entities.dictionary_source, query.source_id)
                if (!defined(read_source)) {
                    showError("Reading source failed: " + query.source_id)
                } else {
                    let read_source_type = await read_entity(Entities.dictionary_source_type, read_source.dictionary_source_type_id)
                    if (!defined(read_source_type)) {
                        showError("Reading source type failed: " + read_source_type.dictionary_source_type_id)
                    } else {
                        await source_control.set_from_title(read_source_type.title, query.source_id)
                    }
                }
            }

            if ((query.alias_alias ?? "") !== "") {
                await alias_control.set_from_title(query.alias_alias)
            }
            console.debug(JSON.stringify(query))
            console.debug("query.missing=" + query.missing_items);

            (query.missing_items ?? [])
                .forEach(e => {
                    find_enum("missing", e).check()
                });

            (query.has_items ?? [])
                .forEach(e => {
                    find_enum("has", e).check()
                });

            createdSelect.set_selected_value(query.created)
            updatedSelect.set_selected_value(query.updated)
            visitedSelect.set_selected_value(query.visited)
            reviewedSelect.set_selected_value(query.reviewed)

            find_enum("repetition", RepetitionMode.Due).set_checked(query.repetition_due ?? true)
            find_enum("repetition", RepetitionMode.NotDue).set_checked(query.repetition_not_due ?? true)
            find_enum("repetition", RepetitionMode.Never).set_checked(query.repetition_never ?? true)

            sortSelect.set_selected_value(query.sort)
            orderSelect.set_selected_value(query.order)

            search_form.remove_class("loading")
            console.debug(JSON.stringify(read_search))
            console.debug(JSON.stringify(JSON.parse(read_search.query_json)))
            deleteBtn.disabled = false
            unloadBtn.disabled = false
            search_json = read_search
            find_dom_element("span_search_id_value").set_text(read_search.id)
        })

        const deleteBtn = make_button_obsolete("🗑 Delete");
        deleteBtn.disabled = true
        deleteBtn.onclick = async () => {
            if (search_json === null) {
                // nothing to do
                return
            }
            let deleted = await delete_entity(Entities.dictionary_search, search_json.id)
            if (!defined(deleted)) {
                showError("Deleting search failed: " + search_json.id + " " + search_json.name)
                return
            }
            search_json = null
            deleteBtn.disabled = true
            unloadBtn.disabled = true
            find_dom_element("span_search_id_value").clear_text()
        }
        buttonRow.appendChild(deleteBtn);


        let space = new Div().set_id("space").css({height: "50px"})

        // ---------- FINAL ----------

        let page_size_label = new Label("Items per page")
            .styles()
            .display(Display.Inline)
            .marginRight(_10PX)
            .marginLeft(_10PX)
            .end()

        let page_size_select = new Select()
            .set_id("page_size_select")
            .styles().display(Display.Inline).width("100px").end()
            .add_options(
                new Option("5", "5"),
                new Option("10", "10"),
                new Option("20", "20"),
                new Option("50", "50"),
                new Option("100", "100"))
            .set_selected_index(1)

        window_content.appendChild(new Span(page_size_label, page_size_select).element())

        let details_label = new Label("Details")
            .styles()
            .display(Display.Inline)
            .marginRight(_10PX)
            .marginLeft(_10PX)
            .end()
        let details_checkbox = new Checkbox()
            .set_id("details_checkbox")
            .set_checked(screen.width > 1200)
            .styles()
            .display(Display.Inline)
            .transform("scale(2)")
            .end()

        window_content.appendChild(new Span(details_label, details_checkbox).element())

        window_content.appendChild(space.element())
        let resultTable = new Table()
            .set_id("resultTable")
            .styles()
            .display(Display.None)
            .margin(_10PX)
            .borderCollapse(BorderCollapse.Collapse)
            .end()
        window_content.appendChild(resultTable)
        window_content.appendChild(progress_bar.element())

        let span_pages_toolbar = new Span()
            .set_id("span_pages_toolbar")
            .hide()

        let button_first_page = document.createElement("button")
        let button_prev_page = document.createElement("button")
        let button_next_page = document.createElement("button")
        let button_last_page = document.createElement("button")
        button_first_page.innerText = "« First"
        button_prev_page.innerText = "‹ Prev"
        button_next_page.innerText = "Next ›"
        button_last_page.innerText = "Last »"
        button_first_page.style.margin = "5px"
        button_prev_page.style.margin = "5px"
        button_next_page.style.margin = "5px"
        button_last_page.style.margin = "5px"

        let input_page_number = new Input()
            .set_value(1)
            .styles()
            .width("50px")
            .marginLeft("10px")
            .marginRight("10px")
            .end()

        let go_page_button = document.createElement("button")
        go_page_button.innerText = "Go"

        input_page_number.on("keydown", (e) => {
            if (e.key === "Enter") {
                e.preventDefault();
                searchBtn.click()
            }
        });

        go_page_button.onclick = e => {
            searchBtn.click()
        }
        span_pages_toolbar.append_many(
            button_first_page,
            button_prev_page,
            input_page_number,
            go_page_button,
            button_next_page,
            button_last_page)

        let span_total = new Span()
            .set_id("span_total_pages")
            .styles().color(Color.Gray).marginLeft(_10PX).end()

        span_pages_toolbar.appendChild(span_total)

        let span_total_pages_text = new Span().set_text("Total pages: ")
        span_total.appendChild(span_total_pages_text)

        let span_total_pages_count = new Span().set_id("span_total_pages_count")
        span_total.appendChild(span_total_pages_count)

        let span_total_count_text = new Span()
            .set_text("Total count: ")
            .styles().marginLeft("10px").end()

        span_total.appendChild(span_total_count_text)

        let span_total_count_count = new Span().set_id("span_total_count_count")
        span_total.appendChild(span_total_count_count)

        let span_search_id_text = new Span()
            .set_text("Search ID: ").styles().marginLeft(_10PX).end()

        span_total.appendChild(span_search_id_text)

        let span_search_id_value = new Span()
            .set_id("span_search_id_value")
            .styles()
            .color(Color.Blue)
            .textDecoration(TextDecoration.Underline)
            .cursor(Cursor.Pointer)
            .end()
            .on("click", e => {
                // Copy the text inside the text field
                navigator.clipboard.writeText(span_search_id_value.get_text());
                showInfo("Search ID " + span_search_id_value.get_text() + " was copied to clipboard.")
            })

        span_total.appendChild(span_search_id_value)

        button_first_page.onclick = (e => {
            let current_page_number = input_page_number.get_value()
            if (current_page_number === "1") {
                showWarn("This is already the first page");
                return
            }
            input_page_number.set_value(1);
            go_page_button.click()
        })
        button_prev_page.onclick = (e => {
            let current_page_number = input_page_number.get_value()
            if (current_page_number === "1") {
                showWarn("This is the first page");
                return
            }
            input_page_number.set_value(current_page_number - 1)
            go_page_button.click()
        })
        button_next_page.onclick = (e => {
            let current_page_number = input_page_number.get_value_as_number()
            let last_page_number = span_total_pages_count.get_text_as_number()

            if (current_page_number >= last_page_number) {
                showWarn("This is the last page");
                return
            }
            input_page_number.set_value(String(current_page_number + 1))
            go_page_button.click()
        })
        button_last_page.onclick = (e => {
            let last_page = span_total_pages_count.get_text()
            let current_page_number = input_page_number.get_value()
            if (current_page_number === last_page) {
                showWarn("This is already the last page");
                return
            }
            input_page_number.set_value(last_page)
            go_page_button.click()
        })
        window_content.appendChild(span_pages_toolbar.element())

        window_content.style.padding = "5px"
    }
}