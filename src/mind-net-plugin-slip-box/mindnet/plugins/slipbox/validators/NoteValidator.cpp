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


#include "mindnet/plugins/slipbox/validators/NoteValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/slipbox/models/Note.hpp"
#include "mindnet/api/Persistence.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPersistenceMethods.hpp"


#define Model Note
#define MODEL NOTE
#define model note

namespace mindnet::plugins::slipbox::validators
{
    using validators::NoteValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult NoteValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return_if(ctx.role < mindnet::essential::UserRole::Editor,
                  403, "User does not have permission to create a note. Role: " + essential::user_role_to_string(ctx.
                      role))

        if (!slipbox::has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to create a note for this map."};
        }

        return ok_result;
    }

    OperationResult NoteValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        auto map = slipbox::find_map(ctx, entity.map_id);
        if (!map.second.empty()) return {400, map.second};

        if (slipbox::has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Read))
        {
            return ok_result;
        }
        return {403, "You do not have permission to read this note."};
    }

    OperationResult NoteValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                 const Model& new_entity) const
    {
        if (!slipbox::has_right_for_map(ctx, old_entity.map_id, plugins::core::enums::SingleRight::Write))
            return {403, "You do not have permission to update this note."};

        return ok_result;
    }

    OperationResult NoteValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        if (slipbox::has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Delete))
        {
            return ok_result;
        }
        return {403, "You do not have permission to delete this note."};
    }

    OperationResult NoteValidator::validate_list_authorization(const RequestContext& ctx,
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
            auto maps = ctx.db->list(plugins::slipbox::models::NOTE_DEFINITION, ctx.token, params);
            if (maps.second.ko()) return maps.second;
            if (maps.first.empty()) break;
            for (auto& values : maps.first)
            {
                Note note;
                note.from_values(values);
                auto check_result = can_read(ctx.db, ctx.token, note.get_id());
                if (check_result.ko())
                    return {
                        400,
                        std::string("You request list containing note with ID ") + std::to_string(note.get_id()) +
                        ", but you cannot read this note. Modify your query."
                    };
            }
            params.page_number++;
        }

        return ok_result;
    }


    OperationResult NoteValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult NoteValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult NoteValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                             const Model& new_entity) const
    {
        return_if(old_entity.content_id != 0 && new_entity.content_id == 0,
                  400, "content_id cannot be set to 0, if already set");

        return ok_result;
    }

    OperationResult NoteValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult NoteValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
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
