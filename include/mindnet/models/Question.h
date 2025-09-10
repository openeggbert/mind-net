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
#ifndef QUESTION_H
#define QUESTION_H


#include <string>

#include "misc/BaseModel.h"
// ***** MACROS : START *****
#define Model Question
#define MODEL QUESTION
#define COLS columns::QuestionColumns
#include "columns/QuestionColumns.h"
// ***** MACROS : END *****


namespace mindnet::models
{
    using misc::def;
    using misc::coldef;
    using_flags();

    inline def QUESTION_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_group("Tests", 200)
        .set_columns(
            {
                coldef(COLS::NOTE_ID, MANDATORY).set_foreign_key("note"),
                coldef(COLS::QUESTION_TEXT, TEXTAREA | MANDATORY),
                coldef(COLS::ANSWERS_JSON, TEXT),
            });
    // *** Definition of model ends ***

    struct Model : misc::BaseModel
    {
        int note_id{};
        string question_text;
        string answers_json;


        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                note_id == other.note_id &&
                question_text == other.question_text &&
                answers_json == other.answers_json;
        }
    };
}

#undef Model
#undef MODEL
#undef COLS
#endif // QUESTION_H
