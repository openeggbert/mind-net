//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/TermValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/slipbox/models/Term.hpp"
#include "mindnet/api/Persistence.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPersistenceMethods.hpp"

#define Model Term
#define MODEL TERM
#define model term

namespace mindnet::plugins::slipbox::validators
{
    using validators::TermValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult TermValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return_if(ctx.role < mindnet::essential::UserRole::Editor, 403, "You can not create Terms.")

        if (!slipbox::has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to create a Term for this map."};
        }

        return ok_result;
    }

    OperationResult TermValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        auto map = slipbox::find_map(ctx, entity.map_id);
        if (!map.second.empty()) return {400, map.second};

        if (!slipbox::has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Read))
        {
            return {403, "You do not have permission to read this Term."};
        }
        return ok_result;
    }

    OperationResult TermValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                 const Model& new_entity) const
    {
        return_if(ctx.role < mindnet::essential::UserRole::Editor, 403, "You can not update Terms.")

        if (!slipbox::has_right_for_map(ctx, new_entity.map_id, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to update a Term for this map."};
        }

        return ok_result;
    }

    OperationResult TermValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return_if(ctx.role < mindnet::essential::UserRole::Editor, 403, "You can not delete Terms.")

        if (slipbox::has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Delete))
        {
            return {403, "You do not have permission to delete this Term."};
        }

        return ok_result;
    }

    OperationResult TermValidator::validate_list_authorization(const RequestContext& ctx,
                                                               const string_map& filter) const
    {
        mandatory_filter(map_id)
        auto map_id = std::stoi(filter.at("map_id"));

        if (!slipbox::has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Read))
            return {
                403,
                std::string(
                    "You do not have permission to list Terms for map with ID " + std::to_string(map_id) + ".")
            };

        return ok_result;
    }


    OperationResult TermValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        if (entity.note_id != 0)
        {
            auto note = slipbox::find_note(ctx, entity.note_id);
            if (!note.second.empty()) return {400, note.second};
            if (note.first.map_id != entity.map_id)
            {
                return {400, "Map ID of Term must be the same as the Map ID of the note."};
            }
            if (note.first.title != entity.title)
            {
                return {400, "Title of Term must be the same as the Title of the note."};
            }
        }

        return ok_result;
    }

    OperationResult TermValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TermValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                             const Model& new_entity) const
    {
        if (new_entity.note_id != 0)
        {
            auto note = slipbox::find_note(ctx, new_entity.note_id);
            if (!note.second.empty()) return {400, note.second};
            if (note.first.map_id != new_entity.map_id)
            {
                return {400, "Map ID of Term must be the same as the Map ID of the note."};
            }

            if (note.first.title != new_entity.title)
            {
                return {400, "Title of Term must be the same as the Title of the note."};
            }
        }
        return ok_result;
    }

    OperationResult TermValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TermValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string TermValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
