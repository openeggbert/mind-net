import {loadModelDefinition} from "./api.js";

import { buildEntitySchemas, buildGlobals } from "./schemas.js";

// State
import {
    entities, entityLabels, actions,
    entitySchemas
} from "./state.js";

// Navigation & UI
import { initializeFromURL } from "./init.js";

(async () => {
    const modelDef = await loadModelDefinition();

    entitySchemas = buildEntitySchemas(modelDef); // populate global
    const globals = buildGlobals(modelDef, entitySchemas);

    entities = globals.entities;           // populate global
    entityLabels = globals.entityLabels;   // populate global
    actions = globals.actions;             // populate global

    console.log("Schemas:", entitySchemas);
    console.log("Entities:", entities);
    console.log("Labels:", entityLabels);
    console.log("Actions:", actions);

    initializeFromURL(); // ← start after model load
})();








