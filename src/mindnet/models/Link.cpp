//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/ExternalLink.h"

namespace mindnet::models
{
    entity_fields ExternalLink::get_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(from_node_id);
        result.push_back(to_url);
        return result;
    }

    void ExternalLink::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        from_node_id = number();
        to_url = text();
    };
}
