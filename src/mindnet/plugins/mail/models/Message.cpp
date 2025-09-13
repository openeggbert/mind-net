//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/mail/models/Message.h"

namespace mindnet::plugins::mail::models
{
    entity_fields Message::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(owner_id);
        result.push_back(sender_id);
        result.push_back(recipient_id);
        result.push_back(subject);
        result.push_back(important);
        result.push_back(body);
        result.push_back(cast64(sent_at));
        result.push_back(system_message);
        result.push_back(folder);
        result.push_back(draft);
        result.push_back(is_read);
        result.push_back(cast64(deleted_at));
        result.push_back(starred);
        return result;
    }

    void Message::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        owner_id = number();
        sender_id = number();
        recipient_id = number();
        subject = text();
        important = number();
        body = text();
        sent_at = number();
        system_message = number();
        folder = text();
        draft = number();
        is_read = number();
        deleted_at = number();
        starred = number();
    };

    string Message::validate()
    {
        using columns::MessageColumns;

        validator_chain_vector list{
        [this] { return test_ne(owner_id, 0, MessageColumns::OWNER_ID);},
        [this] { return test_ne(sender_id, 0, MessageColumns::SENDER_ID);},
            [this] { return test_ne(recipient_id, 0, MessageColumns::RECIPIENT_ID);},
        [this] { return test_ne(sender_id, recipient_id, "sender_id and recipient_id must not be equal");},
        [this]{return test_false(sender_id != owner_id && recipient_id != owner_id, "sender_id and recipient_id must be either owner_id or recipient_id");},
    };
        return ValidatorChain::run(list);
    }
}
