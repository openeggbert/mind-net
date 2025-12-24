export const TermStatus = Object.freeze({
    Any: {id: -1, label: "Any"},
    NotDefined: {id: 0, label: "NotDefined"},

    Stub: {id: 1, label: "Stub"},        // placeholder, title or one sentence
    Draft: {id: 2, label: "Draft"},       // work in progress
    Incomplete: {id: 3, label: "Incomplete"},  // usable, but missing parts
    Verified: {id: 4, label: "Verified"},    // stable, reference knowledge
    Deprecated: {id: 5, label: "Deprecated"},  // historical / do not use
    Deleted: {id: 6, label: "Deleted"}      // deleted
});