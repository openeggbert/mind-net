//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/History.h"

namespace mindnet::models
{
    entity_fields History::get_values() const
    {
        entity_fields result;
        result.emplace_back(id);
        result.emplace_back(cast64(created_at));
        result.emplace_back(cast64(updated_at));
        result.emplace_back(cast64(user_id));
        result.emplace_back(ip_address);
        result.emplace_back(table_name);
        result.emplace_back(cast64(record_id));
        result.emplace_back(cast64(operation));
        result.emplace_back(data_json);
        result.emplace_back(reason);
        return result;
    }

    void History::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        user_id = number();
        ip_address = text();
        table_name = text();
        record_id = number();
        operation = static_cast<enums::Crudl>(number());
        data_json = text();
        reason = text();
    }
}
