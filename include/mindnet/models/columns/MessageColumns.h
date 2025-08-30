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

/**
 *
* @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */
#ifndef MESSAGECOLUMNS_H
#define MESSAGECOLUMNS_H


#include "BaseColumns.h"


namespace mindnet::models::columns
{
    struct MessageColumns : BaseColumns
    {
        MessageColumns() = delete;

        MessageColumns(const MessageColumns&) = delete;
        MessageColumns& operator=(const MessageColumns&) = delete;

        static constexpr const char* MODEL_NAME = "message";

        static constexpr const char* OWNER_ID = "owner_id";
        static constexpr const char* SENDER_ID = "sender_id";
        static constexpr const char* RECIPIENT_ID = "recipient_id";
        static constexpr const char* SUBJECT = "subject";
        static constexpr const char* IMPORTANT = "important";
        static constexpr const char* BODY = "body";
        static constexpr const char* SENT_AT = "sent_at";
        static constexpr const char* SYSTEM_MESSAGE = "system_message";
        static constexpr const char* FOLDER = "folder";
        static constexpr const char* DRAFT = "draft";
        static constexpr const char* IS_READ = "is_read";
        static constexpr const char* DELETED_AT = "deleted_at";
        static constexpr const char* STARRED = "starred";
    };
}
#endif // MESSAGECOLUMNS_H