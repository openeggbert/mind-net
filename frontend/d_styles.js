export const _10PX = "10px"

export class Styles {
    #dom_element
    #style
    constructor(dom_element) {
        this.#dom_element = dom_element
        this.#style = this.#dom_element.style
    }

    #resolve_value(value) {
        if (typeof value === "string") {
            return value
        }

        if (typeof value === "object" && value !== null) {
            if (typeof value.label === "string") {
                return value.label;
            }
            throw new Error("Entity object has no label");
        }

        throw new Error(`Invalid value type: ${typeof value}`);
    }

    #set(key, value) {
        this.#style[key] = this.#resolve_value(value)
        return this
    }
    marginLeft(value) {return this.#set("marginLeft", value)}
    marginRight(value) {return this.#set("marginRight", value)}
    color(value) {return this.#set("color", value)}
    width(value) {return this.#set("width", value)}
    textDecoration(value) {return this.#set("textDecoration", value)}
    cursor(value) {return this.#set("cursor", value)}
    end() {
        return this.#dom_element
    }
}
