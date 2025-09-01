import {
    actionLabels, getActions, getSelectedEntity, getEntityLabels, setSelectedEntity, setSelectedAction,
    setSelectedActionId, getSelectedAction, getSelectedActionId, getEntities,
} from "./state.js";
import {renderCrudMenu, renderEntityNav, selectAction} from "./navigation.js";
import {contentArea, entityNav, entityTitle, getQueryParams} from "./dom.js"




// ========================================
// 9. Initialization
// ========================================

export function initializeFromURL() {
    renderEntityNav();
    const {entity, action, others} = getQueryParams();
    const id = others.id ? Number(others.id) : null;
    if (entity && getEntities().includes(entity)) {
        setSelectedEntity(entity);
        setSelectedAction(action && getActions().includes(action) ? action : 'list');
        setSelectedActionId(id);
        [...entityNav.children].forEach(el => el.classList.remove('active'));
        const activeLink = [...entityNav.children].find(el => el.textContent === getEntityLabels()[getSelectedEntity()]);
        if (activeLink) activeLink.classList.add('active');
        entityTitle.textContent = `${getEntityLabels()[getSelectedEntity()]} – ${actionLabels[getSelectedAction()]}`;
        contentArea.classList.remove('empty');
        renderCrudMenu();
        selectAction(getSelectedAction(), getSelectedActionId());
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
