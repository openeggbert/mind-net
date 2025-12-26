// ========================================
// Window
// ========================================

import {showError, showInfo} from "../../dom.js";
import {Div} from "../dom/elements/Div.js";
import {Span} from "../dom/elements/Span.js";

let activeWindow = null;
let topZ = 1000;
let allWindows = new Set();
let overviewMode = false;
let lastWindowPosition = null;
const CASCADE_OFFSET_X = 20;
const CASCADE_OFFSET_Y = 20;
const OVERVIEW_HOVER_SCALE = 1.08;
const OVERVIEW_TRANSITION = "transform 0.22s cubic-bezier(0.22, 1, 0.36, 1)";
const OVERVIEW_ANIM_MS = 380;
const OVERVIEW_EASING = "cubic-bezier(0.22, 1, 0.36, 1)";

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
    #close_big
    #userPositioned = false;
    #lastTapTime = 0;
    #maximized = false

    #dragging = false;
    #offX = 0;
    #offY = 0;
    #restoreLeft;
    #restoreTop;
    #overview = false;
    #overviewRect = null;
    #overviewScale;

    #overviewPointerHandler = (e) => {
        if (e.target.closest("button")) {
            e.preventDefault();
            e.stopPropagation();
            return;
        }

        e.preventDefault();
        e.stopPropagation();

        if (this.#minimized) {
            this.restore();
        }

        VirtualWindow.exitOverview();
        this.focus();
    };

    static enterOverview() {
        if (overviewMode) return;
        overviewMode = true;

        for (const win of allWindows) {
            win._enterOverview();
        }

        VirtualWindow._reflowOverview();
    }

    static exitOverview() {
        if (!overviewMode) return;
        overviewMode = false;

        for (const win of allWindows) {
            win._exitOverview();
        }
    }

    static _reflowOverview() {
        if (!overviewMode) return;

        const wins = Array.from(allWindows);
        const total = wins.length;
        if (total === 0) return;

        const cols = Math.ceil(Math.sqrt(total));
        const rows = Math.ceil(total / cols);
        const gap = 20;

        const cellW = window.innerWidth / cols;
        const cellH = window.innerHeight / rows;

        wins.forEach((win, index) => {
            win._updateOverviewLayout(index, cellW, cellH, gap);
        });
    }

    constructor({
                    title = "Window",
                    visible = false,
                    width = null,
                    height = null
                } = {}) {

        // ===============================
        // Root
        // ===============================
        allWindows.add(this);
        this.#root = document.createElement("div");
        this.#root.className = "window_container";
        this.#root.addEventListener("mouseenter", () => {
            if (!this.#overview) return;

            this.#root.style.transition = OVERVIEW_TRANSITION;
            this._applyOverviewTransform(OVERVIEW_HOVER_SCALE);

            const s = this.#overviewScale || 1;
            this.#close_big.style.transform =
                `scale(${1.4 / s}) translate(-10px, 15px)`;
            this.#close_big.style.display = "inline-block";
            this.#root.style.zIndex = 2000;
        });


        this.#root.addEventListener("mouseleave", () => {
            if (!this.#overview) return;

            this.#root.style.transition = OVERVIEW_TRANSITION;
            this._applyOverviewTransform(1);

            this.#close_big.style.transform = "";
            this.#close_big.style.display = "none";
            this.#root.style.zIndex = 1000;
        });


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

        this.#close_big = document.createElement("button");
        this.#close_big.className = "window-close";
        this.#close_big.textContent = "✖";
        this.#close_big.onclick = () => this.close();
        this.#close_big.style.display = "none"
        this.#close_big.style.backgroundColor = "#333"
        this.#close_big.style.color = "#ddd"
        this.#close_big.style.borderRadius = "25px"
        this.#close_big.style.width = "35px"
        this.#close_big.style.height = "35px"
        this.#close_big.style.fontWeight = "normal"
        this.#close_big.style.fontSize = "100%"

        this.#close_big.addEventListener("mouseenter", () => {
            if (!this.#overview) return;
            this.#close_big.style.backgroundColor = "#555"
        });

        this.#close_big.addEventListener("mouseleave", () => {
            if (!this.#overview) return;
            this.#close_big.style.backgroundColor = "#333"
        });

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

        this.#header.append(this.#title, new Span(this.#minimize, this.#restore, this.#close, this.#close_big).element());

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

    get_title_dataset() {
        return this.#title.dataset
    }
    _applyOverviewTransform(extraScale = 1) {
        const r = this.#overviewRect;
        if (!r) return;

        const scale = (this.#overviewScale ?? 1) * extraScale;

        const cols = Math.ceil(Math.sqrt(allWindows.size));
        const index = Array.from(allWindows).indexOf(this);

        const col = index % cols;
        const row = Math.floor(index / cols);

        const gap = 20;
        const cellW = window.innerWidth / cols;
        const cellH = window.innerHeight / Math.ceil(allWindows.size / cols);

        const cellX = col * cellW + gap;
        const cellY = row * cellH + gap;

        const innerW = cellW - gap * 2;
        const innerH = cellH - gap * 2;

        const targetCx = cellX + innerW / 2;
        const targetCy = cellY + innerH / 2;

        const srcCx = r.left + r.width / 2;
        const srcCy = r.top + r.height / 2;

        const dx = Math.round(targetCx - srcCx);
        const dy = Math.round(targetCy - srcCy);

        this.#root.style.transform =
            `translate(${dx}px, ${dy}px) scale(${scale})`;
    }

    _enterOverview() {
        if (this.#overview) return;

        this.#root.style.display = "block";
        this.#root.style.transition =
            "left 0.25s ease, top 0.25s ease, width 0.25s ease, height 0.25s ease";

        const rect = this.#root.getBoundingClientRect();
        this.#overviewRect = {
            left: rect.left,
            top: rect.top,
            width: rect.width,
            height: rect.height
        };

        this.#root.classList.add("overview");
        this.#overview = true;

        this.#root.addEventListener(
            "pointerdown",
            this.#overviewPointerHandler,
            true
        );
    }


    _exitOverview() {
        if (!this.#overview) return;

        this.#root.style.transition =
            `transform ${OVERVIEW_ANIM_MS}ms ${OVERVIEW_EASING}`;
        this.#root.style.transform = "none";

        this.#root.classList.remove("overview");
        this.#overview = false;

        this.#root.removeEventListener(
            "pointerdown",
            this.#overviewPointerHandler,
            true
        );
        this.#close_big.style.display = "none"

        setTimeout(() => {
            const r = this.#overviewRect;
            if (!r) return;

            this.#root.style.transition = "none";
            this.#root.style.left   = r.left   + "px";
            this.#root.style.top    = r.top    + "px";
            this.#root.style.width  = r.width  + "px";
            this.#root.style.height = r.height + "px";

            this.#root.style.transform = "";
            this.#overviewRect = null;

            if (this.#minimized) {
                this.#root.style.display = "none";
            }
        }, OVERVIEW_ANIM_MS);
    }

    _updateOverviewLayout(index, cellW, cellH, gap) {
        const cols = Math.ceil(Math.sqrt(allWindows.size));
        const col = index % cols;
        const row = Math.floor(index / cols);

        const cellX = col * cellW + gap;
        const cellY = row * cellH + gap;

        const innerW = cellW - gap * 2;
        const innerH = cellH - gap * 2;

        const r = this.#overviewRect;

        const scale = Math.min(
            innerW / r.width,
            innerH / r.height,
            1
        );
        this.#overviewScale = scale;
        this.#root.style.transition = OVERVIEW_TRANSITION;
        this._applyOverviewTransform();

        const targetCx = cellX + innerW / 2;
        const targetCy = cellY + innerH / 2;

        const srcCx = r.left + r.width / 2;
        const srcCy = r.top + r.height / 2;

        const dx = Math.round(targetCx - srcCx);
        const dy = Math.round(targetCy - srcCy);

        this.#root.style.transition =
            `transform ${OVERVIEW_ANIM_MS}ms ${OVERVIEW_EASING}`;

        this.#root.style.transform =
            `translate(${dx}px, ${dy}px) scale(${scale})`;
    }

    get_created_at() {
        return this.created_at
    }

    // ===============================
    // Public API
    // ===============================

    minimize() {
        if (this.#minimized) return;

        this.#minimized = true;

        this.#minimize.style.display = "none";
        this.#restore.style.display = "inline";

        const s = this.#root.style;
        this.#restoreWidth = s.width;
        this.#restoreHeight = s.height;
        this.#restoreLeft = s.left;
        this.#restoreTop = s.top;

        this.#root.style.display = "none";
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
        this.#root.borderTopLeftRadius = "0";
        this.#root.borderTopRightRadius = "0";

        this.resize(get_inner_width(), get_inner_height());
        this.#maximized = true
    }

    restore() {
        if (!this.#minimized && !this.#maximized) return;

        this.#minimize.style.display = "inline";
        this.#restore.style.display = "none";

        this.#root.style.display = "block";

        this.resize_from_strings(this.#restoreWidth, this.#restoreHeight);

        this.#root.style.left = this.#restoreLeft;
        const restoreTop = parseInt(this.#restoreTop ?? "0", 10);
        this.#root.style.top = clampTop(restoreTop) + "px";

        this.#root.style.resize = "both";
        this.#root.borderTopLeftRadius = "16px";
        this.#root.borderTopRightRadius = "16px";

        this.#minimized = false;
        this.#maximized = false;

        this.focus();
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

                let left, top;

                if (lastWindowPosition) {
                    left = lastWindowPosition.left + CASCADE_OFFSET_X;
                    top = lastWindowPosition.top + CASCADE_OFFSET_Y;
                } else {
                    left = (window.innerWidth - w) / 2;
                    top = (window.innerHeight - h) / 2;
                }

                if (left + w > window.innerWidth) left = 20;
                if (top + h > window.innerHeight) top = 20;

                this.#root.style.left = left + "px";
                this.#root.style.top = clampTop(top) + "px";

                lastWindowPosition = {left, top};

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
        allWindows.delete(this);

        VirtualWindow._reflowOverview();
    }


    close() {
        this.hide();
        this.destroy();
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
    get_internal_content() {
        return this.#content
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

document.addEventListener("mousemove", e => {
    if (e.clientX < 10 && e.clientY < 10) {
        VirtualWindow.enterOverview();
    }
});

window.addEventListener(
    "keydown",
    (e) => {
        if (e.key === "Escape" && overviewMode) {
            e.preventDefault();
            VirtualWindow.exitOverview();
            return;
        }

        if (e.code === "F9") {
            e.preventDefault();
            e.stopPropagation();

            if (!overviewMode) VirtualWindow.enterOverview();
            else VirtualWindow.exitOverview();
        }
    },
);

let touchTimer = null;

document.addEventListener("touchstart", e => {
    if (e.target.closest(".window_container")) return;

    touchTimer = setTimeout(() => {
        VirtualWindow.enterOverview();
    }, 600);
});

document.addEventListener("touchend", () => {
    clearTimeout(touchTimer);
});

