import {ActionType} from "../attributes/ActionType.js";
import {ValueElement} from "./ValueElement.js";
import {InputType} from "../attributes/InputType.js";

export class Input extends ValueElement {
    constructor(input_type = InputType.Text) {
        super("input")
        this.element().type = input_type.label;
        if (input_type === InputType.Checkbox) {
            this.css({
                transform: "scale(2)",
                marginLeft: "10px",
                marginRight: "10px",
                textAlign: "left",
            })
        } else {
            this.css({width: "250px"})
        }
        this.add_action_handler(ActionType.Reset, (self, ...args) => {
            self.clear_value();
            return true;
        });
    }

    set_placeholder(text) {
        this.element().placeholder = text
        return this
    }

    get_placeholder() {
        return this.element().placeholder
    }

}