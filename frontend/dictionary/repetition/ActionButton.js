import {Button} from "../dom/elements/Button.js";

export class ActionButton extends Button {
    constructor(text) {
        super(text)
            .add_class("repetition_button")
            .add_class("action-btn")
    }
}