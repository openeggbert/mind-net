import { HOST } from "./conf.js";
import { PORT } from "./conf.js";
import {showError} from "./dom.js";
import {getEntitySchemas} from "./state.js";


export const API_BASE = `${HOST}:${PORT}/api/v1`;
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

function setTitleCache(entityName, entityId, value, ttlMs = 24 * 60 * 60 * 1000) { // cache for 24 hours
    const key = `titlecache:${entityName}:${entityId}`;
    localStorage.setItem(key, JSON.stringify({
        value,
        expires: Date.now() + ttlMs
    }));
}

function getTitleCache(entityName, entityId) {
    const key = `titlecache:${entityName}:${entityId}`;
    const raw = localStorage.getItem(key);
    if (!raw) return null;
    try {
        const {value, expires} = JSON.parse(raw);
        if (Date.now() > expires) {
            localStorage.removeItem(key);
            return null;
        }
        return value;
    } catch {
        localStorage.removeItem(key);
        return null;
    }
}

export async function resolveForeignKeyValue(fkEntity, id) {
    if (!id) return "";
    const schema = getEntitySchemas()[fkEntity];
    if (!schema) return id;

    // 1. Try cache
    const cached = getTitleCache(fkEntity, id);
    if (cached) return cached;

    // 2. Fetch from API
    const json = await apiFetch(`${API_BASE}/${fkEntity}/${id}`);
    if (!json) return id;

    // 3. Determine correct title field
    const titleField = schema.title_column || schema.titleField || "id";
    const value = json[titleField] ?? id;

    // 4. Save to cache
    setTitleCache(fkEntity, id, value);

    return value;
}