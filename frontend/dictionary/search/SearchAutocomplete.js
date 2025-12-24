import {Button} from "../dom/elements/Button.js";
import {_10PX} from "../styles/Styles.js";
import {EventType} from "../dom/attributes/EventType.js";
import {Autocomplete, defined} from "../../common.js";


class CloseButton extends Button {
    constructor(model, input, autocomplete = null) {
        super()

        this
            .set_title("Clear " + model)
            .set_html_unsafe("&times;")
            .styles().marginLeft(_10PX).end()
            .on(EventType.Click.label, (e => {
                event.preventDefault();
                if (defined(autocomplete)) {
                    autocomplete.reset()
                } else {
                    input.clear_value()
                }
            }))
        input._object.insert_after(this)
    }
}

export class SearchAutocomplete extends Autocomplete {
    constructor(win, model, input, input_min_length, entity, query_params = "") {
        super(
            input,
            input_min_length,
            entity,
            query_params,
            "title",
            "title_part",
            "search_end_" + model)

        this.clear_after_click = false
        this.box.style.positon = "fixed"
        this.box.style.willChange = "transform";
        this.win = win

        new CloseButton(model, input, this)
    }

    after_render(box, input) {
        box.style.position = "fixed";
        box.style.left = "0px";
        box.style.top = "0px";
        box.style.willChange = "transform";

        let i = 0
        runWhileShown(box, () => {
            const r = input.getBoundingClientRect();
            const b = box.getBoundingClientRect();
            box.style.transform =
                `translate3d(${r.left}px, ${r.top + 46}px, 0)`;
            // showInfo("win: " + this.win.left() + " " + this.win.top())
            // showInfo("box: " + b.x + " " + b.y)
            let boxY = Number(b.y)
            let winY = this.win.top()

            let wHeight = Number(this.win.height().slice(0, -2))
            let wTop = Number(this.win.top().slice(0, -2))
            let wBottom = wHeight + wTop

            // showInfo(wHeight + " " + wTop + " " + wBottom + " " + this.win.top())
            let over = boxY < (wTop + 40)
            let under = boxY > (wBottom)
            box.style.visibility = over || under ? "hidden" : "visible"
        });
    }

}

function runWhileShown(el, callback) {
    let running = true;

    function loop() {
        if (!running || el.style.display !== "block") return;

        callback();
        requestAnimationFrame(loop);
    }

    requestAnimationFrame(loop);

    return () => running = false; // optional destroy
}