import {ValueElement} from "./ValueElement.js";

export class Option extends ValueElement {
    constructor(value = null, text = null) {
        super("option");
        if (value) this.set_value(value)
        if (text) this.set_text(text)
        if (value && text === null) this.set_text(value)
    }
}
