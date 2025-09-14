//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/zettelkasten/validators/QuestionValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/zettelkasten/models/Question.h"
#include "mindnet/api/Persistence.h"

#define Model Question
#define MODEL QUESTION
#define model question

namespace mindnet::plugins::zettelkasten::validators
{
    using validators::QuestionValidator;
    using mindnet::OperationResult;

    OperationResult QuestionValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {
        auto note = find_model(note, entity.note_id);
        if (note.second.empty()) return {400, note.second};

        if (has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Write))
        {
            return ok_result;
        }
        return {403, "You do not have permission to create a question for this note."};
    }

    OperationResult QuestionValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
        auto note = find_model(note, entity.note_id);
        if (note.second.empty()) return {400, note.second};

        if (has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Read))
        {
            return ok_result;
        }
        return {403, "You do not have permission to delete this question for this note."};
    }

    OperationResult QuestionValidator::validate_update(const RequestContext& ctx, const Model& old_entity,
                                                            const Model& new_entity) const
    {
        auto note = find_model(note, old_entity.note_id);
        if (note.second.empty()) return {400, note.second};

        if (has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Write))
        {
            return ok_result;
        }
        return {403, "You do not have permission to update this question for this note."};
    }

    OperationResult QuestionValidator::validate_delete(const RequestContext& ctx, const Model& entity) const
    {
        auto note = find_model(note, entity.note_id);
        if (note.second.empty()) return {400, note.second};

        if (has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Delete))
        {
            return ok_result;
        }
        return {403, "You do not have permission to update this question for this note."};
    }

    OperationResult QuestionValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {
        mandatory_filter(note_id)
        auto note_id = std::stoi(filter.at("note_id"));

        auto note = find_model(tag_type, note_id);
        if (note.second.empty()) return {400, note.second};

        if (!has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Read))
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
