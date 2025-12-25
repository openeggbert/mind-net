// ========================================
// Window
// ========================================

import {showError} from "../../dom.js";
import {Div} from "../dom/elements/Div.js";
import {Span} from "../dom/elements/Span.js";

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

function get_inner_width() {
    return window.innerWidth;
}

function get_inner_height() {
    return window.innerHeight
}

function clampTop(y) {
    return Math.max(0, y);
}


export class VirtualWindow {
    #root;
    #header;
    #title;
    #content;
    #minimize;
    #minimized = false;
    #restore;
    #restoreWidth = "800px";
    #restoreHeight = "600px";
    #close;
    #userPositioned = false;
    #lastTapTime = 0;
    #maximized = false

    #dragging = false;
    #offX = 0;
    #offY = 0;
    #restoreLeft;
    #restoreTop;

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
        this.created_at = Date.now()

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
        this.#close.onclick = () => this.close();

        this.#minimize = document.createElement("button");
        this.#minimize.className = "window-minimize";
        this.#minimize.textContent = "➖";
        this.#minimize.style.marginRight = "10px"
        this.#minimize.onclick = () => this.minimize();

        this.#restore = document.createElement("button");
        this.#restore.className = "window-restore";
        this.#restore.textContent = "🗖";
        this.#restore.style.marginRight = "10px"
        this.#restore.style.display = "none"
        this.#restore.onclick = () => this.restore();

        // mouse
        this.#header.addEventListener("dblclick", e => {
            e.preventDefault();
            this.toggle_maximize();
        });

// touch
        this.#header.addEventListener("touchend", e => {
            const now = Date.now();
            const delta = now - this.#lastTapTime;

            if (delta > 0 && delta < 300) {
                e.preventDefault();
                this.toggle_maximize();
                this.#lastTapTime = 0;
            } else {
                this.#lastTapTime = now;
            }
        });

        this.#header.append(this.#title, new Span(this.#minimize, this.#restore, this.#close).element());

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

        this.#root.addEventListener("pointerdown", e => {
            if (e.target.closest("button")) return;
            this.focus();
        });

        this.#header.addEventListener("mousedown", e => {
            if (e.buttons !== 1) return;

            this.focus();

            this.#root.style.opacity = "0.85";

            if (!this.#userPositioned) {
                this.#userPositioned = true;
                this._detachFromCenter();
            }

            const rect = this.#root.getBoundingClientRect();
            const offX = e.clientX - rect.left;
            const offY = e.clientY - rect.top;

            const move = ev => {

                if (this.#maximized && !this.#minimized) return
                this.#root.style.left = (ev.clientX - offX) + "px";
                const newTop = clampTop(ev.clientY - offY);
                this.#root.style.top = newTop + "px";

            };

            const up = () => {
                document.removeEventListener("mousemove", move);
                document.removeEventListener("mouseup", up);
                document.body.style.userSelect = "";
                this.#root.style.opacity = "1";
            };

            document.body.style.userSelect = "none";
            document.addEventListener("mousemove", move);
            document.addEventListener("mouseup", up);
        });

        window.addEventListener("resize", () => {
            const rect = this.#root.getBoundingClientRect();
            if (rect.top < 0) {
                this.#root.style.top = "0px";
            }
        });

        if (visible) {
            this.show();
        }

    }

    get_created_at() {
        return this.created_at
    }
    // ===============================
    // Public API
    // ===============================

    minimize() {
        if (this.#minimized) return;

        this.#minimize.style.display = "none";
        this.#restore.style.display = "inline";

        const s = this.#root.style;
        this.#restoreWidth = s.width;
        this.#restoreHeight = s.height;
        this.#restoreLeft = this.#root.style.left
        this.#restoreTop = this.#root.style.top

        this.#root.style.resize = "none";

        this.resize_from_strings("fit-content", "38px");

        this.#minimized = true;
    }

    toggle_maximize() {
        let maximized = this.#maximized

        if (maximized) {
            //showInfo("Going to restore")
            this.restore()
        } else {
            //showInfo("Going to maximize")
            this.maximize()
        }
    }

    maximize() {
        if (this.#minimized) return;
        if (this.#maximized) return;

        const s = this.#root.style;
        this.#restoreWidth = s.width;
        this.#restoreHeight = s.height;
        this.#restoreLeft = this.#root.style.left
        this.#restoreTop = this.#root.style.top

        this.#root.style.left = "0"
        this.#root.style.top = "0"
        this.#root.style.resize = "none";

        this.resize(get_inner_width(), get_inner_height());
        this.#maximized = true
    }

    restore() {
        if (!(this.#minimized || this.#maximized)) return;

        this.#minimize.style.display = "inline";
        this.#restore.style.display = "none";

        this.resize_from_strings(this.#restoreWidth, this.#restoreHeight);

        this.#root.style.resize = "both";

        this.#root.style.left = this.#restoreLeft
        const restoreTop = parseInt(this.#restoreTop ?? "0", 10);
        this.#root.style.top = clampTop(restoreTop) + "px";

        if (this.#maximized && !this.#minimized) {
            this.#maximized = false;
        }
        if (this.#maximized && this.#minimized) {
            this.#root.style.resize = "none";
        }
        if (this.#minimized) this.#minimized = false;
    }

    _setActive(active) {
        this.#root.classList.toggle("active", active);
    }

    set_content_padding(value = 0) {
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
                const top = (window.innerHeight - h) / 2;
                this.#root.style.top = clampTop(top) + "px";

                this.focus();
            });
        } else {
            this.focus();
        }

        return this;
    }

    hide() {
        this.#root.style.display = "none";
        return this;
    }


    _detachFromCenter() {
        const rect = this.#root.getBoundingClientRect();

        this.#root.style.left = rect.left + "px";
        this.#root.style.top = rect.top + "px";

        this.#root.style.transform = "none";
    }

    left() {
        return this.#root.style.left
    }

    top() {
        return this.#root.style.top
    }

    right() {
        return this.#root.style.right
    }

    bottom() {
        return this.#root.style.top
    }

    height() {
        return this.#root.style.height
    }

    width() {
        return this.#root.style.width
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

    resize_from_strings(width, height) {
        if (width !== null) this.#root.style.width = width;
        if (height !== null) this.#root.style.height = height;
        return this;
    }

    center(x = null, y = null) {
        const rect = this.#root.getBoundingClientRect();
        this.#root.style.left = (x ?? (window.innerWidth - rect.width) / 2) + "px";
        const top = y ?? (window.innerHeight - rect.height) / 2;
        this.#root.style.top = clampTop(top) + "px";

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

export function showWindowFromUrl(title, url) {
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
    iframe.style.border = "0"
    iframe.style.padding = "0"
    iframe.style.margin = "0"

    div.appendChild(iframe)
    win.set_content(div.element())
    div.style().height = "100%";
    win.set_content_padding(0)
    win.show()

    iframe.addEventListener("load", () => {
        const iframeDoc = iframe.contentDocument;
        const titleEl = iframeDoc.querySelector("title");

        if (!titleEl) return;

        const observer = new MutationObserver(() => {
            console.log("New title:", iframeDoc.title);

            onIframeTitleChanged(iframeDoc.title);
        });

        observer.observe(titleEl, {
            childList: true
        });
    });

    function onIframeTitleChanged(newTitle) {
        console.log("Title was change:", newTitle);
        win.set_title(newTitle)
    }
}