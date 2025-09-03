//
// Created by robertvokac on 9/2/25.
//
#ifndef TEAMCRUDLVALIDATOR_H
#define TEAMCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;

    class TeamCrudlValidator : public api::CrudlValidator
    {
    public:
        TeamCrudlValidator() = default;
        ~TeamCrudlValidator() = default; // explicitly make it destructible
        create_h_methods()

    };

}
#endif // TEAMCRUDLVALIDATOR_H
