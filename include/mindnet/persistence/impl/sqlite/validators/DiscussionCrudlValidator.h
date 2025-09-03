//
// Created by robertvokac on 9/2/25.
//
#ifndef DISCUSSIONCRUDLVALIDATOR_H
#define DISCUSSIONCRUDLVALIDATOR_H


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

    class DiscussionCrudlValidator : public api::CrudlValidator
    {
    public:
        DiscussionCrudlValidator() = default;
        ~DiscussionCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // DISCUSSIONCRUDLVALIDATOR_H
