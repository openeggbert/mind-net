/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "hive/plugins/slipbox/validators/MapValidator.hpp"

#include "hive/essential/Global.hpp"
#include "hive/plugins/slipbox/models/Map.hpp"
#include "hive/api/Persistence.hpp"
#include "hive/plugins/slipbox/SlipBoxPersistenceMethods.hpp"

#define Model Map
#define MODEL MAP
#define model map

namespace hive::plugins::slipbox::validators
{
    using validators::MapValidator;
    using hive::api::OperationResult;
    using hive::essential::g_configuration;
    using_loggers()

    OperationResult MapValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return_if(entity.owner_id != ctx.token.user_id,
                  400, "Only owner can create maps")

        return ok_result;
    }

    OperationResult MapValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        if (ctx.role == hive::essential::UserRole::Admin) return ok_result;
        if (entity.owner_id == ctx.token.user_id) return ok_result;
        if (entity.team_id != 0 && plugins::core::enums::can_read(entity.team_rights))
        {
            auto team = core::find_team(ctx, entity.team_id);;
            check_found(team);
            string is_member_of_team_result = core::is_member_of_team(ctx, team.first.get_id());
            if (is_member_of_team_result.empty()) return ok_result;
        }
        if (plugins::core::enums::can_read(entity.other_rights))
        {
            return ok_result;
        }

        return {403, "You can not read this map."};
    }

    OperationResult MapValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                const Model& new_entity) const
    {
        using core::enums::can_write;

        bool owner_can_write = old_entity.owner_id == ctx.token.user_id && can_write(old_entity.owner_rights);
        bool team_can_write = false;

        if (old_entity.team_id != 0 && can_write(old_entity.team_rights))
        {
            auto team = core::find_team(ctx, old_entity.team_id);;
            check_found(team);
            string is_member_of_team_result = core::is_member_of_team(ctx, team.first.get_id());
            team_can_write = is_member_of_team_result.empty();
        }
        bool other_can_write = can_write(old_entity.other_rights);

        if (!owner_can_write && !team_can_write && !other_can_write)
        {
            return {403, "You can not update this map."};
        }

        return ok_result;
    }

    OperationResult MapValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        using plugins::core::enums::can_delete;

        bool owner_can_delete = entity.owner_id == ctx.token.user_id && can_delete(entity.owner_rights);
        bool team_can_delete = false;

        if (entity.team_id != 0 && can_delete(entity.team_rights))
        {
            auto team = core::find_team(ctx, entity.team_id);;
            check_found(team);
            string is_member_of_team_result = core::is_member_of_team(ctx, team.first.get_id());
            team_can_delete = is_member_of_team_result.empty();
        }
        bool other_can_delete = can_delete(entity.other_rights);

        if (!owner_can_delete && !team_can_delete && !other_can_delete)
        {
            return {403, "You can not delete this map."};
        }

        return ok_result;
    }

    OperationResult MapValidator::validate_list_authorization(const RequestContext& ctx,
                                                              const string_map& filter) const
    {
        orm::QueryParams params;
        params.page_size = 100;
        for (auto& [key, value] : filter)
        {
            params.add_filter(key, value);
        }
        while (true)
        {
            auto maps = ctx.db->list(plugins::slipbox::models::MAP_DEFINITION, ctx.token, params);
            if (maps.second.ko()) return maps.second;
            if (maps.first.empty()) break;
            for (auto& values : maps.first)
            {
                plugins::slipbox::models::Map map;
                map.from_values(values);
                auto check_result = can_read(ctx.db, ctx.token, map.get_id());
                if (check_result.ko())
                    return {
                        400,
                        std::string("You request list containing map with ID ") + std::to_string(map.get_id()) +
                        ", but you cannot read this map. The reason: " + check_result.error
                    };
            }
            params.page_number++;
        }
        return ok_result;
    }

    OperationResult MapValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(slipbox::has_map_name(ctx, entity.name),
                  409, "name already exists")

        if (entity.team_id != 0)
        {
            auto team = core::find_team(ctx, entity.team_id);
            if (!team.second.empty()) return {400, "Team does not exist."};
        }

        return ok_result;
    }

    OperationResult MapValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult MapValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                            const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult MapValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult MapValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string MapValidator::get_model_name() const
    {
        experiment << STRINGIFY(model) << commit;
        return STRINGIFY(model);
    }
}
#undef Model
#undef MODEL
#undef model