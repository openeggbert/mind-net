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

#include "../../../model/BaseModel.h"
// ***** MACROS : START *****
#define Model Message
#define MODEL MESSAGE
#define COLS columns::MessageColumns
#include "../columns/MessageColumns.h"
// ***** MACROS : END *****

namespace mindnet::plugins::mail::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def MESSAGE_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_group("Mail", 300).set_title_column(COLS::SUBJECT).allow_reader_write()
        .set_columns(
            {
                coldef(COLS::OWNER_ID, MANDATORY | READONLY).set_foreign_key("user"),
                coldef(COLS::SENDER_ID, MANDATORY).set_foreign_key("user"),
                coldef(COLS::RECIPIENT_ID, MANDATORY).set_foreign_key("user"),
                coldef(COLS::SUBJECT),
                coldef(COLS::IMPORTANT, BOOL).set_default_value(0),
                coldef(COLS::BODY, TEXTAREA | MANDATORY),
                coldef(COLS::SENT_AT, DATETIME),
                coldef(COLS::SYSTEM_MESSAGE, BOOL | READONLY).set_default_value(0),
                coldef(COLS::FOLDER),
                coldef(COLS::DRAFT, BOOL).set_default_value(0),
                coldef(COLS::IS_READ, BOOL).set_default_value(0),
                coldef(COLS::DELETED_AT, DATETIME),
                coldef(COLS::STARRED, BOOL).set_default_value(0)
            })
        .add_custom_list_action("message", "Inbox", {"todo", "{id}"})
        .add_custom_list_action("message", "Sent", {"todo", "{id}"})
        .add_custom_list_action("message", "Drafts", {"todo", "{id}"})
        .add_custom_create_action("collection_item", "Compose", {"todo", "{id}"});
    // *** Definition of model ends ***

    struct Model : mindnet::model::BaseModel
    {
        int owner_id{};
        int sender_id{};
        int recipient_id{};
        string subject;
        bool important{false};
        string body;
        unixtime sent_at{};
        bool system_message{false};
        string folder;
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
    };
}

#undef Model
#undef MODEL
#undef COLS
#endif // MESSAGE_H
