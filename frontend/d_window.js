
// ========================================
// Window
// ========================================

import {set_was_dragged, was_dragged} from "./d_globals.js";

export function makeDraggable(el) {
    const header = el.querySelector('.window_container-header');
    let offsetX = 0, offsetY = 0, dragging = false;

    function startDrag(x, y, ev) {
        // stop fake drags
        if (ev && ev.buttons !== 1) return;

        console.log("START DRAG", {display: el.style.display, left: el.style.left, top: el.style.top});

        if (el.style.display === "none") return;

        const rect = el.getBoundingClientRect();
        dragging = true;
        set_was_dragged(true)

        offsetX = x - rect.left;
        offsetY = y - rect.top;

        el.style.position = "fixed";
        el.style.transform = "none";
    }


    function doDrag(x, y, ev) {
        if (document.body._forceStopDragging) {
            dragging = false;
            document.body._forceStopDragging = false;
            return;
        }
        if (!dragging) return;
        if (ev && ev.buttons !== 1) {
            dragging = false;
            return;
        }
        el.style.left = `${x - offsetX}px`;
        el.style.top = `${y - offsetY}px`;
    }


    function stopDrag() {
        console.log("STOP DRAG");
        dragging = false;
    }

    // --- Mouse support ---
    header.addEventListener('mousedown', e => startDrag(e.clientX, e.clientY, e));
    document.addEventListener('mousemove', e => doDrag(e.clientX, e.clientY, e));
    document.addEventListener('mouseup', stopDrag);

    // --- Touch support ---
    header.addEventListener('touchstart', e => {
        if (e.target.closest('.window-close')) return;

        const t = e.touches[0];
        startDrag(t.clientX, t.clientY);
        e.preventDefault();
    }, {passive: false});


    document.addEventListener('touchmove', e => {
        const t = e.touches[0];
        doDrag(t.clientX, t.clientY);
    }, {passive: false});

    document.addEventListener('touchend', stopDrag);
}

window.closeWindow = closeWindow;

export function closeWindow() {
    const win = document.getElementById('window_container');

    console.log("CLOSE WINDOW before", {
        display: win.style.display,
        left: win.style.left,
        top: win.style.top,
        transform: win.style.transform
    });

    set_was_dragged(false)
    win.hidden = true;

    win.style.display = 'none';
    win.style.left = "";
    win.style.top = "";
    win.style.transform = "translate(-50%, -50%)";
    win.style.position = "fixed";

    setTimeout(() => win.hidden = false, 50);
}

export function showWindow() {
    const win = document.getElementById("window_container");
    document.body._forceStopDragging = true;

    win.style.display = "block";
    win.style.position = "fixed";
    win.style.left = "50%";
    win.style.top = "50%";
    win.style.transform = "translate(-50%, -50%)";

    requestAnimationFrame(() => {
        const rect = win.getBoundingClientRect();

        win.style.setProperty("--win-init-w", rect.width + "px");
        win.style.setProperty("--win-init-h", rect.height + "px");

        if (!was_dragged()) {
            win.style.left = "calc(50% - (var(--win-init-w) / 2))";
            win.style.top = "calc(50% - (var(--win-init-h) / 2))";
            win.style.transform = "none";
        }
    });
}

window.showWindow = showWindow;
export const clearWindow = () => document.getElementById("window_container_content").innerHTML = "";
window.clearWindow = clearWindow;
export const setWindowTitle = title => document.getElementById("window_container_title").innerText = title;
export const getWindowContent = () => document.getElementById("window_container_content");
export const setWindowContent = text => getWindowContent().textContent = text;

export function setWindowContentByUrl(url) {
    clearWindow();
    let iframe = document.createElement("iframe");
    iframe.src = url;
    iframe.scroling = "no"
    iframe.style.display = "block";
    iframe.style.width = "100%";
    iframe.style.height = "100%";

    getWindowContent().appendChild(iframe);
    getWindowContent().style.height = "100%";
}

export function showWindowFrom(title, url) {
    clearWindow()
    setWindowTitle(title)
    if (url === undefined) {
        alert("url is required");
        return;
    }
    setWindowContentByUrl(url)
    showWindow();
}