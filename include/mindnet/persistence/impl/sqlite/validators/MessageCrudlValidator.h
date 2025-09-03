//
// Created by robertvokac on 9/2/25.
//
#ifndef MESSAGECRUDLVALIDATOR_H
#define MESSAGECRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;

    class MessageCrudlValidator : public api::CrudlValidator
    {
    public:
        MessageCrudlValidator() = default;
        ~MessageCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // MESSAGECRUDLVALIDATOR_H
