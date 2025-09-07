//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/Map.h"

namespace mindnet::models
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
        result.push_back(owner_rights);
        result.push_back(team_rights);
        result.push_back(other_rights);
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
        } catch (const std::exception& e)
        {
            team_id = 0;
        }
        owner_rights = number();
        team_rights = number();
        other_rights = number();
    };
    string Map::validate()
    {
        using columns::MapColumns;

        validator_chain_vector list{
        [this] { return testt_between(name, 5, 80, "name");},
        [this] { return testt_between(description, 0, 80, "description");},
        [this] { return testt_between(category, 0, 40, "category");},
        [this] { return test_ne(owner_id, 0, "owner_id");},
        [this] { return test_between(owner_rights, 0, 7, "owner_rights");},
        [this] { return test_between(team_rights, 0, 7, "team_rights");},
        [this] { return test_between(other_rights, 0, 7, "other_rights");},
        };
        return ValidatorChain::run(list);
    }
}
