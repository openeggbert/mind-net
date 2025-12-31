import {DomElement} from "../DomElement.js";

export class A extends DomElement {
    constructor(text = "", href = null) {
        super("a");
        this.set_text(text)
        this.element().title = text
        if(href && href.length > 0) {
            this.element().href = href
        }
    }

}