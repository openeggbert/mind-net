// ========================================
// Imports & Globals
// ========================================

import {
    getUserId, list_all_entities, post_entity,
    list_entities} from "./api.js";
import {
    chooseOption,
    formatDate,
    formatDateTime, formatDateTimeHM,
    get_element,
    getOrFetchFromLocalStorage,
    minutes_to_ms
} from "./dom.js";
import {  } from "./api.js";


let user_id = null
let r_global_settings = {};
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

    user_id = getUserId()
    let main = get_element("main")
    let main_content = document.createElement("div")
    main_content.id = "main_content"
    main_content.style.margin = "0 auto";
    main_content.style.textAlign = "center";
    main.appendChild(main_content)

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
    if(param_screen !== null && param_screen !== undefined) {
        current_screen = param_screen;
    }

    //
    get_element("repetition_header").title = "Go to list of all sessions"
    get_element("repetition_header").style.cursor = "pointer"

    get_element("button_mindnet").addEventListener("click", ()=> {window.location.href='index.html'});
    get_element("button_mindnet").title = "Go to Mind Net generic frontend"

    get_element("button_menu").addEventListener("click", () => {
        current_screen = screen_home;
        render()
    });
    get_element("button_menu").title = "Show the Menu"

    get_element("button_theme").addEventListener("click", () => {document.body.classList.toggle("dark");});
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

        r_user_settings_tmp.forEach(json =>
        {
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
            button.onclick = function() {
                current_screen = screen
                render()
            }
        }

        make_button("New session", screen_new_session)
        make_button("Sessions", screen_sessions)
        make_button("Global settings",screen_global_settings)
        make_button("User settings",screen_user_settings)
    }

    let currentPage = 1;
    let totalPages = 1;
    const pageSize = 10;

    async function render_screen_sessions() {
        refresh_param_screen();
        main_content.innerHTML = "<h2>Your Sessions</h2>";

        const params = "&user_id=" + user_id + "&sort=created_at&order=desc";

        const json = await list_entities("r_session", params, currentPage, pageSize);

        const r_sessions = json.items || [];
        totalPages = json.total_pages || 1;

        if (r_sessions.length === 0) {
            main_content.innerHTML += "<p>No sessions found.</p>";
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

            // Header
            let title = document.createElement("div");
            title.className = "session-title";
            title.innerHTML = `Session #${json.id} — ${algoName(json.algorithm)} (${scopeName(json.scope)})`;
            card.appendChild(title);

            // Meta info
            let meta = document.createElement("div");
            meta.className = "session-meta";
            meta.innerHTML = `
            🕓 Created: ${formatDateTimeHM(json.created_at)}<br>
            🔄 Updated: ${formatDateTimeHM(json.updated_at)}<br>
            🧩 Map ID: ${json.map_id}<br>
            📑 Cloned from: ${json.cloned_from_session_id || "-"}<br>
            📌 Pinned: ${json.pinned ? "Yes" : "No"}
        `;
            card.appendChild(meta);

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
                <tr><th>Algorithm</th><td>${algoName(json.algorithm)}</td></tr>
                <tr><th>Notes</th><td>${json.notes ? "✅" : "❌"}</td></tr>
                <tr><th>Questions</th><td>${json.questions ? "✅" : "❌"}</td></tr>
                <tr><th>Scope</th><td>${scopeName(json.scope)}</td></tr>
                <tr><th>Filter under note</th><td>${json.filter_under_note || "-"}</td></tr>
                <tr><th>Filter date from</th><td>${formatDateTimeHM(json.filter_date_from)}</td></tr>
                <tr><th>Filter date to</th><td>${formatDateTimeHM(json.filter_date_to)}</td></tr>
                <tr><th>Filter tag</th><td>${json.filter_tag || "-"}</td></tr>
                <tr><th>Filter collection</th><td>${json.filter_collection || "-"}</td></tr>
                <tr><th>Selected items</th><td><pre>${formatJson(json.selected_items)}</pre></td></tr>
            </table>
        `;
            card.appendChild(details);

            // Actions
            let actions = document.createElement("div");
            actions.className = "session-actions";

            let btnRun = document.createElement("button");
            btnRun.className = "session-btn";
            btnRun.textContent = "Run";
            btnRun.onclick = () => {
                alert("Run session " + json.id);
            };
            actions.appendChild(btnRun);

            let btnClone = document.createElement("button");
            btnClone.className = "session-btn danger";
            btnClone.textContent = "Clone";
            btnClone.onclick = () => {
                //if (confirm("Clone session #" + json.id + "?")) {
                    clone_from_r_session = json;
                    alert(JSON.stringify(clone_from_r_session));
                    render(screen_new_session);
                //}
            };
            actions.appendChild(btnClone);

            card.appendChild(actions);
            container.appendChild(card);
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
        const btnPrev  = footer.querySelector('button[data-action="prev"]');
        const btnNext  = footer.querySelector('button[data-action="next"]');
        const btnLast  = footer.querySelector('button[data-action="last"]');

        btnFirst.onclick = () => { currentPage = 1; render_screen_sessions(); };
        btnPrev.onclick  = () => { if (currentPage > 1) { currentPage--; render_screen_sessions(); } };
        btnNext.onclick  = () => { if (currentPage < totalPages) { currentPage++; render_screen_sessions(); } };
        btnLast.onclick  = () => { currentPage = totalPages; render_screen_sessions(); };

        main_content.appendChild(footer);
    }

    function algoName(a) {
        switch (a) {
            case 0: return "R-0";
            case 2: return "R-2";
            case 4: return "R-4";
            case 18: return "R-18";
            default: return "Unknown";
        }
    }
    function scopeName(s) {
        switch (s) {
            case 0: return "Manual";
            case 1: return "DueOnly";
            case 2: return "NewOnly";
            case 3: return "DueAndNew";
            default: return "Unknown";
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
        function make_input(text,id,type){
            let div = document.createElement("div")
            main_content.appendChild(div)
            div.style.marginBottom = "10px";
            if(type !== "hidden") div.appendChild(create_label(text + ":",id))
            div.appendChild(create_input(type,id))
            if(type !== "hidden") div.appendChild(create_br())
        }
        function make_select(text, id, options){
            let div = document.createElement("div")

            main_content.appendChild(div)
            div.style.marginBottom = "10px";
            div.appendChild(create_label(text + ":",id))
            let select = document.createElement("select")
            select.name = id
            select.id = id
            div.appendChild(select)
            div.appendChild(create_br())
            options.forEach(option => {
                select.appendChild(option)
            })
        }
        function make_option(text, value, selected = false){
            let option = document.createElement("option")
            option.value = value
            option.innerText = text
            if(selected) option.selected = true
            return option;
        }

        let button_save_session = document.createElement("button")
        main_content.appendChild(button_save_session)
        button_save_session.innerText = "Save"
        main_content.appendChild(create_br())
        main_content.appendChild(create_br())

        let cloned = clone_from_r_session !== null;
        let clone_algorithm = cloned ? clone_from_r_session.algorithm : null
        let clone_scope = cloned ? clone_from_r_session.scope : null
        make_input("Map ID", "new_session_map_id", "number")
        if(cloned) get_element("new_session_map_id").value = clone_from_r_session.map_id

        make_input("Cloned from session", "new_session_cloned_from_session_id", "text")
        get_element("new_session_cloned_from_session_id").readOnly = true
        if(cloned) get_element("new_session_cloned_from_session_id").value = clone_from_r_session.id
        
        let algorithms =  [
            make_option("R-0", 0, clone_algorithm === 0),
            make_option("R-2", 2, clone_algorithm === 2),
            make_option("R-4", 4, clone_algorithm === 4),
            make_option("R-18", 18, cloned ? clone_algorithm === 18 : true),
        ]
        make_select("Algorithm", "new_session_algorithm",algorithms)

        make_input("Notes", "new_session_notes", "checkbox")
        get_element("new_session_notes").checked = true
        if(cloned) get_element("new_session_notes").checked = clone_from_r_session.notes === 1

        make_input("Questions", "new_session_questions", "checkbox")
        if(cloned) get_element("new_session_questions").checked = clone_from_r_session.questions === 1

        let scopes = [
            make_option("Manual", 0, clone_scope === 0),
            make_option("DueOnly", 1, clone_scope === 1),
            make_option("NewOnly", 2, clone_scope === 2),
            make_option("DueAndNew", 3, cloned ? clone_scope === 3 : true)
        ]

        make_select("Scope", "new_session_scope", scopes)
        
        make_input("Filter under note", "new_session_filter_under_note", "text")
        if(cloned) get_element("new_session_filter_under_note").value = clone_from_r_session.filter_under_note

        make_input("Filter date from", "new_session_filter_date_from", "date")
        if(cloned) get_element("new_session_filter_date_from").value = clone_from_r_session.filter_date_from

        make_input("Filter date to", "new_session_filter_date_to", "date")
        if(cloned) get_element("new_session_filter_date_to").value = clone_from_r_session.filter_date_to

        make_input("Filter tag", "new_session_filter_tag", "text")
        if(cloned) get_element("new_session_filter_tag").value = clone_from_r_session.filter_tag

        make_input("Filter collection", "new_session_filter_collection", "text")
        if(cloned) get_element("new_session_filter_collection").value = clone_from_r_session.filter_tag

        make_input("Selected items", "new_session_selected_items", "text")
        make_input("Pinned", "new_session_pinned", "checkbox")
        button_save_session.onclick = async function() {
            let new_session = {};
            new_session["user_id"] = user_id
            new_session["map_id"] = get_element("new_session_map_id").value;
            new_session["cloned_from_session_id"] = get_element("new_session_cloned_from_session_id").value;
            if(new_session["cloned_from_session_id"] === "")new_session["cloned_from_session_id"] = 0
            new_session["algorithm"] = get_element("new_session_algorithm").value;
            new_session["notes"]=get_element("new_session_notes").checked ? 1 : 0;
            new_session["questions"]=get_element("new_session_questions").checked ? 1 : 0;
            new_session["scope"] = get_element("new_session_scope").value;

            new_session["filter_under_note"] = get_element("new_session_filter_under_note").value;
            if(new_session["filter_under_note"] === "")new_session["filter_under_note"] = 0

            function yyyymmdd_to_unix_mx(yyyymmdd) {
                if(yyyymmdd === null || yyyymmdd === undefined) {return 0}
                let result = new Date(yyyymmdd + "T00:00:00Z").getTime()

                if (isNaN(result)) {
                    return 0;
                }
                return result
            }
            new_session["filter_date_from"] = yyyymmdd_to_unix_mx(get_element("new_session_filter_date_from").value);
            if(new_session["filter_date_from"] === "")new_session["filter_date_from"] = 0

            new_session["filter_date_to"] = yyyymmdd_to_unix_mx(get_element("new_session_filter_date_to").value);
            if(new_session["filter_date_to"] === "")new_session["filter_date_to"] = 0

            new_session["filter_tag"] = get_element("new_session_filter_tag").value;
            if(new_session["filter_tag"] === "")new_session["filter_tag"] = 0

            new_session["filter_collection"] = get_element("new_session_filter_collection").value;
            if(new_session["filter_collection"] === "")new_session["filter_collection"] = 0


            new_session["selected_items"] = get_element("new_session_selected_items").value;
            if(new_session["selected_items"] === "")new_session["selected_items"] = "{}"
            new_session["pinned"] = get_element("new_session_pinned").checked ? 1 : 0;

            alert(JSON.stringify(new_session));
            let response = await post_entity("r_session", new_session);
            if(response === null) {
                alert("Saving new session failed.")
                return;
            }
            clone_from_r_session = null
            current_screen = screen_sessions;
            render()
        }
    }

    function render(new_current_screen = null) {
        if(new_current_screen !== null&& new_current_screen !== undefined) {
            current_screen = new_current_screen;
        }
        main_content.style.textAlign = "center";
        r_global_settings = getOrFetchFromLocalStorage("r_global_settings", load_r_global_settings, sixty_minutes)
        r_user_settings = getOrFetchFromLocalStorage("r_user_settings", load_r_user_settings, sixty_minutes)

        switch(current_screen) {
            case screen_new_session: render_screen_new_session(); break;
            case screen_sessions: render_screen_sessions(); break;
            case screen_home: render_screen_home(); break;
            default: alert("Unknown screen " + current_screen); render_screen_home()
        }
    }

    render()


    // await chooseOption(["a", "b", "c"])
    // let new_session_table = document.createElement("table");
    // main.appendChild(new_session_table);
    // let new_session_table_tr_1 = document.createElement("tr");
    // new_session_table.appendChild(new_session_table_tr_1);
    // let new_session_table_tr_2 = document.createElement("tr");
    // new_session_table.appendChild(new_session_table_tr_2);
    // function create_th(text) {
    //     let th = document.createElement("th")
    //     th.innerText = text
    //     return th
    // }
    // new_session_table_tr_1.appendChild(create_th("Map"))
    // new_session_table_tr_1.appendChild(create_th("Cloned from session"))
    // new_session_table_tr_1.appendChild(create_th("Algorithm"))
    // new_session_table_tr_1.appendChild(create_th("Notes"))
    // new_session_table_tr_1.appendChild(create_th("Questions"))
    // new_session_table_tr_1.appendChild(create_th("Scope"))
    // new_session_table_tr_1.appendChild(create_th("Filter under note"))
    // new_session_table_tr_1.appendChild(create_th("Filter date from"))
    // new_session_table_tr_1.appendChild(create_th("Filter date to"))
    // new_session_table_tr_1.appendChild(create_th("Filter tag"))
    // new_session_table_tr_1.appendChild(create_th("Filter collection"))
    // new_session_table_tr_1.appendChild(create_th("Selected items"))
    // new_session_table_tr_1.appendChild(create_th("Pinned"))


});

export function showOrHideAnswer() {

    let back = document.querySelector('.back');
    let showbtn = document.querySelector('.show-btn');
    let shown = showbtn.innerText === "Hide answer"


    document.querySelector('.back').style.display = shown ? 'none' : 'block';
    showbtn.innerText = shown ? "Show answer" : "Hide answer";

}
window.showOrHideAnswer = showOrHideAnswer;

export function rate(q) {
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
         📅 Další opakování za: ${next_I.toFixed(2)} dní<br>
         🧠 Nová stabilita: ${S_after.toFixed(2)} dní`;
}
window.rate = rate;