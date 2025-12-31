import {showWindowFromUrl, VirtualWindow} from "../../window/VirtualWindow.js";
import {Div} from "../../dom/elements/Div.js";
import {Span} from "../../dom/elements/Span.js";
import {Table} from "../../dom/elements/Table.js";
import {Tr} from "../../dom/elements/Tr.js";
import {Th} from "../../dom/elements/Th.js";
import {Td} from "../../dom/elements/Td.js";
import {
    getTitleCache,
    list_all_entities,
    list_entities,
    QueryParams,
    read_entity,
    setTitleCache
} from "../../../api.js";
import {formatDateTimeHMS, showError, showSuccess, showWarn} from "../../../dom.js";
import {BorderCollapse} from "../../styles/properties/BorderCollapse.js";
import {TextAlign} from "../../styles/properties/TextAlign.js";
import {_10PX, _20PX, _40PX} from "../../styles/Styles.js";
import {EventType} from "../../dom/attributes/EventType.js";
import {set_next_visit_source, translate, USER_ID} from "../../globals/Globals.js";
import {Entities} from "../../entities/Entities.js";
import {null_or_undefined} from "../../../common.js";
import {VisitSource} from "../../enums/VisitSource.js";
import {enumValue} from "../../enums/EnumFunctions.js";
import {Button} from "../../dom/elements/Button.js";
import {A} from "../../dom/elements/A.js";
import {HoverTr} from "./HoverTr.js";
import {Input} from "../../dom/elements/Input.js";

async function find_title(term_id) {
    let title = getTitleCache(Entities.dictionary_term, term_id)
    if (title === null || title === undefined) {
        let x = term_id
        let term_ = await read_entity(Entities.dictionary_term, x)
        if (term_ === null) {
            showWarn(translate("dictionary.term.container.error.reading_term_failed") + x);
            title = "??? (# " + x + ")"
        } else {
            title = term_.title
            setTitleCache(Entities.dictionary_term, x, title);
        }
    }
    return title
}

class VisitTrHeader extends Tr {
    constructor() {
        super();

        let th_number = new Th("#")
        let th_id = new Th(translate("dictionary.common.term_id"));
        th_id.data_i18n("dictionary.common.term_id")

        let th_title = new Th(translate("dictionary.common.title"));
        th_title.data_i18n("dictionary.common.title")

        let th_map = new Th("Map");
        let th_source = new Th("Source");

        let th_timestamp = new Th(translate("dictionary.common.timestamp"))
        th_timestamp.data_i18n("dictionary.common.timestamp")

        const th_array = [th_number,th_id,th_title,th_map,th_source,th_timestamp]
        th_array.forEach(e=> this.appendChild(e))

        for (const el of th_array) {
            el
                .styles()
                .minWidth(_20PX)
                .padding(_10PX)
                .border("1px solid black")
                .backgroundColor("#ccc")
                .end()
        }
        th_title.styles().minWidth("200px").end()
    }
}

class VisitedTd extends Td {
    constructor(text = "") {
        super(text);
        this.styles().padding(_10PX).border("1px solid black").end()
    }
}

class VisitRow extends HoverTr {
    constructor(
        history_entry_number,
        visited_term_id,
        title,
        map,
        source,
        timestamp
    ) {
        super();
        this.appendChild(new VisitedTd(history_entry_number))
        this.appendChild(new VisitedTd(visited_term_id))

        let td_title = new VisitedTd()
        td_title.appendChild(title)

        this.appendChild(td_title)
        this.appendChild(new VisitedTd(map))
        this.appendChild(new VisitedTd(source))
        this.appendChild(new VisitedTd(timestamp))
    }
}
const PAGE_SIZE = 50
export class TermVisitHistoryWindow extends VirtualWindow {
    #init_done = false
    #render_term_id_callback = null
    #table
    #rows = new Set()
    #page_number = 1
    #page_count = 0

