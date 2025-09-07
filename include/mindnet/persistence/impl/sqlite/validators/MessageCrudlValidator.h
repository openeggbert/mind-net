//
// Created by robertvokac on 9/2/25.
//
#ifndef MESSAGECRUDLVALIDATOR_H
#define MESSAGECRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;
    using api::ValidatorContext;
    using models::Message;

    class MessageCrudlValidator : public api::CrudlValidatorBase<MessageCrudlValidator, models::Message>
    {
    public:
        MessageCrudlValidator() = default;
        ~MessageCrudlValidator() = default; // explicitly make it destructible
        using Model = Message;

        create_method_prototypes_for_CrudlValidatorBase(Message)

    };

}
#endif // MESSAGECRUDLVALIDATOR_H
