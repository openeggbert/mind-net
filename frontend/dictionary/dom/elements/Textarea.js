import {ValueElement} from "./ValueElement.js";
import {ActionType} from "../attributes/ActionType.js";

export class Textarea extends ValueElement {
    constructor() {
        super("textarea")

        this.add_action_handler(ActionType.Reset, (self, ...args) => {
            self.clear_value();
            return true;
        });
    }

}