//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/repetition/models/RReview.h"

namespace mindnet::plugins::repetition::models
{
    entity_fields RReview::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(user_id);
        result.push_back(map_id);
        result.push_back(r_session_id);
        result.push_back(repetition_algorithm_to_int(algorithm));
        result.push_back(note_id);
        result.push_back(cast64(review_date));
        result.push_back(grade);
        result.push_back(response_data);
        result.push_back(notes);
        result.push_back(cast64(started_at));
        result.push_back(cast64(ended_at));
        result.push_back(latency_ms);
        result.push_back(answer_change_count);
        result.push_back(details_json);
        return result;
    }

    void RReview::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        user_id = number();
        map_id = number();
        r_session_id = number();
        algorithm = enums::int_to_repetition_algorithm(number());
        note_id = number();
        review_date = number();
        grade = number();
        response_data = text();
        notes = text();
        started_at = number();
        ended_at = number();
        latency_ms = number();
        answer_change_count = number();
        details_json = text();
    };

    string RReview::validate()
    {
        using columns::RReviewColumns;

        validator_chain_vector list{
            [this] { return test_between(grade, 0, 5, RReviewColumns::GRADE); },
            [this] { return test_true(started_at < ended_at, "started_at must be less than ended_at"); },

            [this] { return test_at_least(latency_ms, 0, RReviewColumns::LATENCY_MS); },
            [this] { return test_eq(latency_ms, ended_at - started_at, RReviewColumns::LATENCY_MS); },
            [this] { return test_at_least(answer_change_count, 0, RReviewColumns::ANSWER_CHANGE_COUNT); }
        };
        return util::ValidatorChain::run(list);
    }
}
