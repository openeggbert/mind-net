// ========================================
// Globals
// ========================================

import {getUserId} from "./api.js";
import {showInfo} from "./dom.js";

export const debug = false
export const USER_ID = getUserId()

export function showDebug(msg) {
    if (debug) showInfo("Debug: " + msg)
}
