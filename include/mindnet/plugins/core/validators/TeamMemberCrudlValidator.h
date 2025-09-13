//
// Created by robertvokac on 9/2/25.
//
#ifndef TEAMMEMBERCRUDLVALIDATOR_H
#define TEAMMEMBERCRUDLVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::core::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::core::models::TeamMember;

    class TeamMemberCrudlValidator : public persistence::api::CrudlValidatorBase<TeamMemberCrudlValidator, TeamMember>
    {
    public:
        TeamMemberCrudlValidator() = default;
        ~TeamMemberCrudlValidator() = default; // explicitly make it destructible

        using Model = TeamMember;

        create_method_prototypes_for_CrudlValidatorBase(TeamMember)
    };
}
#endif // TEAMMEMBERCRUDLVALIDATOR_H
