import {loadModelDefinition} from "./api.js";

import { buildEntitySchemas, buildGlobals } from "./schemas.js";

// State
import {
    getActions,
    getEntities,
    getEntitySchemas, setActions, setEntities, setEntityLabels, setEntitySchemas
} from "./state.js";

// Navigation & UI
import { initializeFromURL } from "./init.js";

(async () => {
    const modelDef = await loadModelDefinition();
    console.log("modelDef:", modelDef);


    const schemas = buildEntitySchemas(modelDef);
    console.log("buildEntitySchemas returned:", schemas);

    setEntitySchemas(schemas);

    const globals = buildGlobals(modelDef, schemas);
    setEntities(globals.entities);
    setEntityLabels(globals.entityLabels);
    setActions(globals.actions);



    console.log("Model definition:", modelDef);
    console.log("Entity Schemas:", getEntitySchemas());
    console.log("Entities:", getEntities());
    console.log("Actions:", getActions());



    initializeFromURL(); // ← start after model load
})();








