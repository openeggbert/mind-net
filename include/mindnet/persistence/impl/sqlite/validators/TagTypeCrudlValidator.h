//
// Created by robertvokac on 9/2/25.
//
#ifndef TAGTYPECRUDLVALIDATOR_H
#define TAGTYPECRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidator.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;

    class TagTypeCrudlValidator : public api::CrudlValidator
    {
    public:
        TagTypeCrudlValidator() = default;
        ~TagTypeCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // TAGTYPECRUDLVALIDATOR_H
