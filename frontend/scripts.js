const API_BASE = "http://localhost:8888/api";

const entitySchemas = {
    map: {
        label: "Map",
        fields: [
            {name: "name", type: "text", required: true},
            {name: "description", type: "text"},
            {name: "category", type: "text"}
        ]
    },
    tag: {
        label: "Tag",
        fields: [
            {name: "map_id", type: "number", required: true},
            {name: "title", type: "text", required: true}
        ]
    },
    // Add more entities as needed
};


const entities = [
    'map', 'node', 'content', 'node_property',
    'tag', 'node_tag', 'node_link', 'external_link',
    'history'
];

const actions = ['list', 'create', 'read', 'update', 'delete'];

const entityLabels = {
    map: 'Map',
    node: 'Node',
    content: 'Content',
    node_property: 'Node Property',
    tag: 'Tag',
    node_tag: 'Node Tag',
    node_link: 'Node Link',
    external_link: 'External Link',
    history: 'History'
};

const actionLabels = {
    list: '📋 List',
    create: '➕ Create',
    read: '📖 Read',
    update: '✏️ Update',
    delete: '🗑️ Delete'
};

let selectedEntity = null;
let selectedAction = null;

const entityNav = document.getElementById('entityNav');
const crudMenu = document.getElementById('crudMenu');
const entityTitle = document.getElementById('entityTitle');
const contentArea = document.getElementById('contentArea');

// Parse URL parameters
function getQueryParams() {
    const params = new URLSearchParams(window.location.search);
    return {
        entity: params.get('entity'),
        action: params.get('action'),
        others: Object.fromEntries(params.entries())
    };
}

// Render entity navigation
function renderEntityNav() {
    entityNav.innerHTML = '';
    entities.forEach(entity => {
        const link = document.createElement('a');
        link.href = `?entity=${encodeURIComponent(entity)}`;
        link.textContent = entityLabels[entity];
        link.onclick = (e) => {
            e.preventDefault();
            selectEntity(entity);
            history.pushState({}, '', `?entity=${encodeURIComponent(entity)}`);
        };
        entityNav.appendChild(link);
    });
}

// Render CRUD submenu
function renderCrudMenu() {
    crudMenu.innerHTML = '';
    actions.forEach(action => {
        const link = document.createElement('a');
        link.href = `?entity=${encodeURIComponent(selectedEntity)}&action=${encodeURIComponent(action)}`;
        link.textContent = actionLabels[action];
        link.onclick = (e) => {
            e.preventDefault();
            selectAction(action);
            history.pushState({}, '', `?entity=${encodeURIComponent(selectedEntity)}&action=${encodeURIComponent(action)}`);
        };
        crudMenu.appendChild(link);
    });
}

function renderEntityForm(entity, data = {}) {
    const schema = entitySchemas[entity];
    if (!schema) return;

    let html = `<h3>${data.id ? "Edit" : "Create"} ${schema.label}</h3><form id="entityForm">`;
    html += `<input type="hidden" name="id" value="${data.id ?? ""}">`;

    schema.fields.forEach(field => {
        html += `
      <label>${field.name}</label>
      <input type="${field.type}" name="${field.name}"
             value="${data[field.name] ?? ""}"
             ${field.required ? "required" : ""}>
    `;
    });

    html += `<button type="submit">Save</button></form>`;
    contentArea.innerHTML = html;

    document.getElementById("entityForm").addEventListener("submit", async e => {
        e.preventDefault();
        const formData = new FormData(e.target);
        const payload = {};

        formData.forEach((value, key) => {
            if (!(key === "id" && !value)) {
                payload[key] = value;
            }
        });


        const method = payload.id ? "PUT" : "POST";
        const url = payload.id ? `${API_BASE}/${entity}/${payload.id}` : `${API_BASE}/${entity}`;

        await fetch(url, {
            method,
            headers: {"Content-Type": "application/json"},
            body: JSON.stringify(payload)
        });

        selectAction("list");
    });
}

async function renderEntityList(entity) {
    contentArea.innerHTML = `<p class="loading">Loading...</p>`;
    const res = await fetch(`${API_BASE}/${entity}`);
    const json = await res.json();
    const items = json.items;
    const schema = entitySchemas[entity];
    if (!schema) return;

    let html = `<h3>${schema.label} List</h3><table><thead><tr>`;
    html += `<th>ID</th>`;
    schema.fields.forEach(f => html += `<th>${f.name}</th>`);
    html += `<th>Actions</th></tr></thead><tbody>`;

    items.forEach(item => {
        html += `<tr><td>${item.id}</td>`;
        schema.fields.forEach(f => {
            html += `<td>${item[f.name] ?? ""}</td>`;
        });
        html += `<td class="actions">
      <a href="#" onclick="editEntity('${entity}', ${JSON.stringify(item).replace(/"/g, '&quot;')})">✏️ Edit</a>
      <a href="#" onclick="deleteEntity('${entity}', ${item.id})">🗑️ Delete</a>
    </td></tr>`;
    });

    html += `</tbody></table>`;
    contentArea.innerHTML = html;
}

window.editEntity = (entity, data) => {
    renderEntityForm(entity, data);
};

window.deleteEntity = async (entity, id) => {
    await fetch(`${API_BASE}/${entity}/${id}`, {method: "DELETE"});
    renderEntityList(entity);
};

// Select entity
function selectEntity(entity) {
    selectedEntity = entity;
    selectedAction = 'list'; // default action

    [...entityNav.children].forEach(el => el.classList.remove('active'));
    const activeLink = [...entityNav.children].find(el => el.textContent === entityLabels[entity]);
    if (activeLink) activeLink.classList.add('active');

    entityTitle.textContent = `${entityLabels[selectedEntity]} – ${actionLabels[selectedAction]}`;
    contentArea.classList.remove('empty');
    renderCrudMenu();
    selectAction(selectedAction); // auto-load list
}


// Select action
function selectAction(action) {
    selectedAction = action;

    [...crudMenu.children].forEach(el => el.classList.remove('active'));
    const activeLink = [...crudMenu.children].find(el => el.textContent === actionLabels[action]);

    if (activeLink) activeLink.classList.add('active');

    const actionLabel = actionLabels[selectedAction];
    const entityLabel = entityLabels[selectedEntity];
    entityTitle.textContent = `${entityLabel} – ${actionLabel}`;
    contentArea.classList.remove('empty');

    if (action === "list") {
        renderEntityList(selectedEntity);
    } else if (action === "create") {
        renderEntityForm(selectedEntity);
    } else {
        contentArea.innerHTML = `<p style="color:red;">Action <span style="background:yellow;">${actionLabels[selectedAction]}</span> not implemented for ${entityLabels[selectedEntity]}.</p>`;
    }

}

// Helpers for table actions
window.editMap = (id, name, desc, category) => {
    renderMapCreate({id, name, description: desc, category});
};

window.deleteMap = async (id) => {
    await fetch(`${API_BASE}/map/${id}`, {method: "DELETE"});
    renderMapList();
};


// Initialize from URL
function initializeFromURL() {
    const {entity, action} = getQueryParams();
    renderEntityNav();
    if (entity && entities.includes(entity)) {
        selectEntity(entity);
        if (action && actions.includes(action)) {
            selectAction(action);
        }
    }
}

initializeFromURL();
