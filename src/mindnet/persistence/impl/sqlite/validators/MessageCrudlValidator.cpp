//
// Created by robertvokac on 8/6/25.
//

#include "mindnet/persistence/impl/sqlite/validators/MessageCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/Message.h"
#include "mindnet/persistence/Persistence.h"

#define Model Message
#define MODEL MESSAGE
#define model message

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::MessageCrudlValidator;

    operation_result MessageCrudlValidator::can_create(db_ db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_create(Model);

        return_if (new_entity.owner_id != logged_in_user.get_id(),
            403, "You can only create messages for your own user.");
        return_if (new_entity.sent_at != 0,400, "sent_at must not be set during message creation.");
        return_if (!new_entity.draft, 400, "draft must be set to true during message creation.");
        return ok_result;
    }

    operation_result MessageCrudlValidator::can_read(db_ db, http::LoginToken& token, int id) const
    {
        start_can_read(Model, MODEL)

        return_if (entity.owner_id != logged_in_user.get_id(),
            403, "You can only read messages for your own user.");

        return ok_result;
    }

    operation_result MessageCrudlValidator::can_update(db_ db, http::LoginToken& token, entity_fields& ef) const
    {
        start_can_update(Model, MODEL)
        
        return_if (logged_in_user.get_id() != new_entity.owner_id,
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

    operation_result MessageCrudlValidator::can_delete(db_ db, http::LoginToken& token, int id) const
    {
        start_can_delete(Model, MODEL)

        return_if (logged_in_user.get_id() != entity.owner_id,
            403, "You can only delete your own message.");

        return_if (entity.deleted_at == 0,
            400, "message cannot be deleted, if deleted_at flag is not set");

        return ok_result;
    }

    operation_result MessageCrudlValidator::can_list(db_ db, http::LoginToken& token, string_map& filter) const
    {
        start_can_list(Model, MODEL)

        mandatory_filter(owner_id)

        return_if (filter["owner_id"] != std::to_string(logged_in_user.get_id()),
            403, "You can only list messages for your own user.");

        return ok_result;
    }

    string MessageCrudlValidator::get_model_name() const
    {
        return STRING(model);
    }
}

#undef Model
#undef MODEL
#undef model