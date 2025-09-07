//
// Created by robertvokac on 9/7/25.
//

#ifndef MIND_NET_TESTUTILS_H
#define MIND_NET_TESTUTILS_H
#include <expected>
#include <string>
#define CHECK(res) do { auto _r = (res); if (!_r) return _r.error(); } while(0);

namespace mindnet
{
    using std::string;
    typedef std::expected<void, string> test_result;

    test_result test_true(bool condition, string error_if_condition_not_met);
    test_result test_eq(int number, int eq_to, string field);
    test_result test_ne(int number, int eq_to, string field);
    test_result test_at_least(int number, int min_value, string field);
    test_result test_at_most(int number, int max_value, string field);
    test_result test_between(int number, int min_value, int max_value, string field);
    //
    test_result testt_at_least(string text, int min_value, string field);
    test_result testt_at_most(string text, int max_value, string field);
    test_result testt_between(string text, int min_value, int max_value, string field);
    //
    test_result testt_not_empty(string text, string field);
    test_result testt_is_alpha_or_digit(string text, string field);
    //
}
#endif //MIND_NET_TESTUTILS_H
