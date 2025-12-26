import {DomElement} from "../DomElement.js";

export class Th extends DomElement {
    constructor(text = "") {
        super("th");
        this.set_text(text)
    }

}