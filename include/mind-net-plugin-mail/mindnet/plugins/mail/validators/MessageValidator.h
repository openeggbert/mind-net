//
// Created by robertvokac on 9/2/25.
//
#ifndef MESSAGEVALIDATOR_H
#define MESSAGEVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "mindnet/api/ValidatorBase.h"

namespace mindnet::plugins::mail::validators
{
    using api::RequestContext;
    using mindnet::plugins::mail::models::Message;

    class MessageValidator : public api::ValidatorBase<MessageValidator, Message>
    {
    public:
        MessageValidator() = default;
        ~MessageValidator() = default; // explicitly make it destructible
        using Model = Message;

        create_method_prototypes_for_ValidatorBase(Message)
    };
}
#endif // MESSAGEVALIDATOR_H
