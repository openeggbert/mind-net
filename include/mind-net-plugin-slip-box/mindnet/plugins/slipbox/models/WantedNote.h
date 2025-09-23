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
#ifndef WANTEDNOTE_H
#define WANTEDNOTE_H

#include <string>
#include "mindnet/model/BaseModel.h"

// ***** MACROS : START *****
#define Model WantedNote
#define MODEL WANTED_NOTE
#define COLS columns::WantedNoteColumns
#include "../columns/WantedNoteColumns.h"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def WANTED_NOTE_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_rest_operations("rl")
        .set_group("Slip Box", 100)
        .set_title_column(COLS::TO_NOTE_TITLE)
        .set_columns({
            coldef(COLS::FROM_NOTE_ID, MANDATORY | FOREIGN_KEY)
            .set_description("Reference to the source note"),
            coldef(COLS::TO_NOTE_TITLE, MANDATORY)
            .set_description("Title of the target note"),
        });

    struct Model : mindnet::model::BaseModel
    {
        int from_note_id{};
        string to_note_title;

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                from_note_id == other.from_note_id &&
                to_note_title == other.to_note_title;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // WANTEDNOTE_H
