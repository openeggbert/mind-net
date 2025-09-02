
// ========================================
// 7. Navigation a menu
// ========================================
import {API_BASE, apiFetch} from "./api.js";
import {
    mainEntities,
    notMainEntities,
    linkEntities, reviewEntities, collaborationEntities, suggestionEntities, actionLabels,
    getSelectedEntity, getEntityLabels, getSelectedActionId, getCurrentPage, getTotalPages, getEntities,
    getSelectedAction, getActions, setSelectedEntity, setSelectedAction, setSelectedActionId,
    getEntitySchemas,
} from "./state.js";
import {contentArea, crudMenu, entityNav, entityTitle, showError} from "./dom.js";
import {renderEntityForm, renderEntityList, renderEntityRead} from "./crud.js";
import {renderMapExplore} from "./explore.js";



export function renderEntityNav() {
    entityNav.innerHTML = "";

    // --- If the selected entity is in (linkEntities, reviewEntities, collaborationEntities or suggestionEntities), show it on the left ---
    if (getSelectedEntity() &&
        (
            linkEntities.includes(getSelectedEntity()) ||
            reviewEntities.includes(getSelectedEntity()) ||
            collaborationEntities.includes(getSelectedEntity()) ||
            suggestionEntities.includes(getSelectedEntity())
        )
    ) {
        const link = document.createElement('a');
        link.href = `?entity=${encodeURIComponent(getSelectedEntity())}`;

        const label = getEntityLabels()[getSelectedEntity()];
        console.log("Left entity link label:", label, "for entity:", getSelectedEntity());


        link.textContent =

            getEntityLabels()[getSelectedEntity()];
        link.classList.add('active');
        link.onclick = e => {
            e.preventDefault();
            selectEntity(getSelectedEntity());
            history.pushState({}, "", `?entity=${encodeURIComponent(getSelectedEntity())}`);
            renderEntityNav();
        };
        entityNav.appendChild(link);
    }

    // --- MAIN entities ---
    let shownMainEntities = [...mainEntities];

    if (
        getSelectedEntity() &&
        !mainEntities.includes(getSelectedEntity()) &&
        !linkEntities.includes(getSelectedEntity()) &&
        !reviewEntities.includes(getSelectedEntity()) &&
        !collaborationEntities.includes(getSelectedEntity()) &&
        !suggestionEntities.includes(getSelectedEntity())
    ) {
        shownMainEntities = [getSelectedEntity(), ...mainEntities];
    }

    shownMainEntities.forEach(entity => {
        const link = document.createElement('a');
        link.href = `?entity=${encodeURIComponent(entity)}`;

        const label = getEntityLabels()[entity];

        link.textContent = getEntityLabels()[entity];
        link.onclick = e => {
            e.preventDefault();
            selectEntity(entity);
            history.pushState({}, "", `?entity=${encodeURIComponent(entity)}`);
            renderEntityNav();
        };
        if (entity === getSelectedEntity()) link.classList.add('active');
        entityNav.appendChild(link);
    });

    // --- entities ---
    notMainEntities.forEach((entity, index) =>
    {
        const wrapper = document.createElement('div');
        wrapper.className = "dropdown";

        const button_ = document.createElement('a');
        button_.href = "#";
        button_.textContent = "";
        switch (index) {
            case 0: button_.textContent = "Links ▼"; break;
            case 1: button_.textContent = "Review ▼"; break;
            case 2: button_.textContent = "Collaboration ▼"; break;
            case 3: button_.textContent = "Suggestions ▼"; break;
            default: console.error("Too many not main entities");
        }
        button_.onclick = e => {
            e.preventDefault();

            // close all other dropdowns
            document.querySelectorAll('.dropdown-content.show').forEach(el => {
                if (el !== content_) {
                    el.classList.remove('show');
                }
            });

            // toggle current
            content_.classList.toggle('show');
        };


        const content_ = document.createElement('div');
        content_.className = "dropdown-content";
        entity.forEach(entity => {
            if (entity === getSelectedEntity()) return;
            const link = document.createElement('a');
            link.href = `?entity=${encodeURIComponent(entity)}`;
            link.textContent = getEntityLabels()[entity];
            link.onclick = e => {
                e.preventDefault();
                selectEntity(entity);
                content_.classList.remove('show');
                history.pushState({}, "", `?entity=${encodeURIComponent(entity)}`);
                renderEntityNav();
            };
            content_.appendChild(link);
        });
        wrapper.appendChild(button_);
        wrapper.appendChild(content_);
        entityNav.appendChild(wrapper);
    });

    // --- OTHER entities ---
    const otherEntities = getEntities().filter(
        e => !mainEntities.includes(e) &&
            !linkEntities.includes(e) &&
            !reviewEntities.includes(e) &&
            !collaborationEntities.includes(e) &&
            !suggestionEntities.includes(e) &&
            e !== getSelectedEntity()
    );


    if (otherEntities.length > 0) {
        const moreWrapper = document.createElement('div');
        moreWrapper.className = "dropdown";

        const moreButton = document.createElement('a');
        moreButton.href = "#";
        moreButton.textContent = "Other ▼";
        moreButton.onclick = e => {
            e.preventDefault();

            // close all other dropdowns
            document.querySelectorAll('.dropdown-content.show').forEach(el => {
                if (el !== dropdownContent) {
                    el.classList.remove('show');
                }
            });

            // toggle current
            dropdownContent.classList.toggle('show');
        };


        const dropdownContent = document.createElement('div');
        dropdownContent.className = "dropdown-content";
        otherEntities.forEach(entity => {
            const link = document.createElement('a');
            link.href = `?entity=${encodeURIComponent(entity)}`;
            link.textContent = getEntityLabels()[entity];
            link.onclick = e => {
                e.preventDefault();
                selectEntity(entity);
                dropdownContent.classList.remove('show');
                history.pushState({}, "", `?entity=${encodeURIComponent(entity)}`);
                renderEntityNav();
            };
            dropdownContent.appendChild(link);
        });

        moreWrapper.appendChild(moreButton);
        moreWrapper.appendChild(dropdownContent);
        entityNav.appendChild(moreWrapper);
    }
}
export function renderCrudMenu() {
    crudMenu.innerHTML = "";

    const schemas = getEntitySchemas();
    const entity = getSelectedEntity();

    const allowedOps = schemas[entity]?.allowedOperations || [];

    if (allowedOps.length === 0) return; // nic k vykreslení

    // List bude vždy první
    const sortedActions = [...allowedOps];
    const listIndex = sortedActions.indexOf("list");
    if (listIndex > -1) {
        sortedActions.splice(listIndex, 1);
        sortedActions.unshift("list");
    }

    sortedActions.forEach(action => {
        const link = document.createElement("a");
        let href = `?entity=${encodeURIComponent(entity)}&action=${encodeURIComponent(action)}`;

        if (["read", "update", "delete", "explore"].includes(action) && getSelectedActionId()) {
            href += `&id=${getSelectedActionId()}`;
        }

        link.href = href;
        link.textContent = actionLabels[action] || action;
        link.onclick = e => {
            e.preventDefault();
            if (["read", "update", "delete", "explore"].includes(action) && !getSelectedActionId()) {
                showError(`No ID selected for ${actionLabels[action] || action}`);
                return;
            }
            selectAction(action, getSelectedActionId());
            history.pushState({}, "", link.href);
        };
        crudMenu.appendChild(link);
    });
}


