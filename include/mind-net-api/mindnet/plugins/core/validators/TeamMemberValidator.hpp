//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/core/models/TeamMember.hpp"

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

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
