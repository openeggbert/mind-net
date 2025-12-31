import {Tr} from "../../dom/elements/Tr.js";
import {EventType} from "../../dom/attributes/EventType.js";

export class HoverTr extends Tr {
    constructor(...children) {
        super(...children);
        this.on(EventType.MouseEnter.label, ()=> {
            this.styles().backgroundColor("rgba(131,199,230,0.4)").end()
        })
        this.on(EventType.MouseLeave.label, ()=> {
            this.styles().backgroundColor("white").end()
        })
    }
}