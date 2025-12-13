import {list_all_entities, read_entity} from "./api.js";
import {get_element} from "./dom.js";

function debounce(fn, delay) {
    let timer = null;
    return function (...args) {
        clearTimeout(timer);
        timer = setTimeout(() => fn.apply(this, args), delay);
    };
}

export class Autocomplete {
    #callbacks = [];
    addCallback(fn) {
        this.#callbacks.push(fn);
    }

    #runCallbacks(...args) {
        this.#callbacks.forEach(fn => fn(...args));
    }

    constructor(input, input_min_length, entity, query_params, title_column, part_column) {
        this.input = input;
        this.title_column = title_column;
        this.item = null;

        this.fetcher = async query => {
            const qp = query_params + "&" + part_column + "=" + encodeURIComponent(query);
            return await list_all_entities(entity, qp);
        };

        this.box = document.createElement("div");
        this.box.className = "suggestions";
        this.box.style.display = "none";

        if (input.parentNode) {
            input.parentNode.insertBefore(this.box, get_element("button_add_term"));
        }

        this.input_handler = debounce(() => {
            this.search(this.input.value.trim(), input_min_length);
        }, 100);

        this.input.addEventListener("input", this.input_handler);
    }


    async search(q, input_min_length = 3) {
        if (q.length < input_min_length) {
            this.box.style.display = "none";
            this.box.innerHTML = "";
            return;
        }

        const items = await this.fetcher(q);
        this.render(items);
    }

    render(items) {
        this.box.innerHTML = "";

        if (!items || items.length === 0) {
            this.box.style.display = "none";
            return;
        }

        items.forEach(item => {
            const div = document.createElement("div");
            div.className = "suggestion-item";
            let title = item[this.title_column]
            div.textContent = title;

            div.onclick = () => {
                this.input.value = title;
                this.box.style.display = "none";
                this.item = item;
                this.#runCallbacks()
            };

            this.box.appendChild(div);
        });

        this.box.style.display = "block";
    }
    get_item() {
        return this.item;
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