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
#ifndef TEAM_H
#define TEAM_H


#include <string>
#include "../../../model/BaseModel.h"

// ***** MACROS : START *****
#define Model Team
#define MODEL TEAM
#define COLS columns::TeamColumns
#include "../columns/TeamColumns.h"
// ***** MACROS : END *****

namespace mindnet::plugins::core::models
{

    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def TEAM_DEFINITION =
        def(COLS::MODEL_NAME).set_rest_operations("rl")
        .set_group("Collaboration", 300)
        .set_columns({
            //
            coldef(COLS::NAME,MANDATORY),
            coldef(COLS::DESCRIPTION),
            coldef(COLS::CREATED_BY, READONLY | MANDATORY).set_foreign_key("user"),
            coldef(COLS::LEADER_ID,MANDATORY).set_foreign_key("user"),

            //
        });

    struct Model : mindnet::model::BaseModel
    {
        string name;
        string description;
        int created_by{};
        int leader_id{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                name == other.name &&
                description == other.description &&
                created_by == other.created_by &&
                leader_id == other.leader_id;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // TEAM_H
