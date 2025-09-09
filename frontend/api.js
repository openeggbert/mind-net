import { HOST } from "./conf.js";
import { PORT } from "./conf.js";
import {showError} from "./dom.js";
import {getEntitySchemas} from "./state.js";


export const API_BASE = `${HOST}:${PORT}/api`;
export const MODEL_DEFINITION_URL = `${API_BASE}/model_definition`;
export const CACHE_KEY = "model_definition_cache";
export const CACHE_TTL_MS = 3 * 60 * 60 * 1000; // 3 hours

export async function loadModelDefinition() {
    const now = Date.now();
    const cached = localStorage.getItem(CACHE_KEY);

    if (cached) {
        const { timestamp, data } = JSON.parse(cached);
        if (now - timestamp < CACHE_TTL_MS) {
            console.log("Using cached model_definition");
            return data;
        }
    }

    console.log("Downloading model_definition from API...");
    const resp = await fetch(MODEL_DEFINITION_URL);
    if (!resp.ok) throw new Error("Error loading model_definition");
    const data = await resp.json();

    localStorage.setItem(CACHE_KEY, JSON.stringify({ timestamp: now, data }));
    return data;
}

export async function apiFetch(url, options = {}) {
    try {
        const res = await fetch(url, options);
        if (!res.ok) {
            const text = await res.text();
            showError(`Error ${res.status}: ${text || res.statusText}`);
            return null;
        }
        return res.json();
    } catch (err) {
        showError(`Network error: ${err.message}`);
        return null;
    }
}

export async function resolveForeignKeyValue(fkEntity, id) {
    if (!id) return "";
    const schema = getEntitySchemas()[fkEntity];
    if (!schema) return id;
    const json = await apiFetch(`${API_BASE}/${fkEntity}/${id}`);
    if (!json) return id;
    return json[schema.titleField] ?? id;
}
