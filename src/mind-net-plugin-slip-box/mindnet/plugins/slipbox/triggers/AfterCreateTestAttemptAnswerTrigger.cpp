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

#include "mindnet/plugins/slipbox/triggers/AfterCreateTestAttemptAnswerTrigger.hpp"

#include "mindnet/api/AccessTokenContext.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/slipbox/models/Test.hpp"
#include "mindnet/plugins/slipbox/models/TestAttempt.hpp"
#include "mindnet/plugins/slipbox/models/TestAttemptAnswer.hpp"
#include "mindnet/plugins/slipbox/validators/TestAttemptAnswerValidator.hpp"
#include "mindnet/util/Utils.hpp"

namespace mindnet::plugins::slipbox::triggers
{
    using_loggers()

    AfterCreateTestAttemptAnswerTrigger::AfterCreateTestAttemptAnswerTrigger()
        : Trigger(
            "AfterCreateTestAttemptAnswerTrigger",
            "Updates test_attempt, if needed.",
            1000,
            {essential::Crudl::Create},
            api::TriggerPhase::After,
            "test_attempt_answer"
        )
    {
    }

void AfterCreateTestAttemptAnswerTrigger::run_before_or_after(
        mindnet::essential::Crudl operation,
        int stack_depth,
        api::OperationResult& validation_result,
        api::OperationResult& action_result,
        const mindnet::model::ModelDefinition def,
        identification user_id,
        identification id,
        entity_fields& fields,
        entity_fields& old_fields,
        const orm::QueryParams query_params)
    {
        if (action_result.ko())
        {
            return;
        }
        models::TestAttemptAnswer new_test_attempt_answer;
        new_test_attempt_answer.from_values(fields);

        api::AccessTokenContext token = user_id == 0
                                            ? api::AccessTokenContext(user_id, "system", 403)
                                            : api::AccessTokenContext(user_id, "", 200);

        auto read_test_attempt = run_read(models::TEST_ATTEMPT_DEFINITION, token, new_test_attempt_answer.test_attempt_id, stack_depth);
        if (read_test_attempt.second.ko())
        {
            err << read_test_attempt.second.error << commit;
            return;
        }
        models::TestAttempt test_attempt;
        test_attempt.from_values(read_test_attempt.first);

        if (test_attempt.finished_at != 0)
        {
            warn << "Test attempt is already finished! " << std::to_string(test_attempt.get_id()) << commit;
            return;
        }

        ////
        auto read_test = run_read(models::TEST_DEFINITION, token, test_attempt.test_id, stack_depth);
        if (read_test.second.ko())
        {
            err << read_test.second.error << commit;
            return;
        }
        models::Test test;
        test.from_values(read_test.first);
        ////

        int question_count = util::Utils::split_with_commas(test_attempt.question_ids).size();

        unixtime test_attempt_started_at = test_attempt.started_at;
        i64 second_limit = test.time_limit_in_seconds;
        unixtime test_attempt_should_be_finished_until = test_attempt_started_at + second_limit * 1000;
        auto now = util::Utils::current_unix_timestamp_ms();
        bool answered_after_limit = now > test_attempt_should_be_finished_until;

        orm::QueryParams qp;
        qp.add_filter("test_attempt_id", test_attempt.get_id());
        auto list_result = run_list(def, token, qp, stack_depth);
        if (list_result.second.ko())
        {
            err << list_result.second.error << commit;
            return;
        }
        if (list_result.first.size() < question_count && !answered_after_limit)
        {
            debug << "Not all questions in test were answered." << commit;
            return;
        }
        double count_successes{0};
        for (auto& e: list_result.first)
        {
            models::TestAttemptAnswer test_attempt_answer;
            test_attempt_answer.from_values(e);
            bool before_limit = test_attempt_answer.get_created_at() < test_attempt_should_be_finished_until;

            if (test_attempt_answer.is_correct && before_limit) count_successes++;
        }
        double score = count_successes / question_count;
        test_attempt.finished_at = mindnet::util::Utils::current_unix_timestamp_ms();
        test_attempt.score_times_100 = cast64(score * 100.0);
        auto v = test_attempt.to_values();
        auto update_test_attempt = run_update(models::TEST_ATTEMPT_DEFINITION, token, test_attempt.get_id(), v, stack_depth );
        if (update_test_attempt.ko())
        {
            err << "Updating test_attempt failed: " << update_test_attempt.error << commit;
        }

}
}
