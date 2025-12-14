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

#include "mindnet/plugins/dictionary/validators/DictionaryTermVisitValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryTermVisit.hpp"
#include "mindnet/api/Persistence.hpp"
#include "mindnet/plugins/dictionary/DictionaryPersistenceMethods.hpp"

#define Model DictionaryTermVisit
#define MODEL DICTIONARY_TERM_VISIT
#define model dictionary_term_visit

namespace mindnet::plugins::dictionary::validators
{
    using validators::DictionaryTermVisitValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult DictionaryTermVisitValidator::validate_create_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        auto dictionary_term = find_dictionary_term(ctx, entity.dictionary_term_id);
        if (!dictionary_term.second.empty()) return {500, dictionary_term.second};

        if (!dictionary::has_right_for_map(ctx, dictionary_term.first.dictionary_map_id, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to create a TermVisit for this map."};
        }

        return ok_result;
    }

    OperationResult DictionaryTermVisitValidator::validate_read_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        if (entity.user_id != ctx.token.user_id)
        {
            return {403, "You do not have permission to create a TermVisit for this map."};
        }
        return ok_result;
    }

    OperationResult DictionaryTermVisitValidator::validate_update_authorization(
        const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult DictionaryTermVisitValidator::validate_delete_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        if (ctx.token.user_id != entity.user_id)
            return {403, "You do not have permission to delete this visit."};

        auto term = find_dictionary_term(ctx, entity.dictionary_term_id);
        if (!term.second.empty()) return {500, term.second};
        auto map_id = term.first.dictionary_map_id;

        if (!dictionary::has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Delete))
            return {403, "You do not have permission to delete this visit."};

        return ok_result;    }

    OperationResult DictionaryTermVisitValidator::validate_list_authorization(
        const RequestContext& ctx, const string_map& filter) const
    {
        mandatory_filter(user_id)

        return ok_result;
    }

    OperationResult DictionaryTermVisitValidator::validate_create_integrity(
        const RequestContext& ctx, const Model& entity) const
    {
        if (entity.user_id != ctx.token.user_id)
            return {400, "dictionary_term_visit.user_id must be the same as your user id."};
        auto read_term = find_dictionary_term(ctx, entity.dictionary_term_id);
        if (!read_term.second.empty()) return {400, read_term.second};        
        if (entity.dictionary_map_id != read_term.first.dictionary_map_id)
            return {400, "The dictionary_map_id of the term and term visit must be the same."};

        return ok_result;
    }

    OperationResult DictionaryTermVisitValidator::validate_read_integrity(
        const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryTermVisitValidator::validate_update_integrity(
        const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult DictionaryTermVisitValidator::validate_delete_integrity(
        const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryTermVisitValidator::validate_list_integrity(
        const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string DictionaryTermVisitValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
