
// ========================================
// Window
// ========================================

import {Div} from "./d_dom.js";
import {showError} from "./dom.js";

let activeWindow = null;
let topZ = 1000;

export function bringToFront(win) {
    topZ++;

    if (activeWindow && activeWindow !== win) {
        activeWindow._setActive(false);
    }

    activeWindow = win;

    win._setZ(topZ);
    win._setActive(true);
}


export class VirtualWindow {
    #root;
    #header;
    #title;
    #content;
    #close;
    #userPositioned = false;

    #dragging = false;
    #offX = 0;
    #offY = 0;

    constructor({
                    title = "Window",
                    visible = false,
                    width = null,
                    height = null
                } = {}) {

        // ===============================
        // Root
        // ===============================
        this.#root = document.createElement("div");
        this.#root.className = "window_container";

        if (width !== null) {
            this.#root.style.width = width + "px";
        }
        if (height !== null) {
            this.#root.style.height = height + "px";
        }

        // ===============================
        // Header
        // ===============================
        this.#header = document.createElement("header");
        this.#header.className = "window_container-header";

        this.#title = document.createElement("span");
        this.#title.textContent = title;

        this.#close = document.createElement("button");
        this.#close.className = "window-close";
        this.#close.textContent = "✖";
        this.#close.onclick = () => this.hide();

        this.#header.append(this.#title, this.#close);

        // ===============================
        // Content
        // ===============================
        this.#content = document.createElement("div");
        this.#content.className = "window_container-content";

        this.#root.append(this.#header, this.#content);
        document.body.appendChild(this.#root);

        // ===============================
        // Drag support
        // ===============================
        this.#install_drag();

        this.#header.addEventListener("mousedown", e => {
            if (e.buttons !== 1) return;

            this.focus();

            if (!this.#userPositioned) {
                this.#userPositioned = true;
                this._detachFromCenter();
            }

            const rect = this.#root.getBoundingClientRect();
            const offX = e.clientX - rect.left;
            const offY = e.clientY - rect.top;

            const move = ev => {
                this.#root.style.left = (ev.clientX - offX) + "px";
                this.#root.style.top  = (ev.clientY - offY) + "px";
            };

            const up = () => {
                document.removeEventListener("mousemove", move);
                document.removeEventListener("mouseup", up);
                document.body.style.userSelect = "";
            };

            document.body.style.userSelect = "none";
            document.addEventListener("mousemove", move);
            document.addEventListener("mouseup", up);
        });


        if (visible) {
            this.show();
        }

    }

    // ===============================
    // Public API
    // ===============================

    _setActive(active) {
        this.#root.classList.toggle("active", active);
    }

    set_content_padding(value) {
        this.#content.style.padding = value
    }

    get_content_padding(value) {
        return this.#content.style.padding
    }

    show() {
        this.#root.style.display = "block";

        if (!this.#userPositioned) {
            requestAnimationFrame(() => {
                const w = this.#root.offsetWidth;
                const h = this.#root.offsetHeight;
                this.#root.style.left = ((window.innerWidth - w) / 2) + "px";
                this.#root.style.top  = ((window.innerHeight - h) / 2) + "px";
            });
        }

        this.focus();
        return this;
    }

    hide() {
        this.#root.style.display = "none";
        return this;
    }


    _detachFromCenter() {
        const rect = this.#root.getBoundingClientRect();

        this.#root.style.left = rect.left + "px";
        this.#root.style.top  = rect.top  + "px";

        this.#root.style.transform = "none";
    }


    destroy() {
        this.#root.remove();
    }
    close() {
        this.hide()
        this.destroy()
    }

    set_title(text) {
        this.#title.textContent = text;
        return this;
    }

    clear_content() {
        this.#content.innerHTML = "";
        return this;
    }

    set_content(node) {
        this.clear_content();
        if (node instanceof Node) {
            this.#content.appendChild(node);
        }
        return this;
    }

    resize(width, height) {
        if (width !== null) this.#root.style.width = width + "px";
        if (height !== null) this.#root.style.height = height + "px";
        return this;
    }

    center(x = null, y = null) {
        const rect = this.#root.getBoundingClientRect();
        this.#root.style.left = (x ?? (window.innerWidth - rect.width) / 2) + "px";
        this.#root.style.top = (y ?? (window.innerHeight - rect.height) / 2) + "px";
        return this;
    }

    get content() {
        return this.#content;
    }

    focus() {
        bringToFront(this);
    }

    _setZ(z) {
        this.#root.style.zIndex = z;
    }

    // ===============================
    // Dragging
    // ===============================

    #install_drag() {
    }

}

export function showWindowFrom(title, url) {
    if (url === undefined) {
        showError("url is required");
        return;
    }
    let win = new VirtualWindow(
        {
            title: title,
            width: screen.width > 1000 ? 1000 : screen.width - 100,
            height: 800
        }
    )
    let div = new Div()

    let iframe = document.createElement("iframe");
    iframe.src = url;
    iframe.scroling = "no"
    iframe.style.display = "block";
    iframe.style.width = "100%";
    iframe.style.height = "100%";
    iframe.style.border="0"
    iframe.style.padding = "0"
    iframe.style.margin = "0"

    div.appendChild(iframe)
    win.set_content(div.element())
    div.style().height = "100%";
    win.set_content_padding(0)
    win.show()

}