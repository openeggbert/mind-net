//
// Created by robertvokac on 9/2/25.
//
#ifndef TEAMMEMBERVALIDATOR_H
#define TEAMMEMBERVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "../../../api/ValidatorBase.h"

namespace mindnet::plugins::core::validators
{
    using api::RequestContext;
    using mindnet::plugins::core::models::TeamMember;

    class TeamMemberValidator : public api::ValidatorBase<TeamMemberValidator, TeamMember>
    {
    public:
        TeamMemberValidator() = default;
        ~TeamMemberValidator() = default; // explicitly make it destructible

        using Model = TeamMember;

        create_method_prototypes_for_ValidatorBase(TeamMember)
    };
}
#endif // TEAMMEMBERVALIDATOR_H
