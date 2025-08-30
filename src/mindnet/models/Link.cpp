//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/Link.h"

namespace mindnet::models
{
    entity_fields Link::get_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(from_note_id);
        result.push_back(to_url);
        return result;
    }

    void Link::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        from_note_id = number();
        to_url = text();
    };
}
