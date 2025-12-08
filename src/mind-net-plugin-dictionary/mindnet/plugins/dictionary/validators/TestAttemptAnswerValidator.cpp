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

//

#include "mindnet/plugins/dictionary/validators/TestAttemptAnswerValidator.hpp"

#include "mindnet/plugins/dictionary/models/TestAttemptAnswer.hpp"
#include "mindnet/plugins/dictionary/models/TestAttempt.hpp"
#include "mindnet/plugins/dictionary/models/Test.hpp"
#include "mindnet/plugins/dictionary/SlipBoxPersistenceMethods.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/util/Utils.hpp"

#define Model TestAttemptAnswer
#define MODEL TEST_ATTEMPT_ANSWER
#define model test_attempt_answer

namespace mindnet::plugins::dictionary::validators
{
    using mindnet::api::OperationResult;

    static OperationResult ensure_access_to_attempt(const RequestContext& ctx, int test_attempt_id)
    {
        auto attempt = slipbox::find_test_attempt(ctx, test_attempt_id);
        if (!attempt.second.empty()) return {400, attempt.second};

        if (ctx.token.user_id != attempt.first.user_id)
            return {403, "You do not have permission to access this test attempt answers."};

        auto test = slipbox::find_test(ctx, attempt.first.test_id);
        if (!test.second.empty()) return {400, test.second};

        auto note = slipbox::find_note(ctx, test.first.under_note_id);
        if (!note.second.empty()) return {400, note.second};

        if (slipbox::has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Read))
            return ok_result;

        return {403, "You do not have permission to access this test attempt answers."};
    }

    OperationResult TestAttemptAnswerValidator::validate_create_authorization(
        const RequestContext& ctx,
        const Model& entity) const
    {
        return ensure_access_to_attempt(ctx, entity.test_attempt_id);
    }

    OperationResult TestAttemptAnswerValidator::validate_read_authorization(
        const RequestContext& ctx,
        const Model& entity) const
    {
        return ensure_access_to_attempt(ctx, entity.test_attempt_id);
    }

    OperationResult TestAttemptAnswerValidator::validate_update_authorization(
        const RequestContext& ctx,
        const Model& old_entity,
        const Model& new_entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult TestAttemptAnswerValidator::validate_delete_authorization(
        const RequestContext& ctx,
        const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult TestAttemptAnswerValidator::validate_list_authorization(
        const RequestContext& ctx,
        const string_map& filter) const
    {
        mandatory_filter(test_attempt_id)

        identification ta_id = std::stoll(filter.at("test_attempt_id"));
        auto test_attempt_result = find_test_attempt(ctx, ta_id);
        if (!test_attempt_result.second.empty())
        {
            return {500, "Loading test_attempt_failed: " + test_attempt_result.second};
        }
        if (test_attempt_result.first.user_id != ctx.token.user_id)
        {
            return status_403_forbidden;
        }
        return ensure_access_to_attempt(ctx, ta_id);
    }

    OperationResult TestAttemptAnswerValidator::validate_create_integrity(
        const RequestContext& ctx, const Model& entity) const
    {
        auto test_attempt = find_test_attempt(ctx, entity.test_attempt_id);
        if (!test_attempt.second.empty()) return {500, test_attempt.second};
        auto test = find_test(ctx, test_attempt.first.test_id);
        if (!test.second.empty()) return {500, test.second};
        if (test_attempt.first.user_id != ctx.token.user_id) return {
            400, "test_attempt.user_id must be id of your user"
        };
        auto question_ids = mindnet::util::Utils::split_with_commas(test_attempt.first.question_ids);
        bool found = false;
        std::string question_id_string = std::to_string(entity.question_id);
        for (const auto& question_id : question_ids)
        {
            if (question_id.empty()) continue;
            if (question_id == question_id_string)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            return {
                400,
                "Question with ID " + question_id_string + " does not belong to test_attempt with id " + std::to_string(
                    test_attempt.first.get_id()) + "."
            };
        }
        auto question = find_question(ctx, entity.question_id);
        if (!question.second.empty()) return {500, question.second};
        models::QuestionAnswer question_answer(question.first.answers);
        bool user_answer_found = false;
        for (const auto& e : question_answer.get_parsed())
        {
            auto& choice = e.first;
            auto& correct = e.second;
            if (entity.user_answer != choice) continue;
            user_answer_found = true;
            if (correct != entity.is_correct)
            {
                return {
                    400,
                    "The is_correct value must match the correct answers defined for question ID: " + question_id_string
                };
            }
        }
        if (!user_answer_found && entity.is_correct)
        {
            return {
                400,
                "The question.answers does not contain such answers: " + question_id_string + " " + entity.user_answer +
                ", but is_correct is true"
            };
        }
        orm::QueryParams query_params;
        query_params.add_filter("test_attempt_id", entity.test_attempt_id);
        query_params.add_filter("question_id", entity.question_id);
        auto list_answers = ctx.db->list(models::TEST_ATTEMPT_ANSWER_DEFINITION, ctx.token, query_params);
        if (list_answers.second.ko()) return {500, list_answers.second.error};
        if (!list_answers.first.empty()) return {
            400, "There is already such test_attempt with this test_attempt_id and question_id"
        };

        return ok_result;
    }

    OperationResult TestAttemptAnswerValidator::validate_read_integrity(const RequestContext& ctx,
                                                                        const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TestAttemptAnswerValidator::validate_update_integrity(
        const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult TestAttemptAnswerValidator::validate_delete_integrity(
        const RequestContext& ctx, const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult TestAttemptAnswerValidator::validate_list_integrity(const RequestContext& ctx,
                                                                        const string_map&) const
    {
        return ok_result;
    }

    string TestAttemptAnswerValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model