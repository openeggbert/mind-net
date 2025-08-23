const entities = [
  'Map', 'Node', 'Content', 'Node Property',
  'Tag', 'Node Tag', 'Node Link', 'External Link',
  'History'
];

const actions = ['➕ Create', '📖 Read', '✏️ Update', '🗑️ Delete', '📋 List', '🔍 Search'];

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
    link.textContent = entity;
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
    link.textContent = action;
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
  selectedAction = null;

  [...entityNav.children].forEach(el => el.classList.remove('active'));
  const activeLink = [...entityNav.children].find(el => el.textContent === entity);
  if (activeLink) activeLink.classList.add('active');

  entityTitle.textContent = `${entity} – Choose an action`;
  contentArea.classList.add('empty');
  contentArea.innerHTML = 'No action selected.';
  renderCrudMenu();
}

// Select action
function selectAction(action) {
  selectedAction = action;

  [...crudMenu.children].forEach(el => el.classList.remove('active'));
  const activeLink = [...crudMenu.children].find(el => el.textContent === action);
  if (activeLink) activeLink.classList.add('active');

  entityTitle.textContent = `${selectedEntity} – ${action}`;
  contentArea.classList.remove('empty');
  contentArea.innerHTML = `<h3>${action} ${selectedEntity}</h3><p style="color:red;">Not yet implemented (Action: ${action}, Model: ${selectedEntity})</p>`;
}

// Initialize from URL
function initializeFromURL() {
  const { entity, action } = getQueryParams();
  if (entity && entities.includes(entity)) {
    selectEntity(entity);
    if (action && actions.includes(action)) {
      selectAction(action);
    }
  } else {
    renderEntityNav();
  }
}

renderEntityNav();
initializeFromURL();
