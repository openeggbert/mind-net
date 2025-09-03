//
// Created by robertvokac on 9/2/25.
//
#ifndef REFERENCECRUDLVALIDATOR_H
#define REFERENCECRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;

    class ReferenceCrudlValidator : public api::CrudlValidator
    {
    public:
        ReferenceCrudlValidator() = default;
        ~ReferenceCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // REFERENCECRUDLVALIDATOR_H
