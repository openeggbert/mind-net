import {Button} from "../dom/elements/Button.js";

export class RepetitionButton extends Button {
    constructor(text) {
        super(text)
            .add_class("repetition_button")
            .add_class("common-repetition_button")
        this.styles().backgroundColor("rgb(85, 85, 85)").end()
    }
}