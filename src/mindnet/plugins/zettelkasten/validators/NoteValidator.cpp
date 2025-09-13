//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/zettelkasten/validators/NoteValidator.h"

#include "mindnet/Global.h"
#include "mindnet/plugins/zettelkasten/models/Note.h"
#include "mindnet/api/Persistence.h"

#define Model Note
#define MODEL NOTE
#define model note

namespace mindnet::plugins::zettelkasten::validators
{
    using validators::NoteValidator;
    using mindnet::OperationResult;

    OperationResult NoteValidator::validate_create(const RequestContext& ctx, const Model& entity) const
    {
        return_if(ctx.role < plugins::core::enums::UserRole::EDITOR,
                  403, "User does not have permission to create a note.")

        if (has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::WRITE))
        {
            return ok_result;
        }
        return {403, "You do not have permission to create a note for this map."};
    }

    OperationResult NoteValidator::validate_read(const RequestContext& ctx, const Model& entity) const
    {
        auto map = find_model(map, entity.map_id)
        if (!map.second.empty()) return {400, map.second};

        if (has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::READ))
        {
            return ok_result;
        }
        return {403, "You do not have permission to read this note."};
    }

    OperationResult NoteValidator::validate_update(const RequestContext& ctx, const Model& old_entity,
                                                        const Model& new_entity) const
    {
        if (!has_right_for_map(ctx, old_entity.map_id, plugins::core::enums::SingleRight::WRITE))
            return {403, "You do not have permission to update this note."};

        return_if(old_entity.content_id != 0 && new_entity.content_id == 0,
                  400, "content_id cannot be set to 0, if already set");

        return ok_result;
    }

    OperationResult NoteValidator::validate_delete(const RequestContext& ctx, const Model& entity) const
    {
        if (has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::DELETE))
        {
            return ok_result;
        }
        return {403, "You do not have permission to delete this note."};

        return ok_result;
    }

    OperationResult NoteValidator::validate_list(const RequestContext& ctx, const string_map& filter) const
    {
        http::QueryParams params;
        params.page_size = 100;
        for (auto& [key, value] : filter)
        {
            params.add_filter(key, value);
        }
        while (true)
        {
            auto maps = ctx.db->list(plugins::zettelkasten::models::NOTE_DEFINITION, ctx.token, params);
            if (maps.second.ko()) return maps.second;
            if (maps.first.empty()) break;
            for (auto& values : maps.first)
            {
                Note note;
                note.from_values(values);
                auto check_result = can_read(ctx.db, ctx.token, note.get_id());
                if (check_result.ko()) return {
                    400,
                    std::string("You request list containing note with ID ") + std::to_string(note.get_id()) +
                    ", but you cannot read this note. Modify your query."
                };
            }
            params.page_number++;
        }
        return ok_result;
    }

    string NoteValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
