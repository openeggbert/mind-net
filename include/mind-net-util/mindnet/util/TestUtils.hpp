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

#include <expected>
#include <functional>
#include <string>
#include <vector>

namespace mindnet::util
{
    using std::string;
    typedef std::expected<void, string> test_result;
    typedef std::vector<std::function<test_result()>> validator_chain_vector;

    struct ValidatorChain
    {
        template <typename Preds>
        static string run(const Preds& preds)
        {
            for (auto& pred : preds)
            {
                if (auto res = pred(); !res) return res.error();
            }
            return "";
        }
    };

    test_result test_ok();
    test_result test_ko(const string& error);
    test_result test_true(bool condition, const string& error_if_condition_not_met);
    test_result test_false(bool condition, const string& error_if_condition_not_met);
    test_result test_eq(int64_t number, int64_t eq_to, const char* field);
    test_result test_ne(int64_t number, int64_t eq_to, const char* field);
    test_result test_not_null(int64_t number, const char* field);
    test_result test_at_least(int64_t number, int64_t min_value, const char* field);
    test_result test_at_most(int64_t number, int64_t max_value, const char* field);
    test_result test_between(int64_t number, int64_t min_value, int64_t max_value, const char* field);
    //
    test_result testt_at_least(string& text, int64_t min_value, const char* field);
    test_result testt_at_most(string& text, int64_t max_value, const char* field);
    test_result testt_between(string& text, int64_t min_value, int64_t max_value, const char* field);
    //
    test_result testt_not_empty(string& text, const char* field);
    test_result testt_is_alpha_or_digit(string& text, const char* field);
    //
}
