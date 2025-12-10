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
        return_if(ctx.role < mindnet::essential::UserRole::Editor,
                  403, "You can not create TermVisits.")

        if (!dictionary::has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to create a TermVisit for this map."};
        }

        return ok_result;
    }

    OperationResult DictionaryTermVisitValidator::validate_read_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        auto map = dictionary::find_map(ctx, entity.map_id);
        if (!map.second.empty()) return {400, map.second};

        if (!dictionary::has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Read))
        {
            return {403, "You do not have permission to read this TermVisit."};
        }
        return ok_result;
    }

    OperationResult DictionaryTermVisitValidator::validate_update_authorization(
        const RequestContext& ctx, const Model& old_entity, const Model& new_entity) const
    {
        return_if(ctx.role < mindnet::essential::UserRole::Editor,
                  403, "You can not update TermVisits.")

        if (!dictionary::has_right_for_map(ctx, new_entity.map_id, plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to update a TermVisit for this map."};
        }

        return ok_result;
    }

    OperationResult DictionaryTermVisitValidator::validate_delete_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        return_if(ctx.role < mindnet::essential::UserRole::Editor,
                  403, "You can not delete TermVisits.")

        if (!dictionary::has_right_for_map(ctx, entity.map_id, plugins::core::enums::SingleRight::Delete))
        {
            return {403, "You do not have permission to delete this TermVisit."};
        }

        return ok_result;
    }

    OperationResult DictionaryTermVisitValidator::validate_list_authorization(
        const RequestContext& ctx, const string_map& filter) const
    {
        mandatory_filter(map_id)
        auto map_id = std::stoll(filter.at("map_id"));

        if (!dictionary::has_right_for_map(ctx, map_id, plugins::core::enums::SingleRight::Read))
            return {
                403,
                "You do not have permission to list TermVisits for map with ID " +
                std::to_string(map_id) + "."
            };

        return ok_result;
    }

    OperationResult DictionaryTermVisitValidator::validate_create_integrity(
        const RequestContext& ctx, const Model& entity) const
    {
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
        return ok_result;
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
