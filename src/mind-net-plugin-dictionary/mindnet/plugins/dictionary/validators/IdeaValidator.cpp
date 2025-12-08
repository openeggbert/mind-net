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

#include "mindnet/plugins/dictionary/validators/IdeaValidator.hpp"

#include "mindnet/essential/Global.hpp"
#include "mindnet/plugins/dictionary/models/Idea.hpp"
#include "mindnet/api/Persistence.hpp"

#define Model Idea
#define MODEL IDEA
#define model idea

namespace mindnet::plugins::dictionary::validators
{
    using validators::IdeaValidator;
    using mindnet::api::OperationResult;
    using mindnet::essential::g_configuration;

    OperationResult IdeaValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        if (ctx.token.user_id != entity.user_id)
        {
            return {400, "You can create only your own idea"};
        }
        return ok_result;
    }

    OperationResult IdeaValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult IdeaValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        if (!entity.is_public && entity.user_id != ctx.token.user_id)
        {
            return {400, "You can read only: your own idea or public idea."};
        }
        return ok_result;
    }

    OperationResult IdeaValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult IdeaValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                 const Model& new_entity) const
    {
        if (new_entity.user_id != ctx.token.user_id)
        {
            return {400, "You can update only your own ideas."};
        }

        return ok_result;
    }

    OperationResult IdeaValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                             const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult IdeaValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        if (entity.user_id != ctx.token.user_id)
        {
            return {400, "You can delete only your own ideas."};
        }

        return ok_result;
    }

    OperationResult IdeaValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult IdeaValidator::validate_list_authorization(const RequestContext& ctx,
                                                               const string_map& filter) const
    {
        bool user_id_filled = filter.contains("user_id");
        bool is_public_filled = filter.contains("is_public");
        if (user_id_filled && is_public_filled)
            return {
                403, std::string("You can't filter without ") + "user_id or is_public" + "."
            };
        if (is_public_filled)
        {
            if (filter.at("is_public") != "1") { return {403, "You cannot filter is_public=false"}; }
        }
        else
        {
            return ok_result;
        }

        return {403, "Your filter for ideas is wrong."};
    }

    OperationResult IdeaValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        return ok_result;
    }

    string IdeaValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model