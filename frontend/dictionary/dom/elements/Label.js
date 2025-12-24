import {DomElement} from "../DomElement.js";

export class Label extends DomElement {
    constructor(innerText = "", width = "200px") {
        super("div")

        let l = DomElement.create_element("label")
        l.style.cssText = "margin-right:10px; display: inline;white-space: nowrap;"
        l.innerText = innerText
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
