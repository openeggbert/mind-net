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
#include <utility>

#include "mindnet/model/BaseModel.hpp"
// ***** MACROS : START *****
#define Model NoteNavigation
#define MODEL NOTE_NAVIGATION
#define COLS columns::NoteNavigationColumns
#include "../columns/NoteNavigationColumns.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def NOTE_NAVIGATION_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_group("Slip Box", 100)
        .set_rest_operations("r").set_title_column(COLS::ID)
        .set_no_table(true)
        .set_columns({
            //
            coldef(COLS::NOTE_ID, MANDATORY | READONLY | FOREIGN_KEY),
            coldef(COLS::PREV_NOTE_ID, MANDATORY | READONLY | FOREIGN_KEY).set_foreign_key("note"),
            coldef(COLS::NEXT_NOTE_ID, MANDATORY | READONLY | FOREIGN_KEY).set_foreign_key("note"),
        });

    struct Model : mindnet::model::BaseModel
    {
        i64 note_id{};
        i64 prev_note_id{};
        i64 next_note_id{};

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                note_id == other.note_id &&
                prev_note_id == other.prev_note_id &&next_note_id== other.next_note_id &&
                created_at == other.created_at && updated_at == other.updated_at;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
