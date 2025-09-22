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
#ifndef COMMENTCOLUMNS_H
#define COMMENTCOLUMNS_H

namespace mindnet::plugins::chat::columns
{
    struct CommentColumns : model::BaseColumns
    {
        CommentColumns() = delete;

        CommentColumns(const CommentColumns&) = delete;
        CommentColumns& operator=(const CommentColumns&) = delete;

        static constexpr const char* MODEL_NAME = "comment";

        static constexpr const char* DISCUSSION_ID = "discussion_id";
        static constexpr const char* USER_ID = "user_id";
        static constexpr const char* CONTENT = "content";
        static constexpr const char* PARENT_COMMENT_ID = "parent_comment_id";
        static constexpr const char* IS_DELETED = "is_deleted";
    };
}
#endif // COMMENTCOLUMNS_H
