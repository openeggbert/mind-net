// ========================================
// 1. API a ENUM definice
// ========================================

const API_BASE = "http://localhost:8888/api";

// Importance
const Importance = {0:"Undefined",1:"Low",2:"Medium",3:"High"};
const ImportanceValues = Object.keys(Importance).map(Number);

// Difficulty
const Difficulty = {0:"Undefined",1:"Easy",2:"Medium",3:"Hard",4:"Expert"};
const DifficultyValues = Object.keys(Difficulty).map(Number);

// ContentFormat
const ContentFormat = {0:"Markdown",1:"HTML",2:"Plain"};
const ContentFormatValues = Object.keys(ContentFormat).map(Number);

// NodeType
const NodeType = {0:"Generic",1:"Term"};
const NodeTypeValues = Object.keys(NodeType).map(Number);

// ValueType
const ValueType = {0:"String",1:"Number",2:"Boolean",3:"Date"};
const ValueTypeValues = Object.keys(ValueType).map(Number);

// Crudl
const Crudl = {0:"Undefined",1:"Create",2:"Read",3:"Update",4:"Delete",5:"List"};
const CrudlValues = Object.keys(Crudl).map(Number);

// Visibility
const Visibility = {0:"Public",1:"Private",2:"Draft",3:"Archived"};
const VisibilityValues = Object.keys(Visibility).map(Number);

// ========================================
// 2. Entity schema definice
// ========================================

const entitySchemas = {
    map: {label:"Map", titleField:"name", fields:[
            {name:"name", type:"text", required:true},
            {name:"description", type:"text"},
            {name:"category", type:"text"}
        ]},
    tag: {label:"Tag", titleField:"title", fields:[
            {name:"map_id", type:"number", required:true, foreignKey:"map"},
            {name:"title", type:"text", required:true}
        ]},
    node: {label:"Node", titleField:"title", fields:[
            {name:"uuid", type:"text", required:true},
            {name:"map_id", type:"number", required:true, foreignKey:"map"},
            {name:"sibling_position", type:"number", required:true},
            {name:"title", type:"text", required:true},
            {name:"content_id", type:"number", foreignKey:"content"},
            {name:"parent_node_id", type:"number", foreignKey:"node"},
            {name:"type", type:"number", required:true, enum:NodeType},
            {name:"visibility", type:"number", list:false, enum:Visibility},
            {name:"last_shown_at", type:"datetime", list:false},
            {name:"expires_at", type:"datetime", list:false},
            {name:"is_favorite", type:"checkbox"},
            {name:"is_redirect", type:"checkbox", list:false},
            {name:"redirect_node_id", type:"number", list:false, foreignKey:"node"},
            {name:"redirect_reason", type:"text", list:false},
            {name:"importance", type:"number", enum:Importance},
            {name:"difficulty", type:"number", enum:Difficulty}
        ]},
    content: {label:"Content", titleField:"version", fields:[
            {name:"content", type:"textarea", required:true},
            {name:"format", type:"number", enum:ContentFormat},
            {name:"version", type:"number"},
            {name:"node_id", type:"number", foreignKey:"node"}
        ]},
    node_property: {label:"Node Property", titleField:"key", fields:[
            {name:"map_id", type:"number", required:true, foreignKey:"map"},
            {name:"node_id", type:"number", required:true, foreignKey:"node"},
            {name:"key", type:"text", required:true},
            {name:"value", type:"text"},
            {name:"value_type", type:"number", enum:ValueType},
            {name:"is_indexed", type:"checkbox"}
        ]},
    node_tag: {label:"Node Tag", titleField:"id", fields:[
            {name:"node_id", type:"number", required:true, foreignKey:"node"},
            {name:"tag_id", type:"number", required:true, foreignKey:"tag"}
        ]},
    node_link: {label:"Node Link", titleField:"label", fields:[
            {name:"from_node_id", type:"number", required:true, foreignKey:"node"},
            {name:"to_node_id", type:"number", required:true, foreignKey:"node"},
            {name:"label", type:"text"}
        ]},
    external_link: {label:"External Link", titleField:"to_url", fields:[
            {name:"from_node_id", type:"number", required:true, foreignKey:"node"},
            {name:"to_url", type:"text", required:true}
        ]},
    history: {label:"History", titleField:"operation", fields:[
            {name:"table_name", type:"text", required:true},
            {name:"record_id", type:"number", required:true},
            {name:"operation", type:"number", required:true, enum:Crudl},
            {name:"payload", type:"textarea", required:true},
            {name:"reason", type:"text"}
        ]}
};

