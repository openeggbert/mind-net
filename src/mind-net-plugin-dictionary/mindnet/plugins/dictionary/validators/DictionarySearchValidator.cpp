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

#include "mindnet/plugins/dictionary/validators/DictionarySearchValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/plugins/dictionary/models/DictionarySearch.hpp"
#include "mindnet/plugins/dictionary/DictionaryPersistenceMethods.hpp"

#define Model DictionarySearch
#define MODEL DICTIONARY_SEARCH
#define model dictionary_search

namespace mindnet::plugins::dictionary::validators
{
    using mindnet::api::OperationResult;

    // -------------------------------------------------------------------------
    // AUTHORIZATION
    // -------------------------------------------------------------------------

    OperationResult DictionarySearchValidator::validate_create_authorization(
        const RequestContext& ctx,
        const Model& entity) const
    {
        assert_editor()

        bool is_public = entity.is_public;

        if (is_public && !dictionary::has_right_for_map(
                ctx,
                entity.dictionary_map_id,
                plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to create this search."};
        }

        return ok_result;
    }

    OperationResult DictionarySearchValidator::validate_read_authorization(
        const RequestContext& ctx,
        const Model& entity) const
    {
        if (!dictionary::has_right_for_map(
                ctx,
                entity.dictionary_map_id,
                plugins::core::enums::SingleRight::Read))
        {
            return {403, "You do not have permission to read this search."};
        }

        if (entity.is_public) return ok_result;

        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "You do not have permission to read this search."};
        }

        return ok_result;
    }

    OperationResult DictionarySearchValidator::validate_update_authorization(
        const RequestContext& ctx,
        const Model& old_entity,
        const Model& new_entity) const
    {
        bool is_public = new_entity.is_public;
        bool reader = ctx.role <= essential::UserRole::Reader;

        if (is_public && !dictionary::has_right_for_map(
                ctx,
                new_entity.dictionary_map_id,
                plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to update this search."};
        }
        if (reader && new_entity.is_public)
        {
            return {403, "You do not have permission to set is_public to true for this search."};
        }

        return ok_result;
    }

    OperationResult DictionarySearchValidator::validate_delete_authorization(
        const RequestContext& ctx,
        const Model& entity) const
    {
        bool is_public = entity.is_public;

        if (is_public && !dictionary::has_right_for_map(
                ctx,
                entity.dictionary_map_id,
                plugins::core::enums::SingleRight::Delete))
        {
            return {403, "You do not have permission to delete this search."};
        }

        if (!entity.is_public && ctx.token.user_id != entity.user_id)
        {
            return {403, "You do not have permission to delete this search."};
        }

        return ok_result;
    }

    OperationResult DictionarySearchValidator::validate_list_authorization(
        const RequestContext& ctx,
        const string_map& filter) const
    {
        mandatory_filter(dictionary_map_id)

        return ok_result;
    }

    // -------------------------------------------------------------------------
    // INTEGRITY
    // -------------------------------------------------------------------------

    OperationResult DictionarySearchValidator::validate_create_integrity(
        const RequestContext& ctx,
        const Model& entity) const
    {
        if (ctx.token.user_id != entity.user_id)
        {
            return {
                400,
                "Could not create search. The search must belong to the authenticated user."
            };
        }

        return ok_result;
    }

    OperationResult DictionarySearchValidator::validate_read_integrity(
        const RequestContext& ctx,
        const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionarySearchValidator::validate_update_integrity(
        const RequestContext& ctx,
        const Model& old_entity,
        const Model& new_entity) const
    {
        // user_id and dictionary_map_id are READONLY → update forbidden implicitly
        return ok_result;
    }

    OperationResult DictionarySearchValidator::validate_delete_integrity(
        const RequestContext& ctx,
        const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionarySearchValidator::validate_list_integrity(
        const RequestContext& ctx,
        const string_map& filter) const
    {
        return ok_result;
    }

    string DictionarySearchValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
