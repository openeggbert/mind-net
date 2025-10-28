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
        repetitions = number();
        interval = number();
        next_review = number();
        last_review = number();
        last_quality = number();
    };

    string R0State::validate()
    {
        using columns::R0StateColumns;

        validator_chain_vector list{
            [this] { return test_at_least(repetitions, 0, R0StateColumns::REPETITIONS); },
            [this] { return test_at_least(interval, 1, R0StateColumns::INTERVAL); },
            [this] { return test_between(last_quality, 0, 5, R0StateColumns::LAST_QUALITY); },
        };
        return util::ValidatorChain::run(list);
    }
}
