import {DomElement} from "../DomElement.js";
import {Display} from "../../styles/properties/Display.js";

export class Form extends DomElement {
    constructor() {
        super("form")

        this.css({
            display: Display.Flex.label,
            flexDirection: "column",
            gap: "12px",
            padding: "10px"
        })
    }

}
