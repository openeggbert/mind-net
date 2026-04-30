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

#pragma once

#include <string>
#include "hive/model/BaseModel.hpp"

// ***** MACROS : START *****
#define Model Alert
#define MODEL ALERT
#define COLS columns::AlertColumns
#include "../columns/AlertColumns.hpp"
#include "hive/plugins/slipbox/enums/AlertChannel.hpp"
#include "hive/plugins/slipbox/enums/AlertRepeatInterval.hpp"
#include "hive/plugins/slipbox/enums/AlertStatus.hpp"
#include "hive/plugins/slipbox/SlipBoxPlugin.hpp"

// ***** MACROS : END *****

namespace hive::plugins::slipbox::models
{
    using hive::model::def;
    using hive::model::coldef;
    using_flags();

    inline const def ALERT_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_all_rest_operations()
        .set_group("Slip Box #2", 1000).set_title_column(COLS::TITLE)
        .set_columns({
            coldef(COLS::TRIGGER_AT, DATETIME | MANDATORY).set_description("When notification should trigger"),
            coldef(COLS::LAST_TRIGGERED_AT, DATETIME).set_description("Last time the alert was triggered"),
            coldef(COLS::TRIGGER_COUNT, INTEGER).set_description("Number of times triggered"),
            coldef(COLS::SNOOZE_UNTIL, DATETIME).set_description("Snooze until this time"),
            coldef(COLS::EXPIRES_AT, DATETIME).set_description("Expiration time"),

            coldef(COLS::REPEAT_INTERVAL).set_enum_definition(enums::alert_repeat_interval_to_enum_definition()).
                                          set_default_value(0).set_description(
                                              "Repeat interval (e.g. DAILY, WEEKLY)"),
            coldef(COLS::REPEAT_COUNT, INTEGER).set_default_value(0).set_description("Number of times to repeat"),
            coldef(COLS::REPEAT_UNTIL, DATETIME).set_default_value(0).set_description("Repeat until this time"),

            coldef(COLS::USER_ID, MANDATORY | READONLY).set_foreign_key("user").set_description("User ID"),
            coldef(COLS::NOTE_ID).set_foreign_key("note").set_description("Associated note ID"),
            coldef(COLS::URL, TEXT).set_description("Associated URL"),

            coldef(COLS::TITLE, TEXT | MANDATORY).set_description("Alert title"),
            coldef(COLS::MESSAGE, TEXT).set_description("Alert message"),

            coldef(COLS::STATUS).set_enum_definition(enums::alert_status_to_enum_definition()).set_default_value(0).
                                 set_description("Alert status (ACTIVE, TRIGGERED, etc)"),

            coldef(COLS::IMPORTANT, BOOL).set_default_value(0).set_description("Whether this alert is important"),
            coldef(COLS::CHANNEL, INTEGER).set_description("Notification channel").set_enum_definition(
                enums::alert_channel_to_enum_definition())
        });

    struct Model : hive::model::BaseModel
    {
        unixtime trigger_at;
        unixtime last_triggered_at;
        int trigger_count{};
        unixtime snooze_until;
        unixtime expires_at;

        enums::AlertRepeatInterval repeat_interval{};
        int repeat_count{};
        unixtime repeat_until;

        identification user_id{};
        identification note_id{};
        string url;

        string title;
        string message;

        enums::AlertStatus status{};

        bool important{false};
        enums::AlertChannel channel{};

        static constexpr auto fields = std::make_tuple(
            &Model::trigger_at,
            &Model::last_triggered_at,
            &Model::trigger_count,
            &Model::snooze_until,
            &Model::expires_at,
            &Model::repeat_interval,
            &Model::repeat_count,
            &Model::repeat_until,
            &Model::user_id,
            &Model::note_id,
            &Model::url,
            &Model::title,
            &Model::message,
            &Model::status,
            &Model::important,
            &Model::channel
        );

        create_model_h_methods(Model, MODEL)

    };
}
#undef Model
#undef MODEL
#undef COLS