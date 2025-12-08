/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mindnet/plugins/dictionary/models/Alert.hpp"

namespace mindnet::plugins::dictionary::models
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