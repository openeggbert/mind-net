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

#include "mindnet/plugins/dictionary/validators/DictionaryIndexValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/core/enums/SingleRight.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryIndex.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryIndexType.hpp"
#include "mindnet/api/Persistence.hpp"
#include "mindnet/plugins/dictionary/DictionaryPersistenceMethods.hpp"

#define Model DictionaryIndex
#define MODEL DICTIONARY_INDEX
#define model dictionary_index

namespace mindnet::plugins::dictionary::validators
{
    using validators::DictionaryIndexValidator;
    using mindnet::api::OperationResult;

    OperationResult DictionaryIndexValidator::validate_create_authorization(
        const RequestContext& ctx,
        const Model& entity) const
    {
        return_if(ctx.role < mindnet::essential::UserRole::Editor,
                  403, "User does not have permission to modify dictionary indexes.")

        auto index = dictionary::find_dictionary_index(ctx, entity.dictionary_index_type_id);
        if (!index.second.empty()) return {400, index.second};

        if (!dictionary::has_right_for_map(
                ctx,
                index.first.dictionary_map_id,
                plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to create this index."};
        }

        return ok_result;
    }

    OperationResult DictionaryIndexValidator::validate_read_authorization(
        const RequestContext& ctx,
        const Model& entity) const
    {
        auto index = dictionary::find_dictionary_index(ctx, entity.dictionary_index_type_id);
        if (!index.second.empty()) return {400, index.second};

        if (!dictionary::has_right_for_map(
                ctx,
                index.first.dictionary_map_id,
                plugins::core::enums::SingleRight::Read))
        {
            return {403, "You do not have permission to read this index."};
        }

        return ok_result;
    }

    OperationResult DictionaryIndexValidator::validate_update_authorization(
        const RequestContext& ctx,
        const Model& old_entity,
        const Model& new_entity) const
    {
        auto index = dictionary::find_dictionary_index(ctx, new_entity.dictionary_index_type_id);
        if (!index.second.empty()) return {400, index.second};

        if (!dictionary::has_right_for_map(
                ctx,
                index.first.dictionary_map_id,
                plugins::core::enums::SingleRight::Write))
        {
            return {403, "You do not have permission to update this index entry."};
        }

        return ok_result;
    }

    OperationResult DictionaryIndexValidator::validate_delete_authorization(
        const RequestContext& ctx,
        const Model& entity) const
    {
        auto index = dictionary::find_dictionary_index(ctx, entity.dictionary_index_type_id);
        if (!index.second.empty()) return {400, index.second};

        if (!dictionary::has_right_for_map(
                ctx,
                index.first.dictionary_map_id,
                plugins::core::enums::SingleRight::Delete))
        {
            return {403, "You do not have permission to delete this index entry."};
        }

        return ok_result;
    }

    OperationResult DictionaryIndexValidator::validate_list_authorization(
        const RequestContext& ctx,
        const string_map& filter) const
    {
        mandatory_filter(dictionary_index_id)
        auto dictionary_index_id = std::stoll(filter.at("dictionary_index_type_id"));

        auto index = dictionary::find_dictionary_index(ctx, dictionary_index_id);
        if (index.second.empty()) return {400, index.second};

        if (!dictionary::has_right_for_map(
                ctx,
                index.first.dictionary_map_id,
                plugins::core::enums::SingleRight::Read))
        {
            return {403, "You do not have permission to list terms of this index."};
        }

        return ok_result;
    }

    OperationResult DictionaryIndexValidator::validate_create_integrity(
        const RequestContext&,
        const Model&) const
    {
        return ok_result;
    }

    OperationResult DictionaryIndexValidator::validate_read_integrity(
        const RequestContext&,
        const Model&) const
    {
        return ok_result;
    }

    OperationResult DictionaryIndexValidator::validate_update_integrity(
        const RequestContext&,
        const Model&,
        const Model&) const
    {
        return ok_result;
    }

    OperationResult DictionaryIndexValidator::validate_delete_integrity(
        const RequestContext&,
        const Model&) const
    {
        return ok_result;
    }

    OperationResult DictionaryIndexValidator::validate_list_integrity(
        const RequestContext&,
        const string_map&) const
    {
        return ok_result;
    }

    string DictionaryIndexValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
