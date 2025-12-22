
export const entityNav = document.getElementById('entityNav');
export const crudMenu = document.getElementById('crudMenu');
export const entityTitle = document.getElementById('entityTitle');
export const contentArea = document.getElementById('contentArea');
//
export function getQueryParams() {
    const params = new URLSearchParams(window.location.search);
    const entity = params.get('entity');
    const action = params.get('action');
    const others = Object.fromEntries(params.entries());
    return {entity, action, others};
}
export function formatDateTime(
    value,
    showHours = true,
    showMinutes = true,
    showSeconds = true,
    showMilliseconds = true
) {
    if (!value || value === 0) return "";
    const d = new Date(Number(value)); // Unix timestamp in ms
    const pad = n => n.toString().padStart(2, '0');
    const ms = String(d.getMilliseconds()).padStart(3, '0');

    let result = `${d.getFullYear()}-${pad(d.getMonth() + 1)}-${pad(d.getDate())}`;

    const timeParts = [];
    if (showHours) timeParts.push(pad(d.getHours()));
    if (showMinutes) timeParts.push(pad(d.getMinutes()));
    if (showSeconds) timeParts.push(pad(d.getSeconds()));

    if (timeParts.length > 0) {
        result += " " + timeParts.join(":");
        if (showMilliseconds) result += `.${ms}`;
    }

    return result;
}

export function formatDate(value){
    return formatDateTime(value, false, false, false, false);
}
export function formatDateTimeHM(value){
    return formatDateTime(value, true, true, false, false);
}
export function formatDateTimeHMS(value){
    return formatDateTime(value, true, true, true, false);
}

export function parseDateTimeToUnix(str) {
    if (!str) return 0;
    // support for format with ms: "YYYY-MM-DD HH:MM:SS.mmm"
    const match = str.match(/^(\d+)-(\d+)-(\d+) (\d+):(\d+):(\d+)(?:\.(\d{1,3}))?$/);
    if (!match) return 0;
    const [_, y, m, d, h, min, s, ms] = match.map(Number);
    return new Date(y, m - 1, d, h, min, s, ms || 0).getTime(); // ms
}


// key: hidden_columns
// value: { "note": ["created_at","updated_at"], "user": ["password"] }
export function getHiddenColumns() {
    return JSON.parse(localStorage.getItem("hidden_columns") || "{}");
}

export function setHiddenColumns(hiddenCols) {
    localStorage.setItem("hidden_columns", JSON.stringify(hiddenCols));
}

export function isColumnHidden(entity, column) {
    const hiddenCols = getHiddenColumns();
    return hiddenCols[entity]?.includes(column);
}

// --- Toast CSS injection ---
(function injectToastStyles() {
    if (document.getElementById("toast-styles")) return; // ať se nevloží 2x
    const style = document.createElement("style");
    style.id = "toast-styles";
    style.textContent = `
#toast-container {
  position: fixed;
  bottom: 20px;
  right: 20px;
  display: flex;
  flex-direction: column;
  gap: 10px; /* space between toasts */
  z-index: 1000;
}

.toast {
  font-size: 1.1rem;
  background: #9F9;
  color: #333;
  padding: 12px 20px;
  border-radius: 8px;
  box-shadow: 0 2px 6px rgba(0,0,0,0.3);
  opacity: 0;
  transform: translateY(20px);
  transition: opacity 0.3s ease, transform 0.3s ease;
}
.toast.show {
  opacity: 1;
  transform: translateY(0);
}
.toast.error   { background: #F99; }
.toast.info    { background: #9CF; }
.toast.success { background: #9F9; }

`;
    document.head.appendChild(style);
})();


// create toast container only once
function getToastContainer() {
    let container = document.getElementById("toast-container");
    if (!container) {
        container = document.createElement("div");
        container.id = "toast-container";
        document.body.appendChild(container);
    }
    return container;
}

export function showToast(message, timeout = 3000, type = "info") {
    const container = getToastContainer();

    const toast = document.createElement("div");
    toast.className = `toast ${type}`;
    toast.textContent = message;
    container.style.zIndex = "10000"
    container.appendChild(toast);

    requestAnimationFrame(() => toast.classList.add("show"));

    setTimeout(() => {
        toast.classList.remove("show");
        setTimeout(() => toast.remove(), 300);
    }, timeout);
}

export function showError(msg) {
    showToast(msg, 10000, "error");
}
export function showInfo(msg) {
    showToast(msg, 5000, "info");
}
export function showSuccess(msg) {
    showToast(msg, 5000, "success");
}
export function showWarn(msg) {
    showToast(msg, 10000, "warn");
}

export function makeEnum(map) {
    return {
        map,
        getTexts: () => Object.keys(map),
        toNumber: name => map[name] ?? null,
        fromNumber: num =>
            Object.entries(map).find(([_, value]) => value === num)?.[0] ?? null
    };
}

export const sleep_for_seconds = seconds => new Promise(r => setTimeout(r, seconds * 1000));

export const hide_element = id => {
    const el = document.getElementById(id);
    if (el) el.style.display = "none";
};

