//
// Created by robertvokac on 9/7/25.
//
#include "mindnet/TestUtils.h"

#include <algorithm>

namespace mindnet
{
    test_result test_ok()
    {
        return {};
    }

    test_result test_ko(string error)
    {
        return std::unexpected(error);
    }

    test_result test_true(bool condition, string error_if_condition_not_met)
    {
        if (!condition) return std::unexpected(error_if_condition_not_met);
        return {};
    }

    test_result test_false(bool condition, string error_if_condition_not_met)
    {
        return test_true(!condition, error_if_condition_not_met);
    }

    test_result test_eq(int number, int eq_to, string field)
    {
        return test_true(number == eq_to,
                         "Expected " + field + " to be " + std::to_string(eq_to) + " but was " + std::to_string(
                             number));
    }

    test_result test_ne(int number, int eq_to, string field)
    {
        return test_true(number != eq_to,
                         "Expected " + field + " to not be " + std::to_string(eq_to) + " but was " + std::to_string(
                             number));
    }

    test_result test_at_least(int number, int min_value, string field)
    {
        return test_true(number >= min_value,
                         "Expected " + field + " to be at least " + std::to_string(min_value) + " but was " +
                         std::to_string(number));
    }

    test_result test_at_most(int number, int max_value, string field)
    {
        return test_true(number <= max_value,
                         "Expected " + field + " to be at most " + std::to_string(max_value) + " but was " +
                         std::to_string(number));
    }

    test_result test_between(int number, int min_value, int max_value, string field)
    {
        return test_true(number >= min_value && number <= max_value,
                         "Expected " + field + " to be between " + std::to_string(min_value) + " and " +
                         std::to_string(max_value) + " but was " + std::to_string(number));
    }

    //
    test_result testt_at_least(string text, int min_value, string field)
    {
        return test_at_least(text.size(), min_value, field);
    }

    test_result testt_at_most(string text, int max_value, string field)
    {
        return test_at_most(text.size(), max_value, field);
    }

    test_result testt_between(string text, int min_value, int max_value, string field)
    {
        return test_between(text.size(), min_value, max_value, field);
    }

    //
    test_result testt_not_empty(string text, string field)
    {
        return test_true(!text.empty(), field + " must not be empty");
    }

    test_result testt_is_alpha_or_digit(string text, string field)
    {
        return test_true(std::all_of(text.begin(), text.end(), [](char c) { return isalpha(c) || isdigit(c); }),
                         field + " must contain only letters or digits");
    }
}
