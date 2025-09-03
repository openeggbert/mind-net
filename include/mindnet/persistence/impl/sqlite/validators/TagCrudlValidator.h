//
// Created by robertvokac on 9/2/25.
//
#ifndef TAGCRUDLVALIDATOR_H
#define TAGCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;

    class TagCrudlValidator : public api::CrudlValidator
    {
    public:
        TagCrudlValidator() = default;
        ~TagCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // TAGCRUDLVALIDATOR_H
