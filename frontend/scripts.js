// ========================================
// 1. API a cache
// ========================================
const API_BASE = "http://localhost:8888/api";
const MODEL_DEFINITION_URL = `${API_BASE}/model_definition`;
const CACHE_KEY = "model_definition_cache";
const CACHE_TTL_MS = 3 * 60 * 60 * 1000; // 3 hours

async function loadModelDefinition() {
    const now = Date.now();
    const cached = localStorage.getItem(CACHE_KEY);

    if (cached) {
        const { timestamp, data } = JSON.parse(cached);
        if (now - timestamp < CACHE_TTL_MS) {
            console.log("I use cached model_definition");
            return data;
        }
    }

    console.log("I download model_definition z API...");
    const resp = await fetch(MODEL_DEFINITION_URL);
    if (!resp.ok) throw new Error("Error loading model_definition");
    const data = await resp.json();

    localStorage.setItem(CACHE_KEY, JSON.stringify({ timestamp: now, data }));
    return data;
}

function buildEntitySchemas(modelDef) {
    const schemas = {};
    for (const item of modelDef.items) {
        const fields = filterColumnsForForm(item.columns).map(col => {
            let field = {
                name: col.column_name,
                type: mapColumnType(col.column_type),
                required: col.mandatory
            };
            if (col.foreign_key) field.foreignKey = col.foreign_key;
            if (col.enum_definition) {
                const enumObj = {};
                for (const entry of col.enum_definition) {
                    const [label, val] = Object.entries(entry)[0];
                    enumObj[val] = label;
                }
                field.enum = enumObj;
            }
            return field;
        });

        schemas[item.model_name] = {
            label: toLabel(item.model_name),
            titleField: findTitleField(item),
            fields,
            allowedOperations: item.allowed_rest_operations
        };

    }
    return schemas;
}

function mapColumnType(colType) {
    switch (colType) {
        case "TEXT": return "text";
        case "TEXTAREA": return "textarea";
        case "INTEGER": return "number";
        case "BOOL": return "checkbox";
        case "DATETIME": return "datetime";
        case "REAL": return "number";
        case "BLOB": return "file";
        default: return "text";
    }
}

function capitalize(str) {
    return str.charAt(0).toUpperCase() + str.slice(1);
}

function findTitleField(item) {
    const preferred = ["name", "title", "username", "subject"];
    for (const p of preferred) {
        if (item.columns.find(c => c.column_name === p)) {
            return p;
        }
    }
    return item.columns[0]?.column_name || "id";
}



function buildGlobals(modelDef, schemas) {
    const entities = modelDef.items.map(i => i.model_name);

    const entityLabels = {};
    for (const [key, schema] of Object.entries(schemas)) {
        entityLabels[key] = schema.label;
    }

    // unikátní operace
    const actions = [
        ...new Set(modelDef.items.flatMap(i =>
            i.allowed_rest_operations.map(a => a.toLowerCase())
        ))
    ];

    return { entities, entityLabels, actions };
}
let entities = [];
let entityLabels = {};
let actions = [];
let entitySchemas = {};

(async () => {
    const modelDef = await loadModelDefinition();

    entitySchemas = buildEntitySchemas(modelDef); // populate global
    const globals = buildGlobals(modelDef, entitySchemas);

    entities = globals.entities;           // populate global 
    entityLabels = globals.entityLabels;   // populate global
    actions = globals.actions;             // populate global

    console.log("Schemas:", entitySchemas);
    console.log("Entities:", entities);
    console.log("Labels:", entityLabels);
    console.log("Actions:", actions);

    initializeFromURL(); // ← start after model load
})();



// ========================================
// 2. Entity schema definitions
// ========================================



// ========================================
// 3. Global state a DOM reference
// ========================================


const mainEntities = ['map', 'note', 'tag', 'property'];
const linkEntities = ['reference', 'link'];
const reviewEntities = ['review', 'sm2_state'];
const collaborationEntities = ['user', 'team', 'team_member', 'message', 'discussion', 'comment'];
const suggestionEntities = ['suggestion', 'suggestion_review'];
const notMainEntities = [linkEntities, reviewEntities, collaborationEntities, suggestionEntities];


