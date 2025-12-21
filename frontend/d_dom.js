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

    appendChild(child) {
        this.element.appendChild(child)
    }
}