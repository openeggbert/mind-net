import {Div} from "../dom/elements/Div.js";
import {Label} from "../dom/elements/Label.js";
import {ActionType} from "../dom/attributes/ActionType.js";

export class FormRow extends Div {
    constructor(label_text, control) {
        super(new Label(label_text), control);
        this.add_class("form-row")
        this.set_id("form-row_" + label_text.toLowerCase())
        this.control = control
        this.add_action_handler(ActionType.Reset, (self, ...args) => {
            self.control.set_default_values();
            return true;
        })

    }

    get_control() {
        return this.control
    }
}