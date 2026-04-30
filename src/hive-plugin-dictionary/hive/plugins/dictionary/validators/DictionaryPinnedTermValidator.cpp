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

#include "hive/plugins/dictionary/validators/DictionaryPinnedTermValidator.hpp"

#include "hive/essential/Global.hpp"
#include "hive/plugins/dictionary/models/DictionaryPinnedTerm.hpp"
#include "hive/api/Persistence.hpp"
#include "hive/plugins/dictionary/DictionaryPersistenceMethods.hpp"

#define Model DictionaryPinnedTerm
#define MODEL DICTIONARY_PINNED_TERM
#define model dictionary_pinned_term

namespace hive::plugins::dictionary::validators
{
    using validators::DictionaryPinnedTermValidator;
    using hive::api::OperationResult;
    using hive::essential::g_configuration;

    OperationResult DictionaryPinnedTermValidator::validate_create_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        auto dictionary_term = find_dictionary_term(ctx, entity.dictionary_term_id);
        if (!dictionary_term.second.empty()) return {500, dictionary_term.second};

        if (!dictionary::has_right_for_map(ctx, dictionary_term.first.dictionary_map_id, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to create a PinnedTerm for this map."};
        }

        return ok_result;
    }

    OperationResult DictionaryPinnedTermValidator::validate_read_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        if (entity.user_id != ctx.token.user_id)
        {
            return {403, "You do not have permission to read a PinnedTerm for this map."};
        }
        return ok_result;
    }

    OperationResult DictionaryPinnedTermValidator::validate_update_authorization(
        const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult DictionaryPinnedTermValidator::validate_delete_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        if (ctx.token.user_id != entity.user_id)
            return {403, "You do not have permission to delete this PinnedTerm."};

        auto term = find_dictionary_term(ctx, entity.dictionary_term_id);
        if (!term.second.empty()) return {500, term.second};
        auto map_id = term.first.dictionary_map_id;

        if (!dictionary::has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Delete))
            return {403, "You do not have permission to delete this visit."};

        return ok_result;    }

    OperationResult DictionaryPinnedTermValidator::validate_list_authorization(
        const RequestContext& ctx, const string_map& filter) const
    {
        mandatory_filter(user_id)

        return ok_result;
    }

    OperationResult DictionaryPinnedTermValidator::validate_create_integrity(
        const RequestContext& ctx, const Model& entity) const
    {
        if (entity.user_id != ctx.token.user_id)
            return {400, "dictionary_pinned_term.user_id must be the same as your user id."};
        auto read_term = find_dictionary_term(ctx, entity.dictionary_term_id);
        if (!read_term.second.empty()) return {400, read_term.second};        
        if (entity.dictionary_map_id != read_term.first.dictionary_map_id)
            return {400, "The dictionary_map_id of the term and pinned term must be the same."};

        return ok_result;
    }

    OperationResult DictionaryPinnedTermValidator::validate_read_integrity(
        const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryPinnedTermValidator::validate_update_integrity(
        const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult DictionaryPinnedTermValidator::validate_delete_integrity(
        const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryPinnedTermValidator::validate_list_integrity(
        const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string DictionaryPinnedTermValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
