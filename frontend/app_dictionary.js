import {DictionaryApp} from "./dictionary/core/DictionaryApp.js";
import {get_element} from "./dom.js";

document.addEventListener('DOMContentLoaded', async () => {
    await init_dom();
});

async function init_dom() {
    let dictionary_app = null
    dictionary_app = new DictionaryApp()
    await dictionary_app.init()
    get_element("main_container").style.display = "block"
    get_element("loading_div").style.display= "none"

    window.addEventListener("popstate", () => {
        const params = new URLSearchParams(window.location.search);
        console.log(params.get("map_id"), params.get("term_id"));
        let map_id = params.get("map_id")
        let term_id = params.get("term_id")

        if(map_id){
            let map_id_ = Number(map_id)
            if(map_id_>0) dictionary_app.set_selected_map_id(map_id_)
        }
        if(term_id){
            let term_id_ = Number(term_id)
            if(term_id_>0) dictionary_app.render(term_id_)
        }
    });

}
