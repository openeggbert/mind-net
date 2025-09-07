//
// Created by robertvokac on 9/7/25.
//

#ifndef MIND_NET_TESTUTILS_H
#define MIND_NET_TESTUTILS_H
#include <expected>
#include <functional>
#include <string>
#include <vector>

namespace mindnet
{
    using std::string;
    typedef std::expected<void, string> test_result;
    typedef std::vector<std::function<test_result()>> validator_chain_vector;

    struct ValidatorChain {
        template<typename Preds>
        static string run(const Preds& preds) {
            for (auto& pred : preds) {
                if (auto res = pred(); !res) return res.error();
            }
            return "";
        }
    };
    test_result test_ok();
    test_result test_ko(string error);
    test_result test_true(bool condition, string error_if_condition_not_met);
    test_result test_false(bool condition, string error_if_condition_not_met);
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
