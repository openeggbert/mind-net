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

/**
 *
* @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */
#pragma once

#include "mindnet/model/BaseColumns.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/model/ColumnType.hpp"

namespace mindnet::plugins::slipbox::columns
{
    struct AlertColumns : model::BaseColumns
    {
        AlertColumns() = delete;

        AlertColumns(const AlertColumns&) = delete;
        AlertColumns& operator=(const AlertColumns&) = delete;

        static constexpr const char* MODEL_NAME = "alert";

        // Trigger related columns
        static constexpr const char* TRIGGER_AT = "trigger_at";
        static constexpr const char* LAST_TRIGGERED_AT = "last_triggered_at";
        static constexpr const char* TRIGGER_COUNT = "trigger_count";
        static constexpr const char* SNOOZE_UNTIL = "snooze_until";
        static constexpr const char* EXPIRES_AT = "expires_at";

        // Repeat related columns
        static constexpr const char* REPEAT_INTERVAL = "repeat_interval";
        static constexpr const char* REPEAT_COUNT = "repeat_count";
        static constexpr const char* REPEAT_UNTIL = "repeat_until";

        // Reference columns
        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* NOTE_ID = "note_id";
        static constexpr const char* URL = "url";

        // Content columns
        static constexpr const char* TITLE = "title";
        static constexpr const char* MESSAGE = "message";

        // Status and metadata
        static constexpr const char* STATUS = "status";
        static constexpr const char* IMPORTANT = "important";
        static constexpr const char* CHANNEL = "channel";
    };
}