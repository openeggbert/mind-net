//
// Created by robertvokac on 9/2/25.
//
#ifndef TEAMVALIDATOR_H
#define TEAMVALIDATOR_H

#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/core/models/Team.hpp"

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

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // TEAMVALIDATOR_H
