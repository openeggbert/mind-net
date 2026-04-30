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

#include "hive/plugins/dictionary/validators/DictionaryTermAliasValidator.hpp"

#include "hive/essential/Global.hpp"
#include "hive/plugins/dictionary/models/DictionaryTermAlias.hpp"
#include "hive/api/Persistence.hpp"
#include "hive/plugins/dictionary/DictionaryPersistenceMethods.hpp"

#define Model DictionaryTermAlias
#define MODEL DICTIONARY_TERM_ALIAS
#define model dictionary_term_alias

namespace hive::plugins::dictionary::validators
{
    using validators::DictionaryTermAliasValidator;
    using hive::api::OperationResult;
    using hive::essential::g_configuration;

    OperationResult DictionaryTermAliasValidator::validate_create_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        auto dictionary_term = find_dictionary_term(ctx, entity.dictionary_term_id);
        if (!dictionary_term.second.empty()) return {500, dictionary_term.second};

        if (!dictionary::has_right_for_map(ctx, dictionary_term.first.dictionary_map_id, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to create a TermAlias for this map."};
        }

        return ok_result;
    }

    OperationResult DictionaryTermAliasValidator::validate_read_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        auto term = find_dictionary_term(ctx, entity.dictionary_term_id);
        if (!term.second.empty()) return {500, term.second};
        auto map_id = term.first.dictionary_map_id;

        if (!dictionary::has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Read))
            return {403, "You do not have permission to read this alias."};

        return ok_result;
    }

    OperationResult DictionaryTermAliasValidator::validate_update_authorization(
        const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult DictionaryTermAliasValidator::validate_delete_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        auto term = find_dictionary_term(ctx, entity.dictionary_term_id);
        if (!term.second.empty()) return {500, term.second};
        auto map_id = term.first.dictionary_map_id;

        if (!dictionary::has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Delete))
            return {403, "You do not have permission to delete this alias."};

        return ok_result;
    }

    OperationResult DictionaryTermAliasValidator::validate_list_authorization(
        const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    OperationResult DictionaryTermAliasValidator::validate_create_integrity(
        const RequestContext& ctx, const Model& entity) const
    {
        auto dictionary_term = find_dictionary_term(ctx, entity.dictionary_term_id);
        if (!dictionary_term.second.empty()) return {500, dictionary_term.second};

        if (dictionary_term.first.dictionary_map_id != entity.dictionary_map_id)
        {
            return {400, "Map ID of the term and alias must be the same."};
        }

        return ok_result;
    }

    OperationResult DictionaryTermAliasValidator::validate_read_integrity(
        const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryTermAliasValidator::validate_update_integrity(
        const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult DictionaryTermAliasValidator::validate_delete_integrity(
        const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryTermAliasValidator::validate_list_integrity(
        const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string DictionaryTermAliasValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
