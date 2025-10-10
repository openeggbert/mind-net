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
            [this] {return test_true(note_id <= 0 && question_id <= 0, "Either note_id or question_id must be set");},
            [this] {return test_at_least(stability_times_100, 0, R18StateColumns::STABILITY_TIMES_100);},
            [this] {return test_at_least(last_interval_times_100, 0, R18StateColumns::LAST_INTERVAL_TIMES_100);},
            [this] {return test_at_least(repetitions, 0, R18StateColumns::REPETITIONS);},
            [this] {return test_at_least(lapses, 0, R18StateColumns::LAPSES);},
            [this] {return test_between(last_quality, 0, 5, R18StateColumns::LAST_QUALITY);},
        };
        return util::ValidatorChain::run(list);
    }


}
