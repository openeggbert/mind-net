//
// Created by robertvokac on 9/2/25.
//
#ifndef SM2STATECRUDLVALIDATOR_H
#define SM2STATECRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;

    class SM2StateCrudlValidator : public api::CrudlValidator
    {
    public:
        SM2StateCrudlValidator() = default;
        ~SM2StateCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // SM2STATECRUDLVALIDATOR_H