function selectEntity(entity, action = null) {
    setSelectedEntity(entity);
    setSelectedAction(action || 'list');
    [...entityNav.children].forEach(el => el.classList.remove('active'));
    const activeLink = [...entityNav.children].find(el => el.textContent === getEntityLabels()[entity]);
    if (activeLink) activeLink.classList.add('active');
    entityTitle.textContent = `${getEntityLabels()[getSelectedEntity()]} – ${actionLabels[getSelectedAction()]}`;
    contentArea.classList.remove('empty');
    renderCrudMenu();
    selectAction(getSelectedAction());
}

export function selectAction(action, id = null) {
    setSelectedAction(action);
    setSelectedActionId(id);

    // If it's explore but no ID was provided, use default map
    if (getSelectedAction() === 'explore' && !getSelectedActionId() && getSelectedEntity() === 'map') {
        setSelectedActionId(1);
    }

    [...crudMenu.children].forEach(el => el.classList.remove('active'));
    const activeLink = [...crudMenu.children].find(el => el.textContent === actionLabels[action]);
    if (activeLink) activeLink.classList.add('active');

    entityTitle.textContent = `${getEntityLabels()[getSelectedEntity()]} – ${actionLabels[getSelectedAction()]}`;
    contentArea.classList.remove('empty');

    if (action === "list") renderEntityList(getSelectedEntity());
    else if (action === "create") renderEntityForm(getSelectedEntity());
    else if (action === "read") {
        if (getSelectedActionId()) renderEntityRead(getSelectedEntity(), getSelectedActionId());
        else contentArea.innerHTML = `<p style="color:red;">No ID provided for Read action.</p>`;
    } else if (action === "update") {
        if (getSelectedActionId()) {

            const url = `${API_BASE}/${getSelectedEntity()}/${getSelectedActionId()}`;
            apiFetch(url).then(data => {
                if (!data) return;
                renderEntityForm(getSelectedEntity(), data);
            });
        } else contentArea.innerHTML = `<p style="color:red;">No ID provided for Update action.</p>`;
    } else if (action === "delete") {
        if (getSelectedActionId()) deleteEntity(getSelectedEntity(), getSelectedActionId());
        else contentArea.innerHTML = `<p style="color:red;">No ID provided for Delete action.</p>`;
    } else if (action === "explore") {
        if (getSelectedActionId()) renderMapExplore(getSelectedActionId());
        else contentArea.innerHTML = `<p style="color:red;">No ID provided for Explore action.</p>`;
    } else {
        contentArea.innerHTML = `<p style="color:red;">Action <span style="background:yellow;">${actionLabels[getSelectedAction()]}</span> not implemented for ${getEntityLabels()[getSelectedEntity()]}.</p>`;
    }
}
export function changePage(page) {
    if (page < 1) page = 1;
    if (page > getTotalPages()) page = getTotalPages();
    getCurrentPage(page);
    renderEntityList(getSelectedEntity());
}