export const show_element = id => {
    const el = document.getElementById(id);
    if (!el) return;

    const tag = el.tagName.toLowerCase();
    if (tag === "span" || tag === "strong") {
        el.style.display = "inline";
    } else if (tag === "button") {
        el.style.display = "inline-block";
    } else {
        el.style.display = "block";
    }
};

export const hide_elements = (...ids) => ids.forEach(hide_element);
export const show_elements = (...ids) => ids.forEach(show_element);
export const show_or_hide_elements = (show, ...ids) =>
    ids.forEach(id => (show ? show_element(id) : hide_element(id)));
export const show_or_hide_element = (show, id) =>
    show ? show_element(id) : hide_element(id);


export const get_element = id => document.getElementById(id);

export const set_value = (id, value) => {
    const el = get_element(id);
    if (el) el.textContent = value;
};

export function copy_to_clipboard(text) {
    navigator.clipboard.writeText(text);
    showInfo("Copied to clipboard: " + text);
}

export function chooseOption(options, title = "") {
    return new Promise((resolve) => {
        // Overlay
        const overlay = document.createElement("div");
        overlay.style.position = "fixed";
        overlay.style.top = 0;
        overlay.style.left = 0;
        overlay.style.width = "100%";
        overlay.style.height = "100%";
        overlay.style.backgroundColor = "rgba(0,0,0,0.5)";
        overlay.style.display = "flex";
        overlay.style.justifyContent = "center";
        overlay.style.alignItems = "center";
        overlay.style.zIndex = 1000;

        // Panel
        const panel = document.createElement("div");
        panel.style.background = "white";
        panel.style.padding = "20px";
        panel.style.borderRadius = "12px";
        panel.style.boxShadow = "0 4px 10px rgba(0,0,0,0.3)";
        panel.style.display = "flex";
        panel.style.flexDirection = "column";
        panel.style.gap = "10px";
        panel.style.minWidth = "200px";
        if(title !== "") {
            let h1 = document.createElement("h1");
            h1.innerText = title;
            h1.style.textAlign = "center";
            panel.appendChild(h1);
        }

        // Create buttons for all options
        options.forEach((text) => {
            const btn = document.createElement("button");
            btn.textContent = text;
            btn.style.padding = "10px";
            btn.style.border = "1px solid #ccc";
            btn.style.borderRadius = "8px";
            btn.style.cursor = "pointer";
            btn.style.background = "#f0f0f0";
            btn.onmouseenter = () => (btn.style.background = "#e0e0e0");
            btn.onmouseleave = () => (btn.style.background = "#f0f0f0");
            btn.onclick = () => {
                cleanup();
                resolve(text);
            };
            panel.appendChild(btn);
        });

        // Cancel button
        const cancelBtn = document.createElement("button");
        cancelBtn.textContent = "Cancel";
        cancelBtn.style.padding = "10px";
        cancelBtn.style.border = "1px solid #ccc";
        cancelBtn.style.borderRadius = "8px";
        cancelBtn.style.cursor = "pointer";
        cancelBtn.style.background = "#ffe0e0";
        cancelBtn.onmouseenter = () => (cancelBtn.style.background = "#ffcccc");
        cancelBtn.onmouseleave = () => (cancelBtn.style.background = "#ffe0e0");
        cancelBtn.onclick = () => {
            cleanup();
            resolve(null);
        };
        panel.appendChild(cancelBtn);

        overlay.appendChild(panel);
        document.body.appendChild(overlay);

        // Cleanup after closing
        function cleanup() {
            document.body.removeChild(overlay);
        }
    });
}

// 🔹 Save JSON object to localStorage with expiration (in milliseconds)
export function saveToLocalStorage(key, data, ttlMs) {
    try {
        const now = Date.now();
        const item = {
            value: data,
            expires: now + ttlMs
        };
        localStorage.setItem(key, JSON.stringify(item));
    } catch (e) {
        console.error(`Failed to save ${key} to localStorage:`, e);
    }
}

// 🔹 Load JSON object from localStorage (returns null if it doesn't exist or has expired)
export function loadFromLocalStorage(key) {
    const itemStr = localStorage.getItem(key);
    if (!itemStr) return null;

    try {
        const item = JSON.parse(itemStr);
        if (!item.expires || Date.now() > item.expires) {
            localStorage.removeItem(key);
            return null;
        }
        return item.value;
    } catch (e) {
        console.error(`Failed to parse ${key} from localStorage:`, e);
        return null;
    }
}

// 🔹 Get or fetch data, with caching to localStorage
// fetchFn should be an async function returning the data
export async function getOrFetchFromLocalStorage(key, fetchFn, ttlMs) {
    const cached = loadFromLocalStorage(key);
    if (cached !== null) return cached;

    try {
        const data = await fetchFn();
        saveToLocalStorage(key, data, ttlMs);
        return data;
    } catch (e) {
        console.error(`Failed to fetch data for ${key}:`, e);
        return null;
    }
}

export function minutes_to_ms(minutes) {
    return minutes * 60 * 1000;
}
