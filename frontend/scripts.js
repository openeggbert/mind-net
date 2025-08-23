const API_BASE = "http://localhost:8888/api";

const Importance = {
    0: "Undefined",
    1: "Low",
    2: "Medium",
    3: "High"
};
const ImportanceValues = Object.keys(Importance).map(k => Number(k));

const Difficulty = {
    0: "Undefined",
    1: "Easy",
    2: "Medium",
    3: "Hard",
    4: "Expert"
};
const DifficultyValues = Object.keys(Difficulty).map(k => Number(k));

const ContentFormat = {
    0: "Markdown",
    1: "HTML",
    2: "Plain"
};
const ContentFormatValues = Object.keys(ContentFormat).map(k => Number(k));

const NodeType = {
    0: "Generic",
    1: "Term"
};
const NodeTypeValues = Object.keys(NodeType).map(k => Number(k));

const ValueType = {
    0: "String",
    1: "Number",
    2: "Boolean",
    3: "Date"
};
const ValueTypeValues = Object.keys(ValueType).map(k => Number(k));
const Crudl = {
    0: "Undefined",
    1: "Create",
    2: "Read",
    3: "Update",
    4: "Delete",
    5: "List"
};

const CrudlValues = Object.keys(Crudl).map(k => Number(k));

const Visibility = {
    0: "Public",
    1: "Private",
    2: "Draft",
    3: "Archived"
};

const VisibilityValues = Object.keys(Visibility).map(k => Number(k));


