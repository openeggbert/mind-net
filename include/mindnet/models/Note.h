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
#ifndef NOTE_H
#define NOTE_H


#include <string>
#include <utility>

#include "misc/BaseModel.h"
// ***** MACROS : START *****
#define Model Note
#define MODEL NOTE
#define COLS columns::NoteColumns
#include "columns/NoteColumns.h"
// ***** MACROS : END *****

#include "mindnet/enums/Difficulty.h"
#include "mindnet/enums/Importance.h"

namespace mindnet::models
{

    using misc::def;
    using misc::coldef;
    using_flags();

    inline def NOTE_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
    .set_columns({
    //
    coldef(COLS::ID,tttt | MANDATORY),
    coldef(COLS::ID,tttt | MANDATORY),
    coldef(COLS::ID,tttt | MANDATORY),
    coldef(COLS::ID,tttt | MANDATORY),
    coldef(COLS::ID,tttt | MANDATORY),
    coldef(COLS::ID,tttt | MANDATORY),
    coldef(COLS::ID,tttt | MANDATORY),

    //
});
    {
        {
            NodeColumns::ID, ColumnType::INTEGER, true
        }
        ,
        {
            NodeColumns::CREATED_AT, ColumnType::INTEGER, false
        }
        ,
        {
            NodeColumns::UPDATED_AT, ColumnType::INTEGER, false
        }
        ,
        {
            NodeColumns::UUID, ColumnType::TEXT, true
        }
        ,
        {
            NodeColumns::MAP_ID, ColumnType::INTEGER, true
        }
        ,
        {
            NodeColumns::SIBLING_POSITION, ColumnType::INTEGER, true
        }
        ,
        {
            NodeColumns::TITLE, ColumnType::TEXT, true
        }
        ,
        {
            NodeColumns::CONTENT_ID, ColumnType::INTEGER, false
        }
        ,
        {
            NodeColumns::PARENT_NODE_ID, ColumnType::INTEGER, false
        }
        ,
        {
            NodeColumns::TYPE, ColumnType::INTEGER, false
        }
        ,
        {
            NodeColumns::VISIBILITY, ColumnType::INTEGER, false
        }
        ,
        {
            NodeColumns::LAST_SHOWN_AT, ColumnType::INTEGER, true
        }
        ,
        {
            NodeColumns::EXPIRES_AT, ColumnType::INTEGER, false
        }
        ,
        {
            NodeColumns::IS_FAVORITE, ColumnType::INTEGER, false
        }
        ,
        {
            NodeColumns::REDIRECT_NODE_ID, ColumnType::INTEGER, false
        }
        ,
        {
            NodeColumns::REDIRECT_REASON, ColumnType::TEXT, false
        }
        ,
        {
            NodeColumns::IMPORTANCE, ColumnType::INTEGER, false
        }
        ,
        {
            NodeColumns::DIFFICULTY, ColumnType::INTEGER, false
        }
        ,
    }

    ,
};

struct Model : misc::BaseModel
{
    int map_id{};
    string title;
    int parent_note_id{};
    int content_id{};
    int sibling_position{};
    mindnet::enums::Importance importance{mindnet::enums::Importance::UNDEFINED};
    mindnet::enums::Difficulty difficulty{mindnet::enums::Difficulty::UNDEFINED};

    create_model_h_methods(Model, MODEL)

    bool operator==(const Model& other) const
    {
        return id == other.id && uuid == other.uuid && map_id == other.map_id &&
            sibling_position == other.sibling_position && title == other.title &&
            content_id == other.content_id && parent_node_id == other.parent_node_id &&
            type == other.type && visibility == other.visibility &&
            last_shown_at == other.last_shown_at && expires_at == other.expires_at &&
            is_favorite == other.is_favorite &&
            redirect_node_id == other.redirect_node_id && redirect_reason == other.redirect_reason &&
            importance == other.importance && difficulty == other.difficulty &&
            created_at == other.created_at && updated_at == other.updated_at;
    }
};

}
#undef Model
#undef MODEL
#undef COLS
#endif // NOTE_H