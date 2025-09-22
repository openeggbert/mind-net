//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/core/models/TeamMember.h"

namespace mindnet::plugins::core::models
{
    entity_fields TeamMember::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(team_id);
        result.push_back(user_id);
        result.push_back(cast64(role));
        result.push_back(cast64(status));
        result.push_back(cast64(joined_at));
        result.push_back(cast64(left_at));
        return result;
    }

    void TeamMember::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        team_id = number();
        user_id = number();
        role = static_cast<enums::UserRole>(number());
        status = static_cast<enums::UserStatus>(number());
        joined_at = number();
        left_at = number();
    };

    string TeamMember::validate()
    {
        using columns::TeamMemberColumns;

        validator_chain_vector list{

            [this] { return test_ne(team_id, 0, TeamMemberColumns::TEAM_ID); },
            [this] { return test_ne(user_id, 0, TeamMemberColumns::USER_ID); },
            [this] { return test_ne(joined_at, 0, TeamMemberColumns::JOINED_AT); },
        };
        return util::ValidatorChain::run(list);
    }
}
