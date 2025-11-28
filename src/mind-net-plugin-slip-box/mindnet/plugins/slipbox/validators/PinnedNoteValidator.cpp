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

#include "mindnet/plugins/slipbox/validators/PinnedNoteValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/plugins/slipbox/models/PinnedNote.hpp"
#include "mindnet/api/Persistence.hpp"

#define Model PinnedNote
#define MODEL PINNED_NOTE
#define model pinned_note

namespace mindnet::plugins::slipbox::validators
{
    using validators::PinnedNoteValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult PinnedNoteValidator::validate_create_authorization(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        // Only the owner can pin notes
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Unauthorized: Only note owner can pin notes"};
        }
        return ok_result;
    }

    OperationResult PinnedNoteValidator::validate_read_authorization(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        // Can read if public or owner
        if (!entity.is_public && ctx.token.user_id != entity.user_id)
        {
            return {403, "Unauthorized: Cannot read private pinned note"};
        }
        return ok_result;
    }

    OperationResult PinnedNoteValidator::validate_update_authorization(const RequestContext& ctx,
                                                                       const Model& old_entity,
                                                                       const Model& new_entity) const
    {
        // Only owner can update their pins
        if (ctx.token.user_id != old_entity.user_id)
        {
            return {403, "Unauthorized: Only owner can update pinned note"};
        }
        return ok_result;
    }

    OperationResult PinnedNoteValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        // Only owner can unpin notes
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Unauthorized: Only owner can unpin notes"};
        }
        return ok_result;
    }

    OperationResult PinnedNoteValidator::validate_list_authorization(const RequestContext& ctx,
                                                                     const string_map& filter) const
    {
        // Everyone can list public pins
        return ok_result;
    }

    OperationResult PinnedNoteValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult PinnedNoteValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult PinnedNoteValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                   const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult PinnedNoteValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult PinnedNoteValidator::validate_list_integrity(const RequestContext& ctx,
                                                                 const string_map& filter) const
    {
        return ok_result;
    }

    string PinnedNoteValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
