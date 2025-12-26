import {DomElement} from "../DomElement.js";

export class Td extends DomElement {
    constructor(text = "") {
        super("td");
        this.set_text(text)
    }

}