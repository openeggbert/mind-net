//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/repetition/models/R4State.h"

namespace mindnet::plugins::repetition::models
{
    entity_fields R4State::to_values() const
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
        result.push_back(ef_times_100);
        result.push_back(correction_factor_times_100);
        result.push_back(cast64(next_review));
        result.push_back(cast64(last_review));
        result.push_back(last_quality);
        return result;
    }

    void R4State::from_values(const entity_fields& values)
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
        ef_times_100 = number();
        correction_factor_times_100 = number();
        next_review = number();
        last_review = number();
        last_quality = number();
    }

    string R4State::validate()
    {
        using columns::R4StateColumns;

        validator_chain_vector list{
            [this]
            {
                // Validate that either note_id or question_id is set (not both null)
                if (note_id == 0 && question_id == 0)
                {
                    return std::unexpected("Either note_id or question_id must be set");
                }
                // Validate required user_id
                if (user_id == 0)
                {
                    return std::unexpected("user_id is required");
                }
                // Validate ef_times_100 range (100-500)
                if (ef_times_100 < 100 || ef_times_100 > 500)
                {
                    return std::unexpected("ef_times_100 must be between 100 and 500");
                }
                // Validate correction_factor_times_100 has default 100
                if (correction_factor_times_100 <= 0)
                {
                    correction_factor_times_100 = 100;
                }
                // Validate last_quality range (0-5)
                if (last_quality < 0 || last_quality > 5)
                {
                    return std::unexpected("last_quality must be between 0 and 5");
                }
                return std::expected<void>();
            }
        };
        return util::ValidatorChain::run(list);
    }
}