    constructor(render_term_id_callback) {
        super({
                title: translate("dictionary.term.container.visit_window.term_visit_history"),
                width: 1000,
                height: 600,
            }
        );
        this.#render_term_id_callback = render_term_id_callback
        //
        this.set_content_padding(_10PX)
        let content = new Div()

        function make_button(text) {
            return new Button(text).styles().margin(_10PX).textAlign(TextAlign.Center).end();
        }
        let refresh_button = make_button("Refresh")
        this.newest_button = make_button("Newest")
        this.newest_button.element().disabled = true

        this.newer_button = make_button("Newer")
        this.newer_button.element().disabled = true

        let older_button = make_button("Older")
        let oldest_button = make_button("Oldest")
        this.page_input = new Input().styles().width("50px").marginRight(_10PX).end()
        let go_to_page_button = new Button("Go to page")
            .on(
                EventType.Click,
            ()=>{
                    let pn = 0;
                    try {
                        pn = Number(this.page_input.get_value())
                    } catch(e) {
                            return;
                    }
                    this.#page_number = pn
                this.#render_page()
                }
            )

        refresh_button.on(EventType.Click, ()=>{
            this.#render_page()
            showSuccess("Refreshed")
        })

        this.newest_button.on(EventType.Click, ()=> {
            this.#page_number = 1
            this.#render_page()
        })
        this.newer_button.on(EventType.Click, ()=> {
            if(this.#page_number === 1) return
            this.#page_number--
            this.#render_page()
        })
        older_button.on(EventType.Click, ()=> {
            this.#page_number++
            this.#render_page()
        })
        oldest_button.on(EventType.Click, ()=> {
            if(this.#page_count === 0) return
            if(this.#page_count === this.#page_number) return
            this.#page_number = this.#page_count
            this.#render_page()
        })

        content.append_many(new Div(
            refresh_button,
            this.newest_button,
            this.newer_button,
            older_button,
            oldest_button
            ).styles().margin(_10PX).textAlign(TextAlign.Center).end()
        )

        this.page_number = new Span("1").styles().marginRight(_20PX).end()
        this.total_pages = new Span("?").styles().marginRight(_20PX).end()
        this.total_items = new Span("?").styles().marginRight(_20PX).end()
        this.page_toolbar = new Div(
            // new Span("Page number: ").styles().fontWeight("bold").end(),this.page_number,
            // new Span(" Total items: ").styles().fontWeight("bold").end(), this.total_items,
            this.page_input.styles().margin(_10PX).textAlign(TextAlign.Center).end(),
            go_to_page_button.styles().margin(_10PX).textAlign(TextAlign.Center).end(),
            new Span(" Total pages: ").styles().fontWeight("bold").end(),this.total_pages,
        ).styles().margin(_10PX).textAlign(TextAlign.Center).end().hide()

        content.appendChild(this.page_toolbar)

            this.#table = new Table()
            .styles()
            .borderCollapse(BorderCollapse.Collapse)
            .margin("0 auto")
            .marginBottom(_40PX)
            .end()
        content.appendChild(this.#table);
        this.set_content(content)

        this.#table.appendChild(new VisitTrHeader());
    }
    add_row(row) {
        this.#table.appendChild(row)
        this.#rows.add(row)
    }
    clear_rows() {
        this.#rows.forEach(e=> e.remove_element())
        this.#rows.clear()
    }

    async list_visits(page_number = 1, page_size = PAGE_SIZE) {
        return await list_entities(
            "dictionary_term_visit_view",
            new QueryParams()
                .add("user_id", USER_ID)
                .sort("created_at", "desc")
                .build(),
            page_number,
            page_size
        )
    }
    async list_maps() {
        return await list_all_entities(
            "dictionary_map"
        )
    }
    async init() {
        if (this.#init_done) return false
        this.#init_done = true

        this.maps = new Map()
        let maps_ = await this.list_maps()
        if(maps_) {
            maps_.forEach(e=>this.maps.set(e.id, e))
        } else {
            showWarn("Loading list of maps failed.")
        }

     await this.#render_page()
    }
    async #render_page() {
        let visits_result = await this.list_visits(this.#page_number);

        this.clear_rows()
        if (null_or_undefined(visits_result)) {
            showError(translate("dictionary.term.container.visit_window.error.loading_visits_failed"))
            this.page_toolbar.hide()
            this.#page_count = 0
            return;
        }
        // alert(JSON.stringify(visits_result))

        let visits = visits_result.items
        if(visits.length === 0 && this.#page_number > 1) {
            this.#page_number--;
            showWarn("No older history")
            await this.#render_page()
            return
        }

        this.newer_button.element().disabled = this.#page_number === 1
        this.newest_button.element().disabled = this.#page_number === 1
        this.page_toolbar.show()
        this.page_number.set_text(this.#page_number)
        this.page_input.set_value(this.#page_number)
        this.total_pages.set_text(visits_result.total_pages)
        this.total_items.set_text(visits_result.total_items)
        this.#page_count = visits_result.total_pages


        let history_entry_number = (this.#page_number -1) * PAGE_SIZE;
        for (const entry of visits.slice()) {
            history_entry_number++

            let visited_term_id = entry.dictionary_term_id

            let title_ = await find_title(visited_term_id)
            let disambiguation = entry.disambiguation
            if (disambiguation.length > 0) {
                title_ = title_ + " (" + disambiguation + ")"
            }
            let a = new A(title_, "?")
            let callback = async (term_id) => await this.#render_term_id_callback(term_id, true)
            a.on(EventType.Click, async function () {
                event.preventDefault();
                set_next_visit_source(VisitSource.VisitedHistory)
                await callback(visited_term_id)
            })

            let map = this.maps.get(entry.dictionary_map_id)
            let source = enumValue(VisitSource, entry.source)
            let row = new VisitRow(
                history_entry_number,
                visited_term_id,
                a,
                map === null || map === undefined ? "?" : map.name,
                source == null ? VisitSource.Unknown.label : source.label,
                formatDateTimeHMS(entry.created_at, true, true, true)
            )
            this.add_row(row)

        }
    }
}