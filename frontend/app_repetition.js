// ========================================
// Imports & Globals
// ========================================

import {
    getUserId, list_all_entities, post_entity,
    list_entities, put_entity, read_entity
} from "./api.js";
import {
    chooseOption,
    formatDate,
    formatDateTime, formatDateTimeHM,
    get_element,
    getOrFetchFromLocalStorage,
    minutes_to_ms, showError, showInfo,
    saveToLocalStorage, loadFromLocalStorage, hide_element, showWarn
} from "./dom.js";

let user_id = null
let r_global_settings = {};
let r_user_settings = null;
let r_session_id = ""
let r_session_for_reviews = null;
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

let clone_from_r_session = null

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
        el.style.top = `${y - offsetY}px`;
    };

    const stopDrag = () => {
        dragging = false
    };

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
// Utils
// ========================================

class StopWatch {
    start() {
        this.startTime = Date.now();
    }

    stop() {
        this.endTime = Date.now();
    }

    get elapsedMs() {
        if (this.startTime === undefined || this.endTime === undefined) return 0;
        return this.endTime - this.startTime;
    }
}


// ========================================
// Main (DOMContentLoaded)
// ========================================

document.addEventListener('DOMContentLoaded', async () => {

    user_id = getUserId()
    let main = get_element("main")
    let main_content = document.createElement("div")
    main_content.id = "main_content"
    main_content.style.margin = "0 auto";
    main_content.style.textAlign = "center";
    main.appendChild(main_content)

    const screen_demo = "screen_demo"
    const screen_home = "screen_home"
    const screen_sessions = "screen_sessions"
    const screen_new_session = "screen_new_session"
    const screen_new_review = "screen_new_review"
    const screen_reviews = "screen_reviews"
    const screen_global_settings = "screen_global_settings"
    const screen_user_settings = "screen_user_settings"

    let current_screen = screen_home;
    const params = new URLSearchParams(window.location.search);
    let param_screen = params.get("screen");
    if (param_screen !== null && param_screen !== undefined) {
        current_screen = param_screen;
    }

    //
    get_element("repetition_header").title = "Go to list of all sessions"
    get_element("repetition_header").style.cursor = "pointer"

    get_element("button_mindnet").addEventListener("click", () => {
        window.location.href = 'index.html'
    });
    get_element("button_mindnet").title = "Go to Mind Net generic frontend"

    get_element("button_menu").addEventListener("click", () => {
        current_screen = screen_home;
        render()
    });
    get_element("button_menu").title = "Show the Menu"

    get_element("button_theme").addEventListener("click", () => {
        document.body.classList.toggle("dark");
    });
    get_element("button_theme").title = "Switch dark/light theme"
    //
    const win = document.getElementById('window_container');
    makeDraggable(win);

    //


    async function load_r_global_settings() {
        let r_global_settings_tmp = await list_all_entities("r_global_setting")
        r_global_settings = {}

        r_global_settings_tmp.forEach(json => {
            r_global_settings[json.key] = json.value
        });
        return r_global_settings
    }

    async function load_r_user_settings() {
        let r_user_settings_tmp = await list_all_entities("r_user_setting", "user_id=" + user_id)
        r_user_settings = {}

        r_user_settings_tmp.forEach(json => {
            r_user_settings[json.key] = json.value
        });
        return r_user_settings
    }

    const sixty_minutes = minutes_to_ms(60)

    function refresh_param_screen() {
        const url = new URL(window.location.href);
        url.searchParams.set("screen", current_screen);
        window.history.pushState({}, "", url);
    }

    function render_screen_demo() {
        refresh_param_screen()
        main_content.innerHTML = "";

        main_content.innerHTML = `
                <div class="card">
            <div class="front">❓ What does <code>const</code> mean as a function parameter in C++?</div>
            <div class="back">✅ Indicates that the parameter will not be modified within the function.</div>
            <button class="show-btn" onclick="showOrHideAnswer()">Show answer</button>
            <div id="rating">
                <p>How well did you recall the note?</p>
                <div>
                    <button class="rating-btn" id="rating-btn-0" onclick="rate_demo(0)">0</button>
                    <button class="rating-btn" id="rating-btn-1" onclick="rate_demo(1)">1</button>
                    <button class="rating-btn" id="rating-btn-2" onclick="rate_demo(2)">2</button>
                    <button class="rating-btn" id="rating-btn-3" onclick="rate_demo(3)">3</button>
                    <button class="rating-btn" id="rating-btn-4" onclick="rate_demo(4)">4</button>
                    <button class="rating-btn" id="rating-btn-5" onclick="rate_demo(5)">5</button>
                </div>
            </div>
            
            <div id="actions">
                <p>Next action</p>
                <div>
                    <button class="rating-btn" id="send-btn">Send</button>
                    <button class="rating-btn" id="skip-btn">Skip</button>
                </div>
            </div>
            <div class="info" id="result"></div>
        </div>
        `;

    }

    function render_screen_home() {
        refresh_param_screen()
        main_content.innerHTML = "";
        let button_new_session = document.createElement("button");
        let button_sessions = document.createElement("button");

        function make_button(text, screen) {
            let button = document.createElement("button");
            main_content.appendChild(button)
            main_content.appendChild(document.createElement("br"))
            button.innerText = text
            button.style.minWidth = "200px"
            button.onclick = function () {
                current_screen = screen
                render()
            }
        }

        make_button("New session", screen_new_session)
        make_button("Sessions", screen_sessions)
        make_button("Global settings", screen_global_settings)
        make_button("User settings", screen_user_settings)
        make_button("Demo", screen_demo)
    }

    let currentPage = 1;
    let totalPages = 1;
    const pageSize = 10;

    let renderOnlyPinned = false;

    async function render_screen_sessions() {
        refresh_param_screen();
        main_content.innerHTML = "<h2>Your Sessions</h2>";

        main_content.innerHTML += "\n<label for=\"only_pinned\">Only pinned</label>\n";
        main_content.innerHTML += "\n<input id=\"only_pinned\" type=\"checkbox\"/>\n";
        get_element("only_pinned").checked = renderOnlyPinned
        get_element("only_pinned").onclick = function () {
            renderOnlyPinned = !renderOnlyPinned
            get_element("only_pinned").checked = renderOnlyPinned
            render()
        }

        const params = "&user_id=" + user_id + "&sort=created_at&order=desc"
            + (renderOnlyPinned ? "&pinned=1" : "")

        const json = await list_entities("r_session", params, currentPage, pageSize);

        const r_sessions = json.items || [];
        totalPages = json.total_pages || 1;

        if (r_sessions.length === 0) {
            const p = document.createElement("p");
            p.textContent = "No sessions found.";
            main_content.appendChild(p);
            return;
        }

        // Container
        let container = document.createElement("div");
        container.className = "session-list";
        main_content.appendChild(container);

        // Cards
        r_sessions.forEach(json => {
            let card = document.createElement("div");
            card.className = "session-card";
            container.appendChild(card);

            // Header
            let title = document.createElement("div");
            title.className = "session-title";
            title.innerHTML = `Session #${json.id} — ${algoName(json.algorithm)} (${scopeName(json.scope)})`;
            card.appendChild(title);

            // Meta info
            let meta = document.createElement("div");
            meta.className = "session-meta";

            const map_id_ = crypto.randomUUID();
            const map_collection_id_ = crypto.randomUUID();
            meta.innerHTML = `
            🕓 Created: ${formatDateTimeHM(json.created_at)}<br>
            🔄 Updated: ${formatDateTimeHM(json.updated_at)}<br>
            <span id="${map_id_}">🧩  Map ID: ${json.map_id}<br></span>
            <span id="${map_collection_id_}">🧩 Map Collection ID: ${json.map_collection_id}<br></span>
            🧮 Algorithm: ${algoName(json.algorithm)}<br>
            📚 Schedule: ${scheduleName(json.schedule)}<br>
            🎯 Scope: ${scopeName(json.scope)}<br>
            📑 Cloned from: ${json.cloned_from_session_id || "-"}<br>
            📝 Description: ${json.description}
        `;
            card.appendChild(meta);
            if(json.map_id === 0) get_element(map_id_).remove();
            if(json.map_collection_id === 0) get_element(map_collection_id_).remove();

            // Details toggle
            let detailsBtn = document.createElement("button");
            detailsBtn.className = "session-btn";
            detailsBtn.textContent = "Show details";
            detailsBtn.onclick = () => {
                let d = card.querySelector(".session-details");
                let shown = d.style.display !== "none";
                d.style.display = shown ? "none" : "block";
                detailsBtn.textContent = shown ? "Show details" : "Hide details";
            };
            card.appendChild(detailsBtn);

            // Details table
            let details = document.createElement("div");
            details.className = "session-details";
            details.style.display = "none";
            details.innerHTML = `
            <table class="session-table">
                <tr><th>Filter under note</th><td>${json.filter_under_note || "-"}</td></tr>
                <tr><th>Filter date from</th><td>${formatDateTimeHM(json.filter_date_from)}</td></tr>
                <tr><th>Filter date to</th><td>${formatDateTimeHM(json.filter_date_to)}</td></tr>
                <tr><th>Filter tag</th><td>${json.filter_tag || "-"}</td></tr>
                <tr><th>Filter collection</th><td>${json.filter_collection || "-"}</td></tr>
                <tr><th>Selected items</th><td><pre>${formatJson(json.selected_items)}</pre></td></tr>
                <tr><th>Pinned</th><td><span id="Pinned">${json.pinned ? "Yes" : "No"}</span></td></tr>
                
            </table>
        `;
            card.appendChild(details);

            // Actions
            let actions = document.createElement("div");
            actions.className = "session-actions";

            const created = new Date(json.created_at);
            const now = new Date();
            const oneDayMs = 24 * 60 * 60 * 1000;
            const youngerThanDay = (now - created) <= oneDayMs;

            if (youngerThanDay) {
                let btnRun = document.createElement("button");
                btnRun.className = "session-btn";
                btnRun.textContent = "Run";
                btnRun.onclick = () => {
                    r_session_for_reviews = json
                    current_screen = screen_new_review
                    render()
                };
                actions.appendChild(btnRun);
            }

            let btnClone = document.createElement("button");
            btnClone.className = "session-btn danger";
            btnClone.textContent = "Clone";
            btnClone.onclick = () => {
                clone_from_r_session = json;
                render(screen_new_session);
            };
            actions.appendChild(btnClone);

            let btnPinUnpin = document.createElement("button");
            btnPinUnpin.className = "session-btn";
            btnPinUnpin.textContent = json.pinned ? "Unpin" : "Pin";
            btnPinUnpin.onclick = async () => {
                json.pinned = (!json.pinned) ? 1 : 0;
                let response = await put_entity("r_session", json.id, json)

                btnPinUnpin.textContent = json.pinned ? "Unpin" : "Pin";
                get_element("Pinned").innerText = json.pinned ? "Yes" : "No";

                //render(screen_sessions);
            };
            actions.appendChild(btnPinUnpin);

            card.appendChild(actions);
        });

        // Pagination controls
        render_pagination_controls();
    }

    function render_pagination_controls() {
        const footer = document.createElement("div");
        footer.className = "pagination-footer";
        footer.innerHTML = `
        <button class="session-btn" data-action="first" ${currentPage === 1 ? "disabled" : ""}>⏮ First</button>
        <button class="session-btn" data-action="prev" ${currentPage === 1 ? "disabled" : ""}>◀ Prev</button>
        <span class="page-info">Page ${currentPage} / ${totalPages}</span>
        <button class="session-btn" data-action="next" ${currentPage === totalPages ? "disabled" : ""}>Next ▶</button>
        <button class="session-btn" data-action="last" ${currentPage === totalPages ? "disabled" : ""}>Last ⏭</button>
    `;

        const btnFirst = footer.querySelector('button[data-action="first"]');
        const btnPrev = footer.querySelector('button[data-action="prev"]');
        const btnNext = footer.querySelector('button[data-action="next"]');
        const btnLast = footer.querySelector('button[data-action="last"]');

        btnFirst.onclick = () => {
            currentPage = 1;
            render_screen_sessions();
        };
        btnPrev.onclick = () => {
            if (currentPage > 1) {
                currentPage--;
                render_screen_sessions();
            }
        };
        btnNext.onclick = () => {
            if (currentPage < totalPages) {
                currentPage++;
                render_screen_sessions();
            }
        };
        btnLast.onclick = () => {
            currentPage = totalPages;
            render_screen_sessions();
        };

        main_content.appendChild(footer);
    }

    function algoName(a) {
        switch (a) {
            case 0:
                return "R-0";
            case 2:
                return "R-2";
            case 4:
                return "R-4";
            case 18:
                return "R-18";
            default:
                return "Unknown";
        }
    }

    function scopeName(s) {
        switch (s) {
            case 0:
                return "DueOnly";
            case 1:
                return "NewOnly";
            case 2:
                return "DueAndNew";
            case 3:
                return "All";
            default:
                return "Unknown";
        }
    }

    function scheduleName(s) {
        switch (Number(s)) {
            case 0:
                return "Depth First";
            case 1:
                return "Breadth First";
            case 2:
                return "Random";
            case 3:
                return "Depth First Shuffled";
            case 4:
                return "Interleaved";
            case 5:
                return "Difficulty Sorted";
            case 6:
                return "Chronological";
            default:
                return "Unknown";
        }
    }

    function formatJson(str) {
        try {
            let obj = typeof str === "string" ? JSON.parse(str) : str;
            return JSON.stringify(obj, null, 2);
        } catch {
            return str;
        }
    }

    function render_screen_new_session() {
        refresh_param_screen()

        main_content.innerHTML = "";
        main_content.style.textAlign = "left";

        function create_label(text, for_id) {
            let label = document.createElement("label")
            label.innerText = text
            label.for = for_id
            return label
        }

        function create_input(type, id, value = "") {
            let input = document.createElement("input")
            input.type = type;
            input.id = id
            input.name = id
            input.value = value === "" && type === "number" ? 0 : value
            return input
        }

        function create_br() {
            return document.createElement("br")
        }

        function make_input(text, id, type = "text") {
            let div = document.createElement("div")
            main_content.appendChild(div)
            div.style.marginBottom = "10px";
            if (type !== "hidden") div.appendChild(create_label(text + ":", id))
            div.appendChild(create_input(type, id))
            if (type !== "hidden") div.appendChild(create_br())
        }

        function make_select(text, id, options) {
            let div = document.createElement("div")

            main_content.appendChild(div)
            div.style.marginBottom = "10px";
            div.appendChild(create_label(text + ":", id))
            let select = document.createElement("select")
            select.name = id
            select.id = id
            div.appendChild(select)
            div.appendChild(create_br())
            options.forEach(option => {
                select.appendChild(option)
            })
        }

        function make_option(text, value, selected = false) {
            let option = document.createElement("option")
            option.value = value
            option.innerText = text
            if (selected) option.selected = true
            return option;
        }

        let button_save_session = document.createElement("button")
        main_content.appendChild(button_save_session)
        button_save_session.innerText = "Save"
        main_content.appendChild(create_br())
        main_content.appendChild(create_br())

        let cloned = clone_from_r_session !== null;
        let clone_algorithm = cloned ? clone_from_r_session.algorithm : null
        let clone_schedule = cloned ? clone_from_r_session.schedule : null
        let clone_scope = cloned ? clone_from_r_session.scope : null
        make_input("Map ID", "new_session_map_id", "number")
        if (cloned) get_element("new_session_map_id").value = clone_from_r_session.map_id
        make_input("Map Collection ID", "new_session_map_collection_id", "number")
        if (cloned) get_element("new_session_map_collection_id").value = clone_from_r_session.map_collection_id

        make_input("Cloned from session", "new_session_cloned_from_session_id", "text")
        get_element("new_session_cloned_from_session_id").readOnly = true
        if (cloned) get_element("new_session_cloned_from_session_id").value = clone_from_r_session.id

        let algorithms = [
            make_option("R-0", 0, clone_algorithm === 0),
            make_option("R-2", 2, clone_algorithm === 2),
            make_option("R-4", 4, clone_algorithm === 4),
            make_option("R-18", 18, cloned ? clone_algorithm === 18 : true),
        ]
        make_select("Algorithm", "new_session_algorithm", algorithms)

        let schedules = [
            make_option("Depth First", 0, clone_schedule === 0),
            make_option("Breadth First", 1, cloned ? clone_schedule === 1 : true),
            make_option("Random", 2, clone_schedule === 2),
            make_option("Depth First Shuffled", 3, clone_schedule === 3),
            make_option("Interleaved", 4, clone_schedule === 4),
            make_option("Difficulty Sorted", 5, clone_schedule === 5),
            make_option("Chronological", 6, clone_schedule === 6)
        ];

        make_select("Schedule", "new_session_schedule", schedules);
        if (cloned && clone_from_r_session.schedule !== undefined)
            get_element("new_session_schedule").value = clone_from_r_session.schedule;

        let scopes = [
            make_option("DueOnly", 0, clone_scope === 0),
            make_option("NewOnly", 1, clone_scope === 1),
            make_option("DueAndNew", 2, cloned ? clone_scope === 2 : true),
            make_option("All", 3, clone_scope === 3)
        ]

        make_select("Scope", "new_session_scope", scopes)
        make_input("Description", "new_session_description")

        make_input("Filter under note", "new_session_filter_under_note", "text")
        if (cloned) get_element("new_session_filter_under_note").value = clone_from_r_session.filter_under_note

        function unix_ms_to_yyyymmdd(ms) {
            if (ms === null || ms === undefined || ms === 0) return "";
            const d = new Date(ms);
            if (isNaN(d.getTime())) return "";

            const year = d.getUTCFullYear();
            const month = String(d.getUTCMonth() + 1).padStart(2, '0');
            const day = String(d.getUTCDate()).padStart(2, '0');
            return `${year}-${month}-${day}`;
        }

        make_input("Filter date from", "new_session_filter_date_from", "date")
        if (cloned) get_element("new_session_filter_date_from").value = unix_ms_to_yyyymmdd(clone_from_r_session.filter_date_from)

        make_input("Filter date to", "new_session_filter_date_to", "date")
        if (cloned) get_element("new_session_filter_date_to").value = unix_ms_to_yyyymmdd(clone_from_r_session.filter_date_to)

        make_input("Filter tag", "new_session_filter_tag", "text")
        if (cloned) get_element("new_session_filter_tag").value = clone_from_r_session.filter_tag

        make_input("Filter collection", "new_session_filter_collection", "text")
        if (cloned) get_element("new_session_filter_collection").value = clone_from_r_session.filter_tag

        make_input("Selected items", "new_session_selected_items", "hidden")
        make_input("Pinned", "new_session_pinned", "checkbox")
        button_save_session.onclick = async function () {
            let new_session = {};
            new_session["user_id"] = user_id
            new_session["map_id"] = get_element("new_session_map_id").value;
            new_session["map_collection_id"] = get_element("new_session_map_collection_id").value;
            new_session["cloned_from_session_id"] = get_element("new_session_cloned_from_session_id").value;
            if (new_session["cloned_from_session_id"] === "") new_session["cloned_from_session_id"] = 0
            new_session["algorithm"] = get_element("new_session_algorithm").value;
            new_session["schedule"] = get_element("new_session_schedule").value;
            new_session["scope"] = get_element("new_session_scope").value;
            new_session["description"] = get_element("new_session_description").value;

            new_session["filter_under_note"] = get_element("new_session_filter_under_note").value;
            if (new_session["filter_under_note"] === "") new_session["filter_under_note"] = 0

            function yyyymmdd_to_unix_mx(yyyymmdd) {
                if (yyyymmdd === null || yyyymmdd === undefined) {
                    return 0
                }
                let result = new Date(yyyymmdd + "T00:00:00Z").getTime()

                if (isNaN(result)) {
                    return 0;
                }
                return result
            }

            new_session["filter_date_from"] = yyyymmdd_to_unix_mx(get_element("new_session_filter_date_from").value);
            if (new_session["filter_date_from"] === "") new_session["filter_date_from"] = 0

            new_session["filter_date_to"] = yyyymmdd_to_unix_mx(get_element("new_session_filter_date_to").value);
            if (new_session["filter_date_to"] === "") new_session["filter_date_to"] = 0

            new_session["filter_tag"] = get_element("new_session_filter_tag").value;
            if (new_session["filter_tag"] === "") new_session["filter_tag"] = 0

            new_session["filter_collection"] = get_element("new_session_filter_collection").value;
            if (new_session["filter_collection"] === "") new_session["filter_collection"] = 0

            new_session["selected_items"] = get_element("new_session_selected_items").value;
            if (new_session["selected_items"] === "") new_session["selected_items"] = "{}"
            new_session["pinned"] = get_element("new_session_pinned").checked ? 1 : 0;

            let response = await post_entity("r_session", new_session);
            if (response === null) {
                showError("Saving new session failed.")
                return;
            }
            clone_from_r_session = null
            current_screen = screen_sessions;
            render()
        }
    }

// ============================================================
// 🧠 Session-used note tracking (expiring after 1 day)
// ============================================================

// saves note_id to the list of note_ids for the given session 
    function addNoteIdToSession(sessionId, noteId) {
        const key = `r_session_used_notes_${sessionId}`;
        const ttlMs = 24 * 60 * 60 * 1000; // 1 day in milliseconds

        let usedNotes = loadFromLocalStorage(key) || [];

        // only add if not already present
        if (!usedNotes.includes(noteId)) {
            usedNotes.push(noteId);
            saveToLocalStorage(key, usedNotes, ttlMs);
        }
    }

// loads the list of used note_ids (returns [] if expired)
    function getUsedNotesForSession(sessionId) {
        const key = `r_session_used_notes_${sessionId}`;
        return loadFromLocalStorage(key) || [];
    }

// checks if note_id has already been used
    function wasNoteUsed(sessionId, noteId) {
        const usedNotes = getUsedNotesForSession(sessionId);
        return usedNotes.includes(noteId);
    }
    function countOfUsedNotes(sessionId) {
        const usedNotes = getUsedNotesForSession(sessionId);
        return usedNotes.length;
    }

    function showOrHideAnswer() {
        let back = document.querySelector('.back');
        let showbtn = document.querySelector('.show-btn');
        let shown = showbtn.innerText === "Hide answer"

        document.querySelector('.back').style.display = shown ? 'none' : 'block';
        showbtn.innerText = shown ? "Show answer" : "Hide answer";

    }
    window.showOrHideAnswer=showOrHideAnswer

    async function waitForSendOrSkip(selected_grade_ref) {
        return new Promise(resolve => {
            const sendBtn = document.getElementById("send-btn");
            const skipBtn = document.getElementById("skip-btn");

            function cleanup(result) {
                if(sendBtn != null) sendBtn.removeEventListener("click", onSend);
                skipBtn.removeEventListener("click", onSkip);
                resolve(result);
            }

            function onSend() {
                document.activeElement?.blur();
                if (selected_grade_ref.current === -1) {
                    showInfo("⚠️ Please select a grade (0–5) before sending.");
                    return;
                }

                cleanup("send");
            }

            function onSkip() {
                cleanup("skip");
            }

            if(sendBtn != null) sendBtn.addEventListener("click", onSend);
            skipBtn.addEventListener("click", onSkip);
        });
    }


    async function render_screen_new_review() {
        refresh_param_screen()
        main_content.innerHTML = "";

        if (r_session_for_reviews === null || r_session_for_reviews === undefined) {
            showError("Session for reviews is not set. Select session")
            render(screen_sessions)
            return;
        }
        let note_ids = JSON.parse(r_session_for_reviews.selected_items).note_ids
        if (note_ids.length === 0) {
            showInfo("This session has no notes to be reviewed");
            render(screen_sessions)
            return;
        }
        const session_scope_is_all = r_session_for_reviews.scope === 3;
        if(session_scope_is_all) showWarn("The session scope is \"All\". Next review dates will not be updated.")

        let r0 = r_session_for_reviews.algorithm === 0

        const note_id_count = note_ids.length - countOfUsedNotes(r_session_for_reviews.id);
        let note_id_index = 0;
        console.debug(JSON.stringify(note_ids));
        for await (let note_id of note_ids) {
            if (wasNoteUsed(r_session_for_reviews.id, note_id)) {
                console.log("Skipping note with ID " + note_id);
                continue;
            }
            note_id_index++;
            let note = await read_entity("note", note_id)
            main_content.innerHTML = "";

            let progress = document.createElement("div");
            progress.style.fontSize = "1.2em";
            progress.style.marginBottom = "8px";
            progress.innerHTML = `Item ${note_id_index} / ${note_id_count}`;
            main_content.appendChild(progress);

            let content = note.content_id === 0 ? null : await read_entity("content", note.content_id)

            let div_card = document.createElement("div");
            document.createElement("div");
            div_card.classList.add("card");
            main_content.appendChild(div_card);

            let div_front = document.createElement("div");
            div_front.classList.add("front");
            div_front.innerText = "❓ " + note.title;
            div_card.appendChild(div_front);

            let div_back = document.createElement("div");
            div_back.classList.add("back");

            if (content === null || content.value === null) {
                let span = document.createElement("span");
                div_back.innerText = "✅ ";
                span.innerText = "This note is missing the answer";
                div_back.appendChild(span)
            } else {
                function hasMultipleLines(text) {
                    return text.includes('\n');
                }
                let multiple_lines = hasMultipleLines(content.value)
                div_back.innerText = "✅ " + (multiple_lines ? "\n" : "") + content.value;

                if(!multiple_lines) {
                    div_back.style.textAlign = "center";
                }
            }
            div_card.appendChild(div_back);

            let button_show_btn = document.createElement("button");
            button_show_btn.classList.add("show-btn");
            button_show_btn.innerText = "Show answer";
            button_show_btn.addEventListener("click", showOrHideAnswer);
            div_card.appendChild(button_show_btn);

            let div_rating = document.createElement("div");
            div_rating.classList.add("rating");
            div_card.appendChild(div_rating);

            if (!session_scope_is_all) {
                let p_rating = document.createElement("p");
                p_rating.innerText = r0 ? "Did you remember?" : "How well did you recall the note?";
                div_card.appendChild(p_rating);
            }


            let answer_change_count = 0

            let div_rating_buttons = document.createElement("div");
            div_card.appendChild(div_rating_buttons);
            let selected_grade_ref = {current: -1};

            function make_rating_button(grade) {
                let rating_btn = document.createElement("button");
                rating_btn.classList.add("rating-btn");
                rating_btn.id = "rating-btn-" + grade;
                rating_btn.innerText = r0 ? (grade >= 3 ? "Yes" : "No") : grade;

                rating_btn.addEventListener("click", function () {
                    if (selected_grade_ref.current !== -1) {
                        answer_change_count = answer_change_count + 1;
                        console.debug("grade changed")
                    }
                    selected_grade_ref.current = grade;

                    for (let i = 0; i <= 5; i++) {
                        if(r0 && i >0 && i < 5) continue
                        get_element("rating-btn-" + i).className = "rating-btn";
                    }
                    rating_btn.className = "rating-btn rating-btn-selected";
                });
                div_rating_buttons.appendChild(rating_btn);
                return rating_btn;
            }

            if (!session_scope_is_all) {
            if(r0) make_rating_button(5)
            let rating_btn_0 = make_rating_button(0)
            if (!r0) {
                let rating_btn_1 = make_rating_button(1)
                let rating_btn_2 = make_rating_button(2)
                let rating_btn_3 = make_rating_button(3)
                let rating_btn_4 = make_rating_button(4)
            }
            if(!r0) make_rating_button(5)
        }

            // Create collapsible legend for rating scale
            let div_legend_container = document.createElement("div");
            div_legend_container.classList.add("legend-container");
            div_legend_container.style.marginTop = "10px";
            div_card.appendChild(div_legend_container);

// Show/hide button 
            let btn_toggle_legend = session_scope_is_all ? null : document.createElement("button");
            if(!session_scope_is_all && !r0) {
                btn_toggle_legend.classList.add("show-btn");
                btn_toggle_legend.innerText = "Show legend";
                div_legend_container.appendChild(btn_toggle_legend);
            }


// Legend content
            let div_legend = session_scope_is_all ? null : document.createElement("div");
            if(!session_scope_is_all) {
            div_legend.classList.add("rating-legend");
            div_legend.style.display = "none";
            div_legend.style.textAlign = "left";
            div_legend.style.fontSize = "0.9em";
            div_legend.style.maxWidth = "600px";
            div_legend.style.margin = "10px auto";
            div_legend.style.lineHeight = "1.4em";
            div_legend.style.padding = "8px 12px";
            div_legend.style.border = "1px solid rgba(0,0,0,0.1)";
            div_legend.style.borderRadius = "8px";
            div_legend.style.background = "rgba(0,0,0,0.03)";

            function makeLegendLine(num, text) {
                let p = document.createElement("p");
                p.style.margin = "2px 0";
                let b = document.createElement("b");
                b.innerText = num + ": ";
                p.appendChild(b);
                p.append(text);
                return p;
            }

            div_legend.appendChild(makeLegendLine(0, "Total blackout — complete failure to recall the information."));
            div_legend.appendChild(makeLegendLine(1, "Incorrect response, but upon seeing the correct answer it felt familiar."));
            div_legend.appendChild(makeLegendLine(2, "Incorrect response, but upon seeing the correct answer it seemed easy to remember."));
            div_legend.appendChild(makeLegendLine(3, "Correct response, but required significant effort to recall."));
            div_legend.appendChild(makeLegendLine(4, "Correct response, after some hesitation."));
            div_legend.appendChild(makeLegendLine(5, "Correct response with perfect recall."));

            div_legend_container.appendChild(div_legend);
            }

// Toggle display event listener
            if(!session_scope_is_all) btn_toggle_legend.addEventListener("click", () => {
                const isVisible = div_legend.style.display === "block";
                div_legend.style.display = isVisible ? "none" : "block";
                btn_toggle_legend.innerText = isVisible ? "Show legend" : "Hide legend";
            });


            let div_actions = document.createElement("div");
            div_card.appendChild(div_actions);
            if(!session_scope_is_all) {
            let p_actions = document.createElement("p");
            p_actions.innerHTML = "Next action"
            div_actions.appendChild(p_actions);
            }

            let div_next_action = document.createElement("div");
            div_card.appendChild(div_next_action);

            function make_action_button(id, text) {
                let button = document.createElement("button");
                div_next_action.append(button);
                button.classList.add("action-btn");
                button.id = id;
                button.innerText = text;
                return button;
            }

            let button_send_btn = session_scope_is_all ? null : make_action_button("send-btn", "Send")
            let button_skip_btn = make_action_button("skip-btn", session_scope_is_all ? "Next" : "Skip")

            const sw = new StopWatch();
            sw.start();
            const action = await waitForSendOrSkip(selected_grade_ref);
            sw.stop();


            console.debug(sw.elapsedMs + " ms")
            console.log(`User chose: ${action} for note_id=${note_id}`);

            if (action === "send") {
                // store result in DB
                if (selected_grade_ref.current === -1) {
                    showInfo("You did not select a grade.")
                }

                const new_r_review = {
                    user_id: user_id,
                    map_id: r_session_for_reviews.map_id,
                    r_session_id: r_session_for_reviews.id,
                    algorithm: r_session_for_reviews.algorithm,
                    note_id: note_id,
                    review_date: Date.now(),
                    grade: selected_grade_ref.current,
                    response_data: "{}",
                    notes: "",
                    started_at: sw.startTime,
                    ended_at: sw.endTime,
                    latency_ms: sw.elapsedMs,
                    answer_change_count: answer_change_count,
                    details_json: "{}"
                };

                await post_entity("r_review", new_r_review);
                showInfo(`Note was reviewed: #${note_id} ${note.title}`);
                addNoteIdToSession(r_session_for_reviews.id, note_id);

                let states = await list_entities("r" + r_session_for_reviews.algorithm+ "_state","&user_id=" + user_id + "&note_id=" + note_id);
                let state = states.items[0]

                await new Promise(resolve => {
                    main_content.innerHTML = `
            <h3>✅ Note reviewed: ${note.title}</h3>
            <div class="info" id="result">
            📅 Next review : ${state === null ? "?" : formatDateTimeHM(state.next_review)}<br>
            🔢 Repetitions: ${state === null ? "?" : state.repetitions}
            </div>
            <button id="btn_next_note">Next Note</button>
            <button id="btn_test_note">Test</button>
            
        `;

                    get_element("btn_next_note").onclick = () => resolve();
                    get_element("btn_test_note").onclick = async () => {
                        await run_questions_for_note(note_id);
                        resolve();
                    };

                });
            }
            else if (action === "skip") {
                // skipped - don't store anything
                if(!session_scope_is_all) showInfo("Note was skipped: #" + note_id + " " + note.title)
            }

        }

        main_content.innerHTML = "<p>All notes in this session were reviewed.</p>"
        r_session_for_reviews = null;
    }

    async function run_questions_for_note(note_id) {
        main_content.innerHTML = `
        <h3>🧩 Testing questions for note #${note_id}</h3>
        <div id="test_container"></div>
        <div id="test_feedback" style="margin-top:10px;font-weight:bold;"></div>
    `;
        const container = get_element("test_container");
        const feedback = get_element("test_feedback");

        const response = await list_entities("question", "note_id=" + note_id);
        const questions = response.items || response;
        if (!questions || questions.length === 0) {
            showInfo("No questions for this note.");
            return { correctCount: 0, attemptedCount: 0, percent: 0 };
        }

        let correctCount = 0;
        let attemptedCount = 0;
        let skippedCount = 0;

        for (let q of questions) {
            const result = await render_single_question(q, container, feedback);
            if (result === "quit") break;
            if (result === "skip") {
                skippedCount++;
                continue;
            }
            if (result === "correct") correctCount++;
            if (result === "correct" || result === "wrong") attemptedCount++;
        }

        const percent =
            attemptedCount > 0
                ? ((correctCount / attemptedCount) * 100).toFixed(1)
                : "0.0";

        feedback.innerText = "";
        container.innerHTML = `
        <h3>✅ Test completed</h3>
        <p>Correct answers: ${correctCount} / ${attemptedCount}</p>
        <p>Accuracy: ${percent}%</p>
        <p>Skipped: ${skippedCount}</p>
        <button id="btn_back_to_notes">Back to Notes</button>
    `;

        return new Promise(resolve => {
            get_element("btn_back_to_notes").onclick = () =>
                resolve({ correctCount, attemptedCount, percent });
        });
    }

    async function render_single_question(q, container, feedback) {
        return new Promise(resolve => {
            container.innerHTML = "";
            feedback.innerText = "";

            const card = document.createElement("div");
            card.classList.add("card");
            container.appendChild(card);

            const questionDiv = document.createElement("div");
            questionDiv.classList.add("front");
            questionDiv.innerText = "❓ " + q.question_text;
            card.appendChild(questionDiv);

            const answerDiv = document.createElement("div");
            answerDiv.classList.add("back");
            answerDiv.innerText = "Answer: " + (q.answers_json || "unknown");
            answerDiv.style.display = "none";
            card.appendChild(answerDiv);

            const btnYes = document.createElement("button");
            const btnNo = document.createElement("button");
            const btnShow = document.createElement("button");
            const btnSend = document.createElement("button");
            const btnSkip = document.createElement("button");
            const btnQuit = document.createElement("button");

            btnYes.innerText = "Yes";
            btnNo.innerText = "No";
            btnShow.innerText = "Show";
            btnSend.innerText = "Send";
            btnSkip.innerText = "Skip";
            btnQuit.innerText = "Quit";

            const pAsk = document.createElement("p");
            pAsk.innerText = "Did you know?";
            card.appendChild(pAsk);

            let selected = null;

            [btnYes, btnNo].forEach(b => {
                b.classList.add("rating-btn");
                card.appendChild(b);
                b.onclick = () => {
                    selected = b.innerText.toLowerCase();
                    btnYes.classList.remove("rating-btn-selected");
                    btnNo.classList.remove("rating-btn-selected");
                    b.classList.add("rating-btn-selected");
                };
            });

            card.appendChild(document.createElement("br"));
            [btnShow, btnSend, btnSkip, btnQuit].forEach(b => {
                b.classList.add("action-btn");
                card.appendChild(b);
            });

            btnShow.onclick = () => {
                const visible = answerDiv.style.display === "block";
                answerDiv.style.display = visible ? "none" : "block";
                btnShow.innerText = visible ? "Show" : "Hide";
            };

            btnSend.onclick = () => {
                if (!selected) {
                    feedback.innerText = "⚠️ Please select Yes or No first.";
                    return;
                }

                const correct = q.correct_answer
                    ? q.correct_answer.toLowerCase() === selected
                    : selected === "yes"; // fallback if not defined
                feedback.innerText = correct ? "✅ Correct!" : "❌ Wrong!";
                setTimeout(() => resolve(correct ? "correct" : "wrong"), 800);
            };

            btnSkip.onclick = () => {
                feedback.innerText = "⏭️ Skipped.";
                setTimeout(() => resolve("skip"), 400);
            };

            btnQuit.onclick = () => resolve("quit");
        });
    }


    function render(new_current_screen = null) {
        if (new_current_screen !== null && new_current_screen !== undefined) {
            current_screen = new_current_screen;
        }
        main_content.style.textAlign = "center";
        r_global_settings = getOrFetchFromLocalStorage("r_global_settings", load_r_global_settings, sixty_minutes)
        r_user_settings = getOrFetchFromLocalStorage("r_user_settings", load_r_user_settings, sixty_minutes)

        switch (current_screen) {
            case screen_new_session:
                render_screen_new_session();
                break;
            case screen_sessions:
                render_screen_sessions();
                break;
            case screen_new_review :
                render_screen_new_review();
                break;
            case screen_home:
                render_screen_home();
                break;
            case screen_demo:
                render_screen_demo();
                break;
            default:
                alert("Unknown screen " + current_screen);
                render_screen_home()
        }
    }

    render()

});

export function rate_demo(q) {
    get_element("rating-btn-0").className = "rating-btn"
    get_element("rating-btn-1").className = "rating-btn"
    get_element("rating-btn-2").className = "rating-btn"
    get_element("rating-btn-3").className = "rating-btn"
    get_element("rating-btn-4").className = "rating-btn"
    get_element("rating-btn-5").className = "rating-btn"

    get_element("rating-btn-" + q).className = "rating-btn rating-btn-selected"

    const S = 1.0; // current stability
    const b = 0.6;
    const R_target = 0.9;
    const elapsed = 1.0; // days since last repetition
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
        next_I = 0.5; // short retry
    }

    document.getElementById('result').innerHTML =
        `📈 Retrievability: ${R_now.toFixed(3)}<br>
         📅 Next repetition in: ${next_I.toFixed(2)} days<br>
         🧠 New stability: ${S_after.toFixed(2)} days`;
}

window.rate_demo = rate_demo;