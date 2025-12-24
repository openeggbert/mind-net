import {DomElement} from "../DomElement.js";

export class ValueElement extends DomElement {
    set_value(value) {
        this.element().value = value
        return this
    }

    clear_value() {
        return this.set_value("")
    }

    get_value() {
        return this.element().value
    }

    get_value_as_number() {
        return Number(this.get_value())
    }
}