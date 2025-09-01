import {
    selectedEntity,
    selectedActionId,
} from "./state.js";

// ========================================
// Global action helpers
// ========================================

import {API_BASE} from "./api";

window.readEntity = (entity, id) => {
    selectedEntity = entity;
    selectedActionId = id;
    selectAction(entitySchemas[entity] ? "read" : "list", id);
    history.pushState({}, "", `?entity=${encodeURIComponent(entity)}&action=read&id=${encodeURIComponent(id)}`);
}

window.editEntity = (entity, data) => {
    selectedEntity = entity;
    selectedActionId = data.id;
    selectAction("update", data.id);
    history.pushState({}, "", `?entity=${encodeURIComponent(entity)}&action=update&id=${encodeURIComponent(data.id)}`);
}

window.deleteEntity = async (entity, id) => {
    if (!confirm("Do you really want to delete this record?")) return;
    await fetch(`${API_BASE}/${entity}/${id}`, {method: "DELETE"});
    // po smazání vždy přejdi na list
    selectedActionId = null;
    selectAction("list", null);
}
