//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/Property.h"

namespace mindnet::models
{
    entity_fields Property::get_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(map_id);
        result.push_back(node_id);
        result.push_back(key);
        result.push_back(value);
        result.push_back(cast64(value_type));
        result.push_back(is_indexed);
        return result;
    }

    void Property::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        map_id = number();
        node_id = number();
        key = text();
        value = text();
        value_type = static_cast<enums::ValueType>(number());
        is_indexed = number();
    };
}
