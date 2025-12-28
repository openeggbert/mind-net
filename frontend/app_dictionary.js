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
}
