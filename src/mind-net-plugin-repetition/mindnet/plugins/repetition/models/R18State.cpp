//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/repetition/models/R18State.h"

namespace mindnet::plugins::repetition::models
{
    entity_fields R18State::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(user_id);
        result.push_back(note_id);
        result.push_back(question_id);
        result.push_back(stability_times_100);
        result.push_back(last_interval_times_100);
        result.push_back(repetitions);
        result.push_back(lapses);
        result.push_back(cast64(next_review));
        result.push_back(cast64(last_review));
        result.push_back(last_quality);
        return result;
    }

    void R18State::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        user_id = number();
        note_id = number();
        question_id = number();
        stability_times_100 = number();
        last_interval_times_100 = number();
        repetitions = number();
        lapses = number();
        next_review = number();
        last_review = number();
        last_quality = number();
    };

    string R18State::validate()
    {
        using columns::R18StateColumns;

        validator_chain_vector list{
            [this]
            {
                if (note_id == 0 && question_id == 0)
                {
                    return std::unexpected("Either note_id or question_id must be set");
                }
                if (note_id != 0 && question_id != 0)
                {
                    return std::unexpected("Only one of note_id or question_id can be set");
                }
                if (stability_times_100 < 0)
                {
                    return std::unexpected("stability_times_100 cannot be negative");
                }
                if (last_interval_times_100 < 0)
                {
                    return std::unexpected("last_interval_times_100 cannot be negative");
                }
                if (repetitions < 0)
                {
                    return std::unexpected("repetitions cannot be negative");
                }
                if (lapses < 0)
                {
                    return std::unexpected("lapses cannot be negative");
                }
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
