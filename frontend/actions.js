// ========================================
// Global action helpers
// ========================================

import {API_BASE} from "./api.js";
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
        const response = await fetch(`${API_BASE}/${entity}/${id}`, {method: "DELETE"});

        if (!response.ok) {
            let message = "";
            try {
                message = await response.text();
            } catch (e) {
                message = response.statusText;
            }
            showError(`Error deleting record: ${message}`);
            return;
        }

        setSelectedActionId(null);
        selectAction("list", null);
    } catch (err) {
        showError(`Network error: ${err}`);
    }
};

