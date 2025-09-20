//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/SourceValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/slipbox/models/Source.h"
#include "mindnet/api/Persistence.h"

#define Model Source
#define MODEL SOURCE
#define model source

namespace mindnet::plugins::slipbox::validators
{
    using validators::SourceValidator;
    using mindnet::OperationResult;

    OperationResult SourceValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {
        bool everyone_can_do_anything = g_configuration.access_mode == AccessMode::EveryoneCanDoEverything;
        return_if(!everyone_can_do_anything && ctx.role < plugins::core::enums::UserRole::Editor, 403, "You can not create sources.")

        if (!everyone_can_do_anything && !has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to create a source for this map."};
        }

        return ok_result;
    }

    OperationResult SourceValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
        auto map = find_model(map, entity.map_id)
        if (!map.second.empty()) return {400, map.second};

        if (!has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Read))
        {
            return {403, "You do not have permission to read this source."};
        }
        return ok_result;
    }

    OperationResult SourceValidator::validate_update(const RequestContext& ctx, const Model& old_entity,
                                                      const Model& new_entity) const
    {
        bool everyone_can_do_everything = g_configuration.access_mode == AccessMode::EveryoneCanDoEverything;

        return_if(!everyone_can_do_everything && ctx.role < plugins::core::enums::UserRole::Editor, 403, "You can not update sources.")

        if (everyone_can_do_everything && !has_right_for_map(ctx, new_entity.map_id, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to update a source for this map."};
        }

        return ok_result;
    }

    OperationResult SourceValidator::validate_delete(const RequestContext& ctx, const Model& entity) const
    {
        return_if(ctx.role < plugins::core::enums::UserRole::Editor, 403, "You can not delete sources.")

        if (has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Delete))
        {
            return {403, "You do not have permission to delete this source."};
        }
        return ok_result;

    }

    OperationResult SourceValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
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

    string SourceValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
