/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to do so, subject to the
 * following conditions:
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

#include "mindnet/plugins/dictionary/validators/DictionaryNoteValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryNote.hpp"
#include "mindnet/api/Persistence.hpp"
#include "mindnet/plugins/dictionary/DictionaryPersistenceMethods.hpp"

#define Model DictionaryNote
#define MODEL DICTIONARY_NOTE
#define model dictionary_note

namespace mindnet::plugins::dictionary::validators
{
    using validators::DictionaryNoteValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult DictionaryNoteValidator::validate_create_authorization(const RequestContext& ctx,
                                                                           const Model& entity) const
    {
        return_if(ctx.role < mindnet::essential::UserRole::Editor,
                  403, "User does not have permission to create a note. Role: " +
                  essential::user_role_to_string(ctx.role))

        if (!dictionary::has_right_for_map(ctx, entity, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to create a note for this map."};
        }

        return ok_result;
    }

    OperationResult DictionaryNoteValidator::validate_read_authorization(const RequestContext& ctx,
                                                                         const Model& entity) const
    {
        if (dictionary::has_right_for_map(ctx, entity, plugins::core::enums::SingleRight::Read))
        {
            return ok_result;
        }
        return {403, "You do not have permission to read this note."};
    }

    OperationResult DictionaryNoteValidator::validate_update_authorization(const RequestContext& ctx,
                                                                           const Model& old_entity,
                                                                           const Model& new_entity) const
    {
        if (!dictionary::has_right_for_map(ctx, old_entity, plugins::core::enums::SingleRight::Write))
            return {403, "You do not have permission to update this note."};

        return ok_result;
    }

    OperationResult DictionaryNoteValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                           const Model& entity) const
    {
        if (dictionary::has_right_for_map(ctx, entity, plugins::core::enums::SingleRight::Delete))
        {
            return ok_result;
        }
        return {403, "You do not have permission to delete this note."};
    }

    OperationResult DictionaryNoteValidator::validate_list_authorization(const RequestContext& ctx,
                                                                         const string_map& filter) const
    {
        mandatory_filter(dictionary_term_id)
        auto dictionary_term_id = std::stoll(filter.at("dictionary_term_id"));
        auto dictionary_term = find_dictionary_term(ctx, dictionary_term_id);
        if (!dictionary_term.second.empty()) return {500, dictionary_term.second};
        if (dictionary::has_right_for_map(ctx, dictionary_term.first.dictionary_map_id, plugins::core::enums::SingleRight::Read))
        {
            return ok_result;
        }

        return ok_result;
    }

    OperationResult DictionaryNoteValidator::validate_create_integrity(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryNoteValidator::validate_read_integrity(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryNoteValidator::validate_update_integrity(const RequestContext& ctx,
                                                                       const Model& old_entity,
                                                                       const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryNoteValidator::validate_delete_integrity(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryNoteValidator::validate_list_integrity(const RequestContext& ctx,
                                                                     const string_map& filter) const
    {
        return ok_result;
    }

    string DictionaryNoteValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
