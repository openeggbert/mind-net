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

#include "hive/plugins/dictionary/validators/DictionaryUrlValidator.hpp"

#include "hive/essential/Global.hpp"
#include "hive/plugins/core/enums/SingleRight.hpp"
#include "hive/plugins/dictionary/models/DictionaryUrl.hpp"
#include "hive/api/Persistence.hpp"
#include "hive/plugins/dictionary/DictionaryPersistenceMethods.hpp"

#define Model DictionaryUrl
#define MODEL DICTIONARY_URL
#define model dictionary_url

namespace hive::plugins::dictionary::validators
{
    using validators::DictionaryUrlValidator;
    using hive::api::OperationResult;
    using hive::essential::g_configuration;

    OperationResult DictionaryUrlValidator::validate_create_authorization(const RequestContext& ctx,
                                                                          const Model& entity) const
    {
        auto url_type = dictionary::find_dictionary_url_type(ctx, entity.dictionary_url_type_id);
        if (!url_type.second.empty()) return {400, url_type.second};

        if (dictionary::has_right_for_map(ctx, url_type.first.dictionary_map_id, plugins::core::enums::SingleRight::Write))
        {
            return ok_result;
        }
        return {403, "You do not have permission to create a url for this map."};
    }

    OperationResult DictionaryUrlValidator::validate_read_authorization(const RequestContext& ctx,
                                                                        const Model& entity) const
    {
        auto url_type = dictionary::find_dictionary_url_type(ctx, entity.dictionary_url_type_id);
        if (!url_type.second.empty()) return {400, url_type.second};

        auto map = dictionary::find_dictionary_map(ctx, url_type.first.dictionary_map_id);
        if (!map.second.empty()) return {400, map.second};

        if (dictionary::has_right_for_map(ctx, url_type.first.dictionary_map_id, plugins::core::enums::SingleRight::Read))
        {
            return ok_result;
        }
        return {403, "You do not have permission to read this url."};
    }

    OperationResult DictionaryUrlValidator::validate_update_authorization(const RequestContext& ctx,
                                                                          const Model& old_entity,
                                                                          const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryUrlValidator::validate_delete_authorization(const RequestContext& ctx,
                                                                          const Model& entity) const
    {
        auto url_type = dictionary::find_dictionary_url_type(ctx, entity.dictionary_url_type_id);
        if (!url_type.second.empty()) return {400, url_type.second};

        if (dictionary::has_right_for_map(ctx, url_type.first.dictionary_map_id, plugins::core::enums::SingleRight::Delete))
        {
            return ok_result;
        }
        return {403, "You do not have permission to delete this url_type."};
    }

    OperationResult DictionaryUrlValidator::validate_list_authorization(const RequestContext& ctx,
                                                                        const string_map& filter) const
    {
        mandatory_filter(dictionary_url_type_id)
        auto dictionary_url_type_id = std::stoll(filter.at("dictionary_url_type_id"));

        auto dictionary_url_type = dictionary::find_dictionary_url_type(ctx, dictionary_url_type_id);
        if (!dictionary_url_type.second.empty()) return {400, dictionary_url_type.second};

        if (!dictionary::has_right_for_map(ctx, dictionary_url_type.first.dictionary_map_id, plugins::core::enums::SingleRight::Read))
            return {
                403,
                std::string(
                    "You do not have permission to list urls for map with ID " +
                    std::to_string(dictionary_url_type.first.dictionary_map_id) + ".")
            };

        return ok_result;
    }

    OperationResult DictionaryUrlValidator::validate_create_integrity(const RequestContext& ctx,
                                                                      const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryUrlValidator::validate_read_integrity(const RequestContext& ctx,
                                                                    const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryUrlValidator::validate_update_integrity(const RequestContext& ctx,
                                                                      const Model& old_entity,
                                                                      const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryUrlValidator::validate_delete_integrity(const RequestContext& ctx,
                                                                      const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryUrlValidator::validate_list_integrity(const RequestContext& ctx,
                                                                    const string_map& filter) const
    {
        return ok_result;
    }

    string DictionaryUrlValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
