//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/core/models/History.h"

namespace mindnet::plugins::core::models
{
    entity_fields History::to_values() const
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
    string History::validate()
    {
        using columns::HistoryColumns;

        validator_chain_vector list{
            [this] { return test_ne(user_id, 0, HistoryColumns::USER_ID); },
            [this] { return testt_not_empty(table_name, HistoryColumns::TABLE_NAME); },
            [this] { return test_ne(record_id, 0, HistoryColumns::RECORD_ID); },
            [this] { return test_ne(cast64(operation), 0, HistoryColumns::OPERATION); },
            [this] { return testt_not_empty(data_json, HistoryColumns::DATA_JSON); },
        };
        return ValidatorChain::run(list);
    }
}
