//
// Created by robertvokac on 9/2/25.
//
#ifndef TEAMVALIDATOR_H
#define TEAMVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "../../../../../mind-net-api/mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/core/models/Team.h"

namespace mindnet::plugins::core::validators
{
    using api::RequestContext;
    using mindnet::plugins::core::models::Team;

    class TeamValidator : public api::ValidatorBase<TeamValidator, Team>
    {
    public:
        TeamValidator() = default;
        ~TeamValidator() = default; // explicitly make it destructible
        using Model = Team;

        create_method_prototypes_for_ValidatorBase(Team)
    };
}
#endif // TEAMVALIDATOR_H
