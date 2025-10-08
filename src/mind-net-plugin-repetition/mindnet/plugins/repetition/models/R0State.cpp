//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/repetition/models/R0State.h"

namespace mindnet::plugins::repetition::models
{
    entity_fields R0State::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(user_id);
        result.push_back(note_id);
        result.push_back(question_id);
        result.push_back(repetitions);
        result.push_back(interval);
        result.push_back(cast64(next_review));
        result.push_back(cast64(last_review));
        result.push_back(last_quality);
        return result;
    }

    void R0State::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        user_id = number();
        note_id = number();
        question_id = number();
        repetitions = number();
        interval = number();
        next_review = number();
        last_review = number();
        last_quality = number();
    };

    string R0State::validate()
    {
        validator_chain_vector list{
            [this]
            {
                if (user_id <= 0) return std::unexpected("User ID must be positive");
                if (note_id <= 0 && question_id <= 0)
                    return std::unexpected("Either note_id or question_id must be set");
                if (repetitions < 0)
                    return std::unexpected("Repetitions must be non-negative");
                if (interval < 1)
                    return std::unexpected("Interval must be at least 1");
                if (last_quality < 0 || last_quality > 5)
                    return std::unexpected("Last quality must be between 0 and 5");
                return std::expected<void>();
            },
        };
        return util::ValidatorChain::run(list);
    }
}
