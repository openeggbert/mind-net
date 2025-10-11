// ========================================
// Imports & Globals
// ========================================

import {getUserId, list_all_entities} from "./api.js";
import {chooseOption, get_element, getOrFetchFromLocalStorage, minutes_to_ms} from "./dom.js";

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
    function render_screen_home() {

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
    async function render_screen_sessions() {
        main_content.innerHTML = "";

        let r_sessions = await list_all_entities("r_session", "user_id=" + user_id)

        if(r_sessions.length > 0){
            let ul = document.createElement("ul");
            main_content.appendChild(ul)
            r_sessions.forEach(json => {
                let li = document.createElement("li");
                li.innerHTML = JSON.stringify(json.value);
                ul.appendChild(li)
            })
        } else{
            main_content.innerHTML = "<p>No sessions found</p>";
        }

    }
    function render_screen_new_session() {

        main_content.innerHTML = "";

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
            div.appendChild(create_label(text + ":",id))
            div.appendChild(create_input(type,id))
            div.appendChild(create_br())
        }

        make_input("Map ID", "new_session_map_id", "number")
        make_input("Cloned from session", "new_session_clone_id", "text")
        make_input("Algorithm", "new_session_algorithm", "text")
        make_input("Notes", "new_session_notes", "text")
        make_input("Questions", "new_session_questions", "text")
        make_input("Scope", "new_session_scope", "text")
        make_input("Filter under note", "new_session_filter_under_note", "text")
        make_input("Filter date from", "new_session_filter_date_from", "date")
        make_input("Filter date to", "new_session_filter_date_to", "date")
        make_input("Filter tag", "new_session_filter_tag", "text")
        make_input("Filter collection", "new_session_filter_collection", "text")
        make_input("Selected items", "new_session_selected_items", "text")
        make_input("Pinned", "new_session_pinned", "checkbox")
    }

    function render() {
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