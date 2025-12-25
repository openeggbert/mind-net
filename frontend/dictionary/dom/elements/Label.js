import {DomElement} from "../DomElement.js";
import {translate} from "../../globals/Globals.js";

export class Label extends DomElement {
    constructor(innerText = "", width = "200px") {
        super("div")

        let l = DomElement.create_element("label")
        l.style.cssText = "margin-right:10px; display: inline;white-space: nowrap;"
        let inner_text = innerText
        if (inner_text.startsWith("i18n.")) {
            let key = inner_text.slice(5);
            inner_text = translate(key)
            l.dataset.i18n = key
        }
        l.innerText = inner_text
        if (l.innerText.size > 0) {
            let last_character = l.innerText.at(-1)
            if (last_character !== ":") l.innerText = innerText + ": "
        }

        this.appendChild(l)
        this.css({
            width: width,
            display: "inline-block",
            whiteSpace: "nowrap",
        })

    }
}
