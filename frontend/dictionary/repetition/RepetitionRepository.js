import {getUserId, list_all_entities, post_entity, QueryParams, read_entity} from "../../api.js";
import {Entities} from "../entities/Entities.js";

export async function post_review(review) {
    return await post_entity("dictionary_review", review);
}
export async function read_state_18(term_id) {
    let states =  await list_all_entities("dictionary_state_18",
        new QueryParams().add_user_id().add("dictionary_term_id", term_id).build());
    if(!states) {
        console.error("Reading dictionary_state18 failed.")
        return null
    }
    if(states.length === 0) {
        console.error("There is no dictionary_state18 for " + getUserId() + " and term id " + term_id)
        return null
    }
    return states[0]
}
export async function read_review(review_id) {
    return await read_entity("dictionary_review", review_id)
}
export async function list_maps() {
    return await list_all_entities(Entities.dictionary_map, new QueryParams().sort(Entities.dictionary_map.position).build());
}
export async function list_term_searches(model) {
    let json = model.to_json()

    return await list_all_entities(
        "dictionary_term_for_review",
        new QueryParams()
            .add_user_id()
            .add("dictionary_map_id", json.map_id)
            .add("dictionary_search_id", json.search_id)
            .add("is_due", json.due)
            .add("is_not_due", json.not_due)
            .add("is_never", json.never)
            .add("has_definition", json.has_definition)
            .build()
    )
}