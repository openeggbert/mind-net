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

#include "mindnet/plugins/dictionary/validators/DictionaryState18Validator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/dictionary/models/DictionaryState18.hpp"
#include "mindnet/api/Persistence.hpp"
#include "mindnet/plugins/dictionary/DictionaryPersistenceMethods.hpp"

#define Model DictionaryState18
#define MODEL DICTIONARY_STATE_18
#define model dictionary_state_18

namespace mindnet::plugins::dictionary::validators
{
    using validators::DictionaryState18Validator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult DictionaryState18Validator::validate_create_authorization(const RequestContext& ctx,
                                                                           const Model& entity) const
    {
        // User can only create states for themselves
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Can only create states for yourself"};
        }
        return ok_result;
    }

    OperationResult DictionaryState18Validator::validate_read_authorization(const RequestContext& ctx,
                                                                         const Model& entity) const
    {
        // Users can only read their own states
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Can only access your own states"};
        }
        return ok_result;
    }

    OperationResult DictionaryState18Validator::validate_update_authorization(const RequestContext& ctx,
                                                                           const Model& old_entity,
                                                                           const Model& new_entity) const
    {
        return status_405_unsupported_operation;
    }

    OperationResult DictionaryState18Validator::validate_delete_authorization(const RequestContext& ctx,
                                                                           const Model& entity) const
    {
        if (ctx.token.user_id != entity.user_id)
        {
            return {403, "Can only delete your own states 4"};
        }
        return ok_result;    }

    OperationResult DictionaryState18Validator::validate_list_authorization(const RequestContext& ctx,
                                                                         const string_map& filter) const
    {
        mandatory_filter(user_id)

        // Users can only list their own states
        auto it = filter.find("user_id");
        if (it == filter.end() || std::stoll(it->second) != ctx.token.user_id)
        {
            return {403, "Can only list your own states"};
        }
        return ok_result;
    }

    OperationResult DictionaryState18Validator::validate_create_integrity(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryState18Validator::validate_read_integrity(const RequestContext& ctx,
                                                                     const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryState18Validator::validate_update_integrity(const RequestContext& ctx,
                                                                       const Model& old_entity,
                                                                       const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryState18Validator::validate_delete_integrity(const RequestContext& ctx,
                                                                       const Model& entity) const
    {
        return ok_result;
    }

    OperationResult DictionaryState18Validator::validate_list_integrity(const RequestContext& ctx,
                                                                     const string_map& filter) const
    {
        return ok_result;
    }

    string DictionaryState18Validator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
