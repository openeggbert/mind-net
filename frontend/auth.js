import {API_BASE, setAccessToken, setRefreshToken, getRefreshToken, apiFetch, setUserId} from "./api.js";
import {showError} from "./dom.js";

export async function login(username, password) {
    const res = await fetch(`${API_BASE}/auth/login`, {
        method: "POST",
        headers: {"Content-Type": "application/json"},
        body: JSON.stringify({username, password})
    });
    if (!res.ok) {
        const t = await res.text();
        showError(`Login failed: ${t}`);
        return false;
    }
    const json = await res.json();
    setUserId(json.user_id)
    setAccessToken(json.access_token, json.access_token_expires_at);
    setRefreshToken(json.refresh_token, json.refresh_token_expires_at);
    return true;
}

export async function logout() {
    const refresh_token = getRefreshToken();
    if (!refresh_token) return;

    const res = await fetch(`${API_BASE}/auth/logout`, {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
            "Authorization": `Bearer ${localStorage.getItem("access_token")}`
        },
        body: JSON.stringify({refresh_token})
    });

    if (!res.ok) {
        const t = await res.text();
        showError(`Logout failed: ${t || res.statusText}`);
        return false;
    }
    setUserId(null)
    setAccessToken(null);
    setRefreshToken(null);
    return true;
}

export async function register(data) {
    const res = await fetch(`${API_BASE}/auth/register`, {
        method: "POST",
        headers: {"Content-Type": "application/json"},
        body: JSON.stringify(data)
    });
    if (!res.ok) {
        const t = await res.text();
        showError(`Registration failed: ${t || res.statusText}`);
        return false;
    }
    return true;
}


export async function refreshToken() {
    const refresh_token = getRefreshToken();
    if (!refresh_token) return false;

    const res = await fetch(`${API_BASE}/auth/refresh_token`, {
        method: "POST",
        headers: {"Content-Type": "application/json"},
        body: JSON.stringify({refresh_token})
    });
    let res_status = res.status;
    if (!res.ok && (res_status === 401 || res_status === 403)) {
        setUserId(null)
        setAccessToken(null);
        setRefreshToken(null);
        return false;
    }
    const json = await res.json();
    setUserId(json.user_id)
    setAccessToken(json.access_token, json.access_token_expires_at);
    return true;
}

export async function changePassword(old_password, new_password) {
    async function doRequest() {
        return fetch(`${API_BASE}/auth/change_password`, {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
                "Authorization": `Bearer ${localStorage.getItem("access_token")}`
            },
            body: JSON.stringify({old_password, new_password})
        });
    }

    let res = await doRequest();

    if (res.status === 401 && await refreshToken()) {
        res = await doRequest();
    }

    if (!res.ok) {
        const t = await res.text();
        showError(`Change password failed: ${t}`);
        return false;
    }

    return true;
}


export async function configure_get() {
    async function doRequest() {
        return fetch(`${API_BASE}/superadmin/configure`, {
            method: "GET",
            headers: {
                "Content-Type": "application/json",
                "Authorization": `Bearer ${localStorage.getItem("access_token")}`
            },
        });
    }

    let res = await doRequest();

    if (res.status === 401 && await refreshToken()) {
        res = await doRequest();
    }

    if (!res.ok) {
        const t = await res.text();
        showError(`Configure GET failed: ${t}`);
        return t;
    }

    return await res.text();
}
