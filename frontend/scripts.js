const API_BASE = "http://localhost:8888/api";

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

  if (selectedEntity === 'map') {
    if (action === 'list') {
      renderMapList();
    } else if (action === 'create') {
      renderMapForm();
    } else {
      contentArea.innerHTML = `<p style="color:red;">Action <span style="background:yellow;">${actionLabels[selectedAction]}</span> not implemented for Map.</p>`;
    }
  } else {
    contentArea.innerHTML = `<h3>${actionLabel} ${entityLabel}</h3><p style="color:red;">Not yet implemented (Action: ${actionLabel}, Model: ${entityLabel})</p>`;
  }
}

// MAP CRUD IMPLEMENTATION
async function renderMapList() {
    contentArea.innerHTML = `<p class="loading">Loading...</p>`;


//   setTimeout(async () => {

 const res = await fetch(`${API_BASE}/map`);
  const json = await res.json();
  const maps = json.items;

  let html = `
    <h3>Map List</h3>
    <table>
      <thead>
        <tr><th>ID</th><th>Name</th><th>Description</th><th>Category</th><th>Actions</th></tr>
      </thead>
      <tbody>
  `;
  maps.forEach(m => {
    html += `
      <tr>
        <td>${m.id}</td>
        <td>${m.name}</td>
        <td>${m.description ?? ""}</td>
        <td>${m.category ?? ""}</td>
        <td class="actions">
          <a href="#" onclick="editMap(${m.id}, '${m.name}', '${m.description ?? ""}', '${m.category ?? ""}')">✏️ Edit</a>
          <a href="#" onclick="deleteMap(${m.id})">🗑️ Delete</a>
        </td>
      </tr>`;
  });
  html += `</tbody></table>`;
  contentArea.innerHTML = html;

//   },Math.floor(Math.random() * 2001));


}

function renderMapForm(map = null) {
  contentArea.innerHTML = `
    <h3>${map ? "Edit Map" : "Create Map"}</h3>
    <form id="mapForm">
      <input type="hidden" id="mapId" value="${map?.id ?? ""}">
      <input type="text" id="mapName" placeholder="Name" required value="${map?.name ?? ""}">
      <input type="text" id="mapDesc" placeholder="Description" value="${map?.description ?? ""}">
      <input type="text" id="mapCategory" placeholder="Category" value="${map?.category ?? ""}">
      <button type="submit">Save</button>
    </form>
  `;

  document.getElementById("mapForm").addEventListener("submit", async e => {
    e.preventDefault();
    const id = document.getElementById("mapId").value;
    const map = {
      name: document.getElementById("mapName").value,
      description: document.getElementById("mapDesc").value,
      category: document.getElementById("mapCategory").value
    };
    if (id) {
      await fetch(`${API_BASE}/map/${id}`, {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(map)
      });
    } else {
      await fetch(`${API_BASE}/map`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(map)
      });
    }
    selectAction('list');
  });
}

// Helpers for table actions
window.editMap = (id, name, desc, category) => {
  renderMapForm({ id, name, description: desc, category });
};

window.deleteMap = async (id) => {
  await fetch(`${API_BASE}/map/${id}`, { method: "DELETE" });
  renderMapList();
};

// Initialize from URL
function initializeFromURL() {
  const { entity, action } = getQueryParams();
  renderEntityNav();
  if (entity && entities.includes(entity)) {
    selectEntity(entity);
    if (action && actions.includes(action)) {
      selectAction(action);
    }
  }
}

initializeFromURL();
