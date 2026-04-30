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

#include "hive/plugins/dictionary/validators/DictionaryTermForReviewValidator.hpp"

#include "hive/essential/Global.hpp"
#include "hive/api/Persistence.hpp"
#include "hive/plugins/dictionary/DictionaryPersistenceMethods.hpp"

#define Model DictionaryTermForReview
#define MODEL DICTIONARY_TERM_FOR_REVIEW
#define model dictionary_term_for_review

namespace hive::plugins::dictionary::validators
{
    using validators::DictionaryTermForReviewValidator;
    using hive::api::OperationResult;
    using hive::essential::g_configuration;

    OperationResult DictionaryTermForReviewValidator::validate_create_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult DictionaryTermForReviewValidator::validate_read_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult DictionaryTermForReviewValidator::validate_update_authorization(
        const RequestContext& ctx, const Model& old_entity,
        const Model& new_entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult DictionaryTermForReviewValidator::validate_delete_authorization(
        const RequestContext& ctx, const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult DictionaryTermForReviewValidator::validate_list_authorization(const RequestContext& ctx,
                                                                                 const string_map& filter) const
    {
        mandatory_filter(dictionary_map_id)
        auto dictionary_map_id = std::stoll(filter.at("dictionary_map_id"));

        if (!dictionary::has_right_for_map(ctx, dictionary_map_id, plugins::core::enums::SingleRight::Read))
            return {
                403,
                std::string(
                    "You do not have permission to list terms for review for map with ID " + std::to_string(dictionary_map_id)
                    + ".")
            };

        return ok_result;
    }

    OperationResult DictionaryTermForReviewValidator::validate_create_integrity(const RequestContext& ctx,
                                                                               const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult DictionaryTermForReviewValidator::validate_read_integrity(const RequestContext& ctx,
                                                                             const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult DictionaryTermForReviewValidator::validate_update_integrity(const RequestContext& ctx,
                                                                               const Model& old_entity,
                                                                               const Model& new_entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult DictionaryTermForReviewValidator::validate_delete_integrity(const RequestContext& ctx,
                                                                               const Model& entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult DictionaryTermForReviewValidator::validate_list_integrity(const RequestContext& ctx,
                                                                             const string_map& filter) const
    {
        mandatory_filter(dictionary_map_id)
        return ok_result;
    }

    string DictionaryTermForReviewValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
