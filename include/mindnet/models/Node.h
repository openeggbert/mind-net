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
#ifndef NODE_H
#define NODE_H


#include <string>
#include <utility>

#include "misc/BaseModel.h"
#include "columns/NodeColumns.h"
#include "crow/json.h"
#include "mindnet/Helper.h"
#include "mindnet/enums/Difficulty.h"
#include "mindnet/enums/Importance.h"
#include "mindnet/enums/NodeType.h"
#include "mindnet/enums/Visibility.h"

namespace mindnet::models
{
    using enums::ColumnType;
    using misc::BaseModel;
    using misc::ModelDefinition;
    using columns::NodeColumns;

    static ModelDefinition NODE_DEFINITION = {
        NodeColumns::MODEL_NAME,
        true,
        {
            {NodeColumns::ID, ColumnType::INTEGER, true},
            {NodeColumns::CREATED_AT, ColumnType::INTEGER, false},
            {NodeColumns::UPDATED_AT, ColumnType::INTEGER, false},
            {NodeColumns::UUID, ColumnType::TEXT, true},
            {NodeColumns::MAP_ID, ColumnType::INTEGER, true},
            {NodeColumns::SIBLING_POSITION, ColumnType::INTEGER, true},
            {NodeColumns::TITLE, ColumnType::TEXT, true},
            {NodeColumns::CONTENT_ID, ColumnType::INTEGER, false},
            {NodeColumns::PARENT_NODE_ID, ColumnType::INTEGER, false},
            {NodeColumns::TYPE, ColumnType::INTEGER, false},
            {NodeColumns::VISIBILITY, ColumnType::INTEGER, false},
            {NodeColumns::LAST_SHOWN_AT, ColumnType::INTEGER, true},
            {NodeColumns::EXPIRES_AT, ColumnType::INTEGER, false},
            {NodeColumns::IS_FAVORITE, ColumnType::INTEGER, false},
            {NodeColumns::IS_REDIRECT, ColumnType::INTEGER, false},
            {NodeColumns::REDIRECT_NODE_ID, ColumnType::INTEGER, false},
            {NodeColumns::REDIRECT_REASON, ColumnType::TEXT, false},
            {NodeColumns::IMPORTANCE, ColumnType::INTEGER, false},
            {NodeColumns::DIFFICULTY, ColumnType::INTEGER, false},
        },
    };

    struct Node : BaseModel
    {
        str uuid;
        int map_id;
        int sibling_position;
        str title;
        int content_id;
        int parent_node_id;
        enums::NodeType type;
        enums::Visibility visibility;
        unixtime last_shown_at;
        unixtime expires_at;
        bool is_favorite;
        bool is_redirect;
        int redirect_node_id;
        str redirect_reason;
        enums::Importance importance;
        enums::Difficulty difficulty;

        [[nodiscard]] ModelDefinition get_definition() const override
        {
            return NODE_DEFINITION;
        }

        [[nodiscard]] entity_fields get_values() const override;
        void from_values(const entity_fields& values) override;

        friend std::ostream& operator<<(std::ostream& os, const Node& map)
        {
            os << map.to_json();
            return os;
        }

        bool operator==(const Node& other) const
        {
            return id == other.id && uuid == other.uuid && map_id == other.map_id &&
                sibling_position == other.sibling_position && title == other.title &&
                content_id == other.content_id && parent_node_id == other.parent_node_id &&
                type == other.type && visibility == other.visibility &&
                last_shown_at == other.last_shown_at && expires_at == other.expires_at &&
                is_favorite == other.is_favorite && is_redirect == other.is_redirect &&
                redirect_node_id == other.redirect_node_id && redirect_reason == other.redirect_reason &&
                importance == other.importance && difficulty == other.difficulty &&
                created_at == other.created_at && updated_at == other.updated_at;
        }

        Node() = default;

        Node(int id_, str uuid_, int map_id_, int sibling_position_, str title_,
             int content_id_, int parent_node_id_, int type_, int visibility_,
             unixtime last_shown_at_, unixtime expires_at_, bool is_favorite_,
             bool is_redirect_, int redirect_node_id_, str redirect_reason_,
             int importance_, int difficulty_,
             unixtime created_at_, unixtime updated_at_
        )
            : BaseModel(), uuid(std::move(uuid_)), map_id(map_id_), sibling_position(sibling_position_),
              title(std::move(title_)), content_id(content_id_), parent_node_id(parent_node_id_),
              type(static_cast<enums::NodeType>(type_)), visibility(static_cast<enums::Visibility>(visibility_)),
              last_shown_at(last_shown_at_), expires_at(expires_at_), is_favorite(is_favorite_),
              is_redirect(is_redirect_), redirect_node_id(redirect_node_id_),
              redirect_reason(std::move(redirect_reason_)), importance(static_cast<enums::Importance>(importance_)),
              difficulty(static_cast<enums::Difficulty>(difficulty_))
        {
            id = id_;
            created_at = created_at_;
            updated_at = updated_at_;
        }
    };
}

#endif // NODE_H
