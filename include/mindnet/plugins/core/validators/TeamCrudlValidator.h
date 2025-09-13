//
// Created by robertvokac on 9/2/25.
//
#ifndef TEAMCRUDLVALIDATOR_H
#define TEAMCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"


namespace mindnet::plugins::core::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::core::models::Team;

    class TeamCrudlValidator : public persistence::api::CrudlValidatorBase<TeamCrudlValidator, Team>
    {
    public:
        TeamCrudlValidator() = default;
        ~TeamCrudlValidator() = default; // explicitly make it destructible
        using Model = Team;

        create_method_prototypes_for_CrudlValidatorBase(Team)
    };
}
#endif // TEAMCRUDLVALIDATOR_H
