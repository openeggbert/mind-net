import {list_all_entities, read_entity} from "./api.js";
import {get_element, showInfo} from "./dom.js";
import {Div} from "./dictionary/dom/elements/Div.js";
import {Input} from "./dictionary/dom/elements/Input.js";
import {Span} from "./dictionary/dom/elements/Span.js";

function debounce(fn, delay) {
    let timer = null;
    return function (...args) {
        clearTimeout(timer);
        timer = setTimeout(() => fn.apply(this, args), delay);
    };
}

export class Autocomplete {
    #callbacks = [];
    #reset_callbacks = [];
    #search_all_results_callback = null
    #items_map = new Map()

    addCallback(fn) {
        this.#callbacks.push(fn);
    }
    addResetCallback(fn) {
        this.#reset_callbacks.push(fn);
    }

    clear_after_click = true

    #runCallbacks(...args) {
        this.#callbacks.forEach(fn => fn(...args));
    }
    set_search_all_results_callback(fn) {
        this.#search_all_results_callback = fn
    }

    constructor(input, input_min_length, entity, query_params = "", title_column = "title", part_column = "title_part", insert_after_id = "") {
        this.input = input instanceof Input ? input.element() : input;
        this.title_column = title_column;
        this.item = null;
        this.input_min_length = input_min_length
        this.query_params = query_params

        this.fetcher = async query => {
            const qp = this.query_params + "&" + part_column + "=" + encodeURIComponent(query);
            return await list_all_entities(entity, qp);
        };

        this.box = document.createElement("div");
        this.box.className = "suggestions";
        this.box.style.display = "none";
        this.box.style.zIndex = "1000"
        this.box.style.minWidth = "100px"

        if (input.parentNode) {
            if (insert_after_id === null || insert_after_id === undefined || insert_after_id === "") {
                input.parentNode.appendChild(this.box);
            } else {
                let el = get_element(insert_after_id)
                if (el === null) alert("el with id is null: " + insert_after_id)
                el.after(this.box)
            }
        } else {
            throw new Error("input.parentNode === null")
        }

        this.input_handler = debounce(() => {
            this.search(this.input.value.trim(), input_min_length);
        }, 100);

        this.input.addEventListener("input", this.input_handler);
    }
    set_query_params(query_params) {
        this.query_params = query_params
    }

    async search(q, input_min_length = 3, show_box = true) {
        if (q.length < input_min_length) {
            this.box.style.display = "none";
            this.box.innerHTML = "";
            return;
        }

        const items = await this.fetcher(q);
        this.render(items, q, show_box);
    }

    after_render(box, input, items) {

    }

    render(items, q, show_box = true) {
        console.log("Started rendering items: " + items.length + " show_box=" + show_box)
        this.box.innerHTML = "";

        if (!items || items.length === 0) {
            this.box.style.display = "none";
            return;
        }

        let i = 0
        this.#items_map.clear()
        items.forEach(item => {
            const div = document.createElement("div");
            div.className = "suggestion-item";
            let title = item[this.title_column]
            div.textContent = title;

            this.#items_map.set(i, div)
            div.onclick = () => {
                this.input.value = title;
                this.box.style.display = "none";
                this.item = item;
                this.#runCallbacks()
                if (this.clear_after_click) this.input.value = ""
            };

            this.box.appendChild(div);
            i++
        });
        if(items.length >= 20 && this.#search_all_results_callback && q!== "***") {
            {
                const div = document.createElement("div");
                div.className = "suggestion-item";
                let title = "Search all results for \"" + q + "\""
                div.textContent = title;
                div.style.color = "rgb(29,89,147)"

                div.onclick = () => {
                    this.#search_all_results_callback(q)
                    this.box.style.display = "none";
                    if (this.clear_after_click) this.input.value = ""
                };

                this.box.appendChild(div);
                i++
            }
        }
        let tmp_span = new Span()
        this.box.appendChild(tmp_span.element())
        this.after_render(this.box, this.input)

        if (show_box) this.box.style.display = "block";
    }

    set_selected_item(index) {
        if (!this.#items_map.has(index)) {
            return false;
        }
        this.#items_map.get(index).click()
        return true
    }

    get_box() {
        return this.box
    }

    get_item() {
        return this.item;
    }

    get_item_id() {
        return this.item === null ? 0 : this.item.id;
    }

    async set_from_title(title, id = 0) {
        const items = await this.fetcher(title, this.input_min_length);
        if (items.length === 0) return
        let item = null;
        if (id === 0) {
            item = items[0]
        } else {
            for (const e of items) {
                if (e.id === id) {
                    item = e;
                    break;
                }
            }
        }
        if (item === null) return

        this.input.value = title;
        this.item = item;
        if (this.clear_after_click) this.input.value = ""
    }

    reset() {
        this.input.value = ""
        this.item = null
        this.box.innerHTML = "";
        this.box.style.display = "none"
        this.#reset_callbacks.forEach(fn => fn());
    }

    destroy() {
        if (this.box && this.box.parentNode) {
            this.box.remove();
        }

        if (this.input_handler) {
            this.input.removeEventListener("input", this.input_handler);
        }
    }

}

export function null_or_undefined(value) {
    return value === null || value === undefined;
}

export function defined(value) {
    return value !== null && value !== undefined
}