//
// Created by robertvokac on 9/7/25.
//
#include "mindnet/util/TestUtils.h"

#include <algorithm>

namespace mindnet::util
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
                         "Expected " + field_string + " not to be " + std::to_string(eq_to) + " but was " + std::to_string(
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
    test_result testt_at_least(string& text, int64_t min_value, const char* field)
    {
        return test_at_least(text.size(), min_value, field);
    }

    test_result testt_at_most(string& text, int64_t max_value, const char* field)
    {
        return test_at_most(text.size(), max_value, field);
    }

    test_result testt_between(string& text, int64_t min_value, int64_t max_value, const char* field)
    {
        const string field_string = field;
        return test_between(text.size(), min_value, max_value, field);
    }

    //
    test_result testt_not_empty(string& text, const char* field)
    {
        const string field_string = field;
        return test_true(!text.empty(), field_string + " must not be empty");
    }

    test_result testt_is_alpha_or_digit(string& text, const char* field)
    {
        string field_string = field;
        return test_true(std::all_of(text.begin(), text.end(), [](char c) { return isalpha(c) || isdigit(c); }),
                         field_string + " must contain only letters or digits");
    }
}
