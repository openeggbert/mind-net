//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/TagValidator.h"

#include "../../../../../include/mindnet/core/Global.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/plugins/slipbox/models/Tag.h"
#include "mindnet/api/Persistence.h"

#define Model Tag
#define MODEL TAG_H
#define model tag

namespace mindnet::plugins::slipbox::validators
{
    using validators::TagValidator;
    using mindnet::api::OperationResult;using mindnet::core::g_configuration;
    OperationResult TagValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TagValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TagValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult TagValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult TagValidator::validate_list_authorization(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        return ok_result;
    }







    OperationResult TagValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        auto tag_type = find_model(tag_type, entity.tag_type_id);
        if (tag_type.second.empty()) return {400, tag_type.second};

        if (has_right_for_map(ctx, tag_type.first.map_id, plugins::core::enums::SingleRight::Write))
        {
            return ok_result;
        }
        return {403, "You do not have permission to create a tag for this map."};
    }

    OperationResult TagValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        auto tag_type = find_model(tag_type, entity.tag_type_id);
        if (tag_type.second.empty()) return {400, tag_type.second};

        auto map = find_model(map, tag_type.first.map_id)
        if (map.second.empty()) return {400, map.second};

        if (has_right_for_map(ctx, tag_type.first.map_id, plugins::core::enums::SingleRight::Read))
        {
            return ok_result;
        }
        return {403, "You do not have permission to read this tag."};
    }

    OperationResult TagValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                       const Model& new_entity) const
    {
        return {405, "Update of tag_type is forbidden."};
    }

    OperationResult TagValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        auto tag_type = find_model(tag_type, entity.tag_type_id);
        if (tag_type.second.empty()) return {400, tag_type.second};

        if (has_right_for_map(ctx, tag_type.first.map_id, plugins::core::enums::SingleRight::Delete))
        {
            return ok_result;
        }
        return {403, "You do not have permission to delete this tag_type."};
    }

    OperationResult TagValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        mandatory_filter(tag_type_id)
        auto tag_type_id = std::stoi(filter.at("tag_type_id"));

        auto tag_type = find_model(tag_type, tag_type_id);
        if (tag_type.second.empty()) return {400, tag_type.second};

        if (!has_right_for_map(ctx, tag_type.first.map_id, plugins::core::enums::SingleRight::Read))
            return {
                403,
                std::string(
                    "You do not have permission to list tag types for map with ID " + std::to_string(
                        tag_type.first.map_id) + ".")
            };

        return ok_result;
    }

    string TagValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
