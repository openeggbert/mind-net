import {get_element, showError, showWarn} from "../../dom.js";
import {validate_cfg} from "./CrudConfiguration.js";
import {defined} from "../../common.js";
import {list_all_entities} from "../../api.js";
import {translate} from "../globals/Globals.js";

export class _CrudSection {
    #cfg
    #element;
    #input = null
    #autocomplete = null

    #execute_action = null

    constructor(cfg, get_selected_map_id_callback, render_term_id_callback = null) {
        this.get_selected_map_id_callback = get_selected_map_id_callback
        this.render_term_id_callback = render_term_id_callback
        if (!validate_cfg(cfg)) throw translate("dictionary.crud_section.cfg_not_valid", {model: cfg.model})
        this.#cfg = cfg
        this.#element = get_element(cfg.models);
        this.#input = cfg.input ? get_element("input_search_" + this.#cfg.model) : null;
        this.autocomplete = null;
        this.#reset()
    }

    set_execution_action(a) {
        this.#execute_action = a
    }

    execute_action(action) {
        if(this.#execute_action === null || this.#execute_action === undefined) return
        //console.log("execute_action: " + action)
        this.#execute_action(action)
    }

    get_configuration() {
        return this.#cfg
    }

    #setVisible(visible) {
        this.#element.style.display = visible ? "block" : "none";
    }

    show() {
        this.#setVisible(true);
    }

    hide() {
        this.#setVisible(false);
    }

    #reset() {
        this.#element.innerHTML = "";
    }

    get _element() {
        return this.#element;
    }

    async render(dictionary_term_id) {
        this.#reset();

        const items = await this.loadItems(dictionary_term_id);
        if (items === null || items === undefined) {
            showError(translate("dictionary.crud_section.error.listing_models_failed", {models: this.#cfg.models}))
            return
        }
        for (const item of items) {
            if (item === null) throw "item is null"
            if (item === undefined) throw "item is undefined"

            let title = await this.#cfg.resolveTitle(item)
            if (title === null || title === undefined) {
                showError(translate("dictionary.crud_section.error.listing_title_failed", {model: this.#cfg.model, id: item.id}))
                continue
            }
            this.addItem(title, item.id, item);
        }
        if (this.#cfg.input) get_element("div_search_" + this.#cfg.model).style.display = "none"
        this.#setupAutocomplete(dictionary_term_id);
        this.afterRender(dictionary_term_id);
    }

    async loadItems(dictionary_term_id) {
        let filter = this.#cfg.filter
        if (filter === null || filter === undefined) throw "Not implemented";

        return await list_all_entities(
            this.#cfg.table,
            this.#cfg.filter(dictionary_term_id)
        );
    }

    addItem(title, id, item) {
        throw "Not implemented";
    }

    afterRender(dictionary_term_id) {
        // optional hook
    }

    get_input_value() {
        return this.#input.value
    }

    clear_input_value() {
        this.#input.value = ""
    }

    #setupAutocomplete(termId) {
        if (!defined(this.#cfg.createAutocomplete)) return
        if (!defined(this.#cfg.autocompleteCallback)) return

        if (this.#autocomplete) this.#autocomplete.destroy();

        this.#autocomplete = this.#cfg.createAutocomplete(this.#input)

        this.#autocomplete.addCallback(async () => {
            const item = this.#autocomplete.get_item();
            if (!item) {
                console.warn("Autocomplete returned null item");
                return;
            }
            await this.#cfg.autocompleteCallback(item, termId)
        });
    }
}