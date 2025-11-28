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

#include "mindnet/plugins/slipbox/validators/AnnotationValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/slipbox/models/Annotation.hpp"
#include "mindnet/api/Persistence.hpp"
#include "mindnet/plugins/slipbox/SlipBoxPersistenceMethods.hpp"

#define Model Annotation
#define MODEL ANNOTATION
#define model annotation

namespace mindnet::plugins::slipbox::validators
{
    using validators::AnnotationValidator;
    using mindnet::api::OperationResult;

    //
    // --- AUTHORIZATION ---
    //

    OperationResult AnnotationValidator::validate_create_authorization(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        auto note = slipbox::find_note(ctx, entity.note_id);;
        if (!note.second.empty()) return {400, note.second};

        if (slipbox::has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Write))
        {
            return ok_result;
        }
        return {403, "You do not have permission to create a annotation for this note."};
    }

    OperationResult AnnotationValidator::validate_read_authorization(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        auto note = slipbox::find_note(ctx, entity.note_id);;
        if (!note.second.empty()) return {400, note.second};

        if (slipbox::has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Read))
        {
            return ok_result;
        }
        return {403, "You do not have permission to read this annotation for this note."};
    }

    OperationResult AnnotationValidator::validate_update_authorization(const RequestContext& ctx,
                                                                       const Model& old_entity,
                                                                       const Model& new_entity) const
    {
        auto note = slipbox::find_note(ctx, new_entity.note_id);;
        if (!note.second.empty()) return {400, note.second};

        if (slipbox::has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Write))
        {
            return ok_result;
        }
        return {403, "You do not have permission to update this annotation for this note."};
    }

    OperationResult AnnotationValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        auto note = slipbox::find_note(ctx, entity.note_id);;
        if (!note.second.empty()) return {400, note.second};

        if (slipbox::has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Delete))
        {
            return ok_result;
        }
        return {403, "You do not have permission to delete this annotation for this note."};
    }

    OperationResult AnnotationValidator::validate_list_authorization(const RequestContext& ctx,
                                                                     const string_map& filter) const
    {
        mandatory_filter(note_id)
        auto note_id = std::stoll(filter.at("note_id"));

        auto note = slipbox::find_note(ctx, note_id);
        if (!note.second.empty()) return {400, note.second};

        if (slipbox::has_right_for_map(ctx, note.first.map_id, plugins::core::enums::SingleRight::Read))
        {
            return ok_result;
        }
        return {403, "You do not have permission to list annotations for this note."};
    }

    //
    // --- INTEGRITY ---
    //

    OperationResult AnnotationValidator::validate_create_integrity(const RequestContext& ctx,
                                                                   const Model& entity) const
    {
        return ok_result;
    }

    OperationResult AnnotationValidator::validate_read_integrity(const RequestContext& ctx,
                                                                 const Model& entity) const
    {
        return ok_result;
    }

    OperationResult AnnotationValidator::validate_update_integrity(const RequestContext& ctx,
                                                                   const Model& old_entity,
                                                                   const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult AnnotationValidator::validate_delete_integrity(const RequestContext& ctx,
                                                                   const Model& entity) const
    {
        return ok_result;
    }

    OperationResult AnnotationValidator::validate_list_integrity(const RequestContext& ctx,
                                                                 const string_map& filter) const
    {
        return ok_result;
    }

    string AnnotationValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model