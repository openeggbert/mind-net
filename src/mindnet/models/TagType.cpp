//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/TagType.h"

namespace mindnet::models
{
    entity_fields TagType::get_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(map_id);
        result.push_back(title);
        return result;
    }

    void TagType::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        map_id = number();
        title = text();
    };
}
