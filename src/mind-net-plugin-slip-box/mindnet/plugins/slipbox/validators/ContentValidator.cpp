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

#include "mindnet/plugins/slipbox/validators/ContentValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/slipbox/models/Content.hpp"
#include "mindnet/api/Persistence.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPersistenceMethods.hpp"

#define Model Content
#define MODEL CONTENT
#define model content

namespace mindnet::plugins::slipbox::validators
{
    using validators::ContentValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult ContentValidator::validate_create_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return_if(ctx.role<mindnet::essential::UserRole::Editor, 403, "You can not create content.")

        return ok_result;
    }

    OperationResult ContentValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        auto note_id = slipbox::find_note_for_content(ctx, entity.get_id());
        if (!note_id.second.empty()) return {400, note_id.second};
        auto note = slipbox::find_note(ctx, note_id.first);;
        if (!note.second.empty()) return {400, note.second};

        auto note_validator = get_validator("note");
        if (note_validator == nullptr) return {500, "No note validator found"};

        auto note_ = note.first.to_values();

        return note_validator->can_read(ctx.db, ctx.token, note_id.first);
    }

    OperationResult ContentValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                    const Model& new_entity) const
    {
        auto result = slipbox::find_note_for_content(ctx, old_entity.get_id());
        if (!result.second.empty()) return {400, result.second};
        auto one = slipbox::find_note(ctx, result.first);;

        auto ef = one.first.to_values();

        auto note_validator = get_validator("note");
        if (note_validator == nullptr) return {500, "No note validator found"};

        auto new_values = new_entity.to_values();
        return note_validator->can_update(ctx.db, ctx.token, ef, new_values);
    }

    OperationResult ContentValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        auto result = slipbox::find_note_for_content(ctx, entity.get_id());
        if (!result.second.empty()) return {400, result.second};

        auto note_validator = get_validator("note");
        if (note_validator == nullptr) return {500, "No note validator found"};

        return note_validator->can_delete(ctx.db, ctx.token, result.first);
    }

    OperationResult ContentValidator::validate_list_authorization(const RequestContext& ctx,
                                                                  const string_map& filter) const
    {
        return ok_result;
    }

    OperationResult ContentValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(entity.version != 1,
                  404, "version must be 1 during content creation.");
        return_if(entity.semantic_version != 1,
                  404, "semantic_version must be 1 during content creation.");
        return_if(entity.change_ratio != 0,
                  404, "change_ratio_times_100 must be 0 during content creation.");

        return ok_result;
    }

    OperationResult ContentValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ContentValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                                const Model& new_entity) const
    {
        bool last_parse_success_at_changed = old_entity.last_parsed_success_at != new_entity.last_parsed_success_at;
        bool last_parsed_fail_at_at_changed = old_entity.last_parsed_fail_at != new_entity.last_parsed_fail_at;
        return_if(
            !last_parse_success_at_changed && !last_parsed_fail_at_at_changed && new_entity.version != old_entity.
            version + 1, 400,
            "The version must be incremented by exactly 1 during an update.")
        auto& old_semantic_version = old_entity.semantic_version;
        auto& new_semantic_version = new_entity.semantic_version;
        return_if(new_semantic_version != old_semantic_version && new_semantic_version != old_semantic_version + 1, 400,
                  "The semantic_version must be the same or incremented by exactly 1 during an update.")
        return_if(new_semantic_version > old_semantic_version && new_entity.change_ratio == 0, 400,
                  "The change_ratio_times_100 must be more than 0, if the semantic_version was incremented.")
        return ok_result;
    }

    OperationResult ContentValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult ContentValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return {405, "Method not allowed"};
    }

    string ContentValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model