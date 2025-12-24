import {_DivSpan} from "./_DivSpan.js";

export class Div extends _DivSpan {
    constructor(...children) {
        super("div", ...children)
    }
}