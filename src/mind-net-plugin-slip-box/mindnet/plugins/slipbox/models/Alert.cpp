//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/slipbox/models/Alert.hpp"

namespace mindnet::plugins::slipbox::models
{
    entity_fields Alert::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(cast64(trigger_at));
        result.push_back(cast64(last_triggered_at));
        result.push_back(trigger_count);
        result.push_back(cast64(snooze_until));
        result.push_back(cast64(expires_at));
        result.push_back(cast64(repeat_interval));
        result.push_back(repeat_count);
        result.push_back(cast64(repeat_until));
        result.push_back(user_id);
        result.push_back(note_id);
        result.push_back(url);
        result.push_back(title);
        result.push_back(message);
        result.push_back(cast64(status));
        result.push_back(cast64(important));
        result.push_back(cast64(channel));
        return result;
    }

    void Alert::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        trigger_at = number();
        last_triggered_at = number();
        trigger_count = number();
        snooze_until = number();
        expires_at = number();
        repeat_interval = static_cast<enums::AlertRepeatInterval>(number());
        repeat_count = number();
        repeat_until = number();
        user_id = number();
        note_id = number();
        url = text();
        title = text();
        message = text();
        status = static_cast<enums::AlertStatus>(number());
        important = number();
        channel = static_cast<enums::AlertChannel>(number());
    }

    string Alert::validate()
    {
        using columns::AlertColumns;

        validator_chain_vector list{
            [this] { return test_ne(user_id, 0, AlertColumns::USER_ID); },
            [this] { return test_at_least(trigger_at, 0, AlertColumns::TRIGGER_AT); },
            [this] { return testt_between(title, 1, 255, AlertColumns::TITLE); },
            [this]
            {
                return test_false(repeat_until > 0 && repeat_until <= trigger_at,
                                  "repeat_until must be after trigger_at");
            },
        };
        return util::ValidatorChain::run(list);
    }
}
