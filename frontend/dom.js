
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



export function showError(msg) {
    alert(msg);
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

