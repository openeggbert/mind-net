# Frontend

## JavaScript modules

JavaScript files are split into several modules. 
 - Splitting it into modules improves readability, maintenance and testability.

✅ **Benefits:**
* Individual JS parts have a clear purpose.
* Reduced dependency on global variables (you can gradually transition to ES6 module `import/export`).
* Easier testing and maintenance.
* Future extensions e.g. additional entities, custom actions, or new visualizations will be simpler.

### 1️⃣ `api.js` - API and cache

* `API_BASE`, `MODEL_DEFINITION_URL`, `CACHE_KEY`, `CACHE_TTL_MS`
* `loadModelDefinition()`
* `apiFetch()`
* `resolveForeignKeyValue()`

### 2️⃣ `schemas.js` - Entity schema build

* `buildEntitySchemas()`
* `buildGlobals()`
* `mapColumnType()`
* `findTitleField()`
* `filterColumnsForForm()`
* `toLabel()`
* `capitalize()`

### 3️⃣ `state.js` - Global state

* `entities`, `entityLabels`, `actions`, `entitySchemas`
* `selectedEntity`, `selectedAction`, `selectedActionId`
* `currentPage`, `pageSize`, `totalPages`
* `mainEntities`, `linkEntities`, `reviewEntities`, `collaborationEntities`, `suggestionEntities`, `notMainEntities`
* `actionLabels`

### 4️⃣ `dom.js` - DOM references and basic UI helpers

* `entityNav`, `crudMenu`, `entityTitle`, `contentArea`
* `showError()`, `getQueryParams()`, `formatDateTime()`, `parseDateTimeToUnix()`
* `getHiddenColumns()`, `setHiddenColumns()`, `isColumnHidden()`

### 5️⃣ `crud.js` - CRUD render functions

* `renderEntityForm()`
* `renderEntityRead()`
* `renderEntityList()`
* `renderColumnSelector()`
* `executeCustomAction()`
* Event handlers for form submit, pageSize changes, applyColumns

### 6️⃣ `explore.js` - Map/Network visualization

* `renderMapExplore()`
* `loadChildren()`
* `loadTestNodes()`
* `add_node_and_edges()`
* `drawNetwork()`
* `replaceSpacesWithUnderscores()`
* `currentCenterNodeId`, `parentStack`, `network`

### 7️⃣ `navigation.js` - Navigation and menu

* `renderEntityNav()`
* `renderCrudMenu()`
* `updateActiveMenu()`
* `selectEntity()`
* `selectAction()`
* `changePage()`

### 8️⃣ `actions.js` - Global CRUD helpers

* `readEntity()`, `editEntity()`, `deleteEntity()`

### 9️⃣ `init.js` - Initialization

* `initializeFromURL()`
* Mobile menu button handler
* Launching: `initializeFromURL();`


## Components

Autentization from localStorage or cookies

```aiignore
const apps = [
    {name: "message_app", label: "📬 Message"},
    {name: "discussion_app", label: "💬 Discussion"},
    {name: "review_app", label: "✅ Review"},
    {name: "zettelkast_app", label: "🧠 Zettelkast"}
];
```

```

```aiignore
const token = localStorage.getItem("auth_token");

fetch("https://api.mindnet.cz/v1/note", {
  method: "POST", 
  headers: {
    "Content-Type": "application/json",
    "Authorization": `Bearer ${token}`
  },
  body: JSON.stringify({
    title: "New note",
    content: "Note content..."
  })
})
.then(res => res.json())
.then(data => console.log("Note created:", data))
.catch(err => console.error("Error:", err));

```