import {get_element} from "../../dom.js";

function canConvertToNumber(text) {
    return Number.isFinite(Number(text));
}

export function enumValue(EnumObj, value) {

    let final_value = canConvertToNumber(value) ? Number(value) : value

    // number → search by id
    if (typeof final_value === "number") {
        return Object.values(EnumObj).find(v => v.id === final_value) ?? null;
    }

    // text → key (LOW) or label ("Low")
    if (typeof final_value === "string") {
        return (
            EnumObj[final_value] ??
            Object.values(EnumObj).find(v => v.label === final_value) ??
            null
        );
    }

    return null;
}

export function enumValues(EnumObj) {
    return Object.entries(EnumObj).map(([key, value]) => ({
        key,
        id: value.id,
        label: value.label
    }));
}

export function gen_enum_id(text, instance) {
    if (instance === null || instance === undefined) {
        throw new Error("gen_enum_id: instance is null or undefined");
    }

    const id =
        typeof instance === "object"
            ? instance.id
            : instance;

    if (id === undefined) {
        throw new Error("gen_enum_id: invalid instance");
    }

    return `${text}_${id}`;
}

export function find_by_enum_id(text, instance) {
    return get_element(gen_enum_id(text, instance))
}

export function humanizeEnumKey(str) {
    if (typeof str !== "string") return str;

    return str
        // space between lowercase and uppercase letter
        .replace(/([a-z])([A-Z])/g, "$1 $2")
        // space between abbreviation and word (HTTPServer → HTTP Server)
        .replace(/([A-Z]+)([A-Z][a-z])/g, "$1 $2")
        // first letter uppercase, rest lowercase
        .replace(/^./, c => c.toUpperCase());
}
