// ========================================
// Globals
// ========================================

import {getUserId} from "../../api.js";
import {showInfo} from "../../dom.js";
import {I18n} from "../i18n/I18n.js";

export const debug = false
export const USER_ID = getUserId()
let i18n_ = null

export function showDebug(msg) {
    if (debug) showInfo("Debug: " + msg)
}

export function set_i18n(i18n) {
    i18n_ = i18n
}
export function get_i18n(i18n) {
    i18n_ = i18n
}
export function translate(key, params = {}) {
    return i18n_.t(key, params)
}
