//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/Map.h"

namespace mindnet::models
{

    entity_fields Map::get_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(static_cast<int64_t>(created_at));
        result.push_back(static_cast<int64_t>(updated_at));
        result.push_back(name);
        result.push_back(description);
        result.push_back(category);
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

    };
}
