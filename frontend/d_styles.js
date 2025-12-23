import {Cursor} from "./d_styles_enums.js";

export const _5PX  = "5px"
export const _10PX = "10px"
export const _20PX = "20px"

export class Styles {
    #dom_element
    #style

    constructor(dom_element) {
        this.#dom_element = dom_element
        this.#style = this.#dom_element.style()
    }

    #resolve_value(value) {
        if (typeof value === "string") return value
        if (typeof value === "number") return value

        if (typeof value === "object" && value !== null) {
            if (typeof value.label === "string") return value.label
            throw new Error("Entity object has no label")
        }

        throw new Error(`Invalid value type: ${typeof value}`)
    }

    #set(key, value) {
        this.#style[key] = this.#resolve_value(value)
        return this
    }

    /* =========================
     * BOX MODEL
     * ========================= */
    margin(value = 0) { return this.#set("margin", value) }
    marginTop(value = 0) { return this.#set("marginTop", value) }
    marginRight(value = 0) { return this.#set("marginRight", value) }
    marginBottom(value = 0) { return this.#set("marginBottom", value) }
    marginLeft(value = 0) { return this.#set("marginLeft", value) }

    padding(value = 0) { return this.#set("padding", value) }
    paddingTop(value = 0) { return this.#set("paddingTop", value) }
    paddingRight(value = 0) { return this.#set("paddingRight", value) }
    paddingBottom(value = 0) { return this.#set("paddingBottom", value) }
    paddingLeft(value = 0) { return this.#set("paddingLeft", value) }

    boxSizing(value) { return this.#set("boxSizing", value) }

    /* =========================
     * SIZE
     * ========================= */
    width(value) { return this.#set("width", value) }
    height(value) { return this.#set("height", value) }
    minWidth(value) { return this.#set("minWidth", value) }
    maxWidth(value) { return this.#set("maxWidth", value) }
    minHeight(value) { return this.#set("minHeight", value) }
    maxHeight(value) { return this.#set("maxHeight", value) }

    /* =========================
     * POSITIONING
     * ========================= */
    position(value) { return this.#set("position", value) }
    top(value = 0) { return this.#set("top", value) }
    right(value = 0) { return this.#set("right", value) }
    bottom(value = 0) { return this.#set("bottom", value) }
    left(value = 0) { return this.#set("left", value) }
    zIndex(value) { return this.#set("zIndex", value) }

    /* =========================
     * DISPLAY & LAYOUT
     * ========================= */
    display(value = "block") { return this.#set("display", value) }
    overflow(value = 0) { return this.#set("overflow", value) }
    overflowX(value = 0) { return this.#set("overflowX", value) }
    overflowY(value = 0) { return this.#set("overflowY", value) }
    visibility(value = "visible") { return this.#set("visibility", value) }

    /* =========================
     * FLEXBOX
     * ========================= */
    flex(value) { return this.#set("flex", value) }
    flexDirection(value) { return this.#set("flexDirection", value) }
    flexWrap(value) { return this.#set("flexWrap", value) }
    justifyContent(value) { return this.#set("justifyContent", value) }
    alignItems(value) { return this.#set("alignItems", value) }
    alignContent(value) { return this.#set("alignContent", value) }
    alignSelf(value) { return this.#set("alignSelf", value) }
    gap(value) { return this.#set("gap", value) }

    /* =========================
     * GRID
     * ========================= */
    gridTemplateColumns(value) { return this.#set("gridTemplateColumns", value) }
    gridTemplateRows(value) { return this.#set("gridTemplateRows", value) }
    gridColumn(value) { return this.#set("gridColumn", value) }
    gridRow(value) { return this.#set("gridRow", value) }
    gridArea(value) { return this.#set("gridArea", value) }

    /* =========================
     * TEXT
     * ========================= */
    color(value = "black") { return this.#set("color", value) }
    fontFamily(value) { return this.#set("fontFamily", value) }
    fontSize(value = "100%") { return this.#set("fontSize", value) }
    fontWeight(value = "normal") { return this.#set("fontWeight", value) }
    lineHeight(value) { return this.#set("lineHeight", value) }
    letterSpacing(value) { return this.#set("letterSpacing", value) }
    textAlign(value) { return this.#set("textAlign", value) }
    textDecoration(value) { return this.#set("textDecoration", value) }
    whiteSpace(value) { return this.#set("whiteSpace", value) }
    wordBreak(value) { return this.#set("wordBreak", value) }

    /* =========================
     * COLORS & BACKGROUND
     * ========================= */
    background(value) { return this.#set("background", value) }
    backgroundColor(value) { return this.#set("backgroundColor", value) }
    backgroundImage(value) { return this.#set("backgroundImage", value) }
    backgroundSize(value) { return this.#set("backgroundSize", value) }
    backgroundPosition(value) { return this.#set("backgroundPosition", value) }
    backgroundRepeat(value) { return this.#set("backgroundRepeat", value) }

    /* =========================
     * BORDER & EFFECTS
     * ========================= */
    border(value = "") { return this.#set("border", value) }
    borderRadius(value) { return this.#set("borderRadius", value) }
    boxShadow(value) { return this.#set("boxShadow", value) }
    outline(value) { return this.#set("outline", value) }

    /* =========================
     * INTERACTION
     * ========================= */
    cursor(value = Cursor.None) { return this.#set("cursor", value) }
    pointerEvents(value) { return this.#set("pointerEvents", value) }
    userSelect(value) { return this.#set("userSelect", value) }

    /* =========================
     * TRANSFORM & ANIMATION
     * ========================= */
    transform(value) { return this.#set("transform", value) }
    transition(value) { return this.#set("transition", value) }
    transitionDuration(value) { return this.#set("transitionDuration", value) }
    transitionTimingFunction(value) { return this.#set("transitionTimingFunction", value) }
    opacity(value = 1.0) { return this.#set("opacity", value) }

    /* =========================
     * END
     * ========================= */
    end() {
        return this.#dom_element
    }
}
