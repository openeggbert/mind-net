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

        if(has_right_for_map(ctx, entity.map_id, enums::SingleRight::WRITE))
        {return ok_result;}
        return {403, "You do not have permission to create a note for this map."};
    }

    OperationResult NoteCrudlValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
        auto map = find_model(map, entity.map_id)
        if (map.second.empty()) return {400, map.second};

        if(has_right_for_map(ctx, entity.map_id, enums::SingleRight::READ))
        {return ok_result;}
        return {403, "You do not have permission to read this note."};
    }

    OperationResult NoteCrudlValidator::validate_update(const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {

        if(!has_right_for_map(ctx, old_entity.map_id, enums::SingleRight::WRITE))
        return {403, "You do not have permission to update this note."};

        return_if (old_entity.content_id != 0 && new_entity.content_id == 0,
            400, "content_id cannot be set to 0, if already set");
        
        return ok_result;
    }

    OperationResult NoteCrudlValidator::validate_delete(const RequestContext& ctx, const Model& entity)  const
    {
        if(has_right_for_map(ctx, entity.map_id, enums::SingleRight::DELETE))
        {return ok_result;}
        return {403, "You do not have permission to delete this note."};

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