//
// Created by robertvokac on 9/2/25.
//
#ifndef TEAMMEMBERCRUDLVALIDATOR_H
#define TEAMMEMBERCRUDLVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;
    using api::ValidatorContext;
    using models::TeamMember;

    class TeamMemberCrudlValidator : public api::CrudlValidatorBase<TeamMemberCrudlValidator, models::TeamMember>
    {
    public:
        TeamMemberCrudlValidator() = default;
        ~TeamMemberCrudlValidator() = default; // explicitly make it destructible

        using Model = TeamMember;

        create_method_prototypes_for_CrudlValidatorBase(TeamMember)

    };

}
#endif // TEAMMEMBERCRUDLVALIDATOR_H
