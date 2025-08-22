//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/Content.h"

namespace mindnet::models
{

    entity_fields Content::get_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(static_cast<int64_t>(created_at));
        result.push_back(static_cast<int64_t>(updated_at));
        result.push_back(content);
        result.push_back(format);
        result.push_back(version);
        result.push_back(node_id);
        return result;
    }

    void Content::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        //
        content = text();
        format = number();
        version = number();
        node_id = number();
    }
;
}
