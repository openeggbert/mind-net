import {showWindowFromUrl, VirtualWindow} from "../../window/VirtualWindow.js";
import {Div} from "../../dom/elements/Div.js";
import {Span} from "../../dom/elements/Span.js";
import {Table} from "../../dom/elements/Table.js";
import {Tr} from "../../dom/elements/Tr.js";
import {Th} from "../../dom/elements/Th.js";
import {Td} from "../../dom/elements/Td.js";
import {getTitleCache, list_entities, QueryParams, read_entity, setTitleCache} from "../../../api.js";
import {formatDateTimeHMS, showError, showWarn} from "../../../dom.js";
import {BorderCollapse} from "../../styles/properties/BorderCollapse.js";
import {TextAlign} from "../../styles/properties/TextAlign.js";
import {_10PX, _20PX} from "../../styles/Styles.js";
import {EventType} from "../../dom/attributes/EventType.js";
import {set_next_visit_source, translate, USER_ID} from "../../globals/Globals.js";
import {Entities} from "../../entities/Entities.js";
import {null_or_undefined} from "../../../common.js";
import {VisitSource} from "../../enums/VisitSource.js";
import {enumValue} from "../../enums/EnumFunctions.js";
import {Button} from "../../dom/elements/Button.js";
import {A} from "../../dom/elements/A.js";
import {HoverTr} from "./HoverTr.js";

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

        let th_source = new Th("Source");

        let th_timestamp = new Th(translate("dictionary.common.timestamp"))
        th_timestamp.data_i18n("dictionary.common.timestamp")

        const th_array = [th_number,th_id,th_title,th_source,th_timestamp]
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

export class TermVisitHistoryWindow extends VirtualWindow {
    #init_done = false
    #render_term_id_callback = null
    #table
    #rows = new Set()

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

        let button = new Button(translate("dictionary.term.container.visit_window.button_show_all_visits"))
        button.data_i18n("dictionary.term.container.visit_window.button_show_all_visits")
        button.on(EventType.Click, () => {
            let url = "index.html?entity=dictionary_term_visit&action=list&user_id=" + USER_ID
            showWindowFromUrl(translate("dictionary.term.container.visit_window.all_visits"), url)
        })
        button.styles().margin(_20PX).textAlign(TextAlign.Center).end();

        content.appendChild(button)

        this.#table = new Table()
            .styles()
            .borderCollapse(BorderCollapse.Collapse)
            .margin("0 auto")
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
        this.#rows.forEach(e=> e.remove())
        this.#rows.clear()
    }

    async init() {
        if (this.#init_done) return false
        this.#init_done = true

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

            class VisitRow extends HoverTr {
                constructor(
                    history_entry_number,
                    visited_term_id,
                    title,
                    source,
                    timestamp
                ) {
                    super();
                    this.appendChild(new VisitedTd(history_entry_number))
                    this.appendChild(new VisitedTd(visited_term_id))

                    let td_title = new VisitedTd()
                    td_title.appendChild(title)

                    this.appendChild(td_title)
                    this.appendChild(new VisitedTd(source))
                    this.appendChild(new VisitedTd(timestamp))
                }
            }

            let title_ = await find_title(visited_term_id)
            if (disambiguation_map.has(visited_term_id)) {
                let value = disambiguation_map.get(visited_term_id)
                if (value !== "") title_ = title_ + " (" + disambiguation_map.get(visited_term_id) + ")"
            }
            let a = new A(title_, "?")
            let callback = async (term_id) => await this.#render_term_id_callback(term_id, true)
            a.on(EventType.Click, async function () {
                event.preventDefault();
                set_next_visit_source(VisitSource.VisitedHistory)
                await callback(visited_term_id)
            })

            let source = enumValue(VisitSource, entry.source)
            let row = new VisitRow(
                history_entry_number,
                visited_term_id,
                a,
                source == null ? VisitSource.Unknown.label : source.label,
                formatDateTimeHMS(entry.created_at, true, true, true)
            )
            this.add_row(row)

        }
    }
}