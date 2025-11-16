//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/Content.hpp"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Content;

    class ContentValidator : public api::ValidatorBase<ContentValidator, Content>
    {
    public:
        ContentValidator() = default;
        ~ContentValidator() = default; // explicitly make it destructible
        using Model = Content;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
