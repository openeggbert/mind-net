//
// Created by robertvokac on 9/2/25.
//
#pragma once


#include <memory>

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/api/ValidatorBase.hpp"
#include "mindnet/plugins/slipbox/models/Source.hpp"

namespace mindnet::plugins::slipbox::validators
{
    using api::RequestContext;
    using mindnet::plugins::slipbox::models::Source;

    class SourceValidator : public api::ValidatorBase<SourceValidator, Source>
    {
    public:
        SourceValidator() = default;
        ~SourceValidator() = default; // explicitly make it destructible
        using Model = Source;

        create_method_prototypes_for_ValidatorBase(Model)
    };
}
