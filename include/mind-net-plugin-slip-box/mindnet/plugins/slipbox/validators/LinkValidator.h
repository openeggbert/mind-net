//
// Created by robertvokac on 9/2/25.
//
#ifndef LINKVALIDATOR_H
#define LINKVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "../../../../../mind-net-api/mindnet/api/ValidatorBase.h"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Link;

    class LinkValidator : public api::ValidatorBase<LinkValidator, Link>
    {
    public:
        LinkValidator() = default;
        ~LinkValidator() = default; // explicitly make it destructible
        using Model = Link;

        create_method_prototypes_for_ValidatorBase(Link)
    };
}
#endif // LINKVALIDATOR_H
