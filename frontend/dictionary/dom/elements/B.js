import {DomElement} from "../DomElement.js";

export class B extends DomElement {
    constructor(text = "") {
        super("b");
        this.set_text(text)
    }

}