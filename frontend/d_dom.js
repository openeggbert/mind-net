export class DomElement {
    #el = null
    static create_element(name) {
        return document.createElement(name);
    }
    constructor(name) {
        this.element = DomElement.create_element(name)
        this.style = this.element.style
        this.element_object = this
    }

    set_id(id) {
        this.element.id = id
        return this
    }
    get_id() {
        return this.element.id
    }

    #find_node(child, string_too = true) {
        if(child instanceof Node){
            return child
        }

        if(child instanceof DomElement){
            return child.element
        }
        if(string_too && (String(typeof child)) === "string"){
            return child.element
        }
        throw new Error("Unsupported object type: " + typeof child)
    }
    appendChild(child) {
        this.element.appendChild(this.#find_node(child))
        return this
    }
    append(child) {
        this.element.append(this.#find_node(child, true))
        return this
    }
}