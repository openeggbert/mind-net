export const _5PX = "5px"
export const _10PX = "10px"
export const _20PX = "20px"

export class Styles {
    #dom_element
    #style
    constructor(dom_element) {
        this.#dom_element = dom_element
        this.#style = this.#dom_element.style()
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
    marginTop(value) {return this.#set("marginTop", value)}
    marginRight(value) {return this.#set("marginRight", value)}
    marginBottom(value) {return this.#set("marginBottom", value)}
    marginLeft(value) {return this.#set("marginLeft", value)}
    paddingTop(value) {return this.#set("paddingTop", value)}
    paddingRight(value) {return this.#set("paddingRight", value)}
    paddingBottom(value) {return this.#set("paddingBottom", value)}
    paddingLeft(value) {return this.#set("paddingLeft", value)}
    margin(value) {return this.#set("margin", value)}
    padding(value) {return this.#set("padding", value)}
    color(value) {return this.#set("color", value)}
    background(value) {
        return this.#set("background", value)
    }
    width(value) {return this.#set("width", value)}
    textDecoration(value) {return this.#set("textDecoration", value)}
    cursor(value) {return this.#set("cursor", value)}
    display(value) {return this.#set("display", value)}
    transform(value) {return this.#set("transform", value)}
    textAlign(value) {return this.#set("textAlign", value)}
    end() {
        return this.#dom_element
    }
}
