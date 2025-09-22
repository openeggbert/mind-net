//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/other/plugins/slipbox/models/Map.h"

namespace mindnet::plugins::slipbox::models
{
    entity_fields Map::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(name);
        result.push_back(description);
        result.push_back(category);
        result.push_back(owner_id);
        result.push_back(team_id);
        result.push_back(cast64(owner_rights));
        result.push_back(cast64(team_rights));
        result.push_back(cast64(other_rights));
        return result;
    }

    void Map::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        name = text();
        description = text();
        category = text();
        owner_id = number();
        try
        {
            team_id = number();
        }
        catch (const std::exception& e)
        {
            team_id = 0;
        }
        owner_rights = static_cast<core::enums::AccessRight>(number());
        team_rights = static_cast<core::enums::AccessRight>(number());
        other_rights = static_cast<core::enums::AccessRight>(number());
    };

    string Map::validate()
    {
        using columns::MapColumns;

        validator_chain_vector list{
            [this] { return testt_between(name, 1, 80, MapColumns::NAME); },
            [this] { return testt_between(description, 0, 80, MapColumns::DESCRIPTION); },
            [this] { return testt_between(category, 0, 40, MapColumns::CATEGORY); },
            [this] { return test_ne(owner_id, 0, MapColumns::OWNER_ID); },
            [this] { return test_between(cast64(owner_rights), 0, 7, MapColumns::OWNER_RIGHTS); },
            [this] { return test_between(cast64(team_rights), 0, 7, MapColumns::TEAM_RIGHTS); },
            [this] { return test_between(cast64(other_rights), 0, 7, MapColumns::OTHER_RIGHTS); },
        };
        return util::ValidatorChain::run(list);
    }
}
