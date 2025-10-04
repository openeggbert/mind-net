import {loadModelDefinition} from "./api.js";
import {buildEntitySchemas, buildGlobals} from "./schemas.js";
import {
    setEntitySchemas, setEntities, setEntityLabels, setActions,
    getEntitySchemas, getEntities, getActions, getSelectedEntity
} from "./state.js";
import {initializeFromURL} from "./init.js";
import {renderLoginForm, renderAuthStatus} from "./auth-ui.js";
import {getAccessToken, loadApplications} from "./api.js";

function toTitleCase(str) {
    return str
        .replace(/[_-]+/g, " ")           // replace _ and - with space
        .split(" ")                       // split into words
        .map(word =>
            word.charAt(0).toUpperCase() +  // first letter uppercase
            word.slice(1).toLowerCase()     // rest lowercase
        )
        .join(" ");
}


(async () => {
    const modelDef = await loadModelDefinition();
    console.log("modelDef:", modelDef);

    const schemas = buildEntitySchemas(modelDef);
    setEntitySchemas(schemas);

    const globals = buildGlobals(modelDef, schemas);
    setEntities(globals.entities);
    setEntityLabels(globals.entityLabels);
    setActions(globals.actions);

    console.log("Entity Schemas:", getEntitySchemas());
    console.log("Entities:", getEntities());
    console.log("Actions:", getActions());

    // ⚡ initialize UI from URL (CRUD navigation)
    initializeFromURL();

    // 🔑 auth UI
    if (!getSelectedEntity()) {
        if (getAccessToken()) {
            renderAuthStatus();
        } else {
            renderLoginForm();
        }

        let contentArea = document.getElementById("contentArea");
        let apps_h3 = document.createElement("h3");
        apps_h3.innerText = "Available apps"
        contentArea.appendChild(apps_h3);
        let ul = document.createElement("ul");
        contentArea.appendChild(ul);

        await (async () => {
            const apps = await loadApplications();
            apps.forEach(app => {
                let li = document.createElement("li");
                ul.appendChild(li);
                let a = document.createElement("a");
                li.appendChild(a);
                a.innerText = app;
                a.href = "app_" + app + ".html";
            })
        })();
    }


})();
