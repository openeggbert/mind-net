//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/History.h"

namespace mindnet::models
{
    entity_fields History::get_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(table_name);
        result.push_back(cast64(record_id));
        result.push_back(cast64(operation));
        result.push_back(payload);
        result.push_back(reason);
        return result;
    }

    void History::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        table_name = text();
        record_id = number();
        operation = static_cast<enums::Crudl>(number());
        payload = text();
        reason = text();
    }
}
