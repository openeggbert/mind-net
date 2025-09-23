//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/PropertyValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/slipbox/models/Property.h"
#include "../../../../../../include/mind-net-api/mindnet/api/Persistence.h"

#define Model Property
#define MODEL PROPERTY
#define model property

namespace mindnet::plugins::slipbox::validators
{
    using validators::PropertyValidator;
    using mindnet::api::OperationResult;using mindnet::essential::g_configuration;
    OperationResult PropertyValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult PropertyValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult PropertyValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult PropertyValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult PropertyValidator::validate_list_authorization(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        return ok_result;
    }







    OperationResult PropertyValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(ctx.role < mindnet::essential::UserRole::Editor,
                  403, "User does not have permission to create a property.")

        if (has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Write))
        {
            return ok_result;
        }
        return {403, "You do not have permission to create a property for this map."};
    }

    OperationResult PropertyValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        auto map = find_model(map, entity.map_id)
        if (map.second.empty()) return {400, map.second};

        if (has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Read))
        {
            return ok_result;
        }
        return {403, "You do not have permission to read this property."};
    }

    OperationResult PropertyValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                            const Model& new_entity) const
    {
        if (!has_right_for_map(ctx, old_entity.map_id, plugins::core::enums::SingleRight::Write))
            return {403, "You do not have permission to update this property."};

        return ok_result;
    }

    OperationResult PropertyValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        if (has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Delete))
        {
            return ok_result;
        }
        return {403, "You do not have permission to delete this property."};

        return ok_result;
    }

    OperationResult PropertyValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        mandatory_filter(note_id)
        auto note_id = std::stoi(filter.at("note_id"));
        auto note = find_model(note, note_id);
        if (!note.second.empty()) return {400, note.second};

        int map_id = note.first.map_id;

        if (!has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Read))
            return {
                403,
                std::string(
                    "You do not have permission to list properties for note with ID " + std::to_string(note_id) + ".")
            };

        return ok_result;
    }

    string PropertyValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
