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

#include "hive/plugins/slipbox/models/Question.hpp"

namespace hive::plugins::slipbox::models
{
    QuestionAnswer::QuestionAnswer(const std::string& answer_to_be_parsed)
    {
        std::stringstream ss(answer_to_be_parsed);
        std::string item;

        while (std::getline(ss, item, *ANSWER_SEPARATOR))
        {
            bool is_correct = !item.empty() && item[0] != '-';
            if (!is_correct)
            {
                item = item.substr(1);
            }
            value.emplace_back(item, is_correct);
        }
    }

    create_model_cpp_methods(Question)

    string Question::validate()
    {
        using columns::QuestionColumns;

        validator_chain_vector list{
            [this] { return test_ne(note_id, 0, QuestionColumns::NOTE_ID); },
            [this] { return testt_between(question_text, 3, 256, QuestionColumns::QUESTION_TEXT); }
        };
        return util::ValidatorChain::run(list);
    }
}