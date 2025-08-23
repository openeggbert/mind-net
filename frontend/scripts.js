const API_BASE = "http://localhost:8888/api";
const entitySchemas = {
    map: {
        label: "Map",
        fields: [
            { name: "name", type: "text", required: true },
            { name: "description", type: "text" },
            { name: "category", type: "text" }
        ]
    },
    tag: {
        label: "Tag",
        fields: [
            { name: "map_id", type: "number", required: true },
            { name: "title", type: "text", required: true }
        ]
    },
    node: {
        label: "Node",
        fields: [
            { name: "uuid", type: "text", required: true },
            { name: "map_id", type: "number", required: true },
            { name: "sibling_position", type: "number", required: true },
            { name: "title", type: "text", required: true },
            { name: "content_id", type: "number" },
            { name: "parent_node_id", type: "number" },
            { name: "type", type: "number", required: true },
            { name: "visibility", type: "number", list: false },
            { name: "last_shown_at", type: "datetime", list: false },
            { name: "expires_at", type: "datetime", list: false },
            { name: "is_favorite", type: "checkbox" },
            { name: "is_redirect", type: "checkbox", list: false },
            { name: "redirect_node_id", type: "number", list: false },
            { name: "redirect_reason", type: "text", list: false },
            { name: "importance", type: "number" },
            { name: "difficulty", type: "number" }
        ]
    },
    content: {
        label: "Content",
        fields: [
            { name: "content", type: "textarea", required: true },
            { name: "format", type: "number" },
            { name: "version", type: "number" },
            { name: "node_id", type: "number" }
        ]
    },
    node_property: {
        label: "Node Property",
        fields: [
            { name: "map_id", type: "number", required: true },
            { name: "node_id", type: "number", required: true },
            { name: "key", type: "text", required: true },
            { name: "value", type: "text" },
            { name: "value_type", type: "number" },
            { name: "is_indexed", type: "checkbox" }
        ]
    },
    node_tag: {
        label: "Node Tag",
        fields: [
            { name: "node_id", type: "number", required: true },
            { name: "tag_id", type: "number", required: true }
        ]
    },
    node_link: {
        label: "Node Link",
        fields: [
            { name: "from_node_id", type: "number", required: true },
            { name: "to_node_id", type: "number", required: true },
            { name: "label", type: "text" }
        ]
    },
    external_link: {
        label: "External Link",
        fields: [
            { name: "from_node_id", type: "number", required: true },
            { name: "to_url", type: "text", required: true }
        ]
    },
    history: {
        label: "History",
        fields: [
            { name: "table_name", type: "text", required: true },
            { name: "record_id", type: "number", required: true },
            { name: "operation", type: "number", required: true },
            { name: "payload", type: "textarea", required: true },
            { name: "reason", type: "text" }
        ]
    }
};



const entities = [
    'map', 'node', 'content', 'node_property',
    'tag', 'node_tag', 'node_link', 'external_link',
    'history'
];

const actions = ['list', 'create', 'read', 'update'];

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
    update: '✏️ Update'
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

function toLabel(fieldName) {
    return fieldName
        .replace(/_id$/, '')               // remove trailing "_id" if present
        .replace(/_/g, ' ')                // replace underscores with spaces
        .replace(/\b\w/g, c => c.toUpperCase()); // capitalize first letter of each word
}

function renderEntityForm(entity, data = {}) {
    const schema = entitySchemas[entity];
    if (!schema) return;

    let html = `<h3>${data.id ? "Update" : "Create"} ${schema.label}</h3>
                <form id="entityForm">`;
    html += `<input type="hidden" name="id" value="${data.id ?? ""}">`;


    html += `<p style="color: red; font-size: 0.9rem;">* Required fields</p>`;

    schema.fields.forEach(field => {
        let usedType = field.type;
        if (usedType === "datetime") {
            usedType = "text";
        }

        html += `
      <div class="form-row">
        <label for="${field.name}">
          ${toLabel(field.name)}
          ${field.required ? '<span style="color:red;">*</span>' : ''}
        </label>
        <input type="${usedType}" id="${field.name}" name="${field.name}"
               value="${data[field.name] ?? ""}"
               ${field.required ? "required" : ""}>
      </div>
    `;
    });

    html += `<button type="submit" style="text-align:left; padding-left: 20px;">Save</button></form>`;
    contentArea.innerHTML = html;

    document.getElementById("entityForm").addEventListener("submit", async e => {
        e.preventDefault();
        const formData = new FormData(e.target);
        const payload = {};

// First process checkboxes according to schema
        entitySchemas[selectedEntity].fields.forEach(field => {
            if (field.type === "checkbox") {
                payload[field.name] = document.getElementById(field.name).checked ? 1 : 0;
            }
        });

// Then iterate through FormData for other field types
        formData.forEach((value, key) => {
            if (!(key === "id" && !value)) {
                const fieldSchema = entitySchemas[selectedEntity].fields.find(f => f.name === key);
                if (!fieldSchema) return;

                // Checkboxy už jsme zpracovali, přeskočíme je
                if (fieldSchema.type === "checkbox") return;

                // Číselná pole: pokud je prázdné, pošleme 0
                if (fieldSchema.type === "number" && value === "") {
                    payload[key] = 0;
                } else {
                    payload[key] = value;
                }
            }
        });


        const method = payload.id ? "PUT" : "POST";
        const url = payload.id ? `${API_BASE}/${selectedEntity}/${payload.id}` : `${API_BASE}/${selectedEntity}`;

        try {
            const res = await fetch(url, {
                method,
                headers: {"Content-Type": "application/json"},
                body: JSON.stringify(payload)
            });

            if (!res.ok) {
                const text = await res.text();
                showError(`Error ${res.status}: ${text || res.statusText}`);
                return;
            }

            selectAction("list");
        } catch (err) {
            showError(`Network error: ${err.message}`);
        }
    });


}

