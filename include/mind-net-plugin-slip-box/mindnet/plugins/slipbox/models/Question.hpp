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
#include <sstream>

#include "mindnet/model/BaseModel.hpp"
// ***** MACROS : START *****
#define Model Question
#define MODEL QUESTION
#define COLS columns::QuestionColumns
#include "../columns/QuestionColumns.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPlugin.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::slipbox::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();
    static constexpr char ANSWER_SEPARATOR[] = "::::";

    inline const def QUESTION_DEFINITION =
        def(COLS::MODEL_NAME, SLIP_BOX_PLUGIN_NAME)
        .set_all_rest_operations()
        .set_group("Slip Box", 100)
        .set_title_column(COLS::QUESTION_TEXT)
        .set_columns(
            {
                coldef(COLS::NOTE_ID, MANDATORY | FOREIGN_KEY | READONLY),
                coldef(COLS::QUESTION_TEXT, TEXTAREA | MANDATORY),
                coldef(COLS::ANSWERS, TEXT),
            })
    .add_custom_list_action("test_attempt", "List test attempt answers", {"question_id", "{id}"})
    ;
    // *** Definition of model ends ***

    // answers
    // [
    //   { "text": "std::vector", "is_correct": true },
    //   { "text": "std::map", "is_correct": false },
    //   { "text": "std::set", "is_correct": false }
    // ]
    struct QuestionAnswer
    {
        
    private:
        std::vector<std::pair<std::string, bool>> value;

    public:
        explicit QuestionAnswer(const std::string& answer_to_be_parsed);

        const std::vector<std::pair<std::string, bool>>& get_parsed() const
        {
            return value;
        }
    };
    struct Model : mindnet::model::BaseModel
    {
        int note_id{};
        string question_text;
        string answers;

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                note_id == other.note_id &&
                question_text == other.question_text &&
                answers == other.answers;
        }
    };
}

#undef Model
#undef MODEL
#undef COLS
