//
// Created by robertvokac on 9/2/25.
//
#ifndef MESSAGECRUDLVALIDATOR_H
#define MESSAGECRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::mail::validators
{

    using persistence::api::RequestContext;
    using mindnet::plugins::mail::models::Message;

    class MessageCrudlValidator : public persistence::api::CrudlValidatorBase<MessageCrudlValidator, Message>
    {
    public:
        MessageCrudlValidator() = default;
        ~MessageCrudlValidator() = default; // explicitly make it destructible
        using Model = Message;

        create_method_prototypes_for_CrudlValidatorBase(Message)

    };

}
#endif // MESSAGECRUDLVALIDATOR_H
