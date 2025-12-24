import {DomElement} from "../DomElement.js";

export class _DivSpan extends DomElement {
    constructor(tag, ...children) {
        super(tag);
        this.append_many(...children);
    }
}
