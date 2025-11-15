//
// Created by robertvokac on 9/7/25.
//

#ifndef MIND_NET_TESTUTILS_H
#define MIND_NET_TESTUTILS_H
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
#endif //MIND_NET_TESTUTILS_H
