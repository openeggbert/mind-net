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

#include "mindnet/plugins/dictionary/validators/DictionaryFlagValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryFlag.hpp"
#include "mindnet/api/Persistence.hpp"
#include "mindnet/plugins/dictionary/DictionaryPersistenceMethods.hpp"

#define Model DictionaryFlag
#define MODEL DICTIONARY_FLAG
#define model dictionary_flag

namespace mindnet::plugins::dictionary::validators
{
    using mindnet::api::OperationResult;

    OperationResult DictionaryFlagValidator::validate_create_authorization(const RequestContext& ctx,
                                                                          const Model& entity) const
    {
        assert_editor()

        auto term = find_dictionary_term(ctx, entity.dictionary_term_id);
        if (!term.second.empty()) return {500, term.second};

        auto map_id = term.first.dictionary_map_id;

        if (!dictionary::has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Write))
        return {403, "You do not have permission to create this flag."};

        return ok_result;
    }

    OperationResult DictionaryFlagValidator::validate_read_authorization(const RequestContext& ctx,
                                                                         const Model& entity) const
    {
        auto term = find_dictionary_term(ctx, entity.dictionary_term_id);
        if (!term.second.empty()) return {500, term.second};
        auto map_id = term.first.dictionary_map_id;

        if (!dictionary::has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Read))
            return {403, "You do not have permission to read this flag."};

        if (entity.is_public) return ok_result;
        if (ctx.token.user_id != entity.user_id)
            return {403, "You do not have permission to read this flag."};

        return ok_result;
    }

    OperationResult DictionaryFlagValidator::validate_update_authorization(const RequestContext& ctx,
                                                                           const Model& old_entity,
                                                                           const Model& new_entity) const
    {
        auto term = find_dictionary_term(ctx, new_entity.dictionary_term_id);
        if (!term.second.empty()) return {500, term.second};
        auto map_id = term.first.dictionary_map_id;

        if (!dictionary::has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Write))
            return {403, "You do not have permission to update this flag."};

        // if (ctx.token.user_id != new_entity.user_id)
        //     return {403, "You do not have permission to update this flag."};

        return ok_result;
    }

    OperationResult DictionaryFlagValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                           const Model& entity) const
    {
        auto term = find_dictionary_term(ctx, entity.dictionary_term_id);
        if (!term.second.empty()) return {500, term.second};
        auto map_id = term.first.dictionary_map_id;

        if (!dictionary::has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Delete))
            return {403, "You do not have permission to delete this flag."};

        if (!entity.is_public && ctx.token.user_id != entity.user_id)
            return {403, "You do not have permission to delete this flag."};

        return ok_result;
    }

    OperationResult DictionaryFlagValidator::validate_list_authorization(const RequestContext& ctx,
                                                                         const string_map& filter) const
    {
        mandatory_filter(user_id)

        return ok_result;
    }

    using validators::DictionaryFlagValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult DictionaryFlagValidator::validate_create_integrity(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        if (ctx.token.user_id != entity.user_id)
        {
            return {400, "Could not created flag. Your user id and the flag.user_id must be the same."};
        }
        auto term = find_dictionary_term(ctx, entity.dictionary_term_id);
        if (!term.second.empty()) return {500, term.second};

        auto map_id = term.first.dictionary_map_id;
        if (map_id != entity.dictionary_map_id)
            return {400, "Could not created flag. The dictionary_term.dictionary_map_id and the flag.dictionary_map_id must be the same."};

        return ok_result;
    }

    OperationResult DictionaryFlagValidator::validate_read_integrity(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryFlagValidator::validate_update_integrity(const RequestContext& ctx,
                                                                       const Model& old_entity,
                                                                       const Model& new_entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult DictionaryFlagValidator::validate_delete_integrity(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryFlagValidator::validate_list_integrity(const RequestContext& ctx,
                                                                     const string_map& filter) const
    {
        return ok_result;
    }

    string DictionaryFlagValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
