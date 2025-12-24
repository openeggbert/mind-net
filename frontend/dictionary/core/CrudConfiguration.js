import {defined} from "../../common.js";

export class CrudConfiguration {
    //string
    model
    //string
    models
    //string
    table
    //bool
    filter
    input
    resolveTitle
    createAutocomplete
    autocompleteCallback
}

export function validate_cfg(cfg) {
    let to_be_validated = [
        cfg.model, cfg.models, cfg.table, cfg.input, cfg.resolveTitle
    ]
    to_be_validated.forEach(e => {
        if (!defined(e)) return false
    })
    return true
}
