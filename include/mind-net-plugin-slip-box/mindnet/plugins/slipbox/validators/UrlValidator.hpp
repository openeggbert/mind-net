//
// Created by robertvokac on 9/2/25.
//
#ifndef URLVALIDATOR_H
#define URLVALIDATOR_H


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/Url.hpp"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Url;

    class UrlValidator : public api::ValidatorBase<UrlValidator, Url>
    {
    public:
        UrlValidator() = default;
        ~UrlValidator() = default; // explicitly make it destructible
        using Model = Url;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
#endif // URLVALIDATOR_H
