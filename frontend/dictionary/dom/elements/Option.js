import {ValueElement} from "./ValueElement.js";

export class Option extends ValueElement {
    constructor(value = null, text = null) {
        super("option");
        if (value || value === 0) {
            this.set_value(value)
        } else {
            if(text) this.set_value(text)
        }
        if (text) this.set_text(text)
        if ((value || value === 0) && text === null) this.set_text(value)
    }
    set_selected(value = true) {
        this.element().selected = value
    }
}