//const actions = ['list', 'create', 'read', 'update', 'explore'];



const actionLabels = { list: '📋 List', create: '➕ Create', read: '📖 Read', update: '✏️ Update', delete: '🗑️ Delete', explore: '🗺️ Explore' };


let selectedEntity = null;
let selectedAction = null;

const entityNav = document.getElementById('entityNav');
const crudMenu = document.getElementById('crudMenu');
const entityTitle = document.getElementById('entityTitle');
const contentArea = document.getElementById('contentArea');

// Pagination state
let currentPage = 1;
let pageSize = 10;
let totalPages = 1;

// ========================================
// 4. Helper functions
// ========================================

function getQueryParams() {
    const params = new URLSearchParams(window.location.search);
    return {entity: params.get('entity'), action: params.get('action'), others: Object.fromEntries(params.entries())};
}

function toLabel(fieldName) {
    return fieldName.replace(/_id$/, '').replace(/_/g, ' ').replace(/\b\w/g, c => c.toUpperCase());
}

function showError(msg) {
    alert(msg);
}

async function apiFetch(url, options = {}) {
    try {
        const res = await fetch(url, options);
        if (!res.ok) {
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

async function resolveForeignKeyValue(fkEntity, id) {
    if (!id) return "";
    const schema = entitySchemas[fkEntity];
    if (!schema) return id;
    const json = await apiFetch(`${API_BASE}/${fkEntity}/${id}`);
    if (!json) return id;
    return json[schema.titleField] ?? id;
}

function formatDateTime(value) {
    if (!value || value === 0) return "";
    const d = new Date(Number(value) * 1000); // Unix timestamp v sekundách
    const pad = n => n.toString().padStart(2,'0');
    return `${d.getFullYear()}-${pad(d.getMonth()+1)}-${pad(d.getDate())} ${pad(d.getHours())}:${pad(d.getMinutes())}:${pad(d.getSeconds())}`;
}
function parseDateTimeToUnix(str) {
    if (!str) return 0;
    const parts = str.split(/[- :]/);
    if (parts.length < 6) return 0;
    const [y,m,d,h,min,s] = parts.map(Number);
    return Math.floor(new Date(y,m-1,d,h,min,s).getTime()/1000);
}

// ========================================
// 5. CRUD render functions
// ========================================

function filterColumnsForForm(columns) {
    return columns.filter(col => !col.auto);
}


async function renderEntityForm(entity, data = {}) {
    const schema = entitySchemas[entity];
    if (!schema) return;

    let html = `<h3>${data.id ? "Update" : "Create"} ${schema.label}</h3><form id="entityForm">`;
    html += `<input type="hidden" name="id" value="${data.id ?? ""}">`;
    html += `<p style="color:red; font-size:0.9rem;">* Required fields</p>`;

    filterColumnsForForm(schema.fields).forEach(f => {
        let type = f.type === "datetime" ? "text" : f.type;

        let value = data[f.name] ?? "";
        if (f.type === "datetime" && value) {
            if (!isNaN(value)) value = formatDateTime(Number(value));
        }

        if (f.enum) {
            html += `<div class="form-row"><label for="${f.name}">${toLabel(f.name)}${f.required ? ' <span style="color:red;font-weight:bold;">*</span>' : ''}</label>
            <select id="${f.name}" name="${f.name}" ${f.required ? 'required' : ''}>
                ${Object.entries(f.enum).map(([v, l]) => `<option value="${v}" ${data[f.name] == v ? 'selected' : ''}>${l}</option>`).join('')}
            </select></div>`;
        } else {
            html += `<div class="form-row"><label for="${f.name}">${toLabel(f.name)}${f.required ? ' <span style="color:red;font-weight:bold;">*</span>' : ''}</label>
            <input type="${type}" id="${f.name}" name="${f.name}" value="${value}" ${f.required ? "required" : ""}></div>`;
        }
    });


    html += `<button type="submit">Save</button></form>`;
    contentArea.innerHTML = html;

    document.getElementById("entityForm").addEventListener("submit", async e => {
        e.preventDefault();
        const formData = new FormData(e.target);
        const payload = {};

        schema.fields.forEach(f => {
            if (f.type === "checkbox") payload[f.name] = document.getElementById(f.name).checked ? 1 : 0;
        });

        formData.forEach((value, key) => {
            if (!(key === "id" && !value)) {
                const f = schema.fields.find(ff => ff.name === key);

                if (f) {
                    if(f.auto) return;
                    switch (f.type) {
                        case "number":
                            payload[key] = (value === "" || isNaN(value)) ? 0 : Number(value);
                            break;
                        case "checkbox":
                            payload[key] = (value === "on" || value === "1" || value === true) ? 1 : 0;
                            break;
                        case "datetime":
                            payload[key] = parseDateTimeToUnix(value);
                            break;
                        default:
                            payload[key] = value ?? "";
                            break;
                    }

                } else payload[key] = value ?? "";
            }
        });

        const method = payload.id ? "PUT" : "POST";
        const url = payload.id ? `${API_BASE}/${entity}/${payload.id}` : `${API_BASE}/${entity}`;

        try {
            const res = await fetch(url, {
                method,
                headers: {"Content-Type": "application/json"},
                body: JSON.stringify(payload)
            });
            if (!res.ok) {
                const t = await res.text();
                showError(`Error ${res.status}: ${t || res.statusText}`);
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

    html += `<tr><th>ID</th><td data-label="ID">${json.id ?? ""}</td></tr>`;

    if ('created_at' in json) {
        html += `<tr><th>Created At</th><td data-label="Created At">${formatDateTime(json.created_at)}</td></tr>`;
    }
    if ('updated_at' in json) {
        html += `<tr><th>Updated At</th><td data-label="Updated At">${formatDateTime(json.updated_at)}</td></tr>`;
    }

    for (const f of schema.fields.filter(f => !f.auto)) {
        let value = json[f.name];

        if (f.type === "datetime") value = formatDateTime(value);
        else if (f.enum && value in f.enum) value = f.enum[value];
        else if (f.foreignKey) {
            if (!value || value === 0) {
                value = "<span style='color:grey;'>NONE</span>";
            } else {
                const fkTitle = await resolveForeignKeyValue(f.foreignKey, value);
                value = `<a href="#" onclick="readEntity('${f.foreignKey}',${value});return false;">${fkTitle}</a>`;
            }
        }

        html += `<tr><th>${toLabel(f.name)}</th><td data-label="${toLabel(f.name)}">${value ?? ""}</td></tr>`;
    }

    html += "</table>";
    contentArea.innerHTML = html;
}


// ========================================
// 6. List + Pagination
// ========================================

async function renderEntityList(entity) {
    contentArea.innerHTML = `<p class="loading">Loading...</p>`;
    const url = new URL(`${API_BASE}/${entity}`);
    url.searchParams.set("page_number", currentPage);
    url.searchParams.set("page_size", pageSize);

    const json = await apiFetch(url.toString());
    if (!json) return;
    const items = json.items || [];
    const schema = entitySchemas[entity];
    if (!schema) return;

    totalPages = json.total_pages || 1;
    const listFields = schema.fields.filter(f => !f.auto && f.list !== false);

    let html = `<h3>${schema.label} List</h3>`;
    html += `<div style="margin-bottom:10px;"><label>Items per page:</label>
        <select id="pageSizeSelect">${[5, 10, 20, 50, 100].map(s => `<option value="${s}" ${pageSize === s ? 'selected' : ''}>${s}</option>`).join('')}</select></div>`;

    html += `<table><thead><tr><th>ID</th>${listFields.map(f => `<th>${toLabel(f.name)}</th>`).join('')}<th>Actions</th></tr></thead><tbody>`;
    if (items.length === 0) html += `<tr><td colspan="${listFields.length + 2}" style="text-align:center;color:gray;">No records found.</td></tr>`;
    else for (const item of items) {
        html += `<tr><td>${item.id}</td>`;
        for (const f of listFields) {
            let value = item[f.name];
            if (f.type === "datetime") value = formatDateTime(value);
            else if (f.enum && value in f.enum) value = f.enum[value];
            else if (f.foreignKey) {
                if (!value || value === 0) {
                    value = "<span style='color:grey;'>NONE</span>";
                } else {
                    value = `<a href="#" onclick="readEntity('${f.foreignKey}',${value});return false;">${await resolveForeignKeyValue(f.foreignKey, value)}</a>`;
                }
            }

            html += `<td data-label="${toLabel(f.name)}">${value ?? ""}</td>`;
        }
        html += `<td class="actions" data-label="Actions">
    <a href="#" onclick="readEntity('${entity}',${item.id})">📖 Read</a>
    <a href="#" onclick="editEntity('${entity}',${JSON.stringify(item).replace(/"/g, '&quot;')})">✏️ Update</a>
    <a href="#" onclick="deleteEntity('${entity}',${item.id})">🗑️ Delete</a>`;
        if (entity === "map") {
            html += ` <a href="#" onclick="selectAction('explore', ${item.id}); return false;">🗺️ Explore</a>`;
        }
        html += `</td></tr>`;

    }
    html += `</tbody></table>`;
    html += `<div style="margin-top:10px;text-align:center;">
        <button ${currentPage <= 1 ? 'disabled' : ''} onclick="changePage(${currentPage - 1})">Previous</button>
        Page ${currentPage} of ${totalPages}
        <button ${currentPage >= totalPages ? 'disabled' : ''} onclick="changePage(${currentPage + 1})">Next</button>
    </div>`;
    contentArea.innerHTML = html;

    document.getElementById("pageSizeSelect").addEventListener("change", e => {
        pageSize = Number(e.target.value);
        currentPage = 1;
        renderEntityList(entity);
    });


}

// ========================================
// ?. Explore
// ========================================
let currentCenterNodeId = null;
let parentStack = [];

async function renderMapExplore(mapId) {
    contentArea.innerHTML = `<div id="network" style="height:600px;border:1px solid #ccc;border-radius:8px;"></div>
    <div style="margin-top:10px;">
      <button id="backBtn" disabled>⬅️ Back</button>
    </div>`;

    const mapJson = await apiFetch(`${API_BASE}/map/${mapId}`);
    if (!mapJson) return;

    if (!currentCenterNodeId) {
        const nodes = new vis.DataSet([
            {id: "map_" + mapJson.id, label: mapJson.name, color: "#1abc9c", level: 0}
        ]);
        const edges = new vis.DataSet([]);
        drawNetwork(nodes, edges, mapId);
        currentCenterNodeId = "map_" + mapJson.id;
        //loadChildren(mapId, currentCenterNodeId);
        loadTestNodes(mapId, currentCenterNodeId);
    }
}


async function loadChildren(mapId, parentId) {
    let url = `${API_BASE}/note?map_id=${mapId}`;
    if (parentId.startsWith("note_")) {
        const nodeId = parentId.replace("note_", "");
        url += `&parent_note_id=${noteId}`;
    } else {
        // map root notes
        url += `&parent_note_id=0`;
    }
    const json = await apiFetch(url);
    if (!json) return;

    const nodes = network.body.data.nodes;
    const edges = network.body.data.edges;

    json.items.forEach(n => {
        if (!nodes.get("node_" + n.id)) {
            nodes.add({id: "node_" + n.id, label: n.title, color: "#3498db", parentId});

            edges.add({from: parentId, to: "node_" + n.id});
        }
    });
    network.fit();
}

function replaceSpacesWithUnderscores(text) {
    return text.replace(/ /g, "_");
}


function add_node_and_edges(label, color, id_of_parent, level = null) {
    let id_of_new_node = replaceSpacesWithUnderscores(label);
    const nodeData = {id: id_of_new_node, label: label, color: color};
    if (level !== null) nodeData.level = level;  // adds level only if provided
    network.body.data.nodes.add(nodeData);
    network.body.data.edges.add({from: id_of_parent, to: id_of_new_node});
}


async function loadTestNodes(mapId, parentId) {

    add_node_and_edges("ACTIONS", "#8f8484", parentId, 1);
    add_node_and_edges("PARENT", "#8f8484", "ACTIONS", 2);
    add_node_and_edges("CREATE", "#8f8484", "ACTIONS", 2);
    add_node_and_edges("READ", "#8f8484", "ACTIONS", 2);
    add_node_and_edges("UPDATE", "#8f8484", "ACTIONS", 2);
    add_node_and_edges("DELETE", "#8f8484", "ACTIONS", 2);
    add_node_and_edges("History", "#3498db", parentId, 1);
    add_node_and_edges("Basics", "#3498db", parentId, 1);
    add_node_and_edges("Syntax rules", "#3498db", parentId, 1);
    add_node_and_edges("Data types", "#3498db", parentId, 1);
    add_node_and_edges("Operators", "#3498db", parentId, 1);
    add_node_and_edges("OTHER", "#8f8484", parentId, 1);

    add_node_and_edges("content", "#d5db34", "OTHER", 2);
    add_node_and_edges("properties", "#8c5d52", "OTHER", 2);
    add_node_and_edges("tags", "#4cdcbc", "OTHER", 2);
    add_node_and_edges("node_links", "#7e55b3", "OTHER", 2);
    add_node_and_edges("external_links", "#ba6379", "OTHER", 2);

    add_node_and_edges("CREATE.", "#8f8484", "properties", 3);
    add_node_and_edges("colour", "#8c5d52", "properties", 3);
    add_node_and_edges("importance", "#8c5d52", "properties", 3);
    add_node_and_edges("price", "#8c5d52", "properties", 3);


    add_node_and_edges("UPDATE P", "#8f8484", "price", 5);
    add_node_and_edges("DELETE P", "#8f8484", "price", 5);
    add_node_and_edges("key=price", "#8c5d52", "price", 5);
    add_node_and_edges("value=400", "#8c5d52", "price", 5);
    add_node_and_edges("value type=STRING", "#8c5d52", "price", 5);

    network.fit();
}


let network = null;

function drawNetwork(nodes, edges, mapId) {
    const container = document.getElementById('network');
    const data = {nodes, edges};


    let o = 2;
    switch (o) {
        case 1: {
            const options = {physics: {stabilization: true}, interaction: {hover: true}};
            network = new vis.Network(container, data, options);
        }
            break;

        case 2: {
            const options = {
                interaction: {
                    dragNodes: true,
                    zoomView: true,
                    dragView: true,
                    multiselect: true,
                    selectable: true,
                    hover: true,
                    touch: true
                },
                layout: {
                    hierarchical: {
                        direction: "UD",
                        sortMethod: "directed", // nebo 'hubsize'
                        levelSeparation: 80,
                        nodeSpacing: 100
                    }
                },
                physics: {enabled: true}
            };


            network = new vis.Network(container, data, options);
        }
            break;
        case 3: {
            const options = {
                physics: {
                    enabled: true,
                    stabilization: {
                        enabled: true,
                        iterations: 100,
                        updateInterval: 10
                    }
                },
                layout: {
                    improvedLayout: true
                },
                interaction: {
                    hover: true
                }
            };

            network = new vis.Network(container, data, options);

            network.once("stabilizationIterationsDone", () => {
                network.setOptions({physics: false});
            });
        }
    }


    network.on("click", params => {
        if (params.nodes.length > 0) {
            const nodeId = params.nodes[0];
            if (nodeId !== currentCenterNodeId) {
                parentStack.push(currentCenterNodeId);
                currentCenterNodeId = nodeId;
                document.getElementById("backBtn").disabled = false;
                loadChildren(mapId, nodeId);
            }
        }
    });


    document.getElementById("backBtn").onclick = () => {
        if (parentStack.length > 0) {
            currentCenterNodeId = parentStack.pop();
            if (parentStack.length === 0) document.getElementById("backBtn").disabled = true;
            renderMapExplore(1);
        }
    };
}


function changePage(page) {
    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;
    currentPage = page;
    renderEntityList(selectedEntity);
}

// ========================================
// 7. Navigation a menu
// ========================================
function renderEntityNav() {
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
        let href = `?entity=${encodeURIComponent(selectedEntity)}&action=${encodeURIComponent(action)}`;
        if (action === "explore") {
            if (selectedActionId) href += `&id=${selectedActionId}`;
        }

        const link = document.createElement('a');
        link.href = href;
        link.textContent = actionLabels[action];
        link.onclick = e => {
            e.preventDefault();
            if (action === "explore" && !selectedActionId) {
                showError("No map selected for Explore action");
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


    if (selectedAction === 'explore' && !selectedActionId && selectedEntity === 'map') {
        // default to first map in list or ask user to select
        selectedActionId = 1; // or another ID you want to use
    }

    
    [...crudMenu.children].forEach(el => el.classList.remove('active'));
    const activeLink = [...crudMenu.children].find(el => el.textContent === actionLabels[action]);
    if (activeLink) activeLink.classList.add('active');

    entityTitle.textContent = `${entityLabels[selectedEntity]} – ${actionLabels[selectedAction]}`;
    contentArea.classList.remove('empty');

    if (action === "list") renderEntityList(selectedEntity);
    else if (action === "create") renderEntityForm(selectedEntity);
    else if (action === "read") {
        if (id) renderEntityRead(selectedEntity, id);
        else contentArea.innerHTML = `<p style="color:red;">No ID provided for Read action.</p>`;
    } else if (action === "explore") {
        if (id) renderMapExplore(id);
        else contentArea.innerHTML = `<p style="color:red;">No ID provided for Explore action.</p>`;
    } else contentArea.innerHTML = `<p style="color:red;">Action <span style="background:yellow;">${actionLabels[selectedAction]}</span> not implemented for ${entityLabels[selectedEntity]}.</p>`;
}

let selectedActionId = null;

// ========================================
// 8. Global action helpers
// ========================================

window.readEntity = (entity, id) => {
    selectEntity(entity, "read");
    history.pushState({}, "", `?entity=${encodeURIComponent(entity)}&action=read&id=${encodeURIComponent(id)}`);
    renderEntityRead(entity, id);
}
window.editEntity = (entity, data) => {
    selectedEntity = entity;
    selectedAction = "update";
    history.pushState({}, "", `?entity=${encodeURIComponent(entity)}&action=update&id=${encodeURIComponent(data.id)}`);
    renderCrudMenu();
    updateActiveMenu();
    entityTitle.textContent = `${entityLabels[selectedEntity]} – ${actionLabels[selectedAction]}`;
    renderEntityForm(entity, data);
}
window.deleteEntity = async (entity, id) => {
    if (!confirm("Do you really want to delete this record?")) return;
    await fetch(`${API_BASE}/${entity}/${id}`, {method: "DELETE"});
    renderEntityList(entity);
}

// ========================================
// 9. Initialization
// ========================================

function initializeFromURL() {
    renderEntityNav();
    const {entity, action} = getQueryParams();
    if (entity && entities.includes(entity)) {
        selectedEntity = entity;
        selectedAction = action && actions.includes(action) ? action : 'list';
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

const mobileMenuButton = document.getElementById("mobileMenuButton");
if (mobileMenuButton) {
    mobileMenuButton.onclick = () => {
        const nav = document.getElementById("entityNav");
        const submenu = document.getElementById("crudMenu");
        nav.style.display = nav.style.display === "flex" ? "none" : "flex";
        submenu.style.display = submenu.style.display === "flex" ? "none" : "flex";
    };
}
