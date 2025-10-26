//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/repetition/models/R18PredictionLog.h"

namespace mindnet::plugins::repetition::models
{
    entity_fields R18PredictionLog::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(user_id);
        result.push_back(note_id);
        result.push_back(predicted_R_times_100);
        result.push_back(actual_grade);
        result.push_back(was_correct);
        return result;
    }

    void R18PredictionLog::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        user_id = number();
        note_id = number();
        predicted_R_times_100 = number();
        actual_grade = number();
        was_correct = boolean();
    };

    string R18PredictionLog::validate()
    {
        using columns::R18PredictionLogColumns;

        validator_chain_vector list{
            [this] { return test_not_null(user_id, R18PredictionLogColumns::USER_ID); },
            [this] { return test_at_least(predicted_R_times_100, 0, R18PredictionLogColumns::PREDICTED_R_TIMES_100); },
            [this] { return test_at_least(actual_grade, 0, R18PredictionLogColumns::ACTUAL_GRADE); }
        };
        return util::ValidatorChain::run(list);
    }
}
