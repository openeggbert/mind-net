// ========================================
// Global action helpers
// ========================================

import {API_BASE, apiFetch} from "./api.js";
import {selectAction} from "./navigation.js";
import {setSelectedEntity} from "./state.js";
import {setSelectedActionId} from "./state.js";
import {getEntitySchemas} from "./state.js";
import {showError} from "./dom.js";

window.selectAction = selectAction;


window.readEntity = (entity, id) => {
    setSelectedEntity(entity);
    setSelectedActionId(id);
    selectAction(getEntitySchemas()[entity] ? "read" : "list", id);
    history.pushState({}, "", `?entity=${encodeURIComponent(entity)}&action=read&id=${encodeURIComponent(id)}`);
}

window.editEntity = (entity, data) => {
    setSelectedEntity(entity);
    setSelectedActionId(data.id);
    selectAction("update", data.id);
    history.pushState({}, "", `?entity=${encodeURIComponent(entity)}&action=update&id=${encodeURIComponent(data.id)}`);
}

window.deleteEntity = async (entity, id) => {
    if (!confirm("Do you really want to delete this record?")) return;

    try {
        const res_json = await apiFetch(
            `${API_BASE}/${entity}/${id}`,
            { method: "DELETE" }
        );
        if (!res_json) {
            showError(`Error deleting record: ${id}`);
            return;
        }

        setSelectedActionId(null);
        selectAction("list", null);
    } catch (err) {
        showError(`Error deleting record: ${err?.message || "Unknown error"}`);
    }
};
