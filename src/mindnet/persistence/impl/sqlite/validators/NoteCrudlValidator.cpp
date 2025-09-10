//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/NoteCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/Note.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model Note
#define MODEL NOTE
#define model note

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::NoteCrudlValidator;

    OperationResult NoteCrudlValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {
        return_if (ctx.role < enums::UserRole::EDITOR,
               403, "User does not have permission to create a note.")

        auto map = find_model(map, entity.map_id)
        if (map.second.empty()) return {400, map.second};


        bool map_owner_and_can_write = ctx.token.user_id == map.first.owner_id && mindnet::enums::can_write(map.first.owner_rights);
        bool map_team_member_and_can_write = false;

        if (map.first.team_id != 0)
        {
            auto result = is_member_of_team(ctx, map.first.team_id);
            map_team_member_and_can_write = !result.empty() && mindnet::enums::can_write(map.first.team_rights);
        }
        bool other_can_write = mindnet::enums::can_write(map.first.other_rights);
        if (
            ctx.role != enums::UserRole::ADMIN &&
            !map_owner_and_can_write &&
            !map_team_member_and_can_write &&
            !other_can_write
            ) return {403,  "You can not create notes for this map."};

        return ok_result;
    }

    OperationResult NoteCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
        auto map = find_model(map, entity.map_id)
        if (map.second.empty()) return {400, map.second};


        bool map_owner_and_can_read = ctx.token.user_id == map.first.owner_id && mindnet::enums::can_read(map.first.owner_rights);
        bool map_team_member_and_can_read= false;

        if (map.first.team_id != 0)
        {
            auto result = is_member_of_team(ctx, map.first.team_id);
            map_team_member_and_can_read = !result.empty() && mindnet::enums::can_read(map.first.team_rights);
        }
        bool other_can_read = mindnet::enums::can_read(map.first.other_rights);
        if (
            ctx.role != enums::UserRole::ADMIN &&
            !map_owner_and_can_read &&
            !map_team_member_and_can_read &&
            !other_can_read
            ) return {403,  "You can not read this note."};


        return ok_result;
    }

    OperationResult NoteCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        auto ef = old_entity.to_values();
        auto can_create_result = ctx.db->can_create(models::NOTE_DEFINITION, ctx.token, ef);
        if (can_create_result.ko()) { return {403, "You cannot update this note."}; }
        
        return_if (old_entity.content_id != 0 && new_entity.content_id == 0,
            400, "content_id cannot be set to 0, if already set");
        
        return ok_result;
    }

    OperationResult NoteCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity)  const
    {
        auto map = find_model(map, entity.map_id)
        if (map.second.empty()) return {400, map.second};


        bool map_owner_and_can_delete = ctx.token.user_id == map.first.owner_id && mindnet::enums::can_delete(map.first.owner_rights);
        bool map_team_member_and_can_delete= false;

        if (map.first.team_id != 0)
        {
            auto result = is_member_of_team(ctx, map.first.team_id);
            map_team_member_and_can_delete = !result.empty() && mindnet::enums::can_delete(map.first.team_rights);
        }
        bool other_can_delete = mindnet::enums::can_delete(map.first.other_rights);
        if (
            ctx.role != enums::UserRole::ADMIN &&
            !map_owner_and_can_delete &&
            !map_team_member_and_can_delete &&
            !other_can_delete
            ) return {403,  "You can not delete this note."};

        return ok_result;
    }

    OperationResult NoteCrudlValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {
        http::QueryParams params;
        params.page_size = 100;
        for (auto& [key, value] : filter)
        {
            params.add_filter(key, value);
        }
        while (true)
        {
            auto maps = ctx.db->list(models::NOTE_DEFINITION, ctx.token, params);
            if (maps.second.ko()) return maps.second;
            if (maps.first.empty()) break;
            for (auto& values : maps.first)
            {
                models::Note note;
                note.from_values(values);
                auto check_result = can_read(ctx.db, ctx.token, note.get_id());
                if (check_result.ko()) return {400, std::string("You request list containing note with ID ") + std::to_string(note.get_id()) + ", but you cannot read this note. Modify your query."};
            }
            params.page_number++;
        }
        return ok_result;
    }

    string NoteCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}

#undef Model
#undef MODEL
#undef model