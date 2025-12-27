import {VirtualWindow} from "../window/VirtualWindow.js";
import {Form} from "../dom/elements/Form.js";
import {Label} from "../dom/elements/Label.js";
import {Span} from "../dom/elements/Span.js";
import {Select} from "../dom/elements/Select.js";
import {Input} from "../dom/elements/Input.js";
import {Checkbox} from "../dom/elements/Checkbox.js";
import {Button} from "../dom/elements/Button.js";
import {_10PX, _20PX, _5PX} from "../styles/Styles.js";
import {getUserId} from "../../api.js";
import {defined} from "../../common.js";
import {
    formatDateTimeHM,
    formatDateTimeHMS,
    get_element, loadFromLocalStorage, saveToLocalStorage,
    showError,
    showInfo,
    showSuccess,
    showWarn
} from "../../dom.js";
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
import {B} from "../dom/elements/B.js";
import {TextAlign} from "../styles/properties/TextAlign.js";
import {RepetitionButton} from "./RepetitionButton.js";
import {ActionButton} from "./ActionButton.js";
import {RatingButton} from "./RatingButton.js";
import {AskForGradeModel} from "./AskForGradeModel.js";
import {RepetitionModel} from "./RepetitionModel.js";
import {list_maps, list_term_searches, post_review, read_review, read_state_18} from "./RepetitionRepository.js";
import {RatingModel} from "./RatingModel.js";
import {RepetitionResultModel} from "./RepetitionResultModel.js";
import {ReviewResult} from "./ReviewResult.js";

export class RepetitionWindow extends VirtualWindow {
    #centred = false

    constructor(get_selected_map_id_callback, render_term_callback) {
        super({
            title: translate("dictionary.repetition.repetition"),
            width: screen.width > 1000 ? 1000 : screen.width - 100,
            height: 600
        })
        this.get_selected_map_id_callback = get_selected_map_id_callback
        this.render_term_callback = render_term_callback
        this.set_content_padding()
        this.get_title_dataset().i18n = "dictionary.repetition.repetition"
    }

    async init() {
        let model = null
        while (true) {
            model = await this.ask_user_for_model(model)
            let searches = await list_term_searches(model)
            if (!searches) {
                showError(translate("dictionary.repetition.error.loading_terms_for_review_failed"))
                continue
            }
            //searches = Array.from(searches).slice(0, 2)
            if (searches.length === 0) {
                showInfo(translate("dictionary.repetition.no_terms_for_review"))
                continue
            }
            let repetition_result_model = new RepetitionResultModel(searches)

            let aborted = false
            for (let i = 0; i < searches.length; i++) {
                if (aborted) {
                    repetition_result_model.next_done(ReviewResult.Skipped, null)
                    continue
                }
                let search = searches[i]
                let start = Date.now()
                let ask_for_grade_model = await this.ask_user_for_grade(search, i + 1, searches.length)

                if (ask_for_grade_model.grade === -1) {
                    repetition_result_model.next_done(ReviewResult.Skipped, null)
                    continue
                }
                if (ask_for_grade_model.grade < -1) {
                    aborted = true
                    repetition_result_model.next_done(ReviewResult.Skipped, null)
                    continue
                }
                let end = Date.now()
                let latency_ms = end - start
                let old_state = await read_state_18(search.dictionary_term_id)
                let review = await post_review(
                    {
                        user_id: getUserId(),
                        dictionary_map_id: model.map_id,
                        algorithm: 18,
                        dictionary_term_id: search.dictionary_term_id,
                        review_date: Date.now(),
                        grade: ask_for_grade_model.grade,
                        started_at: start,
                        ended_at: end,
                        latency_ms: latency_ms,
                        answer_change_count: ask_for_grade_model.answer_change_count,
                        details_json: "{}"
                    }
                )
                if (!review) {
                    showError(translate("dictionary.repetition.error.posting_review_failed"))
                    repetition_result_model.next_done(ReviewResult.Error, review)
                    continue
                }

                let new_state = await read_state_18(search.dictionary_term_id)
                let state_not_changed = defined(old_state) && old_state.updated_at === new_state.updated_at
                if (state_not_changed) {
                    repetition_result_model.next_done(ReviewResult.NotDueYet, review)
                    showInfo(translate("dictionary.repetition.info.term_is_not_due_yet"))
                }
                if (!old_state) {
                    showInfo(translate("dictionary.repetition.info.term_is_reviewed_for_the_first_time"))
                }
                if(!state_not_changed) repetition_result_model.next_done(ReviewResult.Reviewed, review)
                await this.show_review_result(search, new_state, review.id)
            }
            repetition_result_model.end()
            await this.show_total_result(repetition_result_model)
        }

    }