// ========================================
// 3. Global state a DOM reference
// ========================================

const entities = ['map','node','content','node_property','tag','node_tag','node_link','external_link','history'];
const actions = ['list','create','read','update'];

const entityLabels = {
    map:'Map', node:'Node', content:'Content', node_property:'Node Property',
    tag:'Tag', node_tag:'Node Tag', node_link:'Node Link', external_link:'External Link', history:'History'
};

const actionLabels = {
    list:'📋 List', create:'➕ Create', read:'📖 Read', update:'✏️ Update'
};

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
    return fieldName.replace(/_id$/,'').replace(/_/g,' ').replace(/\b\w/g, c => c.toUpperCase());
}

function showError(msg) {
    alert(msg);
}

async function apiFetch(url, options={}) {
    try {
        const res = await fetch(url, options);
        if (!res.ok) {
            const text = await res.text();
            showError(`Error ${res.status}: ${text || res.statusText}`);
            return null;
        }
        return res.json();
    } catch(err) {
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

// ========================================
// 5. CRUD render functions
// ========================================

async function renderEntityForm(entity, data={}) {
    const schema = entitySchemas[entity];
    if (!schema) return;

    let html = `<h3>${data.id ? "Update" : "Create"} ${schema.label}</h3><form id="entityForm">`;
    html += `<input type="hidden" name="id" value="${data.id ?? ""}">`;
    html += `<p style="color:red; font-size:0.9rem;">* Required fields</p>`;

    schema.fields.forEach(f => {
        let type = f.type === "datetime" ? "text" : f.type;
        if (f.enum) {
            html += `<div class="form-row"><label for="${f.name}">${toLabel(f.name)}${f.required?' *':''}</label>
                <select id="${f.name}" name="${f.name}" ${f.required?'required':''}>
                    ${Object.entries(f.enum).map(([v,l])=>`<option value="${v}" ${data[f.name]==v?'selected':''}>${l}</option>`).join('')}
                </select></div>`;
        } else {
            html += `<div class="form-row"><label for="${f.name}">${toLabel(f.name)}${f.required?' *':''}</label>
                <input type="${type}" id="${f.name}" name="${f.name}" value="${data[f.name]??""}" ${f.required?"required":""}></div>`;
        }
    });

    html += `<button type="submit">Save</button></form>`;
    contentArea.innerHTML = html;

    document.getElementById("entityForm").addEventListener("submit", async e=>{
        e.preventDefault();
        const formData = new FormData(e.target);
        const payload = {};

        schema.fields.forEach(f=>{
            if(f.type==="checkbox") payload[f.name]=document.getElementById(f.name).checked?1:0;
        });

        formData.forEach((value,key)=>{
            if (!(key==="id" && !value)) {
                const f = schema.fields.find(ff=>ff.name===key);
                if(f) {
                    switch(f.type){
                        case "number": payload[key]=(value===""||isNaN(value))?0:Number(value); break;
                        case "checkbox": payload[key]=(value==="on"||value==="1"||value===true)?1:0; break;
                        default: payload[key]=value??""; break;
                    }
                } else payload[key]=value??"";
            }
        });

        const method = payload.id?"PUT":"POST";
        const url = payload.id?`${API_BASE}/${entity}/${payload.id}`:`${API_BASE}/${entity}`;

        try{
            const res = await fetch(url,{method,headers:{"Content-Type":"application/json"},body:JSON.stringify(payload)});
            if(!res.ok){ const t=await res.text(); showError(`Error ${res.status}: ${t||res.statusText}`); return;}
            selectAction("list");
        } catch(err){ showError(`Network error: ${err.message}`);}
    });
}

async function renderEntityRead(entity,id) {
    contentArea.innerHTML=`<p class="loading">Loading...</p>`;
    const json = await apiFetch(`${API_BASE}/${entity}/${id}`);
    if(!json) return;
    const schema = entitySchemas[entity]; if(!schema) return;

    let html=`<h3>Read ${schema.label}</h3><table>`;
    for(const f of schema.fields){
        let value=json[f.name];
        if(f.enum && value in f.enum) value=f.enum[value];
        else if(f.foreignKey && value){
            const fkTitle=await resolveForeignKeyValue(f.foreignKey,value);
            value=`<a href="#" onclick="readEntity('${f.foreignKey}',${value});return false;">${fkTitle}</a>`;
        }
        html+=`<tr><th>${toLabel(f.name)}</th><td>${value??""}</td></tr>`;
    }
    html+="</table>";
    contentArea.innerHTML=html;
}

// ========================================
// 6. List + Pagination
// ========================================

async function renderEntityList(entity){
    contentArea.innerHTML=`<p class="loading">Loading...</p>`;
    const url = new URL(`${API_BASE}/${entity}`);
    url.searchParams.set("page_number",currentPage);
    url.searchParams.set("page_size",pageSize);

    const json = await apiFetch(url.toString());
    if(!json) return;
    const items = json.items || [];
    const schema = entitySchemas[entity]; if(!schema) return;

    totalPages = json.total_pages||1;
    const listFields = schema.fields.filter(f=>f.list!==false);

    let html=`<h3>${schema.label} List</h3>`;
    html+=`<div style="margin-bottom:10px;"><label>Items per page:</label>
        <select id="pageSizeSelect">${[5,10,20,50,100].map(s=>`<option value="${s}" ${pageSize===s?'selected':''}>${s}</option>`).join('')}</select></div>`;

    html+=`<table><thead><tr><th>ID</th>${listFields.map(f=>`<th>${toLabel(f.name)}</th>`).join('')}<th>Actions</th></tr></thead><tbody>`;
    if(items.length===0) html+=`<tr><td colspan="${listFields.length+2}" style="text-align:center;color:gray;">No records found.</td></tr>`;
    else for(const item of items){
        html+=`<tr><td>${item.id}</td>`;
        for(const f of listFields){
            let value=item[f.name];
            if(f.enum && value in f.enum) value=f.enum[value];
            else if(f.foreignKey && value) value=`<a href="#" onclick="readEntity('${f.foreignKey}',${value});return false;">${await resolveForeignKeyValue(f.foreignKey,value)}</a>`;
            html+=`<td>${value??""}</td>`;
        }
        html+=`<td class="actions">
            <a href="#" onclick="readEntity('${entity}',${item.id})">📖 Read</a>
            <a href="#" onclick="editEntity('${entity}',${JSON.stringify(item).replace(/"/g,'&quot;')})">✏️ Update</a>
            <a href="#" onclick="deleteEntity('${entity}',${item.id})">🗑️ Delete</a>
        </td></tr>`;
    }
    html+=`</tbody></table>`;
    html+=`<div style="margin-top:10px;text-align:center;">
        <button ${currentPage<=1?'disabled':''} onclick="changePage(${currentPage-1})">Previous</button>
        Page ${currentPage} of ${totalPages}
        <button ${currentPage>=totalPages?'disabled':''} onclick="changePage(${currentPage+1})">Next</button>
    </div>`;
    contentArea.innerHTML=html;

    document.getElementById("pageSizeSelect").addEventListener("change",e=>{
        pageSize=Number(e.target.value); currentPage=1; renderEntityList(entity);
    });
}

function changePage(page){ if(page<1)page=1; if(page>totalPages)page=totalPages; currentPage=page; renderEntityList(selectedEntity); }

// ========================================
// 7. Navigation a menu
// ========================================

function renderEntityNav(){
    entityNav.innerHTML="";
    entities.forEach(entity=>{
        const link=document.createElement('a');
        link.href=`?entity=${encodeURIComponent(entity)}`;
        link.textContent=entityLabels[entity];
        link.onclick=e=>{e.preventDefault(); selectEntity(entity); history.pushState({},"",`?entity=${encodeURIComponent(entity)}`);}
        entityNav.appendChild(link);
    });
}

function renderCrudMenu(){
    crudMenu.innerHTML="";
    actions.forEach(action=>{
        const link=document.createElement('a');
        link.href=`?entity=${encodeURIComponent(selectedEntity)}&action=${encodeURIComponent(action)}`;
        link.textContent=actionLabels[action];
        link.onclick=e=>{e.preventDefault(); selectAction(action); history.pushState({},"",`?entity=${encodeURIComponent(selectedEntity)}&action=${encodeURIComponent(action)}`);}
        crudMenu.appendChild(link);
    });
}

function updateActiveMenu(){
    [...crudMenu.children].forEach(el=>el.classList.remove('active'));
    const activeLink=[...crudMenu.children].find(el=>el.textContent===actionLabels[selectedAction]);
    if(activeLink) activeLink.classList.add('active');
}

function selectEntity(entity, action=null){
    selectedEntity=entity;
    selectedAction=action||'list';
    [...entityNav.children].forEach(el=>el.classList.remove('active'));
    const activeLink=[...entityNav.children].find(el=>el.textContent===entityLabels[entity]);
    if(activeLink) activeLink.classList.add('active');
    entityTitle.textContent=`${entityLabels[selectedEntity]} – ${actionLabels[selectedAction]}`;
    contentArea.classList.remove('empty');
    renderCrudMenu();
    selectAction(selectedAction);
}

function selectAction(action){
    selectedAction=action;
    [...crudMenu.children].forEach(el=>el.classList.remove('active'));
    const activeLink=[...crudMenu.children].find(el=>el.textContent===actionLabels[action]);
    if(activeLink) activeLink.classList.add('active');

    entityTitle.textContent=`${entityLabels[selectedEntity]} – ${actionLabels[selectedAction]}`;
    contentArea.classList.remove('empty');

    if(action==="list") renderEntityList(selectedEntity);
    else if(action==="create") renderEntityForm(selectedEntity);
    else if(action==="read"){
        const params=getQueryParams();
        if(params.others.id) renderEntityRead(selectedEntity,params.others.id);
        else contentArea.innerHTML=`<p style="color:red;">No ID provided for Read action.</p>`;
    }
    else contentArea.innerHTML=`<p style="color:red;">Action <span style="background:yellow;">${actionLabels[selectedAction]}</span> not implemented for ${entityLabels[selectedEntity]}.</p>`;
}

// ========================================
// 8. Global action helpers
// ========================================

window.readEntity=(entity,id)=>{ selectEntity(entity,"read"); history.pushState({},"",`?entity=${encodeURIComponent(entity)}&action=read&id=${encodeURIComponent(id)}`); renderEntityRead(entity,id);}
window.editEntity=(entity,data)=>{ selectedEntity=entity; selectedAction="update"; history.pushState({},"",`?entity=${encodeURIComponent(entity)}&action=update&id=${encodeURIComponent(data.id)}`); renderCrudMenu(); updateActiveMenu(); entityTitle.textContent=`${entityLabels[selectedEntity]} – ${actionLabels[selectedAction]}`; renderEntityForm(entity,data);}
window.deleteEntity=async(entity,id)=>{if(!confirm("Do you really want to delete this record?"))return; await fetch(`${API_BASE}/${entity}/${id}`,{method:"DELETE"}); renderEntityList(entity);}

// ========================================
// 9. Initialization
// ========================================

function initializeFromURL(){
    renderEntityNav();
    const {entity,action}=getQueryParams();
    if(entity && entities.includes(entity)){
        selectedEntity=entity;
        selectedAction=action && actions.includes(action)?action:'list';
        [...entityNav.children].forEach(el=>el.classList.remove('active'));
        const activeLink=[...entityNav.children].find(el=>el.textContent===entityLabels[selectedEntity]);
        if(activeLink) activeLink.classList.add('active');
        entityTitle.textContent=`${entityLabels[selectedEntity]} – ${actionLabels[selectedAction]}`;
        contentArea.classList.remove('empty');
        renderCrudMenu();
        selectAction(selectedAction);
    }
}

initializeFromURL();
