//
// Created by robertvokac on 9/2/25.
//
#ifndef LINKVALIDATOR_H
#define LINKVALIDATOR_H


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/Link.hpp"

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

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // LINKVALIDATOR_H
