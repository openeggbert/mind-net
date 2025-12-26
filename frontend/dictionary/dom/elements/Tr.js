import {DomElement} from "../DomElement.js";
import {_DivSpan} from "./_DivSpan.js";

export class Tr extends _DivSpan {
    constructor(...children) {
        super("tr", ...children);
    }

}