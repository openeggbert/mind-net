import {get_element} from "./dom.js";

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

export const TermStatus = Object.freeze({
    Any: { id: -1, label: "Any" },
    NotDefined: { id: 0, label: "NotDefined" },

    Stub:       { id: 1, label: "Stub" },        // placeholder, title or one sentence
    Draft:      { id: 2, label: "Draft" },       // work in progress
    Incomplete: { id: 3, label: "Incomplete" },  // usable, but missing parts
    Verified:   { id: 4, label: "Verified" },    // stable, reference knowledge
    Deprecated: { id: 5, label: "Deprecated" },  // historical / do not use
    Deleted:    { id: 6, label: "Deleted" }      // deleted
});

export const Importance = Object.freeze({
    Low:   { id: 1, label: "Low" },
    Medium:{ id: 2, label: "Medium" },
    High:  { id: 3, label: "High" },
});
export const Difficulty = Object.freeze({
    Easy:   { id: 1, label: "Easy" },
    Medium:{ id: 2, label: "Medium" },
    Hard:  { id: 3, label: "Hard" },
});
export const DictionaryItem = Object.freeze({
    Definition: { id: 1, label: "Definition" },
    Tags:       { id: 2, label: "Tags" },
    Flags:      { id: 3, label: "Flags" },
    Links:      { id: 4, label: "Links" },
    Notes:      { id: 5, label: "Notes" },
    Indexes:    { id: 6, label: "Indexes" },
    Sources:    { id: 7, label: "Sources" },
    Aliases:    { id: 8, label: "Aliases" },
});

export const TimeRange = Object.freeze({
    Any:               { id: 0,   label: "Any" },

    LastHour:          { id: 1,   label: "Last hour" },
    Last3Hours:        { id: 2,   label: "Last 3 hours" },
    Last24Hours:       { id: 3,   label: "Last 24 hours" },
    LastWeek:          { id: 4,   label: "Last week" },
    LastMonth:         { id: 5,   label: "Last month" },
    LastYear:          { id: 6,   label: "Last year" },
    Last10Years:       { id: 7,   label: "Last 10 years" },

    NotLastHour:       { id: 21,  label: "Not last hour" },
    NotLast3Hours:     { id: 22,  label: "Not last 3 hours" },
    NotLast24Hours:    { id: 23,  label: "Not last 24 hours" },
    NotLastWeek:       { id: 24,  label: "Not last week" },
    NotLastMonth:      { id: 25,  label: "Not last month" },
    NotLastYear:       { id: 26,  label: "Not last year" },
    NotLast10Years:    { id: 27,  label: "Not last 10 years" },

    Never:             { id: 100, label: "Never" },
});

export const RepetitionMode = Object.freeze({
    Due:         { id: 1, label: "Due" },
    NotDue:      { id: 2, label: "Not Due" },
    Never:       { id: 3, label: "Never" },
});

export const Sort = Object.freeze({
    None:        { id: 0, label: "None" },
    Title:       { id: 1, label: "Title" },
    CreatedAt:   { id: 2, label: "Created at" },
    UpdatedAt:   { id: 3, label: "Updated at" },
    Status:      { id: 11, label: "Status" },
    Difficulty:  { id: 12, label: "Difficulty" },
    Importance:  { id: 13, label: "Importance" },
    NextReview:  { id: 14, label: "Next review" },
    Random:      { id: 30, label: "Random" },
});

export const Order = Object.freeze({
    None: { id: 0, label: "None" },
    Asc: { id: 1, label: "Asc" },
    Desc:       { id: 2, label: "Desc" },
});
