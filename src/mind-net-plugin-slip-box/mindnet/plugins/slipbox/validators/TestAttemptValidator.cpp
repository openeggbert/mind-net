// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/TestAttemptValidator.hpp"

#include "mindnet/plugins/slipbox/models/TestAttempt.hpp"
#include "mindnet/plugins/slipbox/models/Test.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPersistenceMethods.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/api/Persistence.hpp"

#define Model TestAttempt
#define MODEL TEST_ATTEMPT
#define model test_attempt

namespace mindnet::plugins::slipbox::validators
{
    using mindnet::api::OperationResult;

    OperationResult TestAttemptValidator::validate_create_authorization(const RequestContext& ctx,
                                                                        const Model& entity) const
    {
        auto test = slipbox::find_test(ctx, entity.test_id);
        if (!test.second.empty()) return {400, test.second};

        auto note = slipbox::find_note(ctx, test.first.note_id);
        if (!note.second.empty()) return {400, note.second};

        // user must have read access to take test
        if (slipbox::has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Read))
            return ok_result;

        return {403, "You do not have permission to start a test attempt."};
    }

    OperationResult TestAttemptValidator::validate_read_authorization(const RequestContext& ctx,
                                                                      const Model& entity) const
    {
        // auto test = slipbox::find_test(ctx, entity.test_id);
        // if (!test.second.empty()) return {400, test.second};
        //
        // auto note = slipbox::find_note(ctx, test.first.note_id);
        // if (!note.second.empty()) return {400, note.second};
        //
        // if (slipbox::has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Read))
        //     return ok_result;
        if (entity.user_id == ctx.token.user_id) return ok_result;

        return {403, "You do not have permission to read this test attempt."};
    }

    OperationResult TestAttemptValidator::validate_update_authorization(const RequestContext& ctx,
                                                                        const Model& old_entity,
                                                                        const Model& new_entity) const
    {
        if (new_entity.user_id != ctx.token.user_id)
            return {403, "You do not have permission to update this test attempt."};

        return ok_result;
    }

    OperationResult TestAttemptValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                        const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult TestAttemptValidator::validate_list_authorization(const RequestContext& ctx,
                                                                      const string_map& filter) const
    {

        mandatory_filter(test_id)
        mandatory_filter(user_id)

        i64 test_id = std::stoi(filter.at("test_id"));
        i64 user_id = std::stol(filter.at("user_id"));

        auto test = slipbox::find_test(ctx, test_id);
        if (!test.second.empty()) return {400, test.second};

        auto note = slipbox::find_note(ctx, test.first.note_id);
        if (!note.second.empty()) return {400, note.second};

        if (!slipbox::has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Read))
            return {403, "You do not have permission to list test attempts."};
        if (user_id != ctx.token.user_id)
        {
            return {403, "You can list only your own test attempts."};
        }

        return ok_result;
    }


    OperationResult TestAttemptValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        if (entity.user_id != ctx.token.user_id) return {400, "user_id must be id of your user."};
        if (entity.finished_at != 0) return {400, "finished_at must be 0 during creation."};
        if (entity.score_times_100 != 0) return {400, "score_times_100 must be 0 during creation."};
        if (!entity.question_ids.empty()) return {400, "question_ids must be empty during creation."};
        return ok_result;
    }

    OperationResult TestAttemptValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TestAttemptValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        bool finished_at_was_default = old_entity.finished_at == 0;
        bool score_times_100_was_default = old_entity.score_times_100 == 0;
        bool question_ids_was_default = old_entity.question_ids == "";
        if (!finished_at_was_default && old_entity.finished_at != new_entity.finished_at) return {400, "finished_at cannot be changed, if already set"};
        if (!score_times_100_was_default && old_entity.score_times_100 != new_entity.score_times_100) return {400, "score_times_100 cannot be changed, if already set"};
        if (!question_ids_was_default && old_entity.question_ids != new_entity.question_ids) return {400, "question_ids cannot be changed, if already set"};

        return ok_result;
    }

    OperationResult TestAttemptValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TestAttemptValidator::validate_list_integrity(const RequestContext&, const string_map&) const
    {
        return ok_result;
    }

    string TestAttemptValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
