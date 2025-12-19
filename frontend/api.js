import {HOST, PORT} from "./conf.js";
import {formatDateTime, formatDateTimeHM, showError, showWarn} from "./dom.js";
import {getEntitySchemas} from "./state.js";
import {refreshToken} from "./auth.js";


export const API_BASE = `${HOST}:${PORT}/api/v1`;
export const MODEL_DEFINITION_URL = `${API_BASE}/model_definition`;
export const CACHE_KEY = "model_definition_cache";
export const CACHE_TTL_MS = 3 * 60 * 60 * 1000; // 3 hours
export const APPLICATIONS_URL = `${API_BASE}/app`;
export const APPS_CACHE_KEY = "applications_cache";
export const APPS_CACHE_TTL_MS = CACHE_TTL_MS

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

export async function loadApplications() {
    const now = Date.now();
    const cached = localStorage.getItem(APPS_CACHE_KEY);

    if (cached) {
        try {
            const { timestamp, data } = JSON.parse(cached);
            if (now - timestamp < APPS_CACHE_TTL_MS) {
                console.log("Using cached applications");
                return data;
            }
        } catch (e) {
            // if JSON parse fails, delete cache
            localStorage.removeItem(APPS_CACHE_KEY);
        }
    }

    console.log("Downloading applications from API...");
    const resp = await fetch(APPLICATIONS_URL);
    if (!resp.ok) throw new Error("Error loading applications");
    const json = await resp.json();

    const data = json.items || [];
    localStorage.setItem(APPS_CACHE_KEY, JSON.stringify({ timestamp: now, data }));
    return data;
}

const USER_ID = "user_id";
const ACCESS_TOKEN_KEY = "access_token";
const REFRESH_TOKEN_KEY = "refresh_token";
const ACCESS_TOKEN_EXPIRES_AT_KEY = "access_token_expires_at";
const REFRESH_TOKEN_EXPIRES_AT_KEY = "refresh_token_expires_at";

export function getUserId() {
    return localStorage.getItem(USER_ID);
}

export function setUserId(userid) {
    if(userid === null){
        localStorage.removeItem(USER_ID);
    } else {
    localStorage.setItem(USER_ID, userid);
    }
}
export function getAccessToken() {
    return localStorage.getItem(ACCESS_TOKEN_KEY);
}

export function setAccessToken(token, expires_at = null) {
    if (token) {
        localStorage.setItem(ACCESS_TOKEN_KEY, token);
        localStorage.setItem(ACCESS_TOKEN_EXPIRES_AT_KEY, expires_at);
    }
    else {
        localStorage.removeItem(ACCESS_TOKEN_KEY);
        localStorage.removeItem(ACCESS_TOKEN_EXPIRES_AT_KEY);
    }
}

export function getRefreshToken() {
    return localStorage.getItem(REFRESH_TOKEN_KEY);
}

export function setRefreshToken(token, expires_at = null) {
    if (token) {
        localStorage.setItem(REFRESH_TOKEN_KEY, token);
        localStorage.setItem(REFRESH_TOKEN_EXPIRES_AT_KEY, expires_at);
    }
    else {
        localStorage.removeItem(REFRESH_TOKEN_KEY);
        localStorage.removeItem(REFRESH_TOKEN_EXPIRES_AT_KEY);
    }
}

export function getAccessTokenExpiresAt() {
    return localStorage.getItem(ACCESS_TOKEN_EXPIRES_AT_KEY);
}

export function getRefreshTokenExpiresAt() {
    return localStorage.getItem(REFRESH_TOKEN_EXPIRES_AT_KEY);
}

async function ensureFreshAccessToken() {
    const exp = getAccessTokenExpiresAt();
    if (!exp) return;

    const nowMs = Date.now();
    const expMs = Number(exp);

    // if expired or ≤ 60 000 ms remaining
    if (expMs <= nowMs + 60_000) {
        const ok = await refreshToken();
        if (!ok) console.warn("Failed to refresh access token automatically");
    }

}

function maybeCleanupLocalStorage() {
    // 1 in 1000 chance to run
    if (Math.floor(Math.random() * 1000) !== 500) return;

    const now = Date.now();

    for (let i = 0; i < localStorage.length; i++) {
        const key = localStorage.key(i);
        if (!key) continue;

        try {
            const value = localStorage.getItem(key);
            if (!value) continue;

            const parsed = JSON.parse(value);

            // check if it's an object with expires property and not null
            if (parsed && typeof parsed === "object" && "expires" in parsed) {
                const expires = Number(parsed.expires);
                if (!isNaN(expires) && expires < now) {
                    localStorage.removeItem(key);
                    // move index back because localStorage size decreased
                    i--;
                }
            }
        } catch {
            // ignore invalid JSON
        }
    }
    localStorage.setItem("last_maybeCleanupLocalStorage", formatDateTime(Date.now(), true, true, true, true));
}


