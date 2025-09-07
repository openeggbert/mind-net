//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/ContentCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/Content.h"
#include "mindnet/persistence/api/Persistence.h"

#define Model Content
#define MODEL CONTENT
#define model content

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::ContentCrudlValidator;

    operation_result ContentCrudlValidator::validate_create(const ValidatorContext& ctx, const Model& entity) const
    {

        return_if (ctx.logged_user.role < enums::UserRole::EDITOR,403, "You can not create content.")
        return_if (entity.version != 1,
            404, "version must be 1 during message creation.");

        return ok_result;
    }

    operation_result ContentCrudlValidator::validate_read(const ValidatorContext& ctx, const Model& entity) const
    {


        return_if (entity.owner_id != ctx.logged_user.get_id(),
            403, "You can only read messages for your own user.");

        return ok_result;
    }

    operation_result ContentCrudlValidator::validate_update(const ValidatorContext& ctx, const Model& old_entity, const Model& new_entity) const
    {


        return_if (ctx.logged_user.get_id() != new_entity.owner_id,
            403, "You can only update your own message.");
        return_if (old_entity.sent_at != 0 && new_entity.sent_at == 0,
            400, "sent_at cannot be changed, if already set");
        return_if (old_entity.sent_at != 0 && new_entity.draft,
            400, "draft cannot be changed, if sent_at is set");

        if (old_entity.sent_at != 0)
        {
            return_if (old_entity.sender_id != new_entity.sender_id,400, "sender_id cannot be changed, if sent_at is set");
            return_if (old_entity.recipient_id != new_entity.recipient_id,400, "recipient_id cannot be changed, if sent_at is set");
            return_if (old_entity.subject != new_entity.subject,400, "subject cannot be changed, if sent_at is set");
            return_if (old_entity.body != new_entity.body,400, "body cannot be changed, if sent_at is set");
            return_if (old_entity.draft != new_entity.draft,400, "draft cannot be changed, if sent_at is set");
        }

        return ok_result;
    }

    operation_result ContentCrudlValidator::validate_delete(const ValidatorContext& ctx, const Model& entity)  const
    {


        return_if (ctx.logged_user.get_id() != entity.owner_id,
            403, "You can only delete your own message.");

        return_if (entity.deleted_at == 0,
            400, "message cannot be deleted, if deleted_at flag is not set");

        return ok_result;
    }

    operation_result ContentCrudlValidator::validate_list(const ValidatorContext& ctx, const string_map& filter) const
    {


        mandatory_filter(owner_id)

        return_if (filter["owner_id"] != std::to_string(ctx.logged_user.get_id()),
            403, "You can only list messages for your own user.");

        return ok_result;
    }

    string ContentCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}

#undef Model
#undef MODEL
#undef model