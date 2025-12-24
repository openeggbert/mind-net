export const EventType = Object.freeze({
    // mouse events
    Click: {id: 0, label: "click"},
    DblClick: {id: 1, label: "dblclick"},
    MouseDown: {id: 2, label: "mousedown"},
    MouseUp: {id: 3, label: "mouseup"},
    MouseMove: {id: 4, label: "mousemove"},
    MouseEnter: {id: 5, label: "mouseenter"},
    MouseLeave: {id: 6, label: "mouseleave"},
    MouseOver: {id: 7, label: "mouseover"},
    MouseOut: {id: 8, label: "mouseout"},
    ContextMenu: {id: 9, label: "contextmenu"},

    // keyboard events
    KeyDown: {id: 10, label: "keydown"},
    KeyUp: {id: 11, label: "keyup"},
    KeyPress: {id: 12, label: "keypress"}, // deprecated, ale občas se hodí

    // form / input
    Input: {id: 13, label: "input"},
    Change: {id: 14, label: "change"},
    Submit: {id: 15, label: "submit"},
    Focus: {id: 16, label: "focus"},
    Blur: {id: 17, label: "blur"},

    // touch
    TouchStart: {id: 18, label: "touchstart"},
    TouchMove: {id: 19, label: "touchmove"},
    TouchEnd: {id: 20, label: "touchend"},

    // drag & drop
    DragStart: {id: 21, label: "dragstart"},
    DragOver: {id: 22, label: "dragover"},
    Drop: {id: 23, label: "drop"},
    DragEnd: {id: 24, label: "dragend"},

    // window / lifecycle
    Load: {id: 25, label: "load"},
    DOMContentLoaded: {id: 26, label: "DOMContentLoaded"},
    Resize: {id: 27, label: "resize"},
    Scroll: {id: 28, label: "scroll"},
});