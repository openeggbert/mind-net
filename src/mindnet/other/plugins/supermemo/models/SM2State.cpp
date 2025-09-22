//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/other/plugins/supermemo/models/SM2State.h"

namespace mindnet::plugins::supermemo::models
{
    entity_fields SM2State::to_values() const
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
        result.push_back(cast64(next_review));
        result.push_back(cast64(last_review));
        result.push_back(last_quality);
        return result;
    }

    void SM2State::from_values(const entity_fields& values)
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
        next_review = number();
        last_review = number();
        last_quality = number();
    };

    string SM2State::validate()
    {
        using columns::SM2StateColumns;

        validator_chain_vector list{
            [this] { return std::unexpected("Validation for SM2State was not yet implemented."); },
        };
        return util::ValidatorChain::run(list);
    }
}
