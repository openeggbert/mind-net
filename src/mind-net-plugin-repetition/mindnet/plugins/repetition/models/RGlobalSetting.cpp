//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/repetition/models/RGlobalSetting.h"

namespace mindnet::plugins::repetition::models
{
    entity_fields RGlobalSetting::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(key);
        result.push_back(value);
        return result;
    }

    void RGlobalSetting::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        key = text();
        value = text();
    }

    string RGlobalSetting::validate()
    {
        using columns::RGlobalSettingColumns;

        validator_chain_vector list{
            [this]
            {
                if (key.empty())
                {
                    return std::unexpected("Key cannot be empty");
                }
                return std::expected<void>();
            },
            [this]
            {
                if (value.empty())
                {
                    return std::unexpected("Value cannot be empty");
                }
                return std::expected<void>();
            }
        };
        return util::ValidatorChain::run(list);
    }
}
