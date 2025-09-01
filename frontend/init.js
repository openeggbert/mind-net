import {
    selectedEntity,
    selectedActionId,
    selectedAction, entities, entityLabels, actionLabels,
} from "./state.js";
import {renderEntityNav} from "./navigation.js";
import {contentArea, entityNav, entityTitle, getQueryParams} from "./dom.js"




// ========================================
// 9. Initialization
// ========================================

export function initializeFromURL() {
    renderEntityNav();
    const {entity, action, others} = getQueryParams();
    const id = others.id ? Number(others.id) : null;
    if (entity && entities.includes(entity)) {
        selectedEntity = entity;
        selectedAction = action && actions.includes(action) ? action : 'list';
        selectedActionId = id;
        [...entityNav.children].forEach(el => el.classList.remove('active'));
        const activeLink = [...entityNav.children].find(el => el.textContent === entityLabels[selectedEntity]);
        if (activeLink) activeLink.classList.add('active');
        entityTitle.textContent = `${entityLabels[selectedEntity]} – ${actionLabels[selectedAction]}`;
        contentArea.classList.remove('empty');
        renderCrudMenu();
        selectAction(selectedAction, selectedActionId);
    }
}


initializeFromURL();

const mobileMenuButton = document.getElementById("mobileMenuButton");
if (mobileMenuButton) {
    mobileMenuButton.onclick = () => {
        const nav = document.getElementById("entityNav");
        const submenu = document.getElementById("crudMenu");
        nav.style.display = nav.style.display === "flex" ? "none" : "flex";
        submenu.style.display = submenu.style.display === "flex" ? "none" : "flex";
    };
}