    async show_total_result(model) {
        return new Promise(async (resolve, reject) => {

            let content = new Div().add_class("repetition_div")
            this.set_content(content.element())
            this.get_internal_content().style.backgroundColor = "#121212"

            let total_result_span = new Div(translate("dictionary.repetition.stats.total_result")).styles().fontWeight("bold").fontSize("150%").marginBottom(_20PX).end()
            total_result_span.element().dataset.i18n = "dictionary.repetition.stats.total_result"

            let card = new Div().add_class("repetition_card")
            content.appendChild(card)
            card.append_many(total_result_span)
// alert(model.get_reviews())
            {
                let stats_div = new Div().styles().textAlign(TextAlign.Left).fontSize("125%").width("auto").end().add_class(("stats_div"))

                class I18nSpan extends Span {
                    constructor(i18n_key) {
                        super();
                        this.set_text(translate(i18n_key))
                        this.data_i18n(i18n_key)
                    }
                }
                card.appendChild(stats_div)

                class RLabel extends Div{
                    constructor(emoji, text_i18n, value, marginBottom = false, seconds = false) {
                        super();
                        let first_column = new Div()
                        first_column.append_many(new Span(emoji + " "), text_i18n.length === 0 ? "" : new I18nSpan("dictionary.repetition.stats." + text_i18n), text_i18n.length === 0 ? "" :new Span(": "))
                            .styles()
                            .minWidth("200px")
                            .display(Display.InlineBlock)
                            .marginRight(_10PX)
                            .end()
                        let second_column = new Span(String(value))
                        if(seconds) {
                            second_column.appendChild(new Span(" "))
                            second_column.appendChild(new I18nSpan("dictionary.repetition.seconds"))
                        }
                        this.append_many(first_column, second_column)
                        if (marginBottom) {
                            this.styles().marginBottom(_20PX).end()
                        }
                    }
                }

                let reviewed = model.get_count(ReviewResult.Reviewed)
                let errors = model.get_count(ReviewResult.Error)
                let not_due_yet = model.get_count(ReviewResult.NotDueYet)
                let unknown = model.get_count(ReviewResult.Unknown)
                function add_stat(emoji, i18n, value, marginBottom = false, seconds = false) {
                    stats_div.appendChild(new RLabel(emoji, i18n, value, marginBottom, seconds))
                }
                add_stat("🚀", "started_at",formatDateTimeHMS(model.get_started_at()))
                add_stat("🏁", "ended_at", formatDateTimeHMS(model.get_ended_at()))
                add_stat("⏱️", "duration", model.get_duration(), true)
                add_stat("🔢", "total", model.get_total_count(), true)
                add_stat("✔️", "reviewed", reviewed)
                add_stat("⏭️", "skipped", model.get_count(ReviewResult.Skipped))

                if (errors > 0) add_stat("❌", "error", errors)
                if (not_due_yet > 0) add_stat("⏳", "not_due_yet",  not_due_yet)
                if (unknown > 0) add_stat("❓", "unknown", unknown)
                add_stat("️", "", "")

                if(reviewed > 0 || not_due_yet > 0) {
                add_stat("⏱️","total_latency",String((model.get_total_latency_s()).toFixed(2)),false, true)
                add_stat("🔢","average_latency",String((model.get_average_latency_s()).toFixed(2)),false, true)
                add_stat("⏬","min_latency",String((model.get_min_latency_s()).toFixed(2)), false, true)
                add_stat("⏫","max_latency",String((model.get_max_latency_s()).toFixed(2)), false, true)
                add_stat("➗","average_grade",String((model.get_average_grade()).toFixed(1)))
                add_stat("🧮","total_answer_change_count",String(model.get_total_answer_change_count()))
                }

            }

            let continue_button = new ActionButton(translate("dictionary.repetition.continue")).on("click", () => {resolve()})
            continue_button.element().dataset.i18n = "dictionary.repetition.continue"

            card.appendChild(new Br())
            card.appendChild(continue_button)
        })
    }


