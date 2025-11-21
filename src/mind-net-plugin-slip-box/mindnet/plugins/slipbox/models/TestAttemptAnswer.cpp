// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/slipbox/models/TestAttemptAnswer.hpp"

namespace mindnet::plugins::slipbox::models
{
    entity_fields TestAttemptAnswer::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));

        result.push_back(test_attempt_id);
        result.push_back(question_id);
        result.push_back(user_answer);
        result.push_back(is_correct);

        return result;
    }

    void TestAttemptAnswer::from_values(const entity_fields& values)
    {
        int i = 0;
        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();

        test_attempt_id = number();
        question_id = number();
        user_answer = text();
        is_correct = boolean();
    }

    string TestAttemptAnswer::validate()
    {
        using columns::TestAttemptAnswerColumns;

        validator_chain_vector list{
            [this] { return test_ne(test_attempt_id, 0, TestAttemptAnswerColumns::TEST_ATTEMPT_ID); },
            [this] { return test_ne(question_id, 0, TestAttemptAnswerColumns::QUESTION_ID); },
            [this] { return testt_not_empty(user_answer, TestAttemptAnswerColumns::USER_ANSWER); }
        };

        return util::ValidatorChain::run(list);
    }
}
