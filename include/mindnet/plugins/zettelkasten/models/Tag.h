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
#ifndef TAG_H
#define TAG_H


#include <string>

#include "../../../model/BaseModel.h"
// ***** MACROS : START *****
#define Model Tag
#define MODEL TAG
#define COLS columns::TagColumns
#include "../columns/TagColumns.h"
// ***** MACROS : END *****


namespace mindnet::plugins::zettelkasten::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline def TAG_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_columns({
            coldef(COLS::NOTE_ID, MANDATORY | READONLY | FOREIGN_KEY),
            coldef(COLS::TAG_TYPE_ID, MANDATORY | READONLY | FOREIGN_KEY),
        });

struct Model : mindnet::model::BaseModel
{
    int note_id;
    int tag_type_id;

    create_model_h_methods(Model, MODEL)

    bool operator==(const Tag& other) const
    {
        return id == other.id && note_id == other.note_id && tag_type_id == other.tag_type_id &&
            created_at == other.created_at && updated_at == other.updated_at;
    }
};

}
#undef Model
#undef MODEL
#undef COLS
#endif // TAG_H
