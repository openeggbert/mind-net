//
// Created by robertvokac on 9/2/25.
//
#ifndef REVIEWCRUDLVALIDATOR_H
#define REVIEWCRUDLVALIDATOR_H


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

    class ReviewCrudlValidator : public api::CrudlValidator
    {
    public:
        ReviewCrudlValidator() = default;
        ~ReviewCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // REVIEWCRUDLVALIDATOR_H
