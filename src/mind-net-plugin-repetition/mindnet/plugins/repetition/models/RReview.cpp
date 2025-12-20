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

#include "mindnet/plugins/repetition/models/RReview.hpp"

namespace mindnet::plugins::repetition::models
{
    create_model_cpp_methods(RReview)
    string RReview::validate()
    {
        using columns::RReviewColumns;

        validator_chain_vector list{
            [this] { return test_between(grade, 0, 5, RReviewColumns::GRADE); },
            [this] { return test_true(started_at<ended_at, "started_at must be less than ended_at"); },

            [this] { return test_at_least(latency_ms, 0, RReviewColumns::LATENCY_MS); },
            [this] { return test_eq(latency_ms, ended_at - started_at, RReviewColumns::LATENCY_MS); },
            [this] { return test_at_least(answer_change_count, 0, RReviewColumns::ANSWER_CHANGE_COUNT); }
        };
        return util::ValidatorChain::run(list);
    }
}