    async show_review_result(term_for_review, state18, review_id) {
        return new Promise(async (resolve, reject) => {

            let content = new Div().add_class("repetition_div")
            this.set_content(content.element())
            this.get_internal_content().style.backgroundColor = "#121212"

            let review = await read_review(review_id)

            let term_was_reviewed_span = new Span(translate("dictionary.repetition.term_was_reviewed")).styles().fontWeight("bold").end()
            term_was_reviewed_span.element().dataset.i18n = "dictionary.repetition.term_was_reviewed"

            let card = new Div().add_class("repetition_card")
            content.appendChild(card)
            card.appendChild(new Div(new Span("✅ "), term_was_reviewed_span, new Span(": " + term_for_review.title).styles().fontWeight("bold").end()).styles().fontSize("125%").end())
            card.appendChild(new Br())

            {
                let stats_div = new Div().styles().textAlign(TextAlign.Left).fontSize("125%").width("auto").end().add_class(("stats_div"))
                let next_review_span = new Span(translate("dictionary.repetition.next_review"))
                next_review_span.element().dataset.i18n = "dictionary.repetition.next_review"
                let repetitions_span = new Span(translate("dictionary.repetition.repetitions"))
                repetitions_span.element().dataset.i18n = "dictionary.repetition.repetitions"
                let stability_span = new Span(translate("dictionary.repetition.stability"))
                stability_span.element().dataset.i18n = "dictionary.repetition.stability"
                let retrievability_span = new Span(translate("dictionary.repetition.retrievability"))
                retrievability_span.element().dataset.i18n = "dictionary.repetition.retrievability"
//         📅 Next review : 2025-12-27 17:27
// 🔢 Repetitions: 0
// 📈 Stability: 17.61
// 📈 Retrievability: 0.17
                card.appendChild(stats_div)
                let stability = (state18.stability_times_100 / 100).toFixed(2)

                let details_json = defined(review) ? JSON.parse(review.details_json) : null
                let retrievability = null

                if (details_json) {
                    if (details_json.R_now !== null && details_json.R_now !== undefined) {
                        retrievability = Number(details_json.R_now).toFixed(2)
                    }
                }
                stats_div.appendChild(new Div(new Span("📅 "), next_review_span, new Span(": " + formatDateTimeHM(state18.next_review))))
                stats_div.appendChild(new Div(new Span("🔢 "), repetitions_span, new Span(": " + state18.repetitions)))
                stats_div.appendChild(new Div(new Span("📈 "), stability_span, new Span(": " + stability)))
                if (retrievability) stats_div.appendChild(new Div(new Span("⏱️ "), retrievability_span, new Span(": " + retrievability)))
            }


            let send_button = new ActionButton(translate("dictionary.repetition.send")).on("click", () => {

                resolve()
            })
            send_button.element().dataset.i18n = "dictionary.repetition.send"
            card.appendChild(new Br())
            card.appendChild(send_button)
        })
    }

