import {ActionType} from "../attributes/ActionType.js";
import {Input} from "./Input.js";
import {InputType} from "../attributes/InputType.js";

export class Checkbox extends Input {
    constructor() {
        super(InputType.Checkbox);
        this.add_action_handler(ActionType.Reset, (self, ...args) => {
            self.uncheck();
            return true;
        });
    }

    is_checked() {
        return this.element().checked
    }

    set_checked(value) {
        this.element().checked = value
        return this
    }

    check() {
        this.set_checked(true)
        return this
    }

    uncheck() {
        this.set_checked(false)
        return this
    }
}