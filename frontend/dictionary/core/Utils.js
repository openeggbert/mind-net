import {showInfo} from "../../dom.js";

export function set_params(map_id, term_id = null) {
    const url = new URL(window.location.href);

    if (map_id === null) {
        url.searchParams.delete("map_id");
    }
    if (map_id) {
        url.searchParams.set("map_id", map_id);
    }

    if (term_id === null) {
        url.searchParams.delete("term_id");
    }
    if (term_id) {
        url.searchParams.set("term_id", term_id);
    }

    window.history.pushState({}, "", url);
}

export function get_params() {
    const params = new URLSearchParams(window.location.search);

    return {
        map_id: params.get("map_id"),
        term_id: params.get("term_id")
    }
}
