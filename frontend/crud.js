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
    showError, showInfo
} from "./dom.js";
import {filterColumnsForForm, toLabel} from "./schemas.js";
import {selectAction, changePage} from "./navigation.js";







export async function renderEntityForm(entity, data = {}, errors = {}) {
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
        const hasErr = !!errors[f.name];
        const titleAttr = f.description ? `title="${f.description}"` : '';
        const errHtml = hasErr ? `<div class="field-error">${errors[f.name]}</div>` : '';

        if (f.enum) {
            html += `<div class="form-row ${hasErr?'has-error':''}">
            <label for="${f.name}" ${titleAttr}>${toLabel(f.name)}${f.required ? ' <span style="color:red;font-weight:bold;">*</span>' : ''}</label>
            <select id="${f.name}" name="${f.name}" ${f.required ? 'required' : ''}>
                ${Object.entries(f.enum).map(([v, l]) => `<option value="${v}" ${data[f.name] == v ? 'selected' : ''}>${l}</option>`).join('')}
            </select>
            ${errHtml}
        </div>`;
            return;
        }

        if (f.type === 'checkbox') {
            const checked = (data[f.name] === 1 || data[f.name] === '1' || data[f.name] === true);
            html += `<div class="form-row ${hasErr?'has-error':''}">
            <label for="${f.name}" ${titleAttr}>${toLabel(f.name)}${f.required ? ' <span style="color:red;font-weight:bold;">*</span>' : ''}</label>
            <input type="checkbox" id="${f.name}" name="${f.name}" ${checked ? 'checked' : ''}>
            ${errHtml}
        </div>`;
            return;
        }

        // default (text/number/datetime atd.)
        let type = f.type === "datetime" ? "text" : f.type;
        let value = data[f.name] ?? "";
        if (f.type === "datetime" && value) {
            if (!isNaN(value)) value = formatDateTime(Number(value));
        }

        html += `<div class="form-row ${hasErr?'has-error':''}">
        <label for="${f.name}" ${titleAttr}>${toLabel(f.name)}${f.required ? ' <span style="color:red;font-weight:bold;">*</span>' : ''}</label>
        <input type="${type}" id="${f.name}" name="${f.name}" value="${value}" ${f.required ? "required" : ""} autocomplete="on">
        ${errHtml}
    </div>`;
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
                            payload[key] = value ? parseDateTimeToUnix(value) : null;
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
            const res_json = await apiFetch(url, {
                method,
                headers: {"Content-Type": "application/json"},
                body: JSON.stringify(payload)
            });

            // If backend returned null or non-OK response (not caught by apiFetch)
            if (!res_json) {
                // Form stays in place
                showError("Save failed — backend returned error.");
                return;
            }

            showInfo("Saved");
            selectAction("list");
        } catch (err) {
            // For thrown errors (e.g. network error)
            showError(`Save error: ${err?.message || "Unknown error"}`);
            await renderEntityForm(entity, payload);
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
                else if (action.crudl?.toUpperCase() === 'CREATE') prefix = '➕ '
                else if (action.crudl?.toUpperCase() === 'READ') prefix = '📖 ';

                // --- build URL with params ---
                const url = new URL(window.location.origin + window.location.pathname);
                url.searchParams.set('entity', action.model_name);
                let action_text = action.crudl.toLowerCase();
                url.searchParams.set('action', action_text);
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

    // --- NEW: load page number from URL if it exists ---
    const urlPage = Number(queryParams.others.page_number);
    const urlSize = Number(queryParams.others.page_size);

    if (urlPage) setCurrentPage(urlPage);
    if (urlSize) setPageSize(urlSize);

    const pageSize = getPageSize();

    url.searchParams.set("page_number", getCurrentPage());
    url.searchParams.set("page_size", pageSize);



    if (currentSort) url.searchParams.set('sort', currentSort);
    if (currentOrder) url.searchParams.set('order', currentOrder);


    // --- ADD FILTERS FROM URL OR INPUTS ---
    const filters = getQueryParams().others;
    Object.entries(filters).forEach(([key, value]) => {
        if (value) url.searchParams.set(key, value);
    });




    const json = await apiFetch(url.toString());

// when error or empty response, continue anyway
    const items = json?.items || [];

    const schema = getEntitySchemas()[entity];
    if (!schema) return;

// set total pages safely (1 as default)
    setTotalPages(json?.total_pages || 1);

    const listFields = schema.fields.filter(
        f => !f.auto && f.list !== false && !isColumnHidden(entity, f.name)
    );


    let html = `<h3>${schema.label} List</h3>`;



    // --- FILTER UI ---
    html += `<div style="margin-bottom:10px;"><strong>Filters:</strong>`;
    listFields.forEach(f => {
        html += `<label style="margin-right:10px;">
        ${toLabel(f.name)}: <input type="text" class="filter-input" data-field="${f.name}" value="${getQueryParams().others[f.name] || ''}">
    </label>`;
    });
    html += `
    <button id="clearFilters" style="padding:6px 12px; font-size:1rem;">Clear</button>
    <button id="applyFilters" style="padding:6px 12px; font-size:1rem; margin-right:5px;">Search</button>
    
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
// calculate surrounding range (±2 pages around current)
    const delta = 2;
    let startPage = Math.max(1, currentPage - delta);
    let endPage = Math.min(totalPages, currentPage + delta);
    if (currentPage <= delta) {
        endPage = Math.min(totalPages, 1 + delta * 2);
    }
    if (currentPage + delta >= totalPages) {
        startPage = Math.max(1, totalPages - delta * 2);
    }

    html += `<div style="margin-top:10px;text-align:center;">`;

// First / Prev
    html += `<button style="margin-left:5px; margin-right:5px;" ${currentPage === 1 ? "disabled" : ""} onclick="changePage(1)">&laquo; First</button>`;
    html += `<button style="margin-left:5px; margin-right:5px;" ${currentPage === 1 ? "disabled" : ""} onclick="changePage(${currentPage - 1})">&lsaquo; Prev</button>`;

// Numeric links
    for (let i = startPage; i <= endPage; i++) {
        if (i === currentPage) {
            html += `<span style="margin:0 5px; font-weight:bold;">[${i}]</span>`;
        } else {
            html += `<a href="#" onclick="changePage(${i});return false;" style="margin:0 5px;">${i}</a>`;
        }
    }

// Next / Last
    html += `<button style="margin-left:5px; margin-right:5px;" ${currentPage === totalPages ? "disabled" : ""} onclick="changePage(${currentPage + 1})">Next &rsaquo;</button>`;
    html += `<button style="margin-left:5px; margin-right:5px;" ${currentPage === totalPages ? "disabled" : ""} onclick="changePage(${totalPages})">Last &raquo;</button>`;

// Go to page input
    html += `
    <input type="number" id="gotoPageInput" min="1" max="${totalPages}" value="${currentPage}" style="width:60px; margin-left:10px;padding-top:2px;padding-bottom:2px;">
    <button id="gotoPageBtn">Go</button> 
    <span style="color:grey; margin-left:10px;">Total pages: ${totalPages}</span>
</div>`;


    contentArea.innerHTML = `
    <div id="columnSelectorContainer">${renderColumnSelector(entity)}</div>
    <div id="tableContainer">
        ${html}
    </div>
`;


    // --- Assign sorting on header click ---
    document.querySelectorAll("#tableContainer th.sortable").forEach(th => {
        th.onclick = () => {
            const field = th.dataset.field;
            window.sortList(entity, field);
        };
    });


// element now exists -> we can assign listener
    const pageSizeSelect = document.getElementById("pageSizeSelect");
    if (pageSizeSelect) {
        pageSizeSelect.addEventListener("change", e => {
            let size = Number(e.target.value);
            if(size < 5) {size = 5;}
            setPageSize(size);
            setCurrentPage(1);

            // --- NEW: update URL params ---
            const params = new URLSearchParams(window.location.search);
            params.set("page_number", 1);
            params.set("page_size", size);
            history.replaceState({}, "", `${window.location.pathname}?${params.toString()}`);

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

    const gotoBtn = document.getElementById("gotoPageBtn");
    if (gotoBtn) {
        gotoBtn.onclick = () => {
            const val = Number(document.getElementById("gotoPageInput").value);
            if (val >= 1 && val <= getTotalPages()) {
                changePage(val);
            } else {
                showError(`Please enter a number between 1 and ${getTotalPages()}`);
            }
        };
    }



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
