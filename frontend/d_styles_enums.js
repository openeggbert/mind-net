export const Display = Object.freeze({
    None:        { id: 0,  label: "none" },

    Inline:      { id: 1,  label: "inline" },
    Block:       { id: 2,  label: "block" },
    InlineBlock: { id: 3,  label: "inline-block" },

    Flex:        { id: 4,  label: "flex" },
    InlineFlex:  { id: 5,  label: "inline-flex" },

    Grid:        { id: 6,  label: "grid" },
    InlineGrid:  { id: 7,  label: "inline-grid" },

    Contents:    { id: 8,  label: "contents" },
    ListItem:    { id: 9,  label: "list-item" },

    Table:       { id: 10, label: "table" },
    TableRow:    { id: 11, label: "table-row" },
    TableCell:   { id: 12, label: "table-cell" },
});

export const Color = Object.freeze({
    // special keywords
    Transparent:  { id: 0,  label: "transparent" },
    CurrentColor:{ id: 1,  label: "currentColor" },

    // basic colors
    Black:        { id: 2,  label: "black" },
    White:        { id: 3,  label: "white" },
    Red:          { id: 4,  label: "red" },
    Green:        { id: 5,  label: "green" },
    Blue:         { id: 6,  label: "blue" },
    Yellow:       { id: 7,  label: "yellow" },
    Cyan:         { id: 8,  label: "cyan" },
    Magenta:      { id: 9,  label: "magenta" },
    Gray:         { id: 10, label: "gray" },

    // common extended colors
    LightGray:    { id: 11, label: "lightgray" },
    DarkGray:     { id: 12, label: "darkgray" },
    Orange:       { id: 13, label: "orange" },
    Brown:        { id: 14, label: "brown" },
    Purple:       { id: 15, label: "purple" },
    Pink:         { id: 16, label: "pink" },

    // system / inheritance
    Inherit:      { id: 17, label: "inherit" },
    Initial:      { id: 18, label: "initial" },
    Unset:        { id: 19, label: "unset" },
    Revert:       { id: 20, label: "revert" },
});

export const TextDecoration = Object.freeze({
    // none / base
    None:          { id: 0,  label: "none" },

    // line decorations
    Underline:     { id: 1,  label: "underline" },
    Overline:      { id: 2,  label: "overline" },
    LineThrough:   { id: 3,  label: "line-through" },

    // combinations (CSS allows multiple values)
    UnderlineOverline: { id: 4, label: "underline overline" },
    UnderlineLineThrough: { id: 5, label: "underline line-through" },

    // system / inheritance
    Inherit:       { id: 6,  label: "inherit" },
    Initial:       { id: 7,  label: "initial" },
    Unset:         { id: 8,  label: "unset" },
    Revert:        { id: 9,  label: "revert" },
});

export const Cursor = Object.freeze({
    // default / base
    Auto:          { id: 0,  label: "auto" },
    Default:       { id: 1,  label: "default" },
    None:          { id: 2,  label: "none" },

    // basic interaction
    Pointer:       { id: 3,  label: "pointer" },
    Text:          { id: 4,  label: "text" },
    Move:          { id: 5,  label: "move" },

    // selection / dragging
    Grab:          { id: 6,  label: "grab" },
    Grabbing:      { id: 7,  label: "grabbing" },

    // resize
    ColResize:     { id: 8,  label: "col-resize" },
    RowResize:     { id: 9,  label: "row-resize" },
    NResize:       { id: 10, label: "n-resize" },
    EResize:       { id: 11, label: "e-resize" },
    SResize:       { id: 12, label: "s-resize" },
    WResize:       { id: 13, label: "w-resize" },
    NeResize:      { id: 14, label: "ne-resize" },
    NwResize:      { id: 15, label: "nw-resize" },
    SeResize:      { id: 16, label: "se-resize" },
    SwResize:      { id: 17, label: "sw-resize" },

    // state / feedback
    Wait:          { id: 18, label: "wait" },
    Progress:      { id: 19, label: "progress" },
    Help:          { id: 20, label: "help" },
    NotAllowed:    { id: 21, label: "not-allowed" },
    NoDrop:        { id: 22, label: "no-drop" },

    // system / inheritance
    Inherit:       { id: 23, label: "inherit" },
    Initial:       { id: 24, label: "initial" },
    Unset:         { id: 25, label: "unset" },
    Revert:        { id: 26, label: "revert" },
});
