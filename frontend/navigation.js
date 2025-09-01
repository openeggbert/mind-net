
// ========================================
// 7. Navigation a menu
// ========================================
import {API_BASE, apiFetch} from "./api.js";
import {
    selectedEntity,
    selectedAction,
    selectedActionId,
    currentPage,
    mainEntities,
    entityLabels,
    notMainEntities,
    entities,
} from "./state.js";


export function renderEntityNav() {
    entityNav.innerHTML = "";

    // --- If the selected entity is in (linkEntities, reviewEntities, collaborationEntities or suggestionEntities), show it on the left ---
    if (selectedEntity &&
        (
            linkEntities.includes(selectedEntity) ||
            reviewEntities.includes(selectedEntity) ||
            collaborationEntities.includes(selectedEntity) ||
            suggestionEntities.includes(selectedEntity)
        )
    ) {
        const link = document.createElement('a');
        link.href = `?entity=${encodeURIComponent(selectedEntity)}`;
        link.textContent = entityLabels[selectedEntity];
        link.classList.add('active');
        link.onclick = e => {
            e.preventDefault();
            selectEntity(selectedEntity);
            history.pushState({}, "", `?entity=${encodeURIComponent(selectedEntity)}`);
            renderEntityNav();
        };
        entityNav.appendChild(link);
    }

    // --- MAIN entities ---
    let shownMainEntities = [...mainEntities];
    if (
        selectedEntity &&
        !mainEntities.includes(selectedEntity) &&
        !linkEntities.includes(selectedEntity) &&
        !reviewEntities.includes(selectedEntity) &&
        !collaborationEntities.includes(selectedEntity) &&
        !suggestionEntities.includes(selectedEntity)
    ) {
        shownMainEntities = [selectedEntity, ...mainEntities];
    }

    shownMainEntities.forEach(entity => {
        const link = document.createElement('a');
        link.href = `?entity=${encodeURIComponent(entity)}`;
        link.textContent = entityLabels[entity];
        link.onclick = e => {
            e.preventDefault();
            selectEntity(entity);
            history.pushState({}, "", `?entity=${encodeURIComponent(entity)}`);
            renderEntityNav();
        };
        if (entity === selectedEntity) link.classList.add('active');
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
            if (entity === selectedEntity) return;
            const link = document.createElement('a');
            link.href = `?entity=${encodeURIComponent(entity)}`;
            link.textContent = entityLabels[entity];
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
    const otherEntities = entities.filter(
        e => !mainEntities.includes(e) &&
            !linkEntities.includes(e) &&
            !reviewEntities.includes(e) &&
            !collaborationEntities.includes(e) &&
            !suggestionEntities.includes(e) &&
            e !== selectedEntity
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
            link.textContent = entityLabels[entity];
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


function renderCrudMenu() {
    crudMenu.innerHTML = "";

    const sortedActions = [...actions];
    const listIndex = sortedActions.indexOf('list');
    if (listIndex > -1) {
        sortedActions.splice(listIndex, 1);
        sortedActions.unshift('list');
    }

    sortedActions.forEach(action => {
        const link = document.createElement('a');
        let href = `?entity=${encodeURIComponent(selectedEntity)}&action=${encodeURIComponent(action)}`;
        if (['read','update','delete','explore'].includes(action) && selectedActionId) {
            href += `&id=${selectedActionId}`;
        }

        link.href = href;
        link.textContent = actionLabels[action];
        link.onclick = e => {
            e.preventDefault();
            if (['read','update','delete','explore'].includes(action) && !selectedActionId) {
                showError(`No ID selected for ${actionLabels[action]}`);
                return;
            }
            selectAction(action, selectedActionId);
            history.pushState({}, "", link.href);
        };
        crudMenu.appendChild(link);
    });
}
function updateActiveMenu() {
    [...crudMenu.children].forEach(el => el.classList.remove('active'));
    const activeLink = [...crudMenu.children].find(el => el.textContent === actionLabels[selectedAction]);
    if (activeLink) activeLink.classList.add('active');
}


function selectEntity(entity, action = null) {
    selectedEntity = entity;
    selectedAction = action || 'list';
    [...entityNav.children].forEach(el => el.classList.remove('active'));
    const activeLink = [...entityNav.children].find(el => el.textContent === entityLabels[entity]);
    if (activeLink) activeLink.classList.add('active');
    entityTitle.textContent = `${entityLabels[selectedEntity]} – ${actionLabels[selectedAction]}`;
    contentArea.classList.remove('empty');
    renderCrudMenu();
    selectAction(selectedAction);
}

function selectAction(action, id = null) {
    selectedAction = action;
    selectedActionId = id;

    // Pokud je explore, ale nebylo ID, použij default map
    if (selectedAction === 'explore' && !selectedActionId && selectedEntity === 'map') {
        selectedActionId = 1;
    }

    [...crudMenu.children].forEach(el => el.classList.remove('active'));
    const activeLink = [...crudMenu.children].find(el => el.textContent === actionLabels[action]);
    if (activeLink) activeLink.classList.add('active');

    entityTitle.textContent = `${entityLabels[selectedEntity]} – ${actionLabels[selectedAction]}`;
    contentArea.classList.remove('empty');

    if (action === "list") renderEntityList(selectedEntity);
    else if (action === "create") renderEntityForm(selectedEntity);
    else if (action === "read") {
        if (selectedActionId) renderEntityRead(selectedEntity, selectedActionId);
        else contentArea.innerHTML = `<p style="color:red;">No ID provided for Read action.</p>`;
    } else if (action === "update") {
        if (selectedActionId) {

            const url = `${API_BASE}/${selectedEntity}/${selectedActionId}`;
            apiFetch(url).then(data => {
                if (!data) return;
                renderEntityForm(selectedEntity, data);
            });
        } else contentArea.innerHTML = `<p style="color:red;">No ID provided for Update action.</p>`;
    } else if (action === "delete") {
        if (selectedActionId) deleteEntity(selectedEntity, selectedActionId);
        else contentArea.innerHTML = `<p style="color:red;">No ID provided for Delete action.</p>`;
    } else if (action === "explore") {
        if (selectedActionId) renderMapExplore(selectedActionId);
        else contentArea.innerHTML = `<p style="color:red;">No ID provided for Explore action.</p>`;
    } else {
        contentArea.innerHTML = `<p style="color:red;">Action <span style="background:yellow;">${actionLabels[selectedAction]}</span> not implemented for ${entityLabels[selectedEntity]}.</p>`;
    }
}
function changePage(page) {
    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;
    currentPage = page;
    renderEntityList(selectedEntity);
}

