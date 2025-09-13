//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/core/models/Team.h"

namespace mindnet::plugins::core::models
{
    entity_fields Team::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(name);
        result.push_back(description);
        result.push_back(created_by);
        result.push_back(leader_id);
        return result;
    }

    void Team::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        name = text();
        description = text();
        created_by = number();
        leader_id = number();
    };

    string Team::validate()
    {
        using columns::TeamColumns;

        validator_chain_vector list{
            [this] { return testt_between(name, 5, 32, TeamColumns::NAME); },
            [this] { return test_at_most(description.size(), 256, TeamColumns::DESCRIPTION); },
            [this] { return test_ne(created_by, 0, TeamColumns::CREATED_BY); },
            [this] { return test_ne(leader_id, 0, TeamColumns::LEADER_ID); },
        };
        return ValidatorChain::run(list);
    }
}
