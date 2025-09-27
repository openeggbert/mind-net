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

/**
 *
* @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */
#ifndef ALERTCOLUMNS_H
#define ALERTCOLUMNS_H

#include "mindnet/model/BaseColumns.h"
#include "mindnet/essential/Helper.h"
#include "mindnet/model/ColumnType.h"

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
#endif // ALERTCOLUMNS_H
