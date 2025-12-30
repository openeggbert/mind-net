// ========================================
// Globals
// ========================================

import {getUserId} from "../../api.js";
import {showInfo} from "../../dom.js";
import {I18n} from "../i18n/I18n.js";
import {VisitSource} from "../enums/VisitSource.js";

export const debug = false
export const USER_ID = getUserId()
let i18n_ = null
let next_visit_source = VisitSource.Unknown
export function set_next_visit_source(source) {
    next_visit_source = source ?? VisitSource.Unknown
}
export function clear_next_visit_source() {
    set_next_visit_source(null)
}
export function get_next_visit_source() {
    return next_visit_source
}

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
