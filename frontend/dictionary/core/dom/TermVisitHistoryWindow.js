import {showWindowFromUrl, VirtualWindow} from "../../window/VirtualWindow.js";
import {Div} from "../../dom/elements/Div.js";
import {Span} from "../../dom/elements/Span.js";
import {Table} from "../../dom/elements/Table.js";
import {Tr} from "../../dom/elements/Tr.js";
import {Th} from "../../dom/elements/Th.js";
import {Td} from "../../dom/elements/Td.js";
import {getTitleCache, list_entities, QueryParams, read_entity, setTitleCache} from "../../../api.js";
import {formatDateTime, formatDateTimeHMS, showError, showWarn} from "../../../dom.js";
import {BorderCollapse} from "../../styles/properties/BorderCollapse.js";
import {TextAlign} from "../../styles/properties/TextAlign.js";
import {_10PX} from "../../styles/Styles.js";
import {EventType} from "../../dom/attributes/EventType.js";
import {set_next_visit_source, translate, USER_ID} from "../../globals/Globals.js";
import {Entities} from "../../entities/Entities.js";
import {null_or_undefined} from "../../../common.js";
import {VisitSource} from "../../enums/VisitSource.js";
import {enumValue} from "../../enums/EnumFunctions.js";

export class TermVisitHistoryWindow extends VirtualWindow {
    #init_done = false

    constructor() {
        super({
                title: translate("dictionary.term.container.visit_window.term_visit_history"),
                width: 1000,
                height: 600,
            }
        );
    }

    async init() {
        if (this.#init_done) return false
        this.#init_done = true

        this.set_content_padding(_10PX)
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
        this.set_content(content.element())

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
}