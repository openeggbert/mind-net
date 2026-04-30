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

#include "hive/util/TestUtils.hpp"

#include <algorithm>

namespace hive::util
{
    test_result test_ok()
    {
        return {};
    }

    test_result test_ko(const string& error)
    {
        return std::unexpected(error);
    }

    test_result test_true(bool condition, const string& error_if_condition_not_met)
    {
        if (!condition) return std::unexpected(error_if_condition_not_met);
        return {};
    }

    test_result test_false(bool condition, const string& error_if_condition_not_met)
    {
        return test_true(!condition, error_if_condition_not_met);
    }

    test_result test_eq(int64_t number, int64_t eq_to, const char* field)
    {
        string field_string = field;
        return test_true(number == eq_to,
                         "Expected " + field_string + " to be " + std::to_string(eq_to) + " but was " + std::to_string(
                             number));
    }

    test_result test_ne(int64_t number, int64_t eq_to, const char* field)
    {
        string field_string = field;
        return test_true(number != eq_to,
                         "Expected " + field_string + " not to be " + std::to_string(eq_to) + " but was " +
                         std::to_string(
                             number));
    }

    test_result test_not_null(int64_t number, const char* field)
    {
        return test_ne(number, 0, field);
    }

    test_result test_at_least(int64_t number, int64_t min_value, const char* field)
    {
        string field_string = field;
        return test_true(number >= min_value,
                         "Expected " + field_string + " to be at least " + std::to_string(min_value) + " but was " +
                         std::to_string(number));
    }

    test_result test_at_most(int64_t number, int64_t max_value, const char* field)
    {
        string field_string = field;
        return test_true(number <= max_value,
                         "Expected " + field_string + " to be at most " + std::to_string(max_value) + " but was " +
                         std::to_string(number));
    }

    test_result test_between(int64_t number, int64_t min_value, int64_t max_value, const char* field)
    {
        string field_string = field;
        return test_true(number >= min_value && number <= max_value,
                         "Expected " + field_string + " to be between " + std::to_string(min_value) + " and " +
                         std::to_string(max_value) + " but was " + std::to_string(number));
    }

    //
    test_result testt_at_least(const string& text, int64_t min_value, const char* field)
    {
        return test_at_least(text.size(), min_value, field);
    }

    test_result testt_at_most(const string& text, int64_t max_value, const char* field)
    {
        return test_at_most(text.size(), max_value, field);
    }

    test_result testt_between(const string& text, int64_t min_value, int64_t max_value, const char* field)
    {
        const string field_string = field;
        return test_between(text.size(), min_value, max_value, field);
    }
    static bool has_no_leading_or_trailing_whitespace(const std::string& s)
    {
        if (s.empty())
            return true;

        return !std::isspace(static_cast<unsigned char>(s.front())) &&
               !std::isspace(static_cast<unsigned char>(s.back()));
    }

    test_result testt_trimmed(const string& text, const char* field)
    {
        using namespace std::string_literals;

        string field_string = field;
        return test_true(has_no_leading_or_trailing_whitespace(text),
                 "Expected "s + field_string + " to be trimmed, but \"" + text + "\" starts or ends with a whitespace.");

    }

    //
    test_result testt_not_empty(const string& text, const char* field)
    {
        const string field_string = field;
        return test_true(!text.empty(), field_string + " must not be empty");
    }

    test_result testt_is_alpha_or_digit(const string& text, const char* field)
    {
        string field_string = field;
        return test_true(std::all_of(text.begin(), text.end(), [](char c) { return isalpha(c) || isdigit(c); }),
                         field_string + " must contain only letters or digits");
    }
}