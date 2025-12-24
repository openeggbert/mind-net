// Dictionary UI Architecture (vanilla JS)
//
// - Core
// - app_dictionary.js        // lifecycle, init, orchestration
//
// - State & Config
// - d_globals.js
// - d_enums.js
// - d_styles_enums.js
//
// - UI Infrastructure
// - d_dom.js
// - d_window.js
// - d_styles.js
//
// - Domain Features
// - d_entities.js            // term, note, tag, link…
// - d_search.js              // advanced search logic
// - d_markdown.js            // rendering layer

import {DictionaryApp} from "./dictionary/core/DictionaryApp.js";

document.addEventListener('DOMContentLoaded', async () => {
    await init_dom();
});

async function init_dom() {
    let dictionary_app = null
    dictionary_app = new DictionaryApp()
}
