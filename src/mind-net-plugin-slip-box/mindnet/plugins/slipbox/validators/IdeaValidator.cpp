//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/plugins/slipbox/validators/IdeaValidator.h"

#include "mindnet/essential/Global.h"
#include "mindnet/plugins/slipbox/models/Idea.h"
#include "../../../../../../include/mind-net-api/mindnet/api/Persistence.h"

#define Model Idea
#define MODEL IDEA
#define model idea

namespace mindnet::plugins::slipbox::validators
{
    using validators::IdeaValidator;
    using mindnet::api::OperationResult;using mindnet::essential::g_configuration;

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
        if (user_id_filled)
        {
            if (stoi(filter.at("user_id")) != ctx.token.user_id)
            {
                return {403, "You filter using user_id, which is not equal to your user_id"};
            }
            else { return ok_result; }
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
