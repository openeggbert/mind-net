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
#pragma once


#include <string>
#include "mindnet/model/BaseModel.hpp"

// ***** MACROS : START *****
#define Model PinnedNote
#define MODEL PINNED_NOTE
#define COLS columns::PinnedNoteColumns
#include "../columns/PinnedNoteColumns.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def PINNED_NOTE_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_group("Slip Box", 100)
        .set_columns({
            coldef(COLS::USER_ID, MANDATORY | FOREIGN_KEY | READONLY)
            .set_description("User who pinned the note"),
            coldef(COLS::NOTE_ID, MANDATORY | FOREIGN_KEY | READONLY)
            .set_description("ID of the pinned note"),
            coldef(COLS::POSITION, INTEGER)
            .set_description("Optional position for ordering pinned notes"),
            coldef(COLS::IS_PUBLIC, BOOL)
            .set_default_value(0)
            .set_description("Whether this pinned note is public"),
        });

    struct Model : mindnet::model::BaseModel
    {
        int user_id{};
        int note_id{};
        int position{};
        bool is_public{false};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                user_id == other.user_id &&
                note_id == other.note_id &&
                position == other.position &&
                is_public == other.is_public;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
