import {VirtualWindow} from "../../window/VirtualWindow.js";
import {Div} from "../../dom/elements/Div.js";
import {Span} from "../../dom/elements/Span.js";
import {Table} from "../../dom/elements/Table.js";
import {Tr} from "../../dom/elements/Tr.js";
import {Th} from "../../dom/elements/Th.js";
import {Td} from "../../dom/elements/Td.js";
import {read_entity} from "../../../api.js";
import {formatDateTimeHMS, showError} from "../../../dom.js";
import {BorderCollapse} from "../../styles/properties/BorderCollapse.js";
import {TextAlign} from "../../styles/properties/TextAlign.js";
import {_10PX} from "../../styles/Styles.js";
import {EventType} from "../../dom/attributes/EventType.js";
import {HoverTr} from "./HoverTr.js";

export class TermMetricsWindow extends VirtualWindow {
    #term_id = null
    #init_done = false

    constructor(term_id) {
        super(
            {
                title: "📊 Term Metrics",
                width: 600,
                height: 800
            }
        );
        this.#term_id = term_id
    }

    async init() {
        if (this.#init_done) return false
        this.#init_done = true

        this.set_content_padding(_10PX)
        let content = new Div().add_class("term_metrics_window_content")
        content.styles().padding(_10PX).end()
        let table = new Table().add_class("term_metrics_window_table")
        content.appendChild(table)

        table
            .styles()
            .borderCollapse(BorderCollapse.Collapse)
            .margin("0 aut")
            .width("100%")
            .end()

        class MTh extends Th {
            constructor(text = "") {
                super(text);
                this
                    .styles()
                    .border("1px solid black")
                    .padding(_10PX)
                    .textAlign(TextAlign.Left)
                    .verticalAlign("middle")
                    .backgroundColor("#bbb")
                    .end()
            }
        }
        class MTd extends Td {
            constructor(text = "") {
                super(text);
                this
                    .styles()
                    .border("1px solid black")
                    .padding(_10PX)
                    .textAlign(TextAlign.Left)
                    .end()
            }
        }
        table.appendChild(new Tr(new MTh("Section"), new MTh("Value")))

        let read_term_metric = await read_entity("dictionary_term_metric", this.#term_id)
        if (!read_term_metric) {
            showError("Loading term metric failed.");
            return false
        }

        function appendRow(emoji, section, value) {
            let section_td = new MTd();
            section_td.appendChild(new Span(emoji + " "))
            section_td.appendChild(new Span(section))
            let value_td = new MTd(value)
            let empty = false
            if(value === "Yes") {
                value_td.styles().fontWeight("normal").color("green").end()
            }
            if(value === "No") {
                value_td.styles().fontWeight("normal").color("red").end()
                empty = true
            }
            if(value.length === 0) empty = true
            if(value === 0) empty = true
            if(empty) value_td.styles().backgroundColor("rgba(238,238,238,1.00)").end()
            let tr = new HoverTr(section_td, value_td)

            table.appendChild(tr)
        }

        appendRow("🏷️", "Tags",read_term_metric.tag_count)
        appendRow("🚩", "Flags",read_term_metric.flag_count)
        appendRow("➡️", "Aliases",read_term_metric.alias_count)
        appendRow("📑", "Notes",read_term_metric.note_count)
        appendRow("🔗", "Links (outgoing)",read_term_metric.link_count)
        appendRow("↩️", "Links (incoming)",read_term_metric.backlink_count)
        appendRow("🌐", "Urls",read_term_metric.url_count)
        appendRow("📖", "Sources",read_term_metric.source_count)
        appendRow("🗂️", "Indexes",read_term_metric.index_count)
        appendRow("🏷️", "Reviews",read_term_metric.review_count)
        appendRow("🔁", "State 18",read_term_metric.state_18_count > 0? "Yes" : "No")
        appendRow("👀", "Views",read_term_metric.view_count)
        appendRow("✏️", "Updates",read_term_metric.update_count)

        let lwa = read_term_metric.last_viewed_at
        appendRow("👀", "Last viewed at", lwa === null || lwa === 0 ? "" : formatDateTimeHMS(lwa))
        let lua = read_term_metric.last_updated_at
        appendRow("✏️", "Last updated at", lua === null || lua === 0 ? "" : formatDateTimeHMS(lua))

        this.set_content(content.element())

        return true
    }
}