//
// Created by robertvokac on 9/2/25.
//
#ifndef LINKCRUDLVALIDATOR_H
#define LINKCRUDLVALIDATOR_H


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

    class LinkCrudlValidator : public api::CrudlValidator
    {
    public:
        LinkCrudlValidator() = default;
        ~LinkCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // LINKCRUDLVALIDATOR_H