async function renderEntityRead(entity, id) {
    contentArea.innerHTML = `<p class="loading">Loading...</p>`;
    const json = await apiFetch(`${API_BASE}/${entity}/${id}`);
    if (!json) return;

    const schema = entitySchemas[entity];
    if (!schema) return;

    let html = `<h3>Read ${schema.label}</h3><table>`;
    schema.fields.forEach(f => {
        html += `<tr>
                    <th>${toLabel(f.name)}</th>
                    <td>${json[f.name] ?? ""}</td>
                 </tr>`;
    });
    html += `</table>`;
    contentArea.innerHTML = html;
}


async function apiFetch(url, options = {}) {
    try {
        const res = await fetch(url, options);
        if (!res.ok) { // status mimo 200–299
            const text = await res.text();
            showError(`Error ${res.status}: ${text || res.statusText}`);
            return null;
        }
        return res.json();
    } catch (err) {
        showError(`Network error: ${err.message}`);
        return null;
    }
}


function showError(msg) {
    // Alert
    alert(msg);

    //contentArea.innerHTML = `<p style="color:red; font-weight:bold;">${msg}</p>`;
}

async function renderEntityList(entity) {
    contentArea.innerHTML = `<p class="loading">Loading...</p>`;
    const json = await apiFetch(`${API_BASE}/${entity}`);
    if (!json) return;
    const items = json.items || [];
    const schema = entitySchemas[entity];
    if (!schema) return;

    const listFields = schema.fields.filter(f => f.list !== false);

    let html = `<h3>${schema.label} List</h3><table><thead><tr>`;
    html += `<th>ID</th>`;
    listFields.forEach(f => html += `<th>${toLabel(f.name)}</th>`);
    html += `<th>Actions</th></tr></thead><tbody>`;

    if (items.length === 0) {
        html += `<tr><td colspan="${listFields.length + 2}" style="text-align:center; color:gray;">No records found.</td></tr>`;
    } else {
        items.forEach(item => {
            html += `<tr><td>${item.id}</td>`;
            listFields.forEach(f => html += `<td>${item[f.name] ?? ""}</td>`);
            html += `<td class="actions">
                    <a href="#" style="display:inline-flex; align-items:center; gap:4px; word-break:break-word;"
                       onclick="readEntity('${entity}', ${item.id})">
                      <span>📖</span><span>Read</span>
                    </a>
                    <a href="#" style="display:inline-flex; align-items:center; gap:4px; word-break:break-word;"
                       onclick="editEntity('${entity}', ${JSON.stringify(item).replace(/"/g, '&quot;')})">
                      <span>✏️</span><span>Update</span>
                    </a>
                    <a href="#" style="display:inline-flex; align-items:center; gap:4px; word-break:break-word;"
                       onclick="deleteEntity('${entity}', ${item.id})">
                      <span>🗑️</span><span>Delete</span>
                    </a>
                 </td></tr>`;
        });
    }


    html += `</tbody></table>`;
    contentArea.innerHTML = html;
}

window.readEntity = (entity, id) => {
    selectedEntity = entity;
    selectedAction = "read";
    const newUrl = `?entity=${encodeURIComponent(entity)}&action=read&id=${encodeURIComponent(id)}`;
    history.pushState({}, '', newUrl);

    renderCrudMenu();
    updateActiveMenu();
    entityTitle.textContent = `${entityLabels[selectedEntity]} – ${actionLabels[selectedAction]}`;
    renderEntityRead(entity, id);
};

function updateActiveMenu() {
    [...crudMenu.children].forEach(el => el.classList.remove('active'));
    const activeLink = [...crudMenu.children].find(el => el.textContent === actionLabels[selectedAction]);
    if (activeLink) activeLink.classList.add('active');
}

window.editEntity = (entity, data) => {
    selectedEntity = entity;
    selectedAction = "update";

    const newUrl = `?entity=${encodeURIComponent(entity)}&action=update&id=${encodeURIComponent(data.id)}`;
    history.pushState({}, '', newUrl);

    renderCrudMenu();
    updateActiveMenu();
    entityTitle.textContent = `${entityLabels[selectedEntity]} – ${actionLabels[selectedAction]}`;
    renderEntityForm(entity, data);
};


window.deleteEntity = async (entity, id) => {
    const confirmed = window.confirm("Do you really want to delete this record?");
    if (!confirmed) return;

    await fetch(`${API_BASE}/${entity}/${id}`, { method: "DELETE" });
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
    } else if (action === "read") {
        const params = getQueryParams();
        if (params.others.id) {
            renderEntityRead(selectedEntity, params.others.id);
        } else {
            contentArea.innerHTML = `<p style="color:red;">No ID provided for Read action.</p>`;
        }
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
        selectedEntity = entity;

        if (action && actions.includes(action)) {
            selectedAction = action;
        } else {
            selectedAction = 'list'; // fallback
        }

        [...entityNav.children].forEach(el => el.classList.remove('active'));
        const activeLink = [...entityNav.children].find(el => el.textContent === entityLabels[selectedEntity]);
        if (activeLink) activeLink.classList.add('active');

        entityTitle.textContent = `${entityLabels[selectedEntity]} – ${actionLabels[selectedAction]}`;
        contentArea.classList.remove('empty');

        renderCrudMenu();
        selectAction(selectedAction);
    }
}


initializeFromURL();
