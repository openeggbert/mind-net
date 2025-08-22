//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/NodeTag.h"

namespace mindnet::models
{
    entity_fields NodeTag::get_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(static_cast<int64_t>(created_at));
        result.push_back(static_cast<int64_t>(updated_at));
        result.push_back(node_id);
        result.push_back(tag_id);
        return result;
    }

    void NodeTag::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        node_id = number();
        tag_id = number();
    };
}
