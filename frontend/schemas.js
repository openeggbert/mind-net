





export function buildEntitySchemas(modelDef) {
    const schemas = {};
    for (const item of modelDef.items) {
        const fields = filterColumnsForForm(item.columns).map(col => {
            let field = {
                name: col.column_name,
                type: mapColumnType(col.column_type),
                required: col.mandatory,
                description: col.description,
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
            allowedOperations: item.allowed_rest_operations.map(op => op.toLowerCase()),
            customActions: item.custom_actions || []
        };

    }
    return schemas;
}

export function buildGlobals(modelDef, schemas) {
    const entities = modelDef.items.map(i => i.model_name);

    const entityLabels = {};
    for (const [key, schema] of Object.entries(schemas)) {
        entityLabels[key] = schema.label;
    }

    // unique operations
    const actions = [
        ...new Set(modelDef.items.flatMap(i =>
            i.allowed_rest_operations.map(a => a.toLowerCase())
        ))
    ];

    return { entities, entityLabels, actions };
}

export function mapColumnType(colType) {
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

export function findTitleField(item) {
    const preferred = ["name", "title", "username", "subject"];
    for (const p of preferred) {
        if (item.columns.find(c => c.column_name === p)) {
            return p;
        }
    }
    return item.columns[0]?.column_name || "id";
}

export function filterColumnsForForm(columns) {
    return columns.filter(col => !col.auto);
}

export function toLabel(fieldName) {
    return fieldName.replace(/_id$/, '').replace(/_/g, ' ').replace(/\b\w/g, c => c.toUpperCase());
}