// Overload apiFetch to always send Authorization header
export async function apiFetch(url, options = {}) {
    maybeCleanupLocalStorage();

    const headers = options.headers || {};

    const skipAuth = url.includes("/auth/login") ||
        url.includes("/auth/register") ||
        url.includes("/auth/refresh_token");

    if (!skipAuth) {
        await ensureFreshAccessToken();

        const token = getAccessToken();
        if (token) headers["Authorization"] = `Bearer ${token}`;
    }

    options.headers = headers;

    try {
        const res = await fetch(url, options);

        if (res.status === 401 && !skipAuth && getRefreshToken()) {
            const refreshed = await refreshToken();
            if (refreshed) {
                headers["Authorization"] = `Bearer ${getAccessToken()}`;
                return apiFetch(url, options); // retry
            }
        }

        if (!res.ok) {
            const text = await res.text();
            showError(`Error ${res.status}: ${text || res.statusText}`);
            return null;
        }
        const ct = res.headers.get("content-type") || "";
        if (ct.includes("application/json")) {
            return await res.json();
        } else {
            return await res.text();
        }

    } catch (err) {
        showError(`Network error: ${err.message}`);
        return null;
    }
}

export function format_url_params(...values) {
    const max_index = values.length - 1;
    let result = ""
    for (let i = 0; i < values.length; i++) {
        const key = values[i];
        i++
        if(i > max_index) break
        const value = values[i]
        result += "&" + key + "="
        result += encodeURIComponent(value);
    }
    return result
}
export async function list_entities(entity, additional_params = "", page_number = 1, page_size = 20) {
    const url = new URL(`${API_BASE}/${entity}`);

    url.searchParams.set("page_number", page_number.toString());
    url.searchParams.set("page_size", page_size.toString());

    // ensure leading & when params exist
    if (additional_params && !additional_params.startsWith("&") && !additional_params.startsWith("?")) {
        additional_params = "&" + additional_params;
    }

    const finalUrl = url.toString() + additional_params;

    const json = await apiFetch(finalUrl, {
        method: "GET",
        headers: {"Content-Type": "application/json"},
    });

    return json; // ✅ return the whole response (with items, total_pages, etc.)
}

export async function list_all_entities(entity, additional_params = "") {
    const result = [];
    const page_size = 100;
    let page_number = 1;

    while (true) {
        const json = await list_entities(entity, additional_params, page_number++, page_size);
        const items = json?.items || [];

        if (items.length === 0) break; // ✅ fixed condition
        result.push(...items);

        if (result.length >= 1000) {
            showWarn("Omitting some results: 1000 or more results.");
            break;
        }

        if (page_number > (json.total_pages || 1)) break; // ✅ stop after last page
    }

    return result;
}

export async function read_entity(entity, id) {
    const url = new URL(`${API_BASE}/${entity}/${id}`);

    return await apiFetch(url.toString(),
        {
            method: "GET",
            headers: {"Content-Type": "application/json"},
        }

    );
}

export async function delete_entity(entity, id) {
    const url = new URL(`${API_BASE}/${entity}/${id}`);

    return await apiFetch(url.toString(),
        {
            method: "DELETE",
        }
    );
}

export async function put_entity(model_name, id, json) {
    if(id === undefined) {
        throw "id required";
    }
    const url = new URL(`${API_BASE}/${model_name}/${id}`);
    return await apiFetch(url.toString(), {
        method: "PUT",
        headers: {"Content-Type": "application/json"},
        body: JSON.stringify(json)
    });

}

export async function post_entity(model_name, json) {
    const url = new URL(`${API_BASE}/${model_name}`);
    return await apiFetch(url.toString(), {
        method: "POST",
        headers: {"Content-Type": "application/json"},
        body: JSON.stringify(json)
    });
}

export function setTitleCache(entityName, entityId, value, ttlMs = 24 * 60 * 60 * 1000) { // cache for 24 hours
    const key = `titlecache:${entityName}:${entityId}`;
    localStorage.setItem(key, JSON.stringify({
        value,
        expires: Date.now() + ttlMs
    }));
}

export function getTitleCache(entityName, entityId) {
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