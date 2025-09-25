import {loadModelDefinition} from "./api.js";
import {buildEntitySchemas, buildGlobals} from "./schemas.js";
import {setEntitySchemas, setEntities, setEntityLabels, setActions,
    getEntitySchemas, getEntities, getActions} from "./state.js";
import {initializeFromURL} from "./init.js";
import {renderLoginForm, renderAuthStatus} from "./auth-ui.js";
import {getAccessToken} from "./api.js";

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
    if (getAccessToken()) {
        renderAuthStatus();
    } else {
        renderLoginForm();
    }
})();
