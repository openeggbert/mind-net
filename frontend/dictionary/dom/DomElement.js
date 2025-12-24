import {Styles} from "../styles/Styles.js";
import {get_element} from "../../dom.js";
import {ActionType} from "./attributes/ActionType.js";
import {Display} from "../styles/properties/Display";
import {gen_enum_id} from "../enums/EnumFunctions";

/**
 * DomElement
 * ==========
 *
 * A thin, explicit wrapper around native DOM elements.
 *
 * PURPOSE
 * -------
 * DomElement provides a small, fluent API over the native DOM without
 * introducing state, lifecycle management, rendering, or virtual DOM.
 *
 * It is designed to:
 *   - reduce boilerplate when constructing DOM trees
 *   - improve readability of imperative UI code
 *   - enable gradual refactoring of legacy / procedural DOM code
 *
 * NON-GOALS
 * ---------
 * DomElement is NOT:
 *   - a UI framework
 *   - a component system
 *   - a templating engine
 *   - a virtual DOM
 *   - a state container
 *
 * DomElement intentionally does NOT:
 *   - track children
 *   - manage lifecycle
 *   - perform diffing or updates
 *   - hide the underlying DOM element
 *
 * DESIGN PRINCIPLES
 * -----------------
 * 1. Thin abstraction:
 *    The underlying HTMLElement is always accessible via `.element`.
 *
 * 2. No state duplication:
 *    DOM is the single source of truth.
 *
 * 3. Fluent API:
 *    All mutating methods return `this`.
 *
 * 4. Explicit over clever:
 *    No magic rendering, no implicit updates.
 *
 * 5. Long-term stability:
 *    Depends only on standard DOM APIs.
 *
 * USAGE EXAMPLE
 * -------------
 * const button = new DomElement("button")
 *     .set_text("Search")
 *     .add_class("primary")
 *     .on("click", () => runSearch());
 *
 * container.append(button);
 */
export class DomElement {
    #element
    #style
    #styles
    #action_handlers = new Map()

    /**
     * Create a native DOM element.
     *
     * @param {string} name - Tag name (e.g. "div", "span", "input")
     * @returns {HTMLElement}
     */
    static create_element(name) {
        return document.createElement(name);
    }

    /**
     * Construct a DomElement wrapper.
     *
     * @param {string} name - Tag name of the element to create
     */
    constructor(name) {
        /**
         * The underlying native DOM element.
         * This is the single source of truth.
         *
         * @type {HTMLElement}
         */
        this.#element = DomElement.create_element(name);

        /**
         * Direct reference to element.style for convenience.
         *
         * @type {CSSStyleDeclaration}
         */
        this.#style = this.#element.style;

        /**
         * Back-reference from DOM to wrapper.
         * Useful for debugging or introspection.
         */
        this.#element._object = this;

        this.#styles = new Styles(this)
    }

