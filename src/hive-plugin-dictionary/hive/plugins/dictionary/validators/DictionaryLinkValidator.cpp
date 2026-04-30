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

#include "hive/plugins/dictionary/validators/DictionaryLinkValidator.hpp"

#include "hive/essential/Global.hpp"
#include "hive/plugins/core/enums/SingleRight.hpp"
#include "hive/plugins/dictionary/models/DictionaryLink.hpp"
#include "hive/api/Persistence.hpp"
#include "hive/plugins/dictionary/DictionaryPersistenceMethods.hpp"

#define Model DictionaryLink
#define MODEL DICTIONARY_LINK
#define model dictionary_link

namespace hive::plugins::dictionary::validators
{
    using hive::api::OperationResult;

    OperationResult DictionaryLinkValidator::validate_create_authorization(const RequestContext& ctx,
                                                                          const Model& entity) const
    {
        assert_editor()

        auto from_term = find_dictionary_term(ctx, entity.from_dictionary_term_id);
        if (!from_term.second.empty()) return {500, from_term.second};
        auto to_term = find_dictionary_term(ctx, entity.to_dictionary_term_id);
        if (!to_term.second.empty()) return {500, to_term.second};
        // if (from_term.first.dictionary_map_id != to_term.first.dictionary_map_id)
        // {
        //     return {400, "Both the dictionary_map ids must be the same."};
        // }
        auto map_1_id = from_term.first.dictionary_map_id;
        auto map_2_id = to_term.first.dictionary_map_id;

        if (!dictionary::has_right_for_map(ctx, map_1_id, plugins::core::enums::SingleRight::Write))
            return {403, "You do not have permission to create this link."};
        if (map_1_id != map_2_id && !dictionary::has_right_for_map(ctx, map_2_id,
                                                                   plugins::core::enums::SingleRight::Write))
            return {403, "You do not have permission to create this link."};

        return ok_result;
    }

    OperationResult DictionaryLinkValidator::validate_read_authorization(const RequestContext& ctx,
                                                                         const Model& entity) const
    {
        auto from_term = find_dictionary_term(ctx, entity.from_dictionary_term_id);
        if (!from_term.second.empty()) return {500, from_term.second};
        auto map_id = from_term.first.dictionary_map_id;

        if (!dictionary::has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Read))
            return {403, "You do not have permission to read this link."};

        return ok_result;
    }

    OperationResult DictionaryLinkValidator::validate_update_authorization(const RequestContext& ctx,
                                                                           const Model& old_entity,
                                                                           const Model& new_entity) const
    {
        auto from_term = find_dictionary_term(ctx, new_entity.from_dictionary_term_id);
        if (!from_term.second.empty()) return {500, from_term.second};
        auto map_id = from_term.first.dictionary_map_id;

        if (!dictionary::has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Write))
            return {403, "You do not have permission to update this link."};

        return ok_result;
    }

    OperationResult DictionaryLinkValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                           const Model& entity) const
    {
        assert_editor()

        auto from_term = find_dictionary_term(ctx, entity.from_dictionary_term_id);
        if (!from_term.second.empty()) return {500, from_term.second};
        auto map_id = from_term.first.dictionary_map_id;

        if (!dictionary::has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Delete))
            return {403, "You do not have permission to delete this link."};


        return ok_result;
    }

    OperationResult DictionaryLinkValidator::validate_list_authorization(const RequestContext& ctx,
                                                                         const string_map& filter) const
    {
        return ok_result;
    }

    using validators::DictionaryLinkValidator;
    using hive::api::OperationResult;
    using hive::essential::g_configuration;

    OperationResult DictionaryLinkValidator::validate_create_integrity(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryLinkValidator::validate_read_integrity(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryLinkValidator::validate_update_integrity(const RequestContext& ctx,
                                                                       const Model& old_entity,
                                                                       const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryLinkValidator::validate_delete_integrity(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryLinkValidator::validate_list_integrity(const RequestContext& ctx,
                                                                     const string_map& filter) const
    {
        return ok_result;
    }

    string DictionaryLinkValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
