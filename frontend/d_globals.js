// ========================================
// Globals
// ========================================

import {getUserId} from "./api.js";

let wasDragged = false;
export const debug = false
export const USER_ID = getUserId()

export function was_dragged() {
    return wasDragged;
}
export function set_was_dragged(value) {
    wasDragged = value
}