//
// Created by robertvokac on 9/2/25.
//
#ifndef PROPERTYCRUDLVALIDATOR_H
#define PROPERTYCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;

    class PropertyCrudlValidator : public api::CrudlValidator
    {
    public:
        PropertyCrudlValidator() = default;
        ~PropertyCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // PROPERTYCRUDLVALIDATOR_H
