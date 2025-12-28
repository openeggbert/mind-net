export const Entities = Object.freeze({

    dictionary_map: Object.freeze({
        table_name: "dictionary_map",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        name: "name",
        description: "description",
        position: "position",

        owner_id: "owner_id",
        team_id: "team_id",
        owner_rights: "owner_rights",
        team_rights: "team_rights",
        other_rights: "other_rights"
    }),

    dictionary_term: Object.freeze({
        table_name: "dictionary_term",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        dictionary_map_id: "dictionary_map_id",
        title: "title",
        disambiguation: "disambiguation",
        definition: "definition",

        status: "status",
        importance: "importance",
        difficulty: "difficulty"
    }),

    dictionary_term_visit: Object.freeze({
        table_name: "dictionary_term_visit",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        dictionary_term_id: "dictionary_term_id",
        user_id: "user_id",
        dictionary_map_id: "dictionary_map_id"
    }),

    dictionary_link: Object.freeze({
        table_name: "dictionary_link",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        from_dictionary_term_id: "from_dictionary_term_id",
        to_dictionary_term_id: "to_dictionary_term_id",
        type: "type"
    }),

    dictionary_note: Object.freeze({
        table_name: "dictionary_note",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        dictionary_term_id: "dictionary_term_id",
        title: "title",
        content: "content",
        position: "position"
    }),

    dictionary_tag_type: Object.freeze({
        table_name: "dictionary_tag_type",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        dictionary_map_id: "dictionary_map_id",
        title: "title"
    }),

    dictionary_tag: Object.freeze({
        table_name: "dictionary_tag",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        dictionary_term_id: "dictionary_term_id",
        dictionary_tag_type_id: "dictionary_tag_type_id"
    }),

    dictionary_flag: Object.freeze({
        table_name: "dictionary_flag",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        dictionary_term_id: "dictionary_term_id",
        dictionary_map_id: "dictionary_map_id",
        user_id: "user_id",

        title: "title",
        is_public: "is_public"
    }),

    dictionary_review: Object.freeze({
        table_name: "dictionary_review",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        user_id: "user_id",
        dictionary_map_id: "dictionary_map_id",
        dictionary_term_id: "dictionary_term_id",

        algorithm: "algorithm",
        review_date: "review_date",
        grade: "grade",

        started_at: "started_at",
        ended_at: "ended_at",
        latency_ms: "latency_ms",

        answer_change_count: "answer_change_count",
        details_json: "details_json"
    }),

    dictionary_source_type: Object.freeze({
        table_name: "dictionary_source_type",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        title: "title",
        author: "author",
        year: "year",
        publisher: "publisher",
        edition: "edition",
        pages: "pages",
        url: "url",
        type: "type",
        note: "note"
    }),

    dictionary_source: Object.freeze({
        table_name: "dictionary_source",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        dictionary_term_id: "dictionary_term_id",
        dictionary_source_type_id: "dictionary_source_type_id",

        page: "page",
        note: "note"
    }),

    dictionary_url_type: Object.freeze({
        table_name: "dictionary_url_type",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        dictionary_map_id: "dictionary_map_id",

        url: "url",
        title: "title",
        type: "type"
    }),

    dictionary_url: Object.freeze({
        table_name: "dictionary_url",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        dictionary_map_id: "dictionary_map_id",
        dictionary_term_id: "dictionary_term_id",
        dictionary_url_type_id: "dictionary_url_type_id",

        position: "position",
        note: "note"
    }),

    dictionary_state_18: Object.freeze({
        table_name: "dictionary_state_18",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        user_id: "user_id",
        dictionary_term_id: "dictionary_term_id",

        stability_times_100: "stability_times_100",
        last_interval_times_100: "last_interval_times_100",
        repetitions: "repetitions",
        lapses: "lapses",

        next_review: "next_review",
        last_review: "last_review",
        last_quality: "last_quality"
    }),

    dictionary_term_alias: Object.freeze({
        table_name: "dictionary_term_alias",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        dictionary_term_id: "dictionary_term_id",
        dictionary_map_id: "dictionary_map_id",
        alias: "alias"
    }),

    dictionary_index_type: Object.freeze({
        table_name: "dictionary_index_type",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        dictionary_map_id: "dictionary_map_id",
        title: "title",
        description: "description",
        position: "position"
    }),

    dictionary_index: Object.freeze({
        table_name: "dictionary_index",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        dictionary_index_type_id: "dictionary_index_type_id",
        dictionary_term_id: "dictionary_term_id",

        position: "position",
        is_entry_point: "is_entry_point"
    }),

    dictionary_pinned_term: Object.freeze({
        table_name: "dictionary_pinned_term",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        dictionary_term_id: "dictionary_term_id",
        user_id: "user_id",
        dictionary_map_id: "dictionary_map_id"
    }),

    dictionary_search: Object.freeze({
        table_name: "dictionary_search",

        id: "id",
        created_at: "created_at",
        updated_at: "updated_at",

        user_id: "user_id",
        dictionary_map_id: "dictionary_map_id",

        name: "name",
        description: "description",
        query_json: "query_json",
        is_public: "is_public"
    })
});