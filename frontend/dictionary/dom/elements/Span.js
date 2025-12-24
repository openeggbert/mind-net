import {_DivSpan} from "./_DivSpan.js";

export class Span extends _DivSpan {
    constructor(...children) {
        super("span", ...children)
    }
}
