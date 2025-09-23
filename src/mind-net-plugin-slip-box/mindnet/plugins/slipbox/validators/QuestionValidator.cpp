//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/QuestionValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/slipbox/models/Question.h"
#include "../../../../../../include/mind-net-api/mindnet/api/Persistence.h"
#include "mindnet/plugins/slipbox/SlipBoxPersistenceMethods.h"

#define Model Question
#define MODEL QUESTION
#define model question

namespace mindnet::plugins::slipbox::validators
{
    using validators::QuestionValidator;
    using mindnet::api::OperationResult;using mindnet::essential::g_configuration;
    OperationResult QuestionValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult QuestionValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult QuestionValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult QuestionValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult QuestionValidator::validate_list_authorization(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        return ok_result;
    }







    OperationResult QuestionValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        auto note = slipbox::find_note (ctx, entity.note_id);;
        if (note.second.empty()) return {400, note.second};

        if (slipbox::has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Write))
        {
            return ok_result;
        }
        return {403, "You do not have permission to create a question for this note."};
    }

    OperationResult QuestionValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        auto note = slipbox::find_note (ctx, entity.note_id);;
        if (note.second.empty()) return {400, note.second};

        if (slipbox::has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Read))
        {
            return ok_result;
        }
        return {403, "You do not have permission to delete this question for this note."};
    }

    OperationResult QuestionValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                            const Model& new_entity) const
    {
        auto note = slipbox::find_note (ctx, old_entity.note_id);;
        if (note.second.empty()) return {400, note.second};

        if (slipbox::has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Write))
        {
            return ok_result;
        }
        return {403, "You do not have permission to update this question for this note."};
    }

    OperationResult QuestionValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        auto note = slipbox::find_note (ctx, entity.note_id);;
        if (note.second.empty()) return {400, note.second};

        if (slipbox::has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Delete))
        {
            return ok_result;
        }
        return {403, "You do not have permission to update this question for this note."};
    }

    OperationResult QuestionValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        mandatory_filter(note_id)
        auto note_id = std::stoi(filter.at("note_id"));

        auto note = slipbox::find_tag_type (ctx, note_id);;
        if (note.second.empty()) return {400, note.second};

        if (!slipbox::has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Read))
            return {
                403,
                std::string(
                    "You do not have permission to list questions for map with ID " + std::to_string(note.first.map_id)
                    + ".")
            };

        return ok_result;
    }

    string QuestionValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
