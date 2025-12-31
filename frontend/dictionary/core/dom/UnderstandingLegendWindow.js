import {VirtualWindow} from "../../window/VirtualWindow.js";
import {Div} from "../../dom/elements/Div.js";
import {B} from "../../dom/elements/B.js";
import {Span} from "../../dom/elements/Span.js";

export class UnderstandingLegendWindow extends VirtualWindow {
    constructor() {
        super(
            {
                title: "Understanding legend",
                width: 600,
                height: 400
            }
        );
        let content = new Div()

        function appendUnderstanding(content, title, description) {
            content.appendChild(new Div(new B(title)))
            content.appendChild(new Div(
                new Span(" -- "),
                new Span(description)
            ))
        }

        appendUnderstanding(content, "Unknown",
            "I know the term exists, but nothing more.")

        appendUnderstanding(content, "Recognized",
            "I recognize the name and its general context.")

        appendUnderstanding(content, "Understood",
            "I understand the definition and core idea.")

        appendUnderstanding(content, "Applied",
            "I can correctly use it in practice.")

        appendUnderstanding(content, "Internalized",
            "Using it is automatic and requires no conscious effort.")


        this.set_content(content.element())
    }
}