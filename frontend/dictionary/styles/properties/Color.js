export const Color = Object.freeze({
    // special keywords
    Transparent: {id: 0, label: "transparent"},
    CurrentColor: {id: 1, label: "currentColor"},

    // basic colors
    Black: {id: 2, label: "black"},
    White: {id: 3, label: "white"},
    Red: {id: 4, label: "red"},
    Green: {id: 5, label: "green"},
    Blue: {id: 6, label: "blue"},
    Yellow: {id: 7, label: "yellow"},
    Cyan: {id: 8, label: "cyan"},
    Magenta: {id: 9, label: "magenta"},
    Gray: {id: 10, label: "gray"},

    // common extended colors
    LightGray: {id: 11, label: "lightgray"},
    DarkGray: {id: 12, label: "darkgray"},
    Orange: {id: 13, label: "orange"},
    Brown: {id: 14, label: "brown"},
    Purple: {id: 15, label: "purple"},
    Pink: {id: 16, label: "pink"},

    // system / inheritance
    Inherit: {id: 17, label: "inherit"},
    Initial: {id: 18, label: "initial"},
    Unset: {id: 19, label: "unset"},
    Revert: {id: 20, label: "revert"},
});