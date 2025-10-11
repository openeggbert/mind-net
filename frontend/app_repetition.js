// ========================================
// Imports & Globals
// ========================================
import {API_BASE, apiFetch} from "./api.js";

let r_global_setting_id = ""
let r_global_settings = null;
let r_user_setting_id = ""
let r_user_settings = null;
let r_session_id = ""
let r_session = null;
let r_review_id = ""
let r_review = null;
let r0_state_id = ""
let r0_state = null;
let r2_state_id = ""
let r2_state = null;
let r4_state_id = ""
let r4_state = null;
let r18_state_id = ""
let r18_state = null;
let r18_perfagg_id = ""
let r18_perfagg = null;
let wasDragged = false;

// ========================================
// Window
// ========================================

function makeDraggable(el) {
    const header = el.querySelector('.window_container-header');
    let offsetX = 0, offsetY = 0, dragging = false;

    function startDrag(x, y) {
        const rect = el.getBoundingClientRect();
        dragging = true;
        wasDragged = true;
        offsetX = x - rect.left;
        offsetY = y - rect.top;
        el.style.transform = "none";
        el.style.position = "fixed";
    }

    const doDrag = (x, y) => {
        if (!dragging) return;
        el.style.left = `${x - offsetX}px`;
        el.style.top  = `${y - offsetY}px`;
    };

    const stopDrag = () => { dragging = false };

    // mouse
    header.addEventListener('mousedown', e => {
        startDrag(e.clientX, e.clientY);
        e.preventDefault();
    });
    document.addEventListener('mousemove', e => doDrag(e.clientX, e.clientY));
    document.addEventListener('mouseup', stopDrag);

    // touch
    header.addEventListener('touchstart', e => {
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
    win.style.display = 'none';
    window.closeWindow = closeWindow;
}

export function showWindow() {
    const win = document.getElementById("window_container");
    win.style.display = "block";

    if (!wasDragged) {
        Object.assign(win.style, {
            left: "50%",
            top: "50%",
            transform: "translate(-50%, -50%)",
            position: "fixed"
        });
    }
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
    setWindowTitle(title)
    if (url === undefined) {
        alert("url is required");
        return;
    }
    setWindowContentByUrl(url)
    showWindow();
}


// ========================================
// Main (DOMContentLoaded)
// ========================================

document.addEventListener('DOMContentLoaded', async () => {

});











export function showAnswer() {
    document.querySelector('.back').style.display = 'block';
}
window.showAnswer = showAnswer;

export function rate(q) {
    const S = 1.0; // aktuální stabilita
    const b = 0.6;
    const R_target = 0.9;
    const elapsed = 1.0; // dny od posledního opakování

    const R_now = Math.exp(-Math.pow(elapsed / S, b));
    let S_after = S;
    let next_I = 1.0;

    if (q >= 3) {
        const fq = q === 3 ? 0.9 : q === 4 ? 1.0 : 1.1;
        const alpha = 0.3, beta = 0.6, k_over = 0.15;
        const I_opt = S * Math.pow(-Math.log(R_target), 1 / b);
        const overdue = Math.max(0, elapsed / I_opt - 1);
        const g_over = 1 + k_over * overdue;
        const gain = alpha * fq * Math.pow(1 - R_now, beta) * g_over;
        S_after = S * (1 + gain);
        next_I = S_after * Math.pow(-Math.log(R_target), 1 / b);
    } else {
        const gamma = 0.2, delta = 0.4, S_min = 0.5;
        const loss = gamma * Math.pow(R_now, delta);
        S_after = Math.max(S_min, S * (1 - loss));
        next_I = 0.5; // krátký retry
    }

    document.getElementById('result').innerHTML =
        `📈 Retrievability: ${R_now.toFixed(3)}<br>
         📅 Další opakování za: ${next_I.toFixed(2)} dní<br>
         🧠 Nová stabilita: ${S_after.toFixed(2)} dní`;
}
window.rate = rate;