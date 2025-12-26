export class RepetitionModel {
    constructor() {
        this.map_id = 0
        this.search_id = 0
        this.search_name = ""
        this.due = true
        this.not_due = false
        this.never = true
        this.has_definition = true
    }

    to_json() {
        return {
            map_id: this.map_id,
            search_id: this.search_id,
            search_name: this.search_name,
            due: this.due ? 1 : 0,
            not_due: this.not_due ? 1 : 0,
            never: this.never ? 1 : 0,
            has_definition: this.has_definition ? 1 : 0
        }
    }
}