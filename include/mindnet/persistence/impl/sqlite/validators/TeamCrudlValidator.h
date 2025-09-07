//
// Created by robertvokac on 9/2/25.
//
#ifndef TEAMCRUDLVALIDATOR_H
#define TEAMCRUDLVALIDATOR_H


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
    using api::RequestContext;
    using models::Team;

    class TeamCrudlValidator : public api::CrudlValidatorBase<TeamCrudlValidator, models::Team>
    {
    public:
        TeamCrudlValidator() = default;
        ~TeamCrudlValidator() = default; // explicitly make it destructible
        using Model = Team;

        create_method_prototypes_for_CrudlValidatorBase(Team)

    };

}
#endif // TEAMCRUDLVALIDATOR_H