    async ask_user_for_grade(term_for_review, number, size) {
        return new Promise(async (resolve, reject) => {
            let content = new Div().add_class("repetition_div")
            this.set_content(content.element())
            this.get_internal_content().style.backgroundColor = "#121212"

            let card = new Div().add_class("repetition_card")
            content.appendChild(card)

            let item = new Label("i18n.dictionary.repetition.item")
            item.styles().display(Display.Inline).end()
            card
                .appendChild(new Span(item, new Span(number + " / " + size).styles().fontWeight("bold").end()))
                .appendChild(new Br())


            card.appendChild(new Label("❓ " + term_for_review.title).styles().fontSize("150%").margin(_10PX).end())
            let show_hide_definition_button = new RepetitionButton(translate("dictionary.repetition.show_definition"))
            let term_has_no_definition = new Span(translate("dictionary.repetition.no_definition")).styles().display(Display.None).end()
            if (term_for_review.definition.length === 0) {
                show_hide_definition_button.styles().display(Display.None).end()
                term_has_no_definition.element().dataset.i18n = "dictionary.repetition.no_definition"
                term_has_no_definition.styles().display(Display.Inline).color("#bbb").end()
            } else {
                show_hide_definition_button.element().style.minWidth = "180px"
                show_hide_definition_button.element().dataset.i18n = "dictionary.repetition.show_definition"
                show_hide_definition_button.element().dataset.definitionShown = "no"
                show_hide_definition_button.element().onclick = () => {
                    let definitionShown = show_hide_definition_button.element().dataset.definitionShown === "yes"
                    if (definitionShown) {
                        show_hide_definition_button.element().dataset.definitionShown = "no"
                        show_hide_definition_button.set_text(translate("dictionary.repetition.show_definition"))
                        show_hide_definition_button.element().dataset.i18n = "dictionary.repetition.show_definition"
                    } else {
                        show_hide_definition_button.element().dataset.definitionShown = "yes"
                        show_hide_definition_button.set_text(translate("dictionary.repetition.hide_definition"))
                        show_hide_definition_button.element().dataset.i18n = "dictionary.repetition.hide_definition"
                    }
                    get_element("definition_div" + this.get_created_at()).style.display = definitionShown ? "none" : "block"

                }
            }

            let show_term_button = new RepetitionButton(translate("dictionary.repetition.show_term"))
            show_term_button.element().dataset.i18n = "dictionary.repetition.show_term"

            show_term_button.on(EventType.Click.label, async () => {
                await this.render_term_callback(term_for_review.dictionary_term_id)
            })
            card.appendChild(new Div(show_hide_definition_button, term_has_no_definition, show_term_button))
            show_hide_definition_button.styles().marginRight(_5PX).end()
            term_has_no_definition.styles().marginRight(_5PX).end()
                , show_term_button.styles().marginLeft(_5PX).end()
            let definition_div = new Div()
                .styles()
                .padding(_10PX)
                .minHeight("100px")
                .color(Color.Black)
                .display(Display.None)
                .end()
                .add_class("definition_div")
                .set_id("definition_div" + this.get_created_at())
            card.appendChild(definition_div.set_html_unsafe(markdownToHtml(term_for_review.definition)))
            let how_well = new Div(translate("dictionary.repetition.how_well_did_you_recall"))
                .styles()
                .margin(_10PX)
                .end()
            how_well.element().dataset.i18n = "dictionary.repetition.how_well_did_you_recall"
            card.appendChild(how_well)

            let rating_model = new RatingModel()
            let grades = RatingModel.grades
            let rating_buttons_div = new Div()
            card.appendChild(rating_buttons_div)
            grades.forEach(grade => {
                rating_buttons_div.appendChild(new RatingButton(grade, rating_model))
            })

            let show_legend_button = new RepetitionButton(translate("dictionary.repetition.show_legend"))
            show_legend_button.element().style.minWidth = "180px"
            show_legend_button.element().dataset.i18n = "dictionary.repetition.show_legend"
            show_legend_button.element().dataset.legendShown = "no"
            show_legend_button.element().onclick = () => {
                let legendShown = show_legend_button.element().dataset.legendShown === "yes"
                if (legendShown) {
                    show_legend_button.element().dataset.legendShown = "no"
                    show_legend_button.set_text(translate("dictionary.repetition.show_legend"))
                    show_legend_button.element().dataset.i18n = "dictionary.repetition.show_legend"
                } else {
                    show_legend_button.element().dataset.legendShown = "yes"
                    show_legend_button.set_text(translate("dictionary.repetition.hide_legend"))
                    show_legend_button.element().dataset.i18n = "dictionary.repetition.hide_legend"
                }
                get_element("legend_div" + this.get_created_at()).style.display = legendShown ? "none" : "block"

            }

            card.appendChild(show_legend_button).appendChild(new Br())

            let legend_div = new Div()
                .styles()
                .padding(_10PX)
                .display(Display.None)
                .textAlign(TextAlign.Left)
                .end()
                .add_class("rating-legend")
                .set_id("legend_div" + this.get_created_at())
            card.appendChild(legend_div)
            grades.forEach(grade => {
                let span_grade = new B(grade + ": ")
                let key = "dictionary.repetition.legend." + grade
                let legend = new Span(translate(key))
                legend.element().dataset.i18n = "dictionary.repetition.legend." + grade
                legend_div.appendChild(new Div(span_grade, legend))
            })

            let next_action_div = new Div(translate("dictionary.repetition.next_action")).styles().margin("15px").end()
            next_action_div.element().dataset.i18n = "dictionary.repetition.next_action"
            card.appendChild(next_action_div)
            let send_button = new ActionButton(translate("dictionary.repetition.send")).on("click", () => {
                if (rating_model.selected_grade < 0 || rating_model.selected_grade > 5) {
                    showWarn(translate("dictionary.repetition.select_a_grade"))
                    return
                }
                let was_correct = rating_model.selected_grade >= 3;

                let msg = translate("dictionary.repetition.term_was_reviewed") + ": #" + term_for_review.dictionary_term_id + " " + term_for_review.title
                if (was_correct) {
                    showSuccess(msg)
                } else {
                    showInfo(msg)
                }
                resolve(new AskForGradeModel(rating_model.selected_grade, rating_model.answer_change_count))
            })
            send_button.element().dataset.i18n = "dictionary.repetition.send"
            let skip_button = new ActionButton(translate("dictionary.repetition.skip")).on("click", () => {
                showWarn(translate("dictionary.repetition.term_was_skipped") + ": #" + term_for_review.dictionary_term_id + " " + term_for_review.title)
                resolve(new AskForGradeModel())
            })
            skip_button.element().dataset.i18n = "dictionary.repetition.skip"

            let abort_button = new ActionButton(translate("dictionary.repetition.abort")).on("click", () => {
                showWarn(translate("dictionary.repetition.warn.session_was_aborted"))
                resolve(new AskForGradeModel(-2))
            })
            abort_button.element().dataset.i18n = "dictionary.repetition.abort"
            card.appendChild(new Div(send_button, skip_button, abort_button))
        })
    }

