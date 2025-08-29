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
#ifndef MESSAGE_H
#define MESSAGE_H


#include <string>

#include "misc/BaseModel.h"
#include "columns/MessageColumns.h"
#include "crow/json.h"
#include "mindnet/Helper.h"
#include "mindnet/enums/Crudl.h"

// ***** DEFINE TWO MACROS : START *****
#define Model Message
#define MODEL MESSAGE
// ***** DEFINE TWO MACROS : END *****

namespace mindnet::models
{
    using bm = misc::BaseModel;
    using cols = columns::MessageColumns;
    using misc::def;
    using misc::coldef;

    inline def MESSAGE_DEFINITION =
        def(cols::MODEL_NAME)
        .set_columns(
            {
                coldef(cols::OWNER_ID).set_mandatory().set_foreign_key("user"),
                coldef(cols::SENDER_ID).set_mandatory().set_foreign_key("user"),
                coldef(cols::RECIPIENT_ID).set_mandatory().set_foreign_key("user"),
                coldef(cols::SUBJECT),
                coldef(cols::IMPORTANT).bool_column().set_default_value("0"),
                coldef(cols::BODY).textarea().set_mandatory(),
                coldef(cols::SENT_AT).datetime(),
                coldef(cols::SYSTEM_MESSAGE).bool_column().set_default_value("0"),
                coldef(cols::FOLDER),
                coldef(cols::DRAFT).bool_column().set_default_value("0"),
                coldef(cols::IS_READ).bool_column().set_default_value("0"),
                coldef(cols::DELETED_AT).datetime(),
                coldef(cols::STARRED).bool_column().set_default_value("0")
            });
    // *** Definition of model ends ***

    using misc::BaseModel;

    struct Message : BaseModel
    {
        int owner_id{};
        int sender_id{};
        int recipient_id{};
        str subject;
        bool important{false};
        str body;
        unixtime sent_at{};
        bool system_message{false};
        str folder;
        bool draft{false};
        bool is_read{false};
        unixtime deleted_at{};
        bool starred{false};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                owner_id == other.owner_id &&
                sender_id == other.sender_id &&
                recipient_id == other.recipient_id &&
                subject == other.subject &&
                important == other.important &&
                body == other.body &&
                sent_at == other.sent_at &&
                system_message == other.system_message &&
                folder == other.folder &&
                draft == other.draft &&
                is_read == other.is_read &&
                deleted_at == other.deleted_at &&
                starred == other.starred;
        }

        Message() = default;

    };
}

#endif // MESSAGE_H
