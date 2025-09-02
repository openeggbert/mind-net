// ========================================
// 5. CRUD render functions
// ========================================
import {
    getCurrentPage,
    getEntitySchemas, getPageSize, getTotalPages, setCurrentPage, setPageSize, setTotalPages,
} from "./state.js";
import {API_BASE, apiFetch, resolveForeignKeyValue} from "./api.js";
import {
    contentArea,
    formatDateTime,
    getHiddenColumns,
    getQueryParams, isColumnHidden,
    parseDateTimeToUnix,
    setHiddenColumns,
    showError
} from "./dom.js";
import {filterColumnsForForm, toLabel} from "./schemas.js";
import {selectAction, changePage} from "./navigation.js";







export async function renderEntityForm(entity, data = {}) {
    const schema = getEntitySchemas()[entity];
    if (!schema) return;

    // --- Pre-fill from URL parameters only during CREATE ---
    if (!data.id) {
        const params = new URLSearchParams(window.location.search);
        schema.fields.forEach(f => {
            if (params.has(f.name) && !(f.auto || f.type === 'checkbox')) {
                data[f.name] = params.get(f.name);
            }
        });
    }

    let html = `<h3>${data.id ? "Update" : "Create"} ${schema.label}</h3><form id="entityForm">`;
    html += `<input type="hidden" name="id" value="${data.id ?? ""}">`;
    html += `<p style="color:red; font-size:0.9rem;">* Required fields</p>`;

    filterColumnsForForm(schema.fields).forEach(f => {
        let type = f.type === "datetime" ? "text" : f.type;
        let value = data[f.name] ?? "";
        if (f.type === "datetime" && value) {
            if (!isNaN(value)) value = formatDateTime(Number(value));
        }

        const titleAttr = f.description ? `title="${f.description}"` : '';

        if (f.enum) {
            html += `<div class="form-row">
                <label for="${f.name}" ${titleAttr}>${toLabel(f.name)}${f.required ? ' <span style="color:red;font-weight:bold;">*</span>' : ''}</label>
                <select id="${f.name}" name="${f.name}" ${f.required ? 'required' : ''}>
                    ${Object.entries(f.enum).map(([v, l]) => `<option value="${v}" ${data[f.name] == v ? 'selected' : ''}>${l}</option>`).join('')}
                </select>
            </div>`;
        } else {
            html += `<div class="form-row">
                <label for="${f.name}" ${titleAttr}>${toLabel(f.name)}${f.required ? ' <span style="color:red;font-weight:bold;">*</span>' : ''}</label>
                <input type="${type}" id="${f.name}" name="${f.name}" value="${value}" ${f.required ? "required" : ""}>
            </div>`;
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

export async function renderEntityRead(entity, id) {
    contentArea.innerHTML = `<p class="loading">Loading...</p>`;
    const json = await apiFetch(`${API_BASE}/${entity}/${id}`);
    if (!json) return;
    const schema = getEntitySchemas()[entity];
    if (!schema) return;

    let html = `<h3>Read ${schema.label}</h3><table>`;

    html += `<tr><th>ID</th><td>${json.id ?? ""}</td></tr>`;

    if ('created_at' in json) {
        html += `<tr><th>Created At</th><td>${formatDateTime(json.created_at)}</td></tr>`;
    }
    if ('updated_at' in json) {
        html += `<tr><th>Updated At</th><td>${formatDateTime(json.updated_at)}</td></tr>`;
    }

    for (const f of schema.fields.filter(f => !f.auto)) {
        let value = json[f.name];
        if (f.type === "datetime") value = formatDateTime(value);
        else if (f.enum && value in f.enum) value = f.enum[value];
        else if (f.foreignKey) {
            if (!value || value === 0) {
                value = "<span style='color:grey;font-style:italic;'>NONE</span>";
            } else {
                const fkTitle = await resolveForeignKeyValue(f.foreignKey, value);
                value = `<a href="#" onclick="readEntity('${f.foreignKey}',${value});return false;">${fkTitle}</a>`;
            }
        }

        html += `<tr><th>${toLabel(f.name)}</th><td>${value ?? ""}</td></tr>`;
    }

    html += "</table>";
// --- custom actions always visible ---
    html += `<h4 style="margin-bottom: 1em;">Custom Actions</h4>`;




    if (schema.customActions && schema.customActions.length > 0) {
        // group actions by model_name
        const grouped = {};
        schema.customActions.forEach(action => {
            const model = action.model_name || "Other";
            if (!grouped[model]) grouped[model] = [];
            grouped[model].push(action);
        });

        Object.keys(grouped).forEach(model => {
            // Convert model_name to label (replace underscores with spaces and capitalize words)
            const modelLabel = model.replace(/_/g, ' ').replace(/\b\w/g, c => c.toUpperCase());
            html += `<div id="custom-action-model-container"><div class="custom-action-model">${modelLabel}:</div>`;



            grouped[model].forEach(action => {
                const label = action.label ?? action.action;
                let prefix = '';
                if (action.crudl?.toUpperCase() === 'LIST') prefix = '📋 ';
                else if (action.crudl?.toUpperCase() === 'CREATE') prefix = '➕ ';

                // --- build URL with params ---
                const url = new URL(window.location.origin + window.location.pathname);
                url.searchParams.set('entity', action.model_name);
                url.searchParams.set('action', action.crudl === "List" ? "list" : "create");
                url.searchParams.set('id', id);
                Object.entries(action.params || {}).forEach(([k, v]) => {
                    url.searchParams.set(k, v === "{id}" ? id : v);
                });

                html += `<a href="${url.toString()}" class="custom-action-btn">${prefix}${label}</a>`;
            });



            html += `</div>`;
        });
    } else {
        html += `<p style="color:grey;font-style:italic;">No custom actions.</p>`;
    }








    contentArea.innerHTML = html;
}


// ========================================
// 6. List + Pagination
// ========================================


window.sortList = function(entity, field) {
    const params = new URLSearchParams(window.location.search);
    const currentSort = params.get('sort');
    const currentOrder = params.get('order') || 'asc';

    if (currentSort === field) {
        // toggle order
        params.set('order', currentOrder === 'asc' ? 'desc' : 'asc');
    } else {
        params.set('sort', field);
        params.set('order', 'asc');
    }

    history.replaceState({}, "", `${window.location.pathname}?${params.toString()}`);
    setCurrentPage(1);
    renderEntityList(entity);
};


export async function renderEntityList(entity) {

    contentArea.innerHTML = `<p class="loading">Loading...</p>`;
    const url = new URL(`${API_BASE}/${entity}`);


    const queryParams = getQueryParams();
    const currentSort = queryParams.sort || '';
    const currentOrder = queryParams.order || 'asc';



    url.searchParams.set("page_number", getCurrentPage());
    url.searchParams.set("page_size", getPageSize());


    if (currentSort) url.searchParams.set('sort', currentSort);
    if (currentOrder) url.searchParams.set('order', currentOrder);


    // --- ADD FILTERS FROM URL OR INPUTS ---
    const filters = getQueryParams().others;
    Object.entries(filters).forEach(([key, value]) => {
        if (value) url.searchParams.set(key, value);
    });




    const json = await apiFetch(url.toString());
    if (!json) return;
    const items = json.items || [];
    const schema = getEntitySchemas()[entity];
    if (!schema) return;

    setTotalPages(json.total_pages || 1);
    const listFields = schema.fields.filter(f => !f.auto && f.list !== false && !isColumnHidden(entity, f.name));


    let html = `<h3>${schema.label} List</h3>`;



    // --- FILTER UI ---
    html += `<div style="margin-bottom:10px;"><strong>Filters:</strong>`;
    listFields.forEach(f => {
        html += `<label style="margin-right:10px;">
        ${toLabel(f.name)}: <input type="text" class="filter-input" data-field="${f.name}" value="${getQueryParams().others[f.name] || ''}">
    </label>`;
    });
    html += `
    <button id="applyFilters" style="padding:6px 12px; font-size:1rem; margin-right:5px;">Search</button>
    <button id="clearFilters" style="padding:6px 12px; font-size:1rem;">Clear</button>
</div>`;





    html += `<div style="margin-bottom:10px;"><label>Items per page:</label>
        <select id="pageSizeSelect">${[5, 10, 20, 50, 100].map(s => `<option value="${s}" ${getPageSize() === s ? 'selected' : ''}>${s}</option>`).join('')}</select></div>`;

    html += `<table><thead><tr>
    
    <th class="sortable" data-field="id">ID <span>${currentSort==='id' ? (currentOrder==='asc'?'🔼':'🔽') : ''}</span></th>
    
    
    
    
    

${listFields.map(f => `<th class="sortable" data-field="${f.name}" title="${f.description ?? ''}" style="cursor:pointer;">
        ${toLabel(f.name)}${currentSort===f.name ? (currentOrder==='asc'?' 🔼':' 🔽') : ''}
    </th>`).join('')}

    <th>Actions</th>
</tr></thead><tbody>`;

    if (items.length === 0) html += `<tr><td colspan="${listFields.length + 2}" style="text-align:center;color:gray;">No records found.</td></tr>`;
    else for (const item of items) {
        html += `<tr><td>${item.id}</td>`;
        for (const f of listFields) {
            let value = item[f.name];
            if (f.type === "datetime") value = formatDateTime(value);
            else if (f.enum && value in f.enum) value = f.enum[value];
            else if (f.foreignKey) {
                if (!value || value === 0) {
                    value = "<span style='color:grey;font-style:italic;'>NONE</span>";
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
    const currentPage = getCurrentPage();
    const totalPages = getTotalPages();
    html += `</tbody></table>`;
    html += `<div style="margin-top:10px;text-align:center;">
        <button ${currentPage <= 1 ? 'disabled' : ''} onclick="changePage(${currentPage - 1})">Previous</button>
        Page ${currentPage} of ${totalPages}
        <button ${currentPage >= totalPages ? 'disabled' : ''} onclick="changePage(${currentPage + 1})">Next</button>
    </div>`;
    contentArea.innerHTML = `
    <div id="columnSelectorContainer">${renderColumnSelector(entity)}</div>
    <div id="tableContainer">
        ${html}
    </div>
`;


// --- přiřazení řazení na kliknutí hlaviček ---
    document.querySelectorAll("#tableContainer th.sortable").forEach(th => {
        th.onclick = () => {
            const field = th.dataset.field;
            window.sortList(entity, field);
        };
    });





// teď už prvek existuje → můžeme přiřadit listener
    const pageSizeSelect = document.getElementById("pageSizeSelect");
    if (pageSizeSelect) {
        pageSizeSelect.addEventListener("change", e => {
            setPageSize(Number(e.target.value));
            setCurrentPage(1);
            renderEntityList(entity);
        });
    }

    const applyFiltersBtn = document.getElementById("applyFilters");
    if (applyFiltersBtn) applyFiltersBtn.onclick = () => {
        const filterInputs = document.querySelectorAll('.filter-input');
        filterInputs.forEach(input => {
            const field = input.dataset.field;
            const value = input.value.trim();
            const params = new URLSearchParams(window.location.search);
            if (value) params.set(field, value);
            else params.delete(field);
            history.replaceState({}, "", `${window.location.pathname}?${params.toString()}`);
        });
        setCurrentPage(1);
        renderEntityList(entity);
    };

    const clearFiltersBtn = document.getElementById("clearFilters");
    if (clearFiltersBtn) clearFiltersBtn.onclick = () => {
        const filterInputs = document.querySelectorAll('.filter-input');
        filterInputs.forEach(input => input.value = '');

        const params = new URLSearchParams(window.location.search);
        listFields.forEach(f => params.delete(f.name));
        history.replaceState({}, "", `${window.location.pathname}?${params.toString()}`);

        setCurrentPage(1);
        renderEntityList(entity);
    };









    const applyColumnsBtn = document.getElementById("applyColumns");
    if (applyColumnsBtn) {
        applyColumnsBtn.onclick = () => {
            const checkboxes = document.querySelectorAll('.column-selector input[type="checkbox"]');
            const hiddenCols = [];
            checkboxes.forEach(cb => {
                if (!cb.checked) hiddenCols.push(cb.dataset.col);
            });
            const allHidden = getHiddenColumns();
            allHidden[entity] = hiddenCols;
            setHiddenColumns(allHidden);
            renderEntityList(entity); // refresh list
        };
    }




    document.getElementById("pageSizeSelect").addEventListener("change", e => {
        setPageSize(Number(e.target.value));
        setCurrentPage(1);
        renderEntityList(entity);
    });




    document.getElementById("applyFilters").onclick = () => {
        const filterInputs = document.querySelectorAll('.filter-input');
        filterInputs.forEach(input => {
            const field = input.dataset.field;
            const value = input.value.trim();
            const params = new URLSearchParams(window.location.search);
            if (value) params.set(field, value);
            else params.delete(field);
            history.replaceState({}, "", `${window.location.pathname}?${params.toString()}`);
        });
        setCurrentPage(1);
        renderEntityList(entity);
    };

    document.getElementById("clearFilters").onclick = () => {
        const filterInputs = document.querySelectorAll('.filter-input');
        filterInputs.forEach(input => input.value = '');

        const params = new URLSearchParams(window.location.search);
        listFields.forEach(f => params.delete(f.name));
        history.replaceState({}, "", `${window.location.pathname}?${params.toString()}`);

        setCurrentPage(1);
        renderEntityList(entity);
    };





}


export function renderColumnSelector(entity) {
    const schema = getEntitySchemas()[entity];
    const hiddenCols = getHiddenColumns()[entity] || [];
    let html = `<div class="column-selector"><strong>Columns:</strong> `;
    schema.fields.filter(f => !f.auto && f.list !== false).forEach(f => {
        const checked = hiddenCols.includes(f.name) ? "" : "checked";
        html += `<label style="margin-right:10px;">
            <input type="checkbox" data-col="${f.name}" ${checked}> ${toLabel(f.name)}
        </label>`;
    });
    html += `<button id="applyColumns">Apply</button></div>`;
    return html;
}



export async function executeCustomAction(entity, action, id) {
    const schema = getEntitySchemas()[entity];
    if (!schema) {
        showError("Unknown entity: " + entity);
        return;
    }

    const def = schema.customActions.find(a => a.action === action);
    if (!def) {
        showError("Unknown action: " + action);
        return;
    }

    // prepare parameters (replace {id})
    const params = {};
    for (const [k, v] of Object.entries(def.params || {})) {
        params[k] = (v === "{id}") ? id : v;
    }

    // build URL (usually /entity/{id}/{action})
    const url = new URL(`${API_BASE}/${entity}/${id}/${action}`);
    Object.entries(params).forEach(([k, v]) => url.searchParams.set(k, v));

    // call API
    const json = await apiFetch(url.toString());
    if (!json) return;

    // when API returns list of items -> render table
    if (Array.isArray(json.items)) {
        let html = `<h3>${def.label}</h3><table><thead><tr>`;
        if (json.items.length > 0) {
            Object.keys(json.items[0]).forEach(col => {
                html += `<th>${toLabel(col)}</th>`;
            });
        }
        html += `</tr></thead><tbody>`;
        json.items.forEach(row => {
            html += `<tr>`;
            Object.values(row).forEach(val => {
                html += `<td>${val ?? ""}</td>`;
            });
            html += `</tr>`;
        });
        html += `</tbody></table>`;
        html += `<p><a href="#" onclick="readEntity('${entity}', ${id});return false;">⬅️ Back</a></p>`;
        contentArea.innerHTML = html;
    } else {
        // fallback: zobraz JSON
        contentArea.innerHTML = `
            <h3>${def.label}</h3>
            <pre style="background:#f5f5f5;padding:10px;border-radius:6px;">${JSON.stringify(json,null,2)}</pre>
            <p><a href="#" onclick="readEntity('${entity}', ${id});return false;">⬅️ Back</a></p>
        `;
    }
}
