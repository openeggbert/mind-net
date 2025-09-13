//
// Created by robertvokac on 9/2/25.
//
#ifndef LINKVALIDATOR_H
#define LINKVALIDATOR_H

#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/ValidatorBase.h"

namespace mindnet::plugins::zettelkasten::validators
{
    using persistence::api::RequestContext;
    using mindnet::plugins::zettelkasten::models::Link;

    class LinkValidator : public persistence::api::ValidatorBase<LinkValidator, Link>
    {
    public:
        LinkValidator() = default;
        ~LinkValidator() = default; // explicitly make it destructible
        using Model = Link;

        create_method_prototypes_for_ValidatorBase(Link)
    };
}
#endif // LINKVALIDATOR_H
