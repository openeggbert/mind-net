//
// Created by robertvokac on 8/6/25.
//


#include "mindnet/persistence/impl/sqlite/validators/MessageCrudlValidator.h"

#include "mindnet/Global.h"
#include "mindnet/models/Message.h"
#include "mindnet/persistence/Persistence.h"

namespace mindnet::persistence::impl::sqlite::validators
{
    using impl::sqlite::validators::MessageCrudlValidator;
    operation_result MessageCrudlValidator::can_create(db_& d, entity_fields& ef, http::LoginToken& login_token) const
    {
        //2. Authorization
        models::Message new_message;
        new_message.from_values(ef);
        err << new_message << commit;
        models::User logged_in_user = d->find_logged_in_user(login_token).first;

        //3. Request
        if (new_message.owner_id != logged_in_user.get_id()) return operation_result(
            403, "You can only create messages for your own user.");
        if (new_message.sent_at != 0) return operation_result(400, "sent_at must not be set during message creation.");
        if (!new_message.draft) return operation_result(400, "draft must be set to true during message creation.");
        return ok_result;
    }

    operation_result MessageCrudlValidator::can_read(db_ d, int id, http::LoginToken& login_token) const
    {
        //2. Authorization
        models::Message message;
        message.from_values(d->read(id, models::MESSAGE_DEFINITION, login_token).first);
        err << message << commit;

        models::User logged_in_user = d->find_logged_in_user(login_token).first;

        if (message.owner_id != logged_in_user.get_id()) return operation_result(
            403, "You can only read messages for your own user.");

        //3. Request
        return ok_result;
    }

    operation_result MessageCrudlValidator::can_update(db_ d, entity_fields& ef, http::LoginToken& login_token) const
    {
        //2. Authorization
        auto logged_in_user_pair = d->find_logged_in_user(login_token);
        if (logged_in_user_pair.second.ko()) return logged_in_user_pair.second;
        auto logged_in_user = logged_in_user_pair.first;
        //
        models::Message old_message;
        old_message.from_values(ef);
        models::Message new_message;
        new_message.from_values(d->read(old_message.get_id(), models::MESSAGE_DEFINITION, login_token).first);

        //3. Request
        string error = new_message.validate();
        if (!error.empty()) return operation_result(400, error);

        if (logged_in_user.get_id() != new_message.owner_id) return operation_result(
            403, "You can only update your own message.");
        if (old_message.sent_at != 0 && new_message.sent_at == 0) return operation_result(
            400, "sent_at cannot be changed, if already set");
        if (new_message.sent_at != 0 && new_message.draft) return operation_result(
            400, "draft cannot be changed, if sent_at is set");
        if (old_message.system_message != new_message.system_message) return operation_result(
            400, "system_message cannot be changed");
        if (old_message.sent_at != 0)
        {
            if (old_message.sender_id != new_message.sender_id) return operation_result(400, "sender_id cannot be changed, if sent_at is set");
            if (old_message.recipient_id != new_message.recipient_id) return operation_result(400, "recipient_id cannot be changed, if sent_at is set");
            if (old_message.subject != new_message.subject) return operation_result(400, "subject cannot be changed, if sent_at is set");
            if (old_message.body != new_message.body) return operation_result(400, "body cannot be changed, if sent_at is set");
            if (old_message.draft != new_message.draft) return operation_result(400, "draft cannot be changed, if sent_at is set");


        }

        return ok_result;
    }

    operation_result MessageCrudlValidator::can_delete(db_ d, int id, http::LoginToken& login_token) const
    {
        //2. Authorization
        auto logged_in_user_pair = d->find_logged_in_user(login_token);
        if (logged_in_user_pair.second.ko()) return logged_in_user_pair.second;
        auto logged_in_user = logged_in_user_pair.first;
        //
        models::Message message;
        message.from_values(d->read(id, models::MESSAGE_DEFINITION, login_token).first);

        if (logged_in_user.get_id() != message.owner_id) return operation_result(
            403, "You can only delete your own message.");

        //3. Request
        if (message.deleted_at == 0) return operation_result(
            400, "message cannot be deleted, if deleted_at flag is not set");

        return ok_result;
    }

    operation_result MessageCrudlValidator::can_list(db_ d, std::map<std::string, std::string>& filter,
                                                     http::LoginToken& login_token) const
    {
        //2. Authorization
        models::User logged_in_user = d->find_logged_in_user(login_token).first;

        if (filter.find("owner_id") == filter.end()) return {403, "You can't filter without owner_id."};
        if (filter["owner_id"] != std::to_string(logged_in_user.get_id())) return operation_result(
            403, "You can only list messages for your own user.");

        //3. Request
        return ok_result;
    }

    string MessageCrudlValidator::get_model_name() const
    {
        return "message";
    }
}
