
/*
 * ------------------------------------------------------------------
 * class SearchModel
 * ------------------------------------------------------------------
 * PURPOSE:
 *   - Single source of truth for search state
 *   - NO DOM access
 *   - NO REST calls
 *
 * CONTENT:
 *   - Fields correspond EXACTLY to query_json keys:
 *
 * METHODS:
 *   - toJSON()
 *       -> returns object identical to current query_json
 *   - static fromJSON(json)
 *       -> used when loading saved searches
 *
 * RULE:
 *   - Backend contract MUST NOT change.
*/

import {DictionaryItem} from "../enums/DictionaryItem.js";
import {TimeRange} from "../enums/TimeRange.js";
import {enumValue} from "../enums/EnumFunctions.js";
import {Sort} from "../enums/Sort.js";
import {Order} from "../enums/Order.js";
import {TermStatus} from "../enums/TermStatus.js";

export class SearchModel {
    constructor() {
        this.map_id = 0
        this.title_contains = "";
        this.title_starts_with = "";
        this.definition_contains = "";
        this.statuses = [];
        this.pinned_only = false;
        this.importance_low = true
        this.importance_medium = true
        this.importance_high = true
        this.difficulty_easy = true
        this.difficulty_medium = true
        this.difficulty_hard = true
        this.tag_id = 0
        this.flag_title = ""
        this.link_from_term_id = 0
        this.link_to_term_id = 0
        this.note_contains = ""
        this.index_id = 0
        this.source_id = 0
        this.alias_alias = ""
        this.has_items = []
        this.missing_items = []
        this.created = TimeRange.Any
        this.updated = TimeRange.Any
        this.visited = TimeRange.Any
        this.reviewed = TimeRange.Any
        this.repetition_due = true
        this.repetition_not_due = false
        this.repetition_never = true
        this.sort = Sort.None
        this.order = Order.None
        // …
    }

    reset() {
        const def = new SearchModel();
        Object.assign(this, def);
    }

    to_json() {
        return {
            map_id : this.map_id ?? 0,
            title_contains: this.title_contains,
            title_starts_with: this.title_starts_with,
            definition_contains: this.definition_contains,

            statuses: this.statuses.length === 1 && this.statuses[0] === -1 ? [] : this.statuses.map(e => {
                return e.id
            }),
            pinned_only: this.pinned_only,

            importance_low: this.importance_low,
            importance_medium: this.importance_medium,
            importance_high: this.importance_high,

            difficulty_easy: this.difficulty_easy,
            difficulty_medium: this.difficulty_medium,
            difficulty_hard: this.difficulty_hard,

            tag_id: this.tag_id,
            flag_title: this.flag_title,

            link_from_term_id: this.link_from_term_id,
            link_to_term_id: this.link_to_term_id,

            note_contains: this.note_contains,
            index_id: this.index_id,
            source_id: this.source_id,
            alias_alias: this.alias_alias,

            has_items: this.has_items.map(e => {
                return e.id
            }),
            missing_items: this.missing_items.map(e => {
                return e.id
            }),

            created: this.created.id,
            updated: this.updated.id,
            visited: this.visited.id,
            reviewed: this.reviewed.id,

            repetition_due: this.repetition_due,
            repetition_not_due: this.repetition_not_due,
            repetition_never: this.repetition_never,

            sort: this.sort === null ? Sort.None.id : this.sort.id,
            order: this.order === null ? Sort.None.id : this.order.id
        };
    }

    from_json(json) {
        if (!json || typeof json !== "object") {
            this.reset();
            return;
        }

        this.map_id = json.map_id ?? 0
        this.title_contains = json.title_contains ?? "";
        this.title_starts_with = json.title_starts_with ?? "";
        this.definition_contains = json.definition_contains ?? "";

        this.statuses = Array.isArray(json.statuses)
            ? json.statuses
                .map(id => enumValue(TermStatus, id))
                .filter(Boolean)
            : [];

        this.pinned_only = !!json.pinned_only;

        this.importance_low = json.importance_low ?? true;
        this.importance_medium = json.importance_medium ?? true;
        this.importance_high = json.importance_high ?? true;

        this.difficulty_easy = json.difficulty_easy ?? true;
        this.difficulty_medium = json.difficulty_medium ?? true;
        this.difficulty_hard = json.difficulty_hard ?? true;

        this.tag_id = json.tag_id ?? 0;
        this.flag_title = json.flag_title ?? "";

        this.link_from_term_id = json.link_from_term_id ?? 0;
        this.link_to_term_id = json.link_to_term_id ?? 0;

        this.note_contains = json.note_contains ?? "";
        this.index_id = json.index_id ?? 0;
        this.source_id = json.source_id ?? 0;
        this.alias_alias = json.alias_alias ?? "";

        this.has_items = Array.isArray(json.has_items)
            ? json.has_items
                .map(id => enumValue(DictionaryItem, id))
                .filter(Boolean)
            : [];

        this.missing_items = Array.isArray(json.missing_items)
            ? json.missing_items
                .map(id => enumValue(DictionaryItem, id))
                .filter(Boolean)
            : [];

        this.created = enumValue(TimeRange, json.created) ?? TimeRange.Any;
        this.updated = enumValue(TimeRange, json.updated) ?? TimeRange.Any;
        this.visited = enumValue(TimeRange, json.visited) ?? TimeRange.Any;
        this.reviewed = enumValue(TimeRange, json.reviewed) ?? TimeRange.Any;

        this.repetition_due = json.repetition_due ?? true;
        this.repetition_not_due = json.repetition_not_due ?? false;
        this.repetition_never = json.repetition_never ?? true;

        this.sort = json.sort === null ? Sort.None : enumValue(Sort, json.sort)
        this.order = json.order === null ? Order.None : enumValue(Order, json.order);
    }
}