//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/Reference.h"

namespace mindnet::models
{
    entity_fields Reference::get_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(from_note_id);
        result.push_back(to_note_id);
        result.push_back(label);
        return result;
    }

    void Reference::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        from_note_id = number();
        to_note_id = number();
        label = text();
    };
}
