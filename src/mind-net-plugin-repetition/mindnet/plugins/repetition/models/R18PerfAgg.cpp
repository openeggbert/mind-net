//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/repetition/models/R18PerfAgg.h"

namespace mindnet::plugins::repetition::models
{
    entity_fields R18PerfAgg::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(user_id);
        result.push_back(bin_log_t_times_100);
        result.push_back(total);
        result.push_back(correct);
        return result;
    }

    void R18PerfAgg::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        user_id = number();
        bin_log_t_times_100 = number();
        total = number();
        correct = number();
    };

    string R18PerfAgg::validate()
    {
        using columns::R18PerfAggColumns;

        validator_chain_vector list{
            [this] { return test_at_least(bin_log_t_times_100, 0, R18PerfAggColumns::BIN_LOG_T_TIMES_100); },
            [this] { return test_at_least(total, 0, R18PerfAggColumns::TOTAL); },
            [this] { return test_at_least(correct, 0, R18PerfAggColumns::CORRECT); },
            [this] { return test_true(correct > total, "correct cannot be greater than total"); }
        };
        return util::ValidatorChain::run(list);
    }
}
