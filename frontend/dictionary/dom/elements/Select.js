import {DomElement} from "../DomElement.js";
import {ActionType} from "../attributes/ActionType.js";

export class Select extends DomElement {
    constructor() {
        super("select");
        this.styles().width("250px")
        this.add_action_handler(ActionType.Reset, (self, ...args) => {
            self.set_selected_index_to_0();
            return true;
        });
    }

    multiple() {
        this.element().multiple = true
        return this
    }

    add_option(option) {
        this.appendChild(option)
        return this
    }

    add_options(...options) {
        options.forEach(o => this.add_option(o));
        return this;
    }

    set_selected_index(index) {
        this.element().selectedIndex = index
        return this
    }

    set_selected_index_to_0() {
        return this.set_selected_index(0)
    }

    set_no_selected_index() {
        return this.set_selected_index(-1)
    }

    set_selected_values(values) {
        for (const option of this.options()) {
            option.selected = values.includes(Number(option.value));
        }
    }
    get_selected_values() {
        let result = []
        for (const option of this.options()) {
            if(option.selected) result.push(option.value)
        }
        return result
    }

    set_selected_value(value) {
        for (const option of this.options()) {
            if (value !== (Number(option.value))) continue
            option.selected = true
        }
    }

    options() {
        return this.element().options
    }

    selectedOptions() {
        return this.element().selectedOptions
    }
}