///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : Mind map software.
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
#ifndef HISTORY_H
#define HISTORY_H

#include <string>

#include "misc/BaseModel.h"
#include "columns/HistoryColumns.h"
#include "columns/MapColumns.h"
#include "crow/json.h"
#include "mindnet/Helper.h"
#include "mindnet/enums/Crudl.h"

namespace mindnet::models
{
    using enums::ColumnType;
    using misc::BaseModel;
    using misc::ModelDefinition;
    using columns::HistoryColumns;

    static ModelDefinition HISTORY_DEFINITION = {
        HistoryColumns::MODEL_NAME,
        true,
        {
            {HistoryColumns::ID, ColumnType::INTEGER, true},
            {HistoryColumns::CREATED_AT, ColumnType::INTEGER, false},
            {HistoryColumns::UPDATED_AT, ColumnType::INTEGER, false},
            {HistoryColumns::TABLE_NAME, ColumnType::TEXT, true},
            {HistoryColumns::RECORD_ID, ColumnType::INTEGER, true},
            {HistoryColumns::OPERATION, ColumnType::INTEGER, true},
            {HistoryColumns::PAYLOAD, ColumnType::TEXT, true},
            {HistoryColumns::REASON, ColumnType::TEXT, false}
        }
    };

    struct History : BaseModel
    {
        str table_name;
        int record_id{};
        Enum::Crudl operation{};
        str payload;
        str reason;

        [[nodiscard]] ModelDefinition get_definition() const override
        {
            return HISTORY_DEFINITION;
        }

        [[nodiscard]] entity_fields get_values() const override;
        void from_values(const entity_fields& values) override;

        friend std::ostream& operator<<(std::ostream& os, const History& history)
        {
            os << history.to_json();
            return os;
        }

        bool operator==(const History& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                table_name == other.table_name &&
                record_id == other.record_id &&
                operation == other.operation &&
                payload == other.payload &&
                reason == other.reason;
        }

        History() = default;

        History(int i, unixtime cat, unixtime uat, const str& tn, int rid, int op, const str& pl, const str& r)
        {
            id = i;
            created_at = cat;
            updated_at = uat;
            table_name = tn;
            record_id = rid;
            operation = static_cast<Enum::Crudl>(op);
            payload = pl;
            reason = r;
        }
    };
}

#endif // HISTORY_H