    async ask_user_for_model(model = null) {
        return new Promise(async (resolve, reject) => {
            this.get_internal_content().style.backgroundColor = "#121212"
            let content = new Div()
            this.set_content(content.element())
            if (!this.#centred) {
                this.center()
                this.#centred = true
            }
            if (model === null) {
                let model_lc = loadFromLocalStorage("RepetitionModel")
                // alert("model_lc=" + model_lc)
                if (defined(model_lc)) {
                    model = JSON.parse(model_lc)
                }
            }

            const repetition_form = new Form()
            let card = new Div()
                .add_class("repetition_card")
                .styles()
                .maxWidth("800px")
                .width("auto")
                .margin("auto")
                .marginTop(_20PX)
                .end()
            card.appendChild(repetition_form)

            content.appendChild(card.element())

            let maps = await list_maps()
            if (!defined(maps)) {
                showError(translate("dictionary.crud_section.error.listing_models_failed", {models: translate("dictionary.common.maps").toLowerCase()}))
                return
            }

            function add_control(label, control) {
                let row = new FormRow(label, control)
                row.styles().paddingTop("10px").end()
                repetition_form.append(row)
                return row
            }

            let map_select = new Select()
            map_select.styles().backgroundColor(Color.Black).end()
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
            search_input.styles().backgroundColor(Color.Black).end()
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

            class RCheckbox extends Checkbox {
                constructor() {
                    super();
                    this.styles().accentColor(Color.Black).end()
                }
            }
            class ModePair extends Span {
                constructor(label, checked = true) {
                    super(
                        new RCheckbox().set_checked(checked).styles().marginRight(_20PX).end(),
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
            let mode_control = add_control("i18n.dictionary.common.mode", modes)

            let has_definition_checkbox = new RCheckbox().set_checked(defined(model) ? model.has_definition : true)
            let has_definition_control = add_control("i18n.dictionary.common.has_definition", has_definition_checkbox)

            let start_button = new RepetitionButton(translate("dictionary.common.start")).styles().marginTop(_20PX).marginLeft(_20PX).transform("scale(1.25)").end()
            start_button.element().dataset.i18n = "dictionary.common.start"
            card.appendChild(new Div(start_button).css({
                display: "flex",
                justifyContent: "center",
                alignItems: "center"
            }))

            search_autocomplete.addCallback(() => {
                mode_control.hide()
                has_definition_control.hide()
                due_checkbox.get_checkbox().element().disabled = true
                not_due_checkbox.get_checkbox().element().disabled = true
                never_checkbox.get_checkbox().element().disabled = true
                has_definition_checkbox.element().disabled = true
            })
            search_autocomplete.addResetCallback(() => {

                mode_control.show()
                has_definition_control.show()
                due_checkbox.get_checkbox().element().disabled = false
                not_due_checkbox.get_checkbox().element().disabled = false
                never_checkbox.get_checkbox().element().disabled = false
                has_definition_checkbox.element().disabled = false
            })
            if (defined(model)) {
                let search = model.search_id !== 0
                if(search) {
                    mode_control.hide()
                    has_definition_control.hide()
                }
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
                if(model.search_id === 0 && !model.due && !model.not_due && !model.never) {
                    showError(translate("dictionary.repetition.error.no_mode_enabled"))
                    return
                }

                model.has_definition = has_definition_checkbox.is_checked()

                const MILLISECONDS_PER_DAY = 365 * 24 * 60 * 60 * 1000
                saveToLocalStorage("RepetitionModel", JSON.stringify(model.to_json()), MILLISECONDS_PER_DAY)
                content.clear_html()
                content.appendChild(new Div("⏳").css({
                    display: "flex",
                    justifyContent: "center",
                    alignItems: "center",
                    marginTop: "40px",
                    fontSize: "200%"
                }))
                resolve(model)
            })
        })
    }

}