    #get_handler_key(action) {
        return typeof action === "object" ? action.label : action
    }

    add_action_handler(action, handler) {
        let key = this.#get_handler_key(action)
        this.#action_handlers.set(key, handler)
        return this
    }

    has_action_handler(action) {
        let key = this.#get_handler_key(action)
        return this.#action_handlers.has(key)
    }

    is_missing_action_handler(action) {
        let key = this.#get_handler_key(action)
        return !this.#action_handlers.has(key)
    }

    get_action_handler(action) {
        let key = this.#get_handler_key(action)
        return this.#action_handlers.get(key) ?? null
    }

    remove_action_handler(action) {
        let key = this.#get_handler_key(action)
        this.#action_handlers.delete(key)
        return this
    }

    execute_action(action, ...args) {
        let key = this.#get_handler_key(action)
        console.log(`Going to execute action: ${key}`)
        const fn = this.#action_handlers.get(key)
        if (!fn) return false
        console.log(`Action was found: ${key}`)
        return fn(this, ...args)
    }

    set_default_values() {
        return this.execute_action(ActionType.Reset)
    }

    element() {
        return this.#element
    }

    style() {
        return this.#element.style
    }

    styles() {
        return this.#styles
    }

    remove_element() {
        this.#element.remove();
        return this
    }

    /**
     * Set element id.
     *
     * @param {string} id
     * @returns {DomElement}
     */
    set_id(id) {
        this.#element.id = id;
        return this;
    }

    /**
     * Get element id.
     *
     * @returns {string}
     */
    get_id() {
        return this.#element.id;
    }

    /**
     * Resolve supported child types into a DOM Node.
     *
     * Supported types:
     *   - Node
     *   - DomElement
     *   - string (converted to TextNode)
     *
     * @private
     * @param {*} child
     * @param {boolean} string_too
     * @returns {Node}
     */
    #find_node(child, string_too = true) {
        if (child instanceof Node) {
            return child;
        }

        if (child instanceof DomElement) {
            return child.#element;
        }

        if (string_too && typeof child === "string") {
            return document.createTextNode(child);
        }

        if (typeof child === "function") {
            throw new Error("Function passed as DOM child. Did you forget to call .end()?");
        }

        throw new Error("Unsupported object type: " + typeof child);
    }

    /**
     * Append a child node (Node, DomElement, or string).
     *
     * @param {*} child
     * @returns {DomElement}
     */
    appendChild(child) {
        this.#element.appendChild(this.#find_node(child));
        return this;
    }

    /**
     * Append a child using Element.append semantics.
     *
     * @param {*} child
     * @returns {DomElement}
     */
    append(child) {
        console.log("append " + JSON.stringify(child))
        if (this.#find_node(child, true).id === "progress-fill") console.log("calling for fill_search")
        this.#element.append(this.#find_node(child, true));
        return this;
    }

    /**
     * Append multiple children.
     *
     * @param  {...any} children
     * @returns {DomElement}
     */
    append_many(...children) {
        children.forEach(c => this.append(c));
        return this;
    }

    /**
     * Set text content.
     *
     * @param {string} text
     * @returns {DomElement}
     */
    set_text(text) {
        this.#element.textContent = text;
        return this;
    }

    clear_text() {
        return this.set_text("")
    }

    get_text() {
        return this.#element.textContent
    }

    get_text_as_number() {
        return Number(this.get_text())
    }


    /**
     * Set innerHTML.
     *
     * WARNING:
     * This method is intentionally marked as unsafe.
     * The caller is responsible for sanitization.
     *
     * @param {string} html
     * @returns {DomElement}
     */
    set_html_unsafe(html) {
        this.#element.innerHTML = html;
        return this;
    }

    clear_html() {
        return this.set_html_unsafe("")
    }

    /**
     * Apply multiple CSS styles at once.
     *
     * @param {Object<string, string>} styles
     * @returns {DomElement}
     */
    css(styles) {
        Object.assign(this.#element.style, styles);
        return this;
        // new DomElement("div").css({
        //     width: "100px",
        //     margin: "10px"
        // });
    }

    /**
     * Remove all child nodes.
     *
     * @returns {DomElement}
     */
    clear_children() {
        this.#element.replaceChildren();
        return this;
    }

    /**
     * Add an event listener.
     *
     * @param {string} event
     * @param {Function} handler
     * @param {Object} [options]
     * @returns {DomElement}
     */
    on(event, handler, options) {
        this.#element.addEventListener(
            event,
            handler,
            options);
        return this;
    }

    /**
     * Add a one-time event listener.
     *
     * @param {string} event
     * @param {Function} handler
     * @param {Object} [options]
     * @returns {DomElement}
     */
    once(event, handler, options) {
        this.#element.addEventListener(event, handler, {...options, once: true});
        return this;
    }

    /**
     * Remove an event listener.
     *
     * @param {string} event
     * @param {Function} handler
     * @param {Object} [options]
     * @returns {DomElement}
     */
    off(event, handler, options) {
        this.#element.removeEventListener(event, handler, options);
        return this;
    }

    /**
     * Set or remove an attribute with boolean-safe semantics.
     *
     * @param {string} name
     * @param {*} value
     * @returns {DomElement}
     */
    set_attr(name, value) {
        if (value === false || value === null || value === undefined) {
            this.#element.removeAttribute(name);
        } else if (value === true) {
            this.#element.setAttribute(name, "");
        } else {
            this.#element.setAttribute(name, String(value));
        }
        return this;
    }

    /**
     * Get an attribute value.
     *
     * @param {string} name
     * @returns {string|null}
     */
    attr(name) {
        return this.#element.getAttribute(name);
    }

    /**
     * Remove an attribute.
     *
     * @param {string} name
     * @returns {DomElement}
     */
    remove_attr(name) {
        this.#element.removeAttribute(name);
        return this;
    }

    /**
     * Set a data-* attribute.
     *
     * @param {string} key
     * @param {string} value
     * @returns {DomElement}
     */
    data(key, value) {
        this.#element.dataset[key] = value;
        return this;
    }

    /**
     * Add a CSS class.
     *
     * @param {string} cls
     * @returns {DomElement}
     */
    add_class(cls) {
        this.#element.classList.add(cls);
        return this;
    }

    /**
     * Remove a CSS class.
     *
     * @param {string} cls
     * @returns {DomElement}
     */
    remove_class(cls) {
        this.#element.classList.remove(cls);
        return this;
    }

    /**
     * Toggle a CSS class.
     *
     * @param {string} cls
     * @param {boolean} [force]
     * @returns {DomElement}
     */
    toggle_class(cls, force) {
        this.#element.classList.toggle(cls, force);
        return this;
    }

    /**
     * Replace this element with another node.
     *
     * @param {Node|DomElement} node
     */
    replace_with(node) {
        this.#element.replaceWith(
            node instanceof DomElement ? node.#element : node
        );
    }

    /**
     * Insert a node before this element.
     *
     * @param {Node|DomElement} node
     * @returns {DomElement}
     */
    insert_before(node) {
        this.#element.before(
            node instanceof DomElement ? node.#element : node
        );
        return this;
    }

    /**
     * Insert a node after this element.
     *
     * @param {Node|DomElement} node
     * @returns {DomElement}
     */
    insert_after(node) {
        this.#element.after(
            node instanceof DomElement ? node.#element : node
        );
        return this;
    }

    /**
     * Clone the underlying DOM element.
     *
     * @param {boolean} deep
     * @returns {DomElement}
     */
    clone(deep = true) {
        const clone = this.element.cloneNode(deep);
        return new DomElement(clone.tagName.toLowerCase()).replace_with(clone);
    }

    /**
     * Attach a debug label (visible in DevTools).
     *
     * @param {string} label
     * @returns {DomElement}
     */
    debug_label(label) {
        this.#element.dataset.debug = label;
        return this;
    }

    /**
     * Warn if the element is not attached to the DOM.
     *
     * @returns {DomElement}
     */
    assert_attached() {
        if (!this.#element.isConnected) {
            console.warn("DomElement not attached:", this.#element);
        }
        return this;
    }

    #original_display = null

    hide() {
        let s = this.#element.style;
        if (s.display === Display.None.label) return this
        this.#original_display = s.display
        s.display = Display.None.label
        return this
    }

    show(d = null) {
        if (d) {
            this.styles().display(d);
            return this
        }
        if (this.#original_display) {
            this.styles().display(this.#original_display);
            return this
        }
        this.styles().display(Display.Block)
        return this
    }

    focus() {
        this.#element.focus();
        return this;
    }

    blur() {
        this.#element.blur();
        return this;
    }

    parent() {
        return this.#element.parentElement;
    }

    children() {
        return Array.from(this.#element.children);
    }

    describe() {
        return {
            tag: this.#element.tagName,
            id: this.#element.id,
            classes: [...this.#element.classList],
            attached: this.#element.isConnected
        };
    }

    assert_type(tag) {
        if (this.#element.tagName.toLowerCase() !== tag.toLowerCase()) {
            console.warn(`Expected <${tag}>, got <${this.#element.tagName}>`, this.element);
        }
        return this;
    }
}

export function find_dom_element(id) {
    let el = get_element(id)
    if (!el) return null
    let _object = el["_object"]

    if (!_object) return null
    return el._object
}

export function find_enum(text, instance) {
    return find_dom_element(gen_enum_id(text, instance))
}