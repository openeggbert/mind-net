//
// Created by robertvokac on 8/6/25.
//

#include "../../../../../include/mindnet/plugins/zettelkasten/validators/MapCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/zettelkasten/models/Map.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model Map
#define MODEL MAP
#define model map

namespace mindnet::plugins::zettelkasten::validators
{
    using validators::MapCrudlValidator;
    using mindnet::OperationResult;

    OperationResult MapCrudlValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {
        return_if(has_map_name(ctx, entity.name),
                  409, "name already exists")

        return_if(entity.owner_id != ctx.token.user_id,
                  400, "Only owner can create maps")

        if (entity.team_id != 0)
        {
            auto team = find_model(team, entity.team_id)
            if (!team.second.empty()) return {400, "Team does not exist."};
        }

        return ok_result;
    }

    OperationResult MapCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
        if (ctx.role == plugins::core::enums::UserRole::ADMIN) return ok_result;
        if (entity.owner_id == ctx.token.user_id) return ok_result;
        if (entity.team_id != 0 && plugins::core::enums::can_read(entity.team_rights))
        {
            auto team = find_model(team, entity.team_id);
            check_found(team);
            string is_member_of_team_result = is_member_of_team(ctx, team.first.get_id());
            if (is_member_of_team_result.empty()) return ok_result;
        }
        if (plugins::core::enums::can_read(entity.other_rights))
        {
            return ok_result;
        }

        return {403, "You can not read this map."};
    }

    OperationResult MapCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity,
                                                       const Model& new_entity) const
    {
        using plugins::core::enums::can_write;

        bool owner_can_write = old_entity.owner_id == ctx.token.user_id && can_write(old_entity.owner_rights);
        bool team_can_write = false;

        if (old_entity.team_id != 0 && can_write(old_entity.team_rights))
        {
            auto team = find_model(team, old_entity.team_id);
            check_found(team);
            string is_member_of_team_result = is_member_of_team(ctx, team.first.get_id());
            team_can_write = is_member_of_team_result.empty();
        }
        bool other_can_write = can_write(old_entity.other_rights);

        if (!owner_can_write && !team_can_write && !other_can_write)
        {
            return {403, "You can not update this map."};
        }

        return ok_result;
    }

    OperationResult MapCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity) const
    {
        using plugins::core::enums::can_delete;

        bool owner_can_delete = entity.owner_id == ctx.token.user_id && can_delete(entity.owner_rights);
        bool team_can_delete = false;

        if (entity.team_id != 0 && can_delete(entity.team_rights))
        {
            auto team = find_model(team, entity.team_id);
            check_found(team);
            string is_member_of_team_result = is_member_of_team(ctx, team.first.get_id());
            team_can_delete = is_member_of_team_result.empty();
        }
        bool other_can_delete = can_delete(entity.other_rights);

        if (!owner_can_delete && !team_can_delete && !other_can_delete)
        {
            return {403, "You can not delete this map."};
        }

        return ok_result;
    }

    OperationResult MapCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {
        http::QueryParams params;
        params.page_size = 100;
        for (auto& [key, value] : filter)
        {
            params.add_filter(key, value);
        }
        while (true)
        {
            auto maps = ctx.db->list(plugins::zettelkasten::models::MAP_DEFINITION, ctx.token, params);
            if (maps.second.ko()) return maps.second;
            if (maps.first.empty()) break;
            for (auto& values : maps.first)
            {
                plugins::zettelkasten::models::Map map;
                map.from_values(values);
                auto check_result = can_read(ctx.db, ctx.token, map.get_id());
                if (check_result.ko()) return {
                    400,
                    std::string("You request list containing map with ID ") + std::to_string(map.get_id()) +
                    ", but you cannot read this map. The reason: " + check_result.error
                };
            }
            params.page_number++;
        }
        return ok_result;
    }

    string MapCrudlValidator::get_model_name() const
    {
        experiment << STRINGIFY(model) << commit;
        err << "ERROR: " << STRINGIFY(model) << commit;
        return STRINGIFY(model);
    }
}
#undef Model
#undef MODEL
#undef model
