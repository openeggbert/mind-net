export const TextDecoration = Object.freeze({
    // none / base
    None: {id: 0, label: "none"},

    // line decorations
    Underline: {id: 1, label: "underline"},
    Overline: {id: 2, label: "overline"},
    LineThrough: {id: 3, label: "line-through"},

    // combinations (CSS allows multiple values)
    UnderlineOverline: {id: 4, label: "underline overline"},
    UnderlineLineThrough: {id: 5, label: "underline line-through"},

    // system / inheritance
    Inherit: {id: 6, label: "inherit"},
    Initial: {id: 7, label: "initial"},
    Unset: {id: 8, label: "unset"},
    Revert: {id: 9, label: "revert"},
});