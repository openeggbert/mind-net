//
// Created by robertvokac on 9/2/25.
//
#ifndef SOURCEVALIDATOR_H
#define SOURCEVALIDATOR_H

#include <memory>

#include "mindnet/essential/Helper.h"
#include "../../../../../mind-net-api/mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/slipbox/models/Source.h"

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

        create_method_prototypes_for_ValidatorBase(Source)
    };
}
#endif // SOURCEVALIDATOR_H
