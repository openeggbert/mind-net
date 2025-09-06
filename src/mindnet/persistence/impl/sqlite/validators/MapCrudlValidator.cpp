//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/MapCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/Map.h"
#include "mindnet/persistence/Persistence.h"

#define Model Map
#define MODEL MAP
#define model map

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::MapCrudlValidator;

    operation_result MapCrudlValidator::can_create(db_ db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_create(Model);

        return_if(api::has_user_name(db, token, new_entity.name),
                  409, "name already exists")

        return_if(new_entity.owner_id != logged_in_user.get_id(),
                  400, "Only owner can create maps")

        if (new_entity.team_id != 0)
        {
            auto team = find_model(team, new_entity.team_id)
            if (!team.second.empty()) return {400, "Team does not exist."};
        }

        return ok_result;
    }

    operation_result MapCrudlValidator::can_read(db_ db, http::LoginToken& token, int id) const
    {
        start_can_read(Model, MODEL)
        if (is_admin) return ok_result;
        if (entity.owner_id == logged_in_user.get_id()) return ok_result;
        if (entity.team_id != 0 && mindnet::enums::can_read(entity.team_rights))
        {
            auto team = find_model(team, entity.team_id);
            check_found(team);
            string is_member_of_team_result = api::is_member_of_team(db, token, team.first.get_id());
            if (is_member_of_team_result.empty()) return ok_result;
        }
        if (mindnet::enums::can_read(entity.other_rights))
        {
            return ok_result;
        }

        return {403, "You can not read this map."};
    }

    operation_result MapCrudlValidator::can_update(db_ db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_update(Model, MODEL)

        using mindnet::enums::can_write;

        bool owner_can_write = old_entity.owner_id == logged_in_user.get_id() && can_write(old_entity.owner_rights);
        bool team_can_write = false;

        if (old_entity.team_id != 0 && mindnet::enums::can_write(old_entity.team_rights))
        {
            auto team = find_model(team, old_entity.team_id);
            check_found(team);
            string is_member_of_team_result = api::is_member_of_team(db, token, team.first.get_id());
            team_can_write = is_member_of_team_result.empty();
        }
        bool other_can_write = can_write(old_entity.other_rights);

        if (!owner_can_write && !team_can_write && !other_can_write)
        {
            return {403, "You can not update this map."};
        }

        return ok_result;
    }

    operation_result MapCrudlValidator::can_delete(db_ db, http::LoginToken& token, int id) const
    {
        start_can_delete(Model, MODEL)

        using mindnet::enums::can_delete;

        bool owner_can_delete = entity.owner_id == logged_in_user.get_id() && can_delete(entity.owner_rights);
        bool team_can_delete = false;

        if (entity.team_id != 0 && mindnet::enums::can_delete(entity.team_rights))
        {
            auto team = find_model(team, entity.team_id);
            check_found(team);
            string is_member_of_team_result = api::is_member_of_team(db, token, team.first.get_id());
            team_can_delete = is_member_of_team_result.empty();
        }
        bool other_can_delete = can_delete(entity.other_rights);

        if (!owner_can_delete && !team_can_delete && !other_can_delete)
        {
            return {403, "You can not delete this map."};
        }

        return ok_result;
    }

    operation_result MapCrudlValidator::can_list(db_ db, http::LoginToken& token, string_map& filter) const
    {
        http::QueryParams params;
        params.page_size = 100;
        for (auto& [key, value] : filter)
        {
            params.add_filter(key, value);
        }
        while (true)
        {
            auto maps = db->list(params, models::MAP_DEFINITION, token);
            if (maps.second.ko()) return maps.second;
            if (maps.first.empty()) break;
            for (auto& values : maps.first)
            {
                models::Map map;
                map.from_values(values);
                auto check_result = can_read(db, token, map.get_id());
                if (check_result.ko()) return {400, std::string("You request list containing map with ID ") + std::to_string(map.get_id()) + ", but you cannot read this map. Modify your query."};
            }
            params.page_number++;
        }
        return ok_result;
    }

    string MapCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}
