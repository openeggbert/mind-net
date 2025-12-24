export const Cursor = Object.freeze({
    // default / base
    Auto: {id: 0, label: "auto"},
    Default: {id: 1, label: "default"},
    None: {id: 2, label: "none"},

    // basic interaction
    Pointer: {id: 3, label: "pointer"},
    Text: {id: 4, label: "text"},
    Move: {id: 5, label: "move"},

    // selection / dragging
    Grab: {id: 6, label: "grab"},
    Grabbing: {id: 7, label: "grabbing"},

    // resize
    ColResize: {id: 8, label: "col-resize"},
    RowResize: {id: 9, label: "row-resize"},
    NResize: {id: 10, label: "n-resize"},
    EResize: {id: 11, label: "e-resize"},
    SResize: {id: 12, label: "s-resize"},
    WResize: {id: 13, label: "w-resize"},
    NeResize: {id: 14, label: "ne-resize"},
    NwResize: {id: 15, label: "nw-resize"},
    SeResize: {id: 16, label: "se-resize"},
    SwResize: {id: 17, label: "sw-resize"},

    // state / feedback
    Wait: {id: 18, label: "wait"},
    Progress: {id: 19, label: "progress"},
    Help: {id: 20, label: "help"},
    NotAllowed: {id: 21, label: "not-allowed"},
    NoDrop: {id: 22, label: "no-drop"},

    // system / inheritance
    Inherit: {id: 23, label: "inherit"},
    Initial: {id: 24, label: "initial"},
    Unset: {id: 25, label: "unset"},
    Revert: {id: 26, label: "revert"},
});