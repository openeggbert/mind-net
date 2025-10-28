///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see
// <https://www.gnu.org/licenses/> or write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef ALERT_H
#define ALERT_H

#include <string>
#include "mindnet/model/BaseModel.h"

// ***** MACROS : START *****
#define Model Alert
#define MODEL ALERT
#define COLS columns::AlertColumns
#include "../columns/AlertColumns.h"
#include "mindnet/plugins/slipbox/enums/AlertChannel.h"
#include "mindnet/plugins/slipbox/enums/AlertRepeatInterval.h"
#include "mindnet/plugins/slipbox/enums/AlertStatus.h"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def ALERT_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_group("Slip Box", 100).set_title_column(COLS::TITLE)
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

    struct Model : mindnet::model::BaseModel
    {
        unixtime trigger_at;
        unixtime last_triggered_at;
        int trigger_count{};
        unixtime snooze_until;
        unixtime expires_at;

        enums::AlertRepeatInterval repeat_interval{};
        int repeat_count{};
        unixtime repeat_until;

        int user_id{};
        int note_id{};
        string url;

        string title;
        string message;

        enums::AlertStatus status{};

        bool important{false};
        enums::AlertChannel channel{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                trigger_at == other.trigger_at &&
                last_triggered_at == other.last_triggered_at &&
                trigger_count == other.trigger_count &&
                snooze_until == other.snooze_until &&
                expires_at == other.expires_at &&
                repeat_interval == other.repeat_interval &&
                repeat_count == other.repeat_count &&
                repeat_until == other.repeat_until &&
                user_id == other.user_id &&
                note_id == other.note_id &&
                url == other.url &&
                title == other.title &&
                message == other.message &&
                status == other.status &&
                important == other.important &&
                channel == other.channel;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // ALERT_H
