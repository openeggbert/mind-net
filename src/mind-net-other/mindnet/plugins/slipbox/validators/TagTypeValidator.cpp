//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/TagTypeValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/slipbox/models/TagType.h"
#include "mindnet/api/Persistence.h"

#define Model TagType
#define MODEL TAG_TYPE
#define model tag_type

namespace mindnet::plugins::slipbox::validators
{
    using validators::TagTypeValidator;
    using mindnet::api::OperationResult;using mindnet::core::g_configuration;
    OperationResult TagTypeValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TagTypeValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TagTypeValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult TagTypeValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TagTypeValidator::validate_list_authorization(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        return ok_result;
    }







    OperationResult TagTypeValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(ctx.role < mindnet::core::UserRole::Editor,
                  403, "User does not have permission to create a property.")

        if (!has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to create a tag type for this map."};
        }
        return ok_result;

    }

    OperationResult TagTypeValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        auto map = find_model(map, entity.map_id)
        if (map.second.empty()) return {400, map.second};

        if (!has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Read))
        {
            return {403, "You do not have permission to read this tag type."};
        }
        return ok_result;
    }

    OperationResult TagTypeValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                           const Model& new_entity) const
    {
        return {405, "Update of tag_type is forbidden."};
    }

    OperationResult TagTypeValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        if (has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Delete))
        {
            return ok_result;
        }
        return {403, "You do not have permission to delete this tag_type."};
    }

    OperationResult TagTypeValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        mandatory_filter(map_id)
        auto map_id = std::stoi(filter.at("map_id"));

        if (!has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Read))
            return {
                403,
                std::string(
                    "You do not have permission to list tag types for map with ID " + std::to_string(map_id) + ".")
            };

        return ok_result;
    }

    string TagTypeValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
