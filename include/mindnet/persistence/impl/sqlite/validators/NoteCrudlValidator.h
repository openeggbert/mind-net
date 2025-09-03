//
// Created by robertvokac on 9/2/25.
//
#ifndef NOTECRUDLVALIDATOR_H
#define NOTECRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;

    class NoteCrudlValidator : public api::CrudlValidator
    {
    public:
        NoteCrudlValidator() = default;
        ~NoteCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // NOTECRUDLVALIDATOR_H
