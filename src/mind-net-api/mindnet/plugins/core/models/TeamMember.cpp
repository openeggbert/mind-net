/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "mindnet/plugins/core/models/TeamMember.hpp"


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
        role = static_cast<mindnet::essential::UserRole>(number());
        status = static_cast<essential::UserStatus>(number());
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
            [this]
            {
                return test_true(left_at == 0 || left_at > joined_at, "Left at must be 0 or greater than Joined at");
            },
        };
        return util::ValidatorChain::run(list);
    }
}
