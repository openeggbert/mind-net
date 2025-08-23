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

    // Render entity navigation
    entities.forEach(entity => {
      const link = document.createElement('a');
      link.href = '#';
      link.textContent = entity;
      link.onclick = () => {
        selectedEntity = entity;
        selectedAction = null;

        // Remove .active from all actions
        [...entityNav.children].forEach(el => el.classList.remove('active'));
        link.classList.add('active');

        entityTitle.textContent = `${entity} – Choose an action`;
        contentArea.classList.add('empty');
        contentArea.innerHTML = 'No action selected.';
        renderCrudMenu();
      };
      entityNav.appendChild(link);
    });

    // Render CRUD submenu
    function renderCrudMenu() {
      crudMenu.innerHTML = '';
      actions.forEach(action => {
        const link = document.createElement('a');
        link.href = '#';
        link.textContent = action;
        link.onclick = () => {
          selectedAction = action;

          // Remove .active from all actions
          [...crudMenu.children].forEach(el => el.classList.remove('active'));
          link.classList.add('active');

          entityTitle.textContent = `${selectedEntity} – ${action}`;

          contentArea.classList.remove('empty');
          contentArea.innerHTML = `<h3>${action} ${selectedEntity}</h3><p style="color:red;">Not yet implemented (Action: ${action}, Model: ${selectedEntity})</p>`;
        };
        crudMenu.appendChild(link);
      });
    }

