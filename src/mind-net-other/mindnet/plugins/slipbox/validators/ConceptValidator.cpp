//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/ConceptValidator.h"

#include "mindnet/core/Global.h"
#include "mindnet/plugins/slipbox/models/Concept.h"
#include "mindnet/api/Persistence.h"

#define Model Concept
#define MODEL CONCEPT
#define model concept

namespace mindnet::plugins::slipbox::validators
{
    using validators::ConceptValidator;
    using mindnet::api::OperationResult;using mindnet::core::g_configuration;
    OperationResult ConceptValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ConceptValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ConceptValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult ConceptValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ConceptValidator::validate_list_authorization(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        return ok_result;
    }







    OperationResult ConceptValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(ctx.role < mindnet::core::UserRole::Editor, 403, "You can not create concepts.")

        if (!has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to create a concept for this map."};
        }

        if (entity.note_id != 0)
        {
            auto note = find_model(note, entity.note_id)
            if (!note.second.empty()) return {400, note.second};
            if (note.first.map_id != entity.map_id)
            {
                return {400, "Map ID of concept must be the same as the Map ID of the note."};
            }
            if (note.first.title != entity.title)
            {
                return {400, "Title of concept must be the same as the Title of the note."};
            }
        }

        return ok_result;
    }

    OperationResult ConceptValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        auto map = find_model(map, entity.map_id)
        if (!map.second.empty()) return {400, map.second};

        if (!has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Read))
        {
            return {403, "You do not have permission to read this concept."};
        }
        return ok_result;
    }

    OperationResult ConceptValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                      const Model& new_entity) const
    {
        return_if(ctx.role < mindnet::core::UserRole::Editor, 403, "You can not update concepts.")

        if (!has_right_for_map(ctx, new_entity.map_id, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to update a concept for this map."};
        }

        if (new_entity.note_id != 0)
        {
            auto note = find_model(note, new_entity.note_id)
            if (!note.second.empty()) return {400, note.second};
            if (note.first.map_id != new_entity.map_id)
            {
                return {400, "Map ID of concept must be the same as the Map ID of the note."};
            }

            if (note.first.title != new_entity.title)
            {
                return {400, "Title of concept must be the same as the Title of the note."};
            }
        }
        return ok_result;
    }

    OperationResult ConceptValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(ctx.role < mindnet::core::UserRole::Editor, 403, "You can not delete concepts.")

        if (has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Delete))
        {
            return {403, "You do not have permission to delete this concept."};
        }
        return ok_result;

    }

    OperationResult ConceptValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        mandatory_filter(map_id)
        auto map_id = std::stoi(filter.at("map_id"));

        if (!has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Read))
            return {
                403,
                std::string(
                    "You do not have permission to list concepts for map with ID " + std::to_string(map_id) + ".")
            };

        return ok_result;
    }

    string ConceptValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
