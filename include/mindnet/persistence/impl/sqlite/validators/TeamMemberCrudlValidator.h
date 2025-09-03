//
// Created by robertvokac on 9/2/25.
//
#ifndef TEAMMEMBERCRUDLVALIDATOR_H
#define TEAMMEMBERCRUDLVALIDATOR_H


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

    class TeamMemberCrudlValidator : public api::CrudlValidator
    {
    public:
        TeamMemberCrudlValidator() = default;
        ~TeamMemberCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // TEAMMEMBERCRUDLVALIDATOR_H
