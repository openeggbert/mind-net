/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

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
        identification note_id{};
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
