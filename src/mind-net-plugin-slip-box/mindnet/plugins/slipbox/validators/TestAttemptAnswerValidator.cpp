// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/TestAttemptAnswerValidator.hpp"

#include "mindnet/plugins/slipbox/models/TestAttemptAnswer.hpp"
#include "mindnet/plugins/slipbox/models/TestAttempt.hpp"
#include "mindnet/plugins/slipbox/models/Test.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPersistenceMethods.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"

#define Model TestAttemptAnswer
#define MODEL TEST_ATTEMPT_ANSWER
#define model test_attempt_answer

namespace mindnet::plugins::slipbox::validators
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

        auto note = slipbox::find_note(ctx, test.first.note_id);
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

        i64 ta_id = std::stoll(filter.at("test_attempt_id"));
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

    OperationResult TestAttemptAnswerValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        auto test_attempt = find_test_attempt(ctx, entity.test_attempt_id);
        if (!test_attempt.second.empty()) return {500, test_attempt.second};
        auto test = find_test(ctx, test_attempt.first.test_id);
        if (!test.second.empty()) return {500, test.second};
        if (test_attempt.first.user_id != ctx.token.user_id) return {400, "test_attempt.user_id must be id of your user"};
        auto question_ids = util::Utils::split_with_commas(test_attempt.first.question_ids);
        bool found = false;
        std::string question_id_string = std::to_string(entity.question_id);
        for (const auto& question_id : question_ids)
        {
            if (question_id == question_id_string)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            return {400, "Question with ID " + question_id_string + " does not belong to test_attempt with id " + std::to_string(test_attempt.first.get_id()) + "."};
        }
        auto question = find_question(ctx, entity.question_id);
        if (!question.second.empty()) return {500, question.second};
        models::QuestionAnswer question_answer(question.first.answers);
        bool user_answer_found = false;
        for (const auto& e: question_answer.get_parsed())
        {
            auto& choice = e.first;
            auto& correct = e.second;
            if (entity.user_answer != choice) continue;
            user_answer_found = true;
            if (correct != entity.is_correct)
            {
                return {
                    400,
                    "The is_correct value must match the correct answers defined for question ID: " + question_id_string};
            }

        }
        if (!user_answer_found)
        {
            return {
                400,
                "The question.answers does not contain such answers: " + question_id_string + " " + entity.user_answer + "."};
        }




        return ok_result;
    }

    OperationResult TestAttemptAnswerValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TestAttemptAnswerValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult TestAttemptAnswerValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult TestAttemptAnswerValidator::validate_list_integrity(const RequestContext& ctx, const string_map&) const
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
