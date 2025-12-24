import {ActionType} from "../dom/attributes/ActionType.js";
import {Form} from "../dom/elements/Form.js";

export class SearchForm extends Form {
    #controls = []

    constructor() {
        super();
        this
            .set_id("form_search")
    }

    add_control(control) {
        super.appendChild(control)
        this.#controls.push(control)
        this.add_action_handler(ActionType.Reset, (self, ...args) => {
            let success = true
            this.#controls.forEach(e => {
                let success_e = e.set_default_values()
                if (!success_e) success = false
            })
            return success
        })

    }

    appendChild(child) {
        throw new Error("Function appendChild is not supported. Use add_control instead.")
    }

}