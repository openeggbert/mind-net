import {login, logout, changePassword, register} from "./auth.js";
import {getAccessToken, getRefreshToken} from "./api.js";
import {contentArea, showError, showToast} from "./dom.js";

export function renderLoginForm() {
    contentArea.innerHTML = `
        <h3>Login</h3>
        <form id="loginForm">
            <label>Username <input type="text" id="username" required></label><br>
            <label>Password <input type="password" id="password" required></label><br>
            <button type="submit">Login</button>
        </form>
        <p>Don’t have an account? <a href="#" id="showRegister">Register</a></p>
    `;

    document.getElementById("loginForm").onsubmit = async e => {
        e.preventDefault();
        const username = document.getElementById("username").value;
        const password = document.getElementById("password").value;
        const ok = await login(username, password);
        if (ok) {
            showToast("✅ Login successful", 3000, "success");
            renderAuthStatus();
        } else {
            showToast("❌ Login failed", 5000, "error");
        }
    };

    document.getElementById("showRegister").onclick = e => {
        e.preventDefault();
        renderRegisterForm();
    };
}

export function renderRegisterForm() {
    contentArea.innerHTML = `
        <h3>Register</h3>
        <form id="registerForm">
            <label>Username <input type="text" id="reg_username" required></label><br>
            <label>Password <input type="password" id="reg_password" required></label><br>
            <label>Email <input type="email" id="reg_email"></label><br>
            <label>Display name <input type="text" id="reg_display_name"></label><br>
            <label>Profile text <input type="text" id="reg_profile_text"></label><br>
            <button type="submit">Register</button>
        </form>
        <p>Already have an account? <a href="#" id="showLogin">Login</a></p>
    `;

    document.getElementById("registerForm").onsubmit = async e => {
        e.preventDefault();
        const payload = {
            username: document.getElementById("reg_username").value,
            password: document.getElementById("reg_password").value,
            email: document.getElementById("reg_email").value,
            display_name: document.getElementById("reg_display_name").value,
            profile_text: document.getElementById("reg_profile_text").value
        };

        const ok = await register(payload);
        if (ok) {
            showToast("✅ Registration successful, you can log in now.", 4000, "success");
            renderLoginForm();
        } else {
            showToast("❌ Registration failed", 5000, "error");
        }
    };

    document.getElementById("showLogin").onclick = e => {
        e.preventDefault();
        renderLoginForm();
    };
}

export function renderChangePasswordForm() {
    contentArea.innerHTML = `
        <h3>Change Password</h3>
        <form id="changePwdForm">
            <label>Old password <input type="password" id="old_password" required></label><br>
            <label>New password <input type="password" id="new_password" required></label><br>
            <button type="submit">Change Password</button>
        </form>
    `;

    document.getElementById("changePwdForm").onsubmit = async e => {
        e.preventDefault();
        const old_password = document.getElementById("old_password").value;
        const new_password = document.getElementById("new_password").value;
        const ok = await changePassword(old_password, new_password);
        if (ok) {
            showToast("✅ Password changed successfully", 3000, "success");
            renderAuthStatus();
        } else {
            showToast("❌ Failed to change password", 5000, "error");
        }
    };
}

export function renderAuthStatus() {
    const access = getAccessToken();
    const refresh = getRefreshToken();
    contentArea.innerHTML = `
        <h3>Auth Status</h3>
        <p>Access token: ${access ? "✅ stored" : "❌ none"}</p>
        <p>Refresh token: ${refresh ? "✅ stored" : "❌ none"}</p>
        <button id="logoutBtn">Logout</button>
        <button id="changePwdBtn">Change Password</button>
    `;

    document.getElementById("logoutBtn").onclick = async () => {
        let result = await logout();
        if(result) {
            showToast("👋 Logged out", 3000, "info");
        } else {
            showError("Logged out failed");
            return;
        }
        renderLoginForm();
    };

    document.getElementById("changePwdBtn").onclick = () => {
        renderChangePasswordForm();
    };
}
