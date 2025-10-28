//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/repetition/models/RUserSetting.h"

#include "mindnet/plugins/repetition/RSetting.h"

namespace mindnet::plugins::repetition::models
{
    entity_fields RUserSetting::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(user_id);
        result.push_back(key);
        result.push_back(value);
        return result;
    }

    void RUserSetting::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        user_id = number();
        key = text();
        value = text();
    };

    string RUserSetting::validate()
    {
        using columns::RUserSettingColumns;

        validator_chain_vector list{
            [this] { return testt_at_least(key, 1, RUserSettingColumns::KEY); },
            [this] { return test_true(r_setting_keys.contains(key), "Key " + key + " is not allowed."); }
        };
        return util::ValidatorChain::run(list);
    }
}
