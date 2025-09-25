
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
export function formatDateTime(value) {
    if (!value || value === 0) return "";
    const d = new Date(Number(value) * 1000); // Unix timestamp in seconds
    const pad = n => n.toString().padStart(2,'0');
    return `${d.getFullYear()}-${pad(d.getMonth()+1)}-${pad(d.getDate())} ${pad(d.getHours())}:${pad(d.getMinutes())}:${pad(d.getSeconds())}`;
}
export function parseDateTimeToUnix(str) {
    if (!str) return 0;
    const parts = str.split(/[- :]/);
    if (parts.length < 6) return 0;
    const [y,m,d,h,min,s] = parts.map(Number);
    return Math.floor(new Date(y,m-1,d,h,min,s).getTime()/1000);
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
    container.appendChild(toast);

    requestAnimationFrame(() => toast.classList.add("show"));

    setTimeout(() => {
        toast.classList.remove("show");
        setTimeout(() => toast.remove(), 300);
    }, timeout);
}

export function showError(msg) {
    showToast(msg, 5000, "error");
}
export function showInfo(msg) {
    showToast(msg, 5000, "info");
}
export function showSuccess(msg) {
    showToast(msg, 5000, "success");
}