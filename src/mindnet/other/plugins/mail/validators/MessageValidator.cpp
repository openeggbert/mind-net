//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/other/plugins/mail/validators/MessageValidator.h"

#include "mindnet/core/Global.h"
#include "mindnet/other/plugins/core/enums/SingleRight.h"
#include "mindnet/other/plugins/mail/models/Message.h"
#include "mindnet/other/api/Persistence.h"

#define Model Message
#define MODEL MESSAGE
#define model message

namespace mindnet::plugins::mail::validators
{
    using validators::MessageValidator;
    using mindnet::api::OperationResult;using mindnet::core::g_configuration;
    OperationResult MessageValidator::validate_create_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult MessageValidator::validate_read_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult MessageValidator::validate_update_authorization(const RequestContext& ctx, const Model& old_entity,
                                                                  const Model& new_entity) const
    {
        return ok_result;
    }

    OperationResult MessageValidator::validate_delete_authorization(const RequestContext& ctx, const Model& entity) const
    {
        return ok_result;
    }

    OperationResult MessageValidator::validate_list_authorization(const RequestContext& ctx,
                                                                const string_map& filter) const
    {
        return ok_result;
    }







    OperationResult MessageValidator::validate_create_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(entity.owner_id != ctx.token.user_id,
                  403, "You can only create messages for your own user.");
        return_if(entity.sent_at != 0, 400, "sent_at must not be set during message creation.");
        return_if(!entity.draft, 400, "draft must be set to true during message creation.");
        return ok_result;
    }

    OperationResult MessageValidator::validate_read_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(entity.owner_id != ctx.token.user_id,
                  403, "You can only read messages for your own user.");

        return ok_result;
    }

    OperationResult MessageValidator::validate_update_integrity(const RequestContext& ctx, const Model& old_entity,
                                                           const Model& new_entity) const
    {
        return_if(ctx.token.user_id != new_entity.owner_id,
                  403, "You can only update your own message.");
        return_if(old_entity.sent_at != 0 && new_entity.sent_at == 0,
                  400, "sent_at cannot be changed, if already set");
        return_if(old_entity.sent_at != 0 && new_entity.draft,
                  400, "draft cannot be changed, if sent_at is set");

        if (old_entity.sent_at != 0)
        {
            return_if(old_entity.sender_id != new_entity.sender_id, 400,
                      "sender_id cannot be changed, if sent_at is set");
            return_if(old_entity.recipient_id != new_entity.recipient_id, 400,
                      "recipient_id cannot be changed, if sent_at is set");
            return_if(old_entity.subject != new_entity.subject, 400, "subject cannot be changed, if sent_at is set");
            return_if(old_entity.body != new_entity.body, 400, "body cannot be changed, if sent_at is set");
            return_if(old_entity.draft != new_entity.draft, 400, "draft cannot be changed, if sent_at is set");
        }

        return ok_result;
    }

    OperationResult MessageValidator::validate_delete_integrity(const RequestContext& ctx, const Model& entity) const
    {
        return_if(ctx.token.user_id != entity.owner_id,
                  403, "You can only delete your own message.");

        return_if(entity.deleted_at == 0,
                  400, "message cannot be deleted, if deleted_at flag is not set");

        return ok_result;
    }

    OperationResult MessageValidator::validate_list_integrity(const RequestContext& ctx, const string_map& filter) const
    {
        mandatory_filter(owner_id)

        return_if(filter.at("owner_id") != std::to_string(ctx.token.user_id),
                  403, "You can only list messages for your own user.");

        return ok_result;
    }

    string MessageValidator::get_model_name() const
    {
        return STRINGIFY(model);
    }
}

#undef Model
#undef MODEL
#undef model
