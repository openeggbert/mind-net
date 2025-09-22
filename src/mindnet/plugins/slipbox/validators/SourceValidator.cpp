//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/SourceValidator.h"

#include "../../../../../include/mindnet/core/Global.h"
#include "mindnet/plugins/slipbox/models/Source.h"
#include "mindnet/api/Persistence.h"

#define Model Source
#define MODEL SOURCE
#define model source

namespace mindnet::plugins::slipbox::validators
{
    using validators::SourceValidator;
    using mindnet::OperationResult;using mindnet::core::g_configuration;

    OperationResult SourceValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        assert_editor()

        if (!has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to create a source for this map."};
        }

        return ok_result;
    }

    OperationResult SourceValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult SourceValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        auto map = find_model(map, entity.map_id)
        if (!map.second.empty()) return {400, map.second};

        if (!has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Read))
        {
            return {403, "You do not have permission to read this source."};
        }
        return ok_result;
    }

    OperationResult SourceValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult SourceValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                   const Model& new_entity) const
    {
        assert_editor()

        if (!has_right_for_map(ctx, new_entity.map_id, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to update a source for this map."};
        }

        return ok_result;
    }

    OperationResult SourceValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                               const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult SourceValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        assert_editor()

        if (!has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Delete))
        {
            return {403, "You do not have permission to delete this source."};
        }

        return ok_result;
    }

    OperationResult SourceValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult SourceValidator::validate_list_authorization(const RequestContext& ctx,
                                                                 const string_map& filter) const
    {
        mandatory_filter(map_id)
        auto map_id = std::stoi(filter.at("map_id"));

        if (!has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Read))
            return {
                403,
                std::string(
                    "You do not have permission to list sources for map with ID " + std::to_string(map_id) + ".")
            };

        return ok_result;
    }

    OperationResult SourceValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string SourceValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
