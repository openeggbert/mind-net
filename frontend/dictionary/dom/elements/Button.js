import {DomElement} from "../DomElement.js";

export class Button extends DomElement {
    constructor(text = "") {
        super("button");
        this.set_text(text)
    }

    set_title(text) {
        this.element().title = text
        return this
    }

    get_title() {
        return this.element().title
    }

    set_type(type) {
        console.log(type.label)
        this.element().type = type.label
        return this
    }
    click() {
        this.element().click()
        return this
    }
}