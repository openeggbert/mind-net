export class DomElement {

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

        if (string_too && typeof child === "string") {
            return document.createTextNode(child)
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

    append_many(...children) {
        children.forEach(c => this.append(c));
        return this;
    }

    set_text(text) {
        this.element.textContent = text;
        return this;
    }

    set_html_unsafe(html) {
        this.element.innerHTML = html;
        return this;
    }
    css(styles) {
        Object.assign(this.element.style, styles);
        return this;
        // new DomElement("div").css({
        //     width: "100px",
        //     margin: "10px"
        // });

    }

    clear() {
        this.element.replaceChildren();
        return this;
    }

    on(event, handler, options) {
        this.element.addEventListener(event, handler, options);
        return this;
    }

    once(event, handler, options) {
        this.element.addEventListener(event, handler, { ...options, once: true });
        return this;
    }

    off(event, handler, options) {
        this.element.removeEventListener(event, handler, options);
        return this;
    }


    set_attr(name, value) {
        if (value === false || value === null || value === undefined) {
            this.element.removeAttribute(name);
        } else if (value === true) {
            this.element.setAttribute(name, "");
        } else {
            this.element.setAttribute(name, String(value));
        }
        return this;
    }

    attr(name) {
        return this.element.getAttribute(name);
    }

    remove_attr(name) {
        this.element.removeAttribute(name);
        return this;
    }

    data(key, value) {
        this.element.dataset[key] = value;
        return this;
    }


    add_class(cls) {
        this.element.classList.add(cls);
        return this;
    }

    remove_class(cls) {
        this.element.classList.remove(cls);
        return this;
    }

    toggle_class(cls, force) {
        this.element.classList.toggle(cls, force);
        return this;
    }

    replace_with(node) {
        this.element.replaceWith(
            node instanceof DomElement ? node.element : node
        );
    }

    insert_before(node) {
        this.element.before(
            node instanceof DomElement ? node.element : node
        );
        return this;
    }

    insert_after(node) {
        this.element.after(
            node instanceof DomElement ? node.element : node
        );
        return this;
    }

    clone(deep = true) {
        const clone = this.element.cloneNode(deep);
        return new DomElement(clone.tagName.toLowerCase()).replace_with(clone);
    }

    debug_label(label) {
        this.element.dataset.debug = label;
        return this;
    }

    assert_attached() {
        if (!this.element.isConnected) {
            console.warn("DomElement not attached:", this.element);
        }
        return this;
    }

    hide() {
        this.element.style.display = "none";
        return this;
    }

    show(display = "") {
        this.element.style.display = display;
        return this;
    }

    focus() {
        this.element.focus();
        return this;
    }

    blur() {
        this.element.blur();
        return this;
    }

    value(v) {
        if (v === undefined) return this.element.value;
        this.element.value = v;
        return this;
    }

    parent() {
        return this.element.parentElement;
    }

    children() {
        return Array.from(this.element.children);
    }

    describe() {
        return {
            tag: this.element.tagName,
            id: this.element.id,
            classes: [...this.element.classList],
            attached: this.element.isConnected
        };
    }

    assert_type(tag) {
        if (this.element.tagName.toLowerCase() !== tag.toLowerCase()) {
            console.warn(`Expected <${tag}>, got <${this.element.tagName}>`, this.element);
        }
        return this;
    }




}