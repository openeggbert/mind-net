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
#ifndef REFERENCE_H
#define REFERENCE_H

#include <string>
#include <utility>

#include "../../../model/BaseModel.h"
// ***** MACROS : START *****
#define Model Reference
#define MODEL REFERENCE
#define COLS columns::ReferenceColumns
#include "../columns/ReferenceColumns.h"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def REFERENCE_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_group("Links", 100)
        .set_columns({
            coldef(COLS::FROM_NOTE_ID, MANDATORY | READONLY).set_foreign_key("note"),
            coldef(COLS::TO_NOTE_ID, MANDATORY | READONLY).set_foreign_key("note"),
            coldef(COLS::LABEL),
        });
    ;

    struct Model : mindnet::model::BaseModel
    {
        int from_note_id;
        int to_note_id;
        string label;

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id && from_note_id == other.from_note_id && to_note_id == other.to_note_id &&
                label == other.label && created_at == other.created_at && updated_at == other.
                updated_at;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // REFERENCE_H
