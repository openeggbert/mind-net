import {Button} from "../dom/elements/Button.js";
import {EventType} from "../dom/attributes/EventType.js";

export class RatingButton extends Button {

    constructor(text, model) {
        super(text)
            .add_class("repetition_button")
            .add_class("rating-btn")
        this.model = model
        model.rating_buttons.push(this)
        this.on(EventType.Click.label, () => {
            model.selected_grade = Number(text)
            model.rating_buttons.forEach((e) => {
                e.remove_class("rating-btn-selected")
            })
            this.add_class("rating-btn-selected")
            model.answer_change_count++
        })
    }
}