const entitySchemas = {
    map: {
        label: "Map",
        titleField: "name",
        fields: [
            {name: "name", type: "text", required: true},
            {name: "description", type: "text"},
            {name: "category", type: "text"}
        ]
    },
    tag: {
        label: "Tag",
        titleField: "title",
        fields: [
            {name: "map_id", type: "number", required: true, foreignKey: "map"},
            {name: "title", type: "text", required: true}
        ]
    },
    node: {
        label: "Node",
        titleField: "title",
        fields: [
            {name: "uuid", type: "text", required: true},
            {name: "map_id", type: "number", required: true, foreignKey: "map"},
            {name: "sibling_position", type: "number", required: true},
            {name: "title", type: "text", required: true},
            {name: "content_id", type: "number", foreignKey: "content"},
            {name: "parent_node_id", type: "number", foreignKey: "node"},
            {name: "type", type: "number", required: true, enum: NodeType},
            {name: "visibility", type: "number", list: false, enum: Visibility},
            {name: "last_shown_at", type: "datetime", list: false},
            {name: "expires_at", type: "datetime", list: false},
            {name: "is_favorite", type: "checkbox"},
            {name: "is_redirect", type: "checkbox", list: false},
            {name: "redirect_node_id", type: "number", list: false, foreignKey: "node"},
            {name: "redirect_reason", type: "text", list: false},
            {name: "importance", type: "number", enum: Importance},
            {name: "difficulty", type: "number", enum: Difficulty},
        ]
    },
    content: {
        label: "Content",
        titleField: "version",
        fields: [
            {name: "content", type: "textarea", required: true},
            {name: "format", type: "number", enum: ContentFormat},
            {name: "version", type: "number"},
            {name: "node_id", type: "number", foreignKey: "node"}
        ]
    },
    node_property: {
        label: "Node Property",
        titleField: "key",
        fields: [
            {name: "map_id", type: "number", required: true, foreignKey: "map"},
            {name: "node_id", type: "number", required: true, foreignKey: "node"},
            {name: "key", type: "text", required: true},
            {name: "value", type: "text"},
            {name: "value_type", type: "number",  enum: ValueType},
            {name: "is_indexed", type: "checkbox"}
        ]
    },
    node_tag: {
        label: "Node Tag",
        titleField: "id",
        fields: [
            {name: "node_id", type: "number", required: true, foreignKey: "node"},
            {name: "tag_id", type: "number", required: true, foreignKey: "tag"}
        ]
    },
    node_link: {
        label: "Node Link",
        titleField: "label",
        fields: [
            {name: "from_node_id", type: "number", required: true, foreignKey: "node"},
            {name: "to_node_id", type: "number", required: true, foreignKey: "node"},
            {name: "label", type: "text"}
        ]
    },
    external_link: {
        label: "External Link",
        titleField: "to_url",
        fields: [
            {name: "from_node_id", type: "number", required: true, foreignKey: "node"},
            {name: "to_url", type: "text", required: true}
        ]
    },
    history: {
        label: "History",
        titleField: "operation",
        fields: [
            {name: "table_name", type: "text", required: true},
            {name: "record_id", type: "number", required: true},
            {name: "operation", type: "number", required: true, enum: Crudl},
            {name: "payload", type: "textarea", required: true},
            {name: "reason", type: "text"}
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

        if (field.enum) {
            html += `
        <div class="form-row">
            <label for="${field.name}">${toLabel(field.name)}${field.required ? ' <span style="color:red;">*</span>' : ''}</label>
            <select id="${field.name}" name="${field.name}" ${field.required ? 'required' : ''}>
                ${Object.entries(field.enum).map(([value, label]) => `
                    <option value="${value}" ${data[field.name] == value ? 'selected' : ''}>${label}</option>
                `).join('')}
            </select>
        </div>
        `;
        } else {
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
        }
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

        formData.forEach((value, key) => {
            if (!(key === "id" && !value)) {
                const fieldSchema = entitySchemas[selectedEntity].fields.find(f => f.name === key);

                if (fieldSchema) {
                    switch (fieldSchema.type) {
                        case "number":
                            // if empty or not a number, set to 0
                            payload[key] = (value === "" || isNaN(value)) ? 0 : Number(value);
                            break;
                        case "checkbox":
                            // checkbox → 1 if checked, otherwise 0
                            payload[key] = (value === "on" || value === "1" || value === true) ? 1 : 0;
                            break;
                        case "text":
                        case "textarea":
                        case "datetime":
                        default:
                            payload[key] = value ?? "";
                            break;
                    }
                } else {
                    payload[key] = value ?? "";
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
    for (const f of schema.fields) {
        let value = json[f.name];

        if (f.enum && value in f.enum) {
            value = f.enum[value];
        } else if (f.foreignKey) {
            const fkEntity = f.foreignKey;
            const fkId = value;
            if (fkId) {
                const fkTitle = await resolveForeignKeyValue(fkEntity, fkId);
                value = `<a href="#" onclick="readEntity('${fkEntity}', ${fkId}); return false;">${fkTitle}</a>`;
            } else {
                value = "";
            }
        }

        html += `<tr><th>${toLabel(f.name)}</th><td>${value ?? ""}</td></tr>`;
    }
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

async function resolveForeignKeyValue(fkEntity, id) {
    if (!id) return "";
    const schema = entitySchemas[fkEntity];
    if (!schema) return id;

    try {
        const json = await apiFetch(`${API_BASE}/${fkEntity}/${id}`);
        if (!json) return id;
        const titleField = schema.titleField || "id";
        return json[titleField] ?? id;
    } catch (err) {
        return id;
    }
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
        for (const item of items) {
            html += `<tr><td>${item.id}</td>`;
            for (const f of listFields) {
                let value = item[f.name];
                if (f.enum && value in f.enum) {
                    value = f.enum[value];
                } else if (f.foreignKey) {
                    // pokud je foreignKey, udělat klikatelné
                    const fkEntity = f.foreignKey;
                    const fkId = value;
                    if (fkId) {
                        value = `<a href="#" onclick="readEntity('${fkEntity}', ${fkId}); return false;">${await resolveForeignKeyValue(fkEntity, fkId)}</a>`;
                    } else {
                        value = "";
                    }
                }
                html += `<td>${value ?? ""}</td>`;
            }
            html += `<td class="actions">
                <a href="#" onclick="readEntity('${entity}', ${item.id})">📖 Read</a><br>
                <a href="#" onclick="editEntity('${entity}', ${JSON.stringify(item).replace(/"/g, '&quot;')})">✏️ Update</a><br>
                <a href="#" onclick="deleteEntity('${entity}', ${item.id})">🗑️ Delete</a><br>
            </td></tr>`;
        }
    }

    html += `</tbody></table>`;
    contentArea.innerHTML = html;
}


window.readEntity = (entity, id) => {
    selectEntity(entity, "read");
    const newUrl = `?entity=${encodeURIComponent(entity)}&action=read&id=${encodeURIComponent(id)}`;
    history.pushState({}, '', newUrl);
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

    await fetch(`${API_BASE}/${entity}/${id}`, {method: "DELETE"});
    renderEntityList(entity);
};


// Select entity
function selectEntity(entity, action = null) {
    selectedEntity = entity;
    if (action) {
        selectedAction = action;
    } else {
        selectedAction = 'list'; // default action
    }

    [...entityNav.children].forEach(el => el.classList.remove('active'));
    const activeLink = [...entityNav.children].find(el => el.textContent === entityLabels[entity]);
    if (activeLink) activeLink.classList.add('active');

    entityTitle.textContent = `${entityLabels[selectedEntity]} – ${actionLabels[selectedAction]}`;
    contentArea.classList.remove('empty');
    renderCrudMenu();
    selectAction(selectedAction);
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
