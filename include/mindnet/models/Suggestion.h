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
#include "mindnet/enums/NodeType.h"

namespace mindnet::models
{
    // *** Definition of model starts ***
    using type = enums::ColumnType;
    using cols = columns::HistoryColumns;
    using coldef = misc::ColumnDefinition;
    using def = misc::ModelDefinition;
    static def HISTORY_DEFINITION =
        def(cols::MODEL_NAME)
        .set_auto_inc(true)
        .set_columns(
            {
                coldef(cols::TABLE_NAME).set_mandatory(true),
                coldef(cols::RECORD_ID).set_mandatory(true),
                coldef(cols::OPERATION, type::INTEGER)
                .set_mandatory(true)
                .set_enum_definition(enums::crudl_to_enum_definition()),
                coldef(cols::DATA_JSON).set_mandatory(true),
                coldef(cols::REASON, type::TEXT)
            })
        .set_operations({enums::Crudl::READ, enums::Crudl::LIST});
    // *** Definition of model ends ***

    using misc::BaseModel;

    struct History : BaseModel
    {
        str table_name;
        int record_id{};
        enums::Crudl operation{};
        str data_json;
        str reason;

        [[nodiscard]] const def& get_definition() const override
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
                data_json == other.data_json &&
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
            operation = static_cast<enums::Crudl>(op);
            data_json = pl;
            reason = r;
        }
    };
}

#endif // HISTORY_